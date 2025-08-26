#include "AlexaController.h"
#include <WiFi.h>

// ===== COSTANTI =====
#define PREFERENCES_NAMESPACE "alexa_ctrl"
#define MAX_INPUT_LENGTH 100
#define MENU_RETURN_DELAY 2000

AlexaController::AlexaController() : currentState(ALEXA_IDLE), alexaEnabled(false), initialized(false) {
    deviceManager = nullptr;
    alexaManager = nullptr;
    systemWifiManager = nullptr;
}

AlexaController::~AlexaController() {
    if (alexaManager) delete alexaManager;
    if (deviceManager) delete deviceManager;
}

bool AlexaController::initialize(WiFiManager* wifiManager) {
    Serial.println("\n🚀 Inizializzazione Alexa Controller...");
    
    systemWifiManager = wifiManager;
    
    // Inizializza Preferences
    preferences.begin(PREFERENCES_NAMESPACE, false);
    
    // Crea le istanze dei manager
    deviceManager = new DeviceManager(&preferences);
    alexaManager = new AlexaManager(&fauxmo, deviceManager);
    
    // Carica i dispositivi salvati
    deviceManager->loadDevices();
    
    initialized = true;
    printWelcome();
    
    // Verifica connessione WiFi
    if (systemWifiManager && systemWifiManager->isWiFiConnected()) {
        Serial.println("✅ WiFi già connesso dal sistema principale");
        if (deviceManager->getDeviceCount() > 0) {
            alexaManager->initialize();
            alexaEnabled = true;
        }
        showMainMenu();
    } else if (WiFi.status() == WL_CONNECTED) {
        Serial.println("✅ WiFi connesso (gestione diretta)");
        if (deviceManager->getDeviceCount() > 0) {
            alexaManager->initialize();
            alexaEnabled = true;
        }
        showMainMenu();
    } else {
        Serial.println("⚠️ WiFi non connesso. Usa il sistema principale per configurare WiFi.");
        showMainMenu();
    }
    
    return true;
}

void AlexaController::handle() {
    if (!initialized) return;
    
    // Gestione Alexa
    if (alexaEnabled && alexaManager) {
        alexaManager->handle();
    }
    
    // Gestione input seriale
    handleSerialInput();
}

void AlexaController::enableAlexa(bool enable) {
    alexaEnabled = enable;
    if (alexaManager) {
        bool wifiConnected = (systemWifiManager && systemWifiManager->isWiFiConnected()) || 
                            (WiFi.status() == WL_CONNECTED);
        
        if (enable && wifiConnected && deviceManager->getDeviceCount() > 0) {
            alexaManager->initialize();
        } else if (!enable) {
            alexaManager->shutdown();
        }
    }
}

bool AlexaController::isAlexaInitialized() const {
    return alexaManager ? alexaManager->isAlexaInitialized() : false;
}

bool AlexaController::addDeviceFromSystem(const String& name, const String& url) {
    if (!deviceManager) return false;
    
    if (deviceManager->addDevice(name, url)) {
        if (alexaEnabled && ((systemWifiManager && systemWifiManager->isWiFiConnected()) || WiFi.status() == WL_CONNECTED) && alexaManager) {
            alexaManager->restart();
        }
        return true;
    }
    return false;
}

void AlexaController::handleSerialInput() {
    if (Serial.available()) {
        char c = Serial.read();
        
        if (c == '\n' || c == '\r') {
            if (inputBuffer.length() > 0) {
                processInput(inputBuffer);
                inputBuffer = "";
            }
        } else if (c >= 32 && c <= 126) {
            inputBuffer += c;
            if (inputBuffer.length() > MAX_INPUT_LENGTH) {
                inputBuffer = "";
                Serial.println("❌ Input troppo lungo");
                if (currentState != ALEXA_IDLE) {
                    Serial.print("👉 ");
                }
            }
        }
    }
}

void AlexaController::processInput(String input) {
    input.trim();
    
    switch (currentState) {
        case ALEXA_IDLE:
            handleMainCommand(input);
            break;
        case ALEXA_WAITING_DEVICE_NAME:
            handleDeviceName(input);
            break;
        case ALEXA_WAITING_DEVICE_URL:
            handleDeviceURL(input);
            break;
        case ALEXA_WAITING_RESET_CONFIRM:
            handleResetConfirm(input);
            break;
        case ALEXA_WAITING_DEVICE_REMOVE:
            handleDeviceRemove(input);
            break;
    }
}

void AlexaController::resetSerialState() {
    currentState = ALEXA_IDLE;
    pendingDeviceName = "";
}

void AlexaController::startSerialInput(AlexaSerialState newState) {
    currentState = newState;
}

void AlexaController::printWelcome() {
    Serial.println("\n🎤 ESP32 Alexa Controller - Solo URL Completi");
    Serial.println("===============================================");
    Serial.printf("🔧 Max dispositivi: %d\n", MAX_DEVICES);
    Serial.println("🌐 Ogni dispositivo richiede un URL completo");
}

void AlexaController::showMainMenu() {
    resetSerialState();
    
    bool wifiConnected = (systemWifiManager && systemWifiManager->isWiFiConnected()) || 
                        (WiFi.status() == WL_CONNECTED);
    
    Serial.println("\n╔═══════════════════════════════════════╗");
    Serial.println("║      🎤 ALEXA CONTROLLER MENU         ║");
    Serial.println("╠═══════════════════════════════════════╣");
    Serial.printf("║ WiFi: %-31s ║\n", wifiConnected ? "✅ Connesso" : "❌ Disconnesso");
    Serial.printf("║ Dispositivi: %2d/%-3d                  ║\n", deviceManager ? deviceManager->getDeviceCount() : 0, MAX_DEVICES);
    Serial.printf("║ Alexa: %-30s ║\n", isAlexaInitialized() ? "✅ Attivo" : "❌ Non attivo");
    Serial.println("╠═══════════════════════════════════════╣");
    Serial.println("║  a1. 📱 Mostra dispositivi Alexa      ║");
    Serial.println("║  a2. ➕ Aggiungi dispositivo (URL)    ║");
    Serial.println("║  a3. 🗑️  Rimuovi dispositivo          ║");
    Serial.println("║  a4. 📊 Status sistema                ║");
    Serial.println("║  a5. 🔄 Reset configurazione          ║");
    Serial.println("║  a6. 🎤 Riavvia Alexa                 ║");
    Serial.println("║  a0. 👋 Chiudi menu Alexa             ║");
    Serial.println("╚═══════════════════════════════════════╝");
    Serial.print("👉 Comando Alexa (a0-a6): ");
}

void AlexaController::handleMainCommand(const String& input) {
    if (!input.startsWith("a")) return;
    
    String cmd = input.substring(1);
    int choice = cmd.toInt();
    
    switch (choice) {
        case 1:
            if (deviceManager) deviceManager->printDevices();
            break;
        case 2:
            startAddDevice();
            return;
        case 3:
            startRemoveDevice();
            return;
        case 4:
            showSystemStatus();
            break;
        case 5:
            startResetConfiguration();
            return;
        case 6:
            restartAlexa();
            break;
        case 0:
            Serial.println("👋 Menu Alexa chiuso.");
            return;
        default:
            Serial.println("❌ Comando Alexa non riconosciuto!");
            break;
    }
    
    delay(MENU_RETURN_DELAY);
    showMainMenu();
}

void AlexaController::startAddDevice() {
    if (!deviceManager) {
        Serial.println("❌ DeviceManager non inizializzato");
        showMainMenu();
        return;
    }
    
    if (deviceManager->isFull()) {
        Serial.printf("❌ Limite massimo dispositivi raggiunto (%d)\n", MAX_DEVICES);
        delay(MENU_RETURN_DELAY);
        showMainMenu();
        return;
    }
    
    Serial.println("\n➕ Aggiunta nuovo dispositivo Alexa");
    Serial.println("Nome del dispositivo (0=annulla):");
    Serial.print("👉 ");
    pendingDeviceName = "";
    startSerialInput(ALEXA_WAITING_DEVICE_NAME);
}

void AlexaController::startRemoveDevice() {
    if (!deviceManager) {
        Serial.println("❌ DeviceManager non inizializzato");
        showMainMenu();
        return;
    }
    
    if (deviceManager->getDeviceCount() == 0) {
        Serial.println("❌ Nessun dispositivo da rimuovere");
        delay(MENU_RETURN_DELAY);
        showMainMenu();
        return;
    }
    
    Serial.println("\n🗑️ Dispositivi disponibili:");
    deviceManager->printDevices();
    Serial.print("👉 Nome dispositivo da rimuovere (0=annulla): ");
    
    startSerialInput(ALEXA_WAITING_DEVICE_REMOVE);
}

void AlexaController::handleDeviceRemove(const String& input) {
    String deviceName = input;
    deviceName.trim();
    
    if (deviceName == "0") {
        Serial.println("❌ Rimozione annullata");
        resetSerialState();
        showMainMenu();
        return;
    }
    
    if (deviceName.length() > 0 && deviceManager) {
        if (deviceManager->removeDevice(deviceName)) {
            Serial.println("✅ Dispositivo '" + deviceName + "' rimosso");
            if (((systemWifiManager && systemWifiManager->isWiFiConnected()) || WiFi.status() == WL_CONNECTED) && alexaManager) {
                alexaManager->restart();
            }
        } else {
            Serial.println("❌ Dispositivo non trovato");
        }
    }
    
    delay(MENU_RETURN_DELAY);
    showMainMenu();
}

void AlexaController::handleDeviceName(const String& input) {
    String name = input;
    name.trim();
    
    if (name == "0") {
        Serial.println("❌ Aggiunta annullata");
        resetSerialState();
        showMainMenu();
        return;
    }
    
    if (name.length() == 0) {
        Serial.println("❌ Nome vuoto! Riprova:");
        Serial.print("👉 ");
        return;
    }
    
    if (deviceManager && deviceManager->deviceExists(name)) {
        Serial.println("❌ Dispositivo esistente! Riprova:");
        Serial.print("👉 ");
        return;
    }
    
    pendingDeviceName = name;
    Serial.println("✅ Nome: " + name);
    Serial.println("\n🌐 URL completo del dispositivo:");
    Serial.println("   Esempio: http://192.168.1.100/toggle?pin=2");
    Serial.println("   Esempio: http://192.168.1.200:8080/api/light/on");
    Serial.print("👉 URL (0=annulla): ");
    startSerialInput(ALEXA_WAITING_DEVICE_URL);
}

void AlexaController::handleDeviceURL(const String& input) {
    if (input == "0") {
        Serial.println("❌ Aggiunta annullata");
        resetSerialState();
        showMainMenu();
        return;
    }
    
    String url = input;
    url.trim();
    
    if (url.length() == 0 || !url.startsWith("http")) {
        Serial.println("❌ URL non valido (deve iniziare con http)!");
        Serial.println("   Esempi validi:");
        Serial.println("   - http://192.168.1.100/toggle?pin=2");
        Serial.println("   - https://mioserver.com/api/device/toggle");
        Serial.print("👉 Riprova: ");
        return;
    }
    
    if (deviceManager && deviceManager->addDevice(pendingDeviceName, url)) {
        Serial.printf("✅ '%s' configurato!\n", pendingDeviceName.c_str());
        Serial.printf("   URL: %s\n", url.c_str());
        
        if (((systemWifiManager && systemWifiManager->isWiFiConnected()) || WiFi.status() == WL_CONNECTED) && alexaManager) {
            alexaManager->restart();
        }
    } else {
        Serial.println("❌ Errore aggiunta dispositivo");
    }
    
    resetSerialState();
    delay(MENU_RETURN_DELAY);
    showMainMenu();
}

void AlexaController::startResetConfiguration() {
    Serial.println("⚠️  RESET COMPLETO CONFIGURAZIONE ALEXA!");
    Serial.println("Verranno cancellati tutti i dispositivi.");
    Serial.println("Confermi? (y/N/0=annulla):");
    Serial.print("👉 ");
    startSerialInput(ALEXA_WAITING_RESET_CONFIRM);
}

void AlexaController::handleResetConfirm(const String& input) {
    String choice = input;
    choice.toLowerCase();
    
    if (choice == "y" || choice == "yes") {
        Serial.println("🔄 Reset in corso...");
        
        if (alexaManager) alexaManager->shutdown();
        preferences.clear();
        if (deviceManager) deviceManager->clear();
        
        alexaEnabled = false;
        
        Serial.println("✅ Reset completato!");
        
        resetSerialState();
        delay(MENU_RETURN_DELAY);
        showMainMenu();
    } else {
        Serial.println("❌ Reset annullato");
        resetSerialState();
        delay(MENU_RETURN_DELAY);
        showMainMenu();
    }
}

void AlexaController::showSystemStatus() {
    Serial.println("\n📊 Status Sistema Alexa:");
    Serial.println("========================");
    
    bool wifiConnected = (systemWifiManager && systemWifiManager->isWiFiConnected()) || 
                        (WiFi.status() == WL_CONNECTED);
    
    Serial.printf("🌐 WiFi: %s\n", wifiConnected ? "✅ Connesso" : "❌ Disconnesso");
    if (wifiConnected) {
        if (systemWifiManager) {
            Serial.printf("   SSID: %s\n", systemWifiManager->getSSID());
            Serial.printf("   IP: %s\n", systemWifiManager->getIP().c_str());
            Serial.printf("   Gestito da: Sistema principale\n");
        } else {
            Serial.printf("   SSID: %s\n", WiFi.SSID().c_str());
            Serial.printf("   IP: %s\n", WiFi.localIP().toString().c_str());
            Serial.printf("   Gestito da: WiFi diretto\n");
        }
    }
    
    Serial.printf("🎤 Alexa: %s\n", alexaEnabled ? "✅ Attivo" : "❌ Non attivo");
    if (alexaManager) {
        Serial.printf("   Dispositivi: %d\n", deviceManager ? deviceManager->getDeviceCount() : 0);
    }
    
    Serial.printf("💾 Memoria libera: %d KB\n", ESP.getFreeHeap() / 1024);
    Serial.printf("⏱️ Uptime: %lu secondi\n", millis() / 1000);
}

void AlexaController::restartAlexa() {
    bool wifiConnected = (systemWifiManager && systemWifiManager->isWiFiConnected()) || 
                        (WiFi.status() == WL_CONNECTED);
    
    if (wifiConnected && deviceManager && deviceManager->getDeviceCount() > 0 && alexaManager) {
        alexaManager->restart();
        alexaEnabled = true;
        Serial.println("✅ Alexa riavviato");
    } else {
        Serial.println("❌ WiFi non connesso o nessun dispositivo configurato");
    }
}

void AlexaController::printDeviceList() {
    if (deviceManager) {
        deviceManager->printDevices();
    } else {
        Serial.println("❌ DeviceManager non inizializzato");
    }
}

void AlexaController::clearAllDevices() {
    if (deviceManager) {
        deviceManager->clear();
        Serial.println("✅ Tutti i dispositivi rimossi");
    } else {
        Serial.println("❌ DeviceManager non inizializzato");
    }
}