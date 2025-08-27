#include "AlexaController.h"

// ===== COSTANTI =====
#define PREFERENCES_NAMESPACE "alexa_ctrl"
#define MAX_INPUT_LENGTH 100
#define MENU_RETURN_DELAY 2000

AlexaController::AlexaController() : currentState(ALEXA_IDLE), alexaEnabled(false), initialized(false) {
    deviceManager = nullptr;
    udpServer = nullptr;
    templateEngine = nullptr;
    systemWifiManager = nullptr;
    externalServer = nullptr;
}

AlexaController::~AlexaController() {
    if (udpServer) delete udpServer;
    if (templateEngine) delete templateEngine;
    if (deviceManager) delete deviceManager;
}

bool AlexaController::initialize(WiFiManager* wifiManager, AsyncWebServer* server) {
    Serial.println("\n🚀 Inizializzazione Alexa Controller Modulare...");
    
    systemWifiManager = wifiManager;
    externalServer = server;
    
    // Inizializza Preferences
    preferences.begin(PREFERENCES_NAMESPACE, false);
    
    // Crea le istanze dei manager modulari
    deviceManager = new DeviceManager(&preferences);
    udpServer = new AlexaUDP();
    templateEngine = new AlexaTemplate();
    
    // Carica i dispositivi salvati
    deviceManager->loadDevices();
    
    // Setup handler HTTP se abbiamo un server esterno
    if (externalServer) {
        setupAlexaHTTPHandlers();
        Serial.println("✅ Handler HTTP Alexa configurati su server condiviso");
    }
    
    initialized = true;
    printWelcome();
    
    // Verifica connessione WiFi e avvia servizi
    if (systemWifiManager && systemWifiManager->isWiFiConnected()) {
        Serial.println("✅ WiFi già connesso dal sistema principale");
        if (deviceManager->getDeviceCount() > 0) {
            enableAlexa(true);
        }
        showMainMenu();
    } else if (WiFi.status() == WL_CONNECTED) {
        Serial.println("✅ WiFi connesso (gestione diretta)");
        if (deviceManager->getDeviceCount() > 0) {
            enableAlexa(true);
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
    
    // Gestione UDP Discovery
    if (alexaEnabled && udpServer) {
        udpServer->handle();
    }
    
    // Gestione input seriale
    handleSerialInput();
}

void AlexaController::enableAlexa(bool enable) {
    alexaEnabled = enable;
    
    bool wifiConnected = (systemWifiManager && systemWifiManager->isWiFiConnected()) || 
                        (WiFi.status() == WL_CONNECTED);
    
    if (enable && wifiConnected && deviceManager && deviceManager->getDeviceCount() > 0) {
        if (udpServer && udpServer->begin()) {
            Serial.println("✅ Alexa Discovery attivato");
        } else {
            Serial.println("❌ Errore attivazione Alexa Discovery");
            alexaEnabled = false;
        }
    } else if (!enable && udpServer) {
        udpServer->stop();
        Serial.println("🛑 Alexa Discovery fermato");
    }
}

bool AlexaController::isAlexaInitialized() const {
    return alexaEnabled && udpServer && udpServer->isActive();
}

bool AlexaController::addDeviceFromSystem(const String& name, const String& url) {
    if (!deviceManager) return false;
    
    if (deviceManager->addDevice(name, url)) {
        if (alexaEnabled && udpServer && udpServer->isActive()) {
            Serial.println("🔄 Dispositivo aggiunto - Alexa già attivo");
        }
        return true;
    }
    return false;
}

void AlexaController::setupAlexaHTTPHandlers() {
    if (!externalServer || !templateEngine) return;
    
    Serial.println("🔗 Configurazione handler HTTP Alexa...");
    
    // Handler per description.xml
    externalServer->on("/description.xml", HTTP_GET, [this](AsyncWebServerRequest *request){
        this->handleDescriptionXML(request);
    });
    
    // Handler per API Alexa - GET
    externalServer->on("/api/*", HTTP_GET, [this](AsyncWebServerRequest *request){
        this->handleAlexaAPI(request, true);
    });
    
    // Handler per API Alexa - POST/PUT
    externalServer->on("/api/*", HTTP_POST, [this](AsyncWebServerRequest *request){
        this->handleAlexaAPI(request, false);
    }, NULL, [this](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
        // ➕ CORREZIONE: Gestione body senza usare metodi privati
        if (index == 0) {
            request->_tempObject = malloc(total + 1);
            if (request->_tempObject) {
                ((char*)request->_tempObject)[total] = '\0';
            }
        }
        if (request->_tempObject && index + len <= total) {
            memcpy(((char*)request->_tempObject) + index, data, len);
        }
    });
    
    externalServer->on("/api/*", HTTP_PUT, [this](AsyncWebServerRequest *request){
        this->handleAlexaAPI(request, false);
    }, NULL, [this](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
        // ➕ CORREZIONE: Gestione body senza usare metodi privati
        if (index == 0) {
            request->_tempObject = malloc(total + 1);
            if (request->_tempObject) {
                ((char*)request->_tempObject)[total] = '\0';
            }
        }
        if (request->_tempObject && index + len <= total) {
            memcpy(((char*)request->_tempObject) + index, data, len);
        }
    });
}

void AlexaController::handleDescriptionXML(AsyncWebServerRequest* request) {
    Serial.println("📋 Richiesta description.xml ricevuta");
    
    if (!templateEngine) {
        request->send(500, "text/plain", "Template engine non disponibile");
        return;
    }
    
    String xml = templateEngine->generateDescriptionXML();
    request->send(200, "text/xml", xml);
}

void AlexaController::handleAlexaAPI(AsyncWebServerRequest* request, bool isGet) {
    String url = request->url();
    String body = "";
    
    // ➕ CORREZIONE: Leggi il body da _tempObject se presente
    if (!isGet && request->_tempObject) {
        body = String((char*)request->_tempObject);
        free(request->_tempObject);
        request->_tempObject = nullptr;
    }
    
    Serial.printf("🔍 API Alexa: %s %s\n", isGet ? "GET" : (request->method() == HTTP_PUT ? "PUT" : "POST"), url.c_str());
    if (body.length() > 0) {
        Serial.printf("📦 Body: %s\n", body.c_str());
    }
    
    // Gestione autenticazione
    if (body.indexOf("devicetype") >= 0) {
        Serial.println("🔐 Richiesta autorizzazione Alexa");
        String response = templateEngine->generateAuthSuccessJSON();
        request->send(200, "application/json", response);
        return;
    }
    
    // Gestione elenco dispositivi
    if (url.indexOf("/lights") >= 0 && isGet) {
        handleLightsRequest(request, url);
        return;
    }
    
    // Gestione comandi controllo
    if (url.indexOf("/lights/") >= 0 && url.indexOf("/state") >= 0 && !isGet) {
        handleLightControl(request, url, body);
        return;
    }
    
    // Default response
    request->send(404, "application/json", "{}");
}

void AlexaController::handleLightsRequest(AsyncWebServerRequest* request, const String& url) {
    Serial.println("📋 Richiesta elenco dispositivi");
    
    if (!templateEngine || !deviceManager) {
        request->send(500, "application/json", "{}");
        return;
    }
    
    // Verifica se è richiesta di un singolo dispositivo
    int lightsPos = url.indexOf("/lights");
    String devicePart = url.substring(lightsPos + 7);  // Dopo "/lights"
    
    if (devicePart.length() == 0 || devicePart == "/") {
        // Lista completa dispositivi
        String json = templateEngine->generateDeviceListJSON(deviceManager);
        request->send(200, "application/json", json);
    } else {
        // Singolo dispositivo
        int deviceId = devicePart.substring(1).toInt() - 1;  // Rimuovi "/" e converti a zero-based
        
        if (deviceId >= 0 && deviceId < deviceManager->getDeviceCount()) {
            const Device& device = deviceManager->getDevice(deviceId);
            String json = templateEngine->generateSingleDeviceJSON(device);
            request->send(200, "application/json", json);
        } else {
            String error = templateEngine->generateErrorResponse(3, "/lights/" + String(deviceId + 1), "resource not available");
            request->send(404, "application/json", error);
        }
    }
}

void AlexaController::handleLightControl(AsyncWebServerRequest* request, const String& url, const String& body) {
    Serial.println("🎛️ Comando controllo dispositivo");
    
    if (!templateEngine || !deviceManager) {
        request->send(500, "application/json", "{}");
        return;
    }
    
    // Estrai ID dispositivo dall'URL
    int lightsPos = url.indexOf("/lights/");
    int statePos = url.indexOf("/state");
    String deviceIdStr = url.substring(lightsPos + 8, statePos);
    int deviceId = deviceIdStr.toInt() - 1;  // Converti a zero-based
    
    if (deviceId < 0 || deviceId >= deviceManager->getDeviceCount()) {
        String error = templateEngine->generateErrorResponse(3, "/lights/" + deviceIdStr, "resource not available");
        request->send(404, "application/json", error);
        return;
    }
    
    const Device& device = deviceManager->getDevice(deviceId);
    
    // Determina comando (on/off)
    bool turnOn = body.indexOf("\"on\":true") >= 0 || body.indexOf("bri") >= 0;
    
    Serial.printf("🎯 Controllo '%s': %s\n", device.name.c_str(), turnOn ? "ON" : "OFF");
    
    // Esegui comando HTTP
    if (deviceManager->sendHTTPRequest(device.url)) {
        String response = templateEngine->generateSuccessResponse(deviceId + 1, turnOn);
        request->send(200, "application/json", response);
    } else {
        String error = templateEngine->generateErrorResponse(1, "/lights/" + String(deviceId + 1) + "/state", "resource not available");
        request->send(500, "application/json", error);
    }
}

void AlexaController::printWelcome() {
    Serial.println("\n🎤 ESP32 Alexa Controller Modulare - Implementazione Pulita");
    Serial.println("============================================================");
    Serial.printf("🔧 Max dispositivi: %d\n", MAX_DEVICES);
    Serial.println("🌐 Architettura: UDP Discovery + HTTP API + Template Engine");
}

void AlexaController::showMainMenu() {
    resetSerialState();
    
    bool wifiConnected = (systemWifiManager && systemWifiManager->isWiFiConnected()) || 
                        (WiFi.status() == WL_CONNECTED);
    
    Serial.println("\n╔═══════════════════════════════════════╗");
    Serial.println("║      🎤 ALEXA CONTROLLER MODULARE     ║");
    Serial.println("╠═══════════════════════════════════════╣");
    Serial.printf("║ WiFi: %-31s ║\n", wifiConnected ? "✅ Connesso" : "❌ Disconnesso");
    Serial.printf("║ Dispositivi: %2d/%-3d                  ║\n", deviceManager ? deviceManager->getDeviceCount() : 0, MAX_DEVICES);
    Serial.printf("║ UDP Discovery: %-22s ║\n", (udpServer && udpServer->isActive()) ? "✅ Attivo" : "❌ Non attivo");
    Serial.printf("║ HTTP Handlers: %-22s ║\n", externalServer ? "✅ Configurati" : "❌ Non configurati");
    Serial.println("╠═══════════════════════════════════════╣");
    Serial.println("║  a1. 📱 Mostra dispositivi            ║");
    Serial.println("║  a2. ➕ Aggiungi dispositivo          ║");
    Serial.println("║  a3. 🗑️  Rimuovi dispositivo          ║");
    Serial.println("║  a4. 📊 Status sistema modulare       ║");
    Serial.println("║  a5. 🔄 Reset configurazione          ║");
    Serial.println("║  a6. 🎤 Riavvia Alexa Discovery       ║");
    Serial.println("║  a7. 📄 Info Template Engine          ║");
    Serial.println("║  a8. 🌐 Status UDP Discovery          ║");
    Serial.println("║  a0. 👋 Chiudi menu Alexa             ║");
    Serial.println("╚═══════════════════════════════════════╝");
    Serial.print("👉 Comando Alexa (a0-a8): ");
}

// ===== METODI GESTIONE INPUT SERIALE =====

void AlexaController::handleSerialInput() {
    if (Serial.available()) {
        char c = Serial.read();
        
        if (c == '\n' || c == '\r') {
            if (inputBuffer.length() > 0) {
                processInput(inputBuffer);
                inputBuffer = "";
            }
        } else if (c == '\b' && inputBuffer.length() > 0) {
            inputBuffer.remove(inputBuffer.length() - 1);
            Serial.print("\b \b");
        } else if (c >= 32 && c <= 126 && inputBuffer.length() < MAX_INPUT_LENGTH) {
            inputBuffer += c;
            Serial.print(c);
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
    inputBuffer = "";
    pendingDeviceName = "";
}

void AlexaController::startSerialInput(AlexaSerialState newState) {
    currentState = newState;
    inputBuffer = "";
}

void AlexaController::handleMainCommand(const String& input) {
    Serial.println();
    
    if (input == "a0") {
        Serial.println("👋 Menu Alexa chiuso");
        return;
    } else if (input == "a1") {
        printDeviceList();
    } else if (input == "a2") {
        startAddDevice();
        return;
    } else if (input == "a3") {
        startRemoveDevice();
        return;
    } else if (input == "a4") {
        showSystemStatus();
    } else if (input == "a5") {
        startResetConfiguration();
        return;
    } else if (input == "a6") {
        restartAlexa();
    } else if (input == "a7") {
        if (templateEngine) {
            templateEngine->printTemplateInfo();
        } else {
            Serial.println("❌ Template Engine non inizializzato");
        }
    } else if (input == "a8") {
        if (udpServer) {
            udpServer->printStatus();
        } else {
            Serial.println("❌ UDP Server non inizializzato");
        }
    } else {
        Serial.println("❌ Comando non riconosciuto");
    }
    
    delay(MENU_RETURN_DELAY);
    showMainMenu();
}

void AlexaController::printDeviceList() {
    if (!deviceManager) {
        Serial.println("❌ DeviceManager non inizializzato");
        return;
    }
    
    Serial.println("\n📱 Dispositivi Alexa Configurati:");
    Serial.println("=================================");
    
    if (deviceManager->getDeviceCount() == 0) {
        Serial.println("📭 Nessun dispositivo configurato");
        Serial.println("💡 Usa comando 'a2' per aggiungere un dispositivo");
    } else {
        for (int i = 0; i < deviceManager->getDeviceCount(); i++) {
            const Device& device = deviceManager->getDevice(i);
            Serial.printf("%d. 🏠 %s\n", i + 1, device.name.c_str());
            Serial.printf("   🔗 %s\n", device.url.c_str());
            Serial.printf("   🆔 %s\n", device.uuid.c_str());
            Serial.println();
        }
    }
}

void AlexaController::startAddDevice() {
    if (!deviceManager) {
        Serial.println("❌ DeviceManager non inizializzato");
        delay(MENU_RETURN_DELAY);
        showMainMenu();
        return;
    }
    
    if (deviceManager->getDeviceCount() >= MAX_DEVICES) {
        Serial.printf("❌ Massimo %d dispositivi raggiunto\n", MAX_DEVICES);
        delay(MENU_RETURN_DELAY);
        showMainMenu();
        return;
    }
    
    Serial.println("\n➕ Aggiunta Nuovo Dispositivo");
    Serial.println("==============================");
    Serial.println("Esempi di nomi validi:");
    Serial.println("  - Luce Soggiorno");
    Serial.println("  - Ventilatore Camera");
    Serial.println("  - Presa Cucina");
    Serial.print("📝 Nome dispositivo: ");
    
    startSerialInput(ALEXA_WAITING_DEVICE_NAME);
}

void AlexaController::handleDeviceName(const String& input) {
    if (input.length() == 0) {
        Serial.println("\n❌ Nome non può essere vuoto");
        delay(MENU_RETURN_DELAY);
        showMainMenu();
        return;
    }
    
    pendingDeviceName = input;
    Serial.println("\n📡 URL di controllo del dispositivo");
    Serial.println("Esempi di URL validi:");
    Serial.println("  - http://192.168.1.100/toggle");
    Serial.println("  - http://192.168.1.50/relay/1/on");
    Serial.println("  - http://homeassistant.local/api/light/toggle");
    Serial.print("🔗 URL: ");
    
    startSerialInput(ALEXA_WAITING_DEVICE_URL);
}

void AlexaController::handleDeviceURL(const String& input) {
    if (input.length() == 0 || (!input.startsWith("http://") && !input.startsWith("https://"))) {
        Serial.println("\n❌ URL non valido (deve iniziare con http:// o https://)");
        delay(MENU_RETURN_DELAY);
        showMainMenu();
        return;
    }
    
    if (deviceManager && deviceManager->addDevice(pendingDeviceName, input)) {
        Serial.printf("\n✅ Dispositivo '%s' aggiunto con successo!\n", pendingDeviceName.c_str());
        
        // Riavvia Alexa se già attivo per includere il nuovo dispositivo
        if (alexaEnabled && udpServer && udpServer->isActive()) {
            Serial.println("🔄 Riavvio Discovery per includere nuovo dispositivo...");
            enableAlexa(false);
            delay(1000);
            enableAlexa(true);
        }
    } else {
        Serial.println("\n❌ Errore aggiunta dispositivo");
    }
    
    delay(MENU_RETURN_DELAY);
    showMainMenu();
}

void AlexaController::startRemoveDevice() {
    if (!deviceManager || deviceManager->getDeviceCount() == 0) {
        Serial.println("❌ Nessun dispositivo da rimuovere");
        delay(MENU_RETURN_DELAY);
        showMainMenu();
        return;
    }
    
    Serial.println("\n🗑️ Rimozione Dispositivo");
    Serial.println("========================");
    printDeviceList();
    Serial.print("🔢 Numero dispositivo da rimuovere (0 per annullare): ");
    
    startSerialInput(ALEXA_WAITING_DEVICE_REMOVE);
}

void AlexaController::handleDeviceRemove(const String& input) {
    int deviceIndex = input.toInt();
    
    if (deviceIndex == 0) {
        Serial.println("\n❌ Rimozione annullata");
    } else if (deviceManager && deviceIndex > 0 && deviceIndex <= deviceManager->getDeviceCount()) {
        const Device& device = deviceManager->getDevice(deviceIndex - 1);
        String deviceName = device.name;
        
        // ➕ CORREZIONE: removeDevice prende il nome, non l'indice
        if (deviceManager->removeDevice(deviceName)) {
            Serial.printf("\n✅ Dispositivo '%s' rimosso con successo\n", deviceName.c_str());
        } else {
            Serial.println("\n❌ Errore rimozione dispositivo");
        }
    } else {
        Serial.println("\n❌ Numero dispositivo non valido");
    }
    
    delay(MENU_RETURN_DELAY);
    showMainMenu();
}

void AlexaController::startResetConfiguration() {
    Serial.println("\n🔄 Reset Configurazione Alexa");
    Serial.println("==============================");
    Serial.println("⚠️ ATTENZIONE: Questa operazione rimuoverà TUTTI i dispositivi configurati!");
    Serial.print("❓ Confermi il reset? (si/no): ");
    
    startSerialInput(ALEXA_WAITING_RESET_CONFIRM);
}

void AlexaController::handleResetConfirm(const String& input) {
    if (input == "si" || input == "SI" || input == "yes" || input == "YES" || input == "y" || input == "Y") {
        clearAllDevices();
        Serial.println("\n✅ Configurazione Alexa resettata completamente");
        Serial.println("💡 Aggiungi nuovi dispositivi con comando 'a2'");
    } else {
        Serial.println("\n❌ Reset annullato");
    }
    
    delay(MENU_RETURN_DELAY);
    showMainMenu();
}

void AlexaController::showSystemStatus() {
    Serial.println("\n📊 Status Sistema Alexa Modulare");
    Serial.println("================================");
    
    bool wifiConnected = (systemWifiManager && systemWifiManager->isWiFiConnected()) || 
                        (WiFi.status() == WL_CONNECTED);
    
    Serial.printf("🌐 WiFi: %s\n", wifiConnected ? "✅ Connesso" : "❌ Disconnesso");
    if (wifiConnected) {
        Serial.printf("📍 IP: %s\n", WiFi.localIP().toString().c_str());
        Serial.printf("🔧 MAC: %s\n", WiFi.macAddress().c_str());
    }
    
    Serial.printf("🎤 Alexa Controller: %s\n", initialized ? "✅ Inizializzato" : "❌ Non inizializzato");
    Serial.printf("📱 Dispositivi: %d/%d\n", deviceManager ? deviceManager->getDeviceCount() : 0, MAX_DEVICES);
    Serial.printf("🌐 UDP Discovery: %s\n", (udpServer && udpServer->isActive()) ? "✅ Attivo" : "❌ Non attivo");
    Serial.printf("🌐 HTTP Handlers: %s\n", externalServer ? "✅ Configurati" : "❌ Non configurati");
    Serial.printf("📄 Template Engine: %s\n", templateEngine ? "✅ Attivo" : "❌ Non attivo");
    
    Serial.printf("💾 Memoria libera: %d bytes\n", ESP.getFreeHeap());
    Serial.printf("⏱️ Uptime: %lu secondi\n", millis() / 1000);
}

void AlexaController::restartAlexa() {
    Serial.println("\n🔄 Riavvio Alexa Discovery...");
    
    enableAlexa(false);
    delay(2000);
    
    if (deviceManager && deviceManager->getDeviceCount() > 0) {
        enableAlexa(true);
        Serial.println("✅ Alexa Discovery riavviato");
    } else {
        Serial.println("⚠️ Nessun dispositivo configurato - Discovery non avviato");
    }
}

void AlexaController::clearAllDevices() {
    if (deviceManager) {
        enableAlexa(false);  // Ferma Alexa prima di pulire
        // ➕ CORREZIONE: Usa clear() invece di clearAllDevices()
        deviceManager->clear();
    }
}