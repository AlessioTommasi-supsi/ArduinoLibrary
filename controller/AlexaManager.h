#ifndef ALEXA_MANAGER_H
#define ALEXA_MANAGER_H

#include <fauxmoESP.h>
#include <HTTPClient.h>
#include "AlexaConfig.h"
#include "DeviceManager.h"

class AlexaManager {
private:
    fauxmoESP* fauxmo;
    DeviceManager* deviceManager;
    bool isInitialized;
    
    // 🔒 SAFETY: Variabili per protezione callback
    static volatile bool callbackSafe;
    static AlexaManager* safeInstance;
    
    // Callback functions con protezione
    static void onDeviceStateChanged(unsigned char device_id, const char* device_name, bool state, unsigned char value);
    
    // Metodi privati
    void addDevices();
    void saveGeneratedUUID(int deviceIndex, unsigned char fauxmoDeviceId);
    void handleDeviceCommand(const char* device_name, bool state);
    void printStatus();
    
    // 🛡️ SAFETY: Metodi di sicurezza
    void enableCallbacks();
    void disableCallbacks();
    bool isCallbackSafe() const;
    
public:
    AlexaManager(fauxmoESP* fauxmoInstance, DeviceManager* devManager);
    ~AlexaManager();
    
    // Inizializzazione Alexa
    bool initialize();
    void shutdown();
    bool restart();
    
    // Stato
    bool isAlexaInitialized() const { return isInitialized; }
    void handle();
};

#endif