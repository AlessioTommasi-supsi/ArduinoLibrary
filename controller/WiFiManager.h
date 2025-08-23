#ifndef WiFiManager_h
#define WiFiManager_h

#include <Arduino.h>
#include <WiFi.h>
#include "Config.h"
#include <string>
#include <vector>

// Forward declaration per evitare dipendenze circolari
class WebServer;

class WiFiManager
{

public:
    const char *ssid;
    const char *password;
    std::string ip_address="none";
    bool isFirstStart = true;
    bool isAP = true;
    
    // Variabili per riconnessione automatica
    unsigned long lastConnectionCheck = 0;
    unsigned long lastReconnectAttempt = 0;
    bool isConnected = false;
    bool autoReconnectEnabled = true;

    WebServer *my_webServer;

    WiFiManager(const char *ssid, const char *password);
    WiFiManager();
    ~WiFiManager(); // Distruttore
    void connect();
    void smoothConnect();
    void setupAP();
    void clear_var();
    
    // Nuove funzioni per riconnessione automatica
    void checkConnection();
    bool autoReconnect();
    void enableAutoReconnect(bool enable = true);
    bool isWiFiConnected() const;
    void updateConnectionStatus();
    
    void setNetwork(const char *ssid, const char *password);

    std::vector<std::string> scanNetworks();
};

#endif