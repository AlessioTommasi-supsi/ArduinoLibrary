#include "AlexaManager.h"
#include <WiFi.h>

// 🔒 SAFETY: Variabili statiche per protezione callback
volatile bool AlexaManager::callbackSafe = false;
AlexaManager* AlexaManager::safeInstance = nullptr;

AlexaManager::AlexaManager(fauxmoESP* fauxmoInstance, DeviceManager* devManager) 
    : fauxmo(fauxmoInstance), deviceManager(devManager), isInitialized(false) {
    // 🛡️ SAFETY: Registrazione sicura dell'istanza
    safeInstance = this;
    callbackSafe = false;
}

AlexaManager::~AlexaManager() {
    // 🛡️ SAFETY: Pulizia sicura
    disableCallbacks();
    shutdown();
    safeInstance = nullptr;
}

bool AlexaManager::initialize() {
    Serial.println("\n🎤 Inizializzazione Alexa...");
    
    // Verifica connessione WiFi PRIMA di inizializzare
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("❌ WiFi non connesso! Alexa non può funzionare.");
        return false;
    }
    
    Serial.printf("✅ WiFi connesso - IP: %s\n", WiFi.localIP().toString().c_str());
    Serial.printf("   SSID: %s\n", WiFi.SSID().c_str());
    Serial.printf("   Gateway: %s\n", WiFi.gatewayIP().toString().c_str());
    
    // Pulizia completa per evitare duplicati
    if (isInitialized) {
        shutdown();
    }
    
    // 🛡️ SAFETY: Disabilita callback durante inizializzazione
    disableCallbacks();
    
    Serial.printf("🔧 Configurazione FauxmoESP sulla porta %d...\n", FAUXMO_PORT);
    
    fauxmo->createServer(true);
    fauxmo->setPort(FAUXMO_PORT);
    fauxmo->enable(true);
    
    // Aggiungi dispositivi
    Serial.println("📱 Aggiunta dispositivi a FauxmoESP...");
    addDevices();
    
    // 🛡️ SAFETY: Imposta callback DOPO aver configurato tutto
    fauxmo->onSetState(onDeviceStateChanged);
    
    isInitialized = true;
    
    // 🛡️ SAFETY: Abilita callback SOLO alla fine
    enableCallbacks();
    
    printStatus();
    
    // 🔍 DEBUG: Verifica che FauxmoESP sia attivo
    Serial.println("🔍 Verifica stato FauxmoESP...");
    Serial.printf("   Porta: %d\n", FAUXMO_PORT);
    Serial.printf("   IP ESP32: %s\n", WiFi.localIP().toString().c_str());
    Serial.printf("   Dispositivi aggiunti: %d\n", deviceManager->getDeviceCount());
    
    return true;
}

void AlexaManager::shutdown() {
    if (isInitialized) {
        // 🛡️ SAFETY: Disabilita callback PRIMA dello shutdown
        disableCallbacks();
        
        Serial.println("🔄 Shutdown Alexa...");
        fauxmo->enable(false);
        delay(FAUXMO_DISABLE_DELAY);
        
        isInitialized = false;
    }
}

bool AlexaManager::restart() {
    Serial.println("🔄 Riavvio sistema Alexa...");
    shutdown();
    delay(ALEXA_RESTART_DELAY);
    return initialize();
}

// 🛡️ SAFETY: Metodi di protezione callback
void AlexaManager::enableCallbacks() {
    callbackSafe = true;
    Serial.println("🔒 Callback Alexa abilitati");
}

void AlexaManager::disableCallbacks() {
    callbackSafe = false;
    delay(50); // Aspetta che eventuali callback in corso terminino
    Serial.println("🔒 Callback Alexa disabilitati");
}

bool AlexaManager::isCallbackSafe() const {
    return callbackSafe && safeInstance != nullptr && isInitialized;
}

void AlexaManager::addDevices() {
    for (int i = 0; i < deviceManager->getDeviceCount(); i++) {
        const Device& device = deviceManager->getDevice(i);
        
        // Aggiungi dispositivo a FauxmoESP
        unsigned char deviceId = fauxmo->addDevice(device.name.c_str());
        Serial.printf("   ➕ %s", device.name.c_str());
        
        // 🎯 GESTIONE UUID PERSISTENTE
        if (device.uuid.length() > 0) {
            // UUID già salvato - lo ripristiniamo
            fauxmo->setDeviceUniqueId(deviceId, device.uuid.c_str());
            Serial.printf(" (UUID: %s)\n", device.uuid.substring(0, 8).c_str());
        } else {
            // Nessun UUID salvato - generiamo e salviamo quello nuovo
            saveGeneratedUUID(i, deviceId);
            Serial.println(" (UUID: Nuovo generato)");
        }
    }
}

void AlexaManager::saveGeneratedUUID(int deviceIndex, unsigned char fauxmoDeviceId) {
    // Generiamo l'UUID come fa FauxmoESP internamente
    String mac = WiFi.macAddress();
    char generatedUUID[28];  // FAUXMO_DEVICE_UNIQUE_ID_LENGTH = 27 + 1
    snprintf(generatedUUID, sizeof(generatedUUID), "%02X:%s:%s", fauxmoDeviceId, mac.c_str(), "00:00");
    
    // Salviamo l'UUID nel DeviceManager
    Device* device = deviceManager->findDevice(deviceManager->getDevice(deviceIndex).name);
    if (device) {
        device->uuid = String(generatedUUID);
        deviceManager->saveDevices();  // Salva subito in EEPROM
        Serial.printf("💾 UUID salvato: %s\n", generatedUUID);
    }
}

void AlexaManager::handleDeviceCommand(const char* device_name, bool state) {
    // 🛡️ SAFETY: Controlla se è sicuro procedere
    if (!isCallbackSafe()) {
        Serial.println("⚠️ Callback ignorato - sistema non sicuro");
        return;
    }
    
    // 🌐 URL COMPLETO: Usa il DeviceManager per eseguire l'azione
    Serial.printf("🎯 Comando Alexa: '%s' -> %s\n", device_name, state ? "ACCENDI" : "SPEGNI");
    
    bool success = deviceManager->executeDeviceAction(String(device_name), state);
    
    if (success) {
        Serial.printf("✅ Comando eseguito: %s\n", device_name);
    } else {
        Serial.printf("❌ Errore comando: %s\n", device_name);
    }
}

// 🛡️ SAFETY: Callback statico completamente protetto
void AlexaManager::onDeviceStateChanged(unsigned char device_id, const char* device_name, bool state, unsigned char value) {
    // 🔒 CONTROLLI DI SICUREZZA MULTIPLI
    if (!callbackSafe) {
        return; // Callback disabilitato
    }
    
    if (safeInstance == nullptr) {
        return; // Istanza non valida
    }
    
    if (!safeInstance->isInitialized) {
        return; // Sistema non inizializzato
    }
    
    // 🛡️ SAFETY: Controllo validità parametri
    if (device_name == nullptr || strlen(device_name) == 0) {
        return; // Nome dispositivo non valido
    }
    
    Serial.printf("🗣️ Alexa: '%s' -> %s\n", device_name, state ? "ON" : "OFF");
    
    // 🔒 SAFE: Chiamata protetta al metodo dell'istanza
    safeInstance->handleDeviceCommand(device_name, state);
}

void AlexaManager::handle() {
    if (isInitialized && isCallbackSafe()) {
        fauxmo->handle();
    }
}

void AlexaManager::printStatus() {
    Serial.println("\n🎉 Alexa Controller pronto!");
    Serial.printf("📱 %d dispositivi configurati\n", deviceManager->getDeviceCount());
    
    if (deviceManager->getDeviceCount() > 0) {
        Serial.println("📱 Comandi Alexa disponibili:");
        for (int i = 0; i < deviceManager->getDeviceCount() && i < 3; i++) {
            Serial.println("   - 'Alexa, accendi " + deviceManager->getDevice(i).name + "'");
            Serial.println("   - 'Alexa, spegni " + deviceManager->getDevice(i).name + "'");
        }
        if (deviceManager->getDeviceCount() > 3) {
            Serial.println("   - ... e altri dispositivi");
        }
        Serial.println("\n💡 'Alexa, scopri dispositivi' se necessario");
        Serial.println("🌐 Ogni dispositivo usa il proprio URL completo");
    }
}