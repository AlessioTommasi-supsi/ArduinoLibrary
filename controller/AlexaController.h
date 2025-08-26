#ifndef ALEXA_CONTROLLER_H
#define ALEXA_CONTROLLER_H

#include <Arduino.h>
#include <Preferences.h>
#include <fauxmoESP.h>
#include "AlexaConfig.h"
#include "DeviceManager.h"
#include "AlexaManager.h"
#include "WiFiManager.h"

enum AlexaSerialState {
    ALEXA_IDLE,
    ALEXA_WAITING_DEVICE_NAME,
    ALEXA_WAITING_DEVICE_URL,
    ALEXA_WAITING_RESET_CONFIRM,
    ALEXA_WAITING_DEVICE_REMOVE
};

class AlexaController {
private:
    // Istanze delle classi
    Preferences preferences;
    fauxmoESP fauxmo;
    DeviceManager* deviceManager;
    AlexaManager* alexaManager;
    WiFiManager* systemWifiManager;
    
    // Gestione input seriale
    AlexaSerialState currentState;
    String inputBuffer;
    String pendingDeviceName;
    bool alexaEnabled;
    bool initialized;
    
    // Metodi privati per gestione seriale
    void handleSerialInput();
    void processInput(String input);
    void resetSerialState();
    void startSerialInput(AlexaSerialState newState);
    
    // Menu e comandi
    void showMainMenu();
    void handleMainCommand(const String& input);
    void startAddDevice();
    void startRemoveDevice();
    void handleDeviceRemove(const String& input);
    void handleDeviceName(const String& input);
    void handleDeviceURL(const String& input);
    void startResetConfiguration();
    void handleResetConfirm(const String& input);
    void showSystemStatus();
    void restartAlexa();
    
    // Utilità
    void printWelcome();
    
public:
    AlexaController();
    ~AlexaController();
    
    // Metodi pubblici principali
    bool initialize(WiFiManager* wifiManager = nullptr);
    void handle();
    void enableAlexa(bool enable);
    bool isAlexaEnabled() const { return alexaEnabled; }
    bool isAlexaInitialized() const;
    
    // Metodi per aggiungere dispositivi dal sistema - SOLO URL
    bool addDeviceFromSystem(const String& name, const String& url);
    DeviceManager* getDeviceManager() { return deviceManager; }
    
    // Metodi di utilità
    void printDeviceList();
    void clearAllDevices();
};

#endif