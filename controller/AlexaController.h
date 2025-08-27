#ifndef ALEXA_CONTROLLER_H
#define ALEXA_CONTROLLER_H

#include <Arduino.h>
#include <Preferences.h>
// ➕ RIMOSSE DIPENDENZE FAUXMO - IMPLEMENTAZIONE PULITA
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebSrv.h>

#include "AlexaConfig.h"
#include "DeviceManager.h"
#include "AlexaUDP.h"        // ➕ Nuovo server UDP modulare
#include "AlexaTemplate.h"   // ➕ Template XML/JSON separati
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
    // ➕ IMPLEMENTAZIONE PULITA - NO FAUXMO
    Preferences preferences;
    DeviceManager* deviceManager;
    AlexaUDP* udpServer;           // ➕ Server UDP modulare
    AlexaTemplate* templateEngine; // ➕ Template engine separato
    WiFiManager* systemWifiManager;
    AsyncWebServer* externalServer;
    
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
    
    // ➕ Metodi per gestione server HTTP integrato
    void setupAlexaHTTPHandlers();
    void handleDescriptionXML(AsyncWebServerRequest* request);
    void handleAlexaAPI(AsyncWebServerRequest* request, bool isGet);
    void handleLightsRequest(AsyncWebServerRequest* request, const String& url);
    void handleLightControl(AsyncWebServerRequest* request, const String& url, const String& body);
    
public:
    AlexaController();
    ~AlexaController();
    
    // Metodi pubblici principali
    bool initialize(WiFiManager* wifiManager = nullptr, AsyncWebServer* server = nullptr);
    void handle();
    void enableAlexa(bool enable);
    bool isAlexaEnabled() const { return alexaEnabled; }
    bool isAlexaInitialized() const;
    
    // Metodi per aggiungere dispositivi dal sistema
    bool addDeviceFromSystem(const String& name, const String& url);
    DeviceManager* getDeviceManager() { return deviceManager; }
    
    // Metodi di utilità
    void printDeviceList();
    void clearAllDevices();
};

#endif