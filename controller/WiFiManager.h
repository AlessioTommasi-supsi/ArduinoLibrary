#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <WiFi.h>
#include <vector>
#include <string>
#include "Config.h"
#include "CredentialsStorage.h"

class WebServer;

// Dichiarazione della variabile globale my_webServer
extern WebServer *my_webServer;

class WiFiManager
{
private:
    std::string ap_ssid;
    std::string ap_password;
    std::string sta_ssid;
    std::string sta_password;
    std::string sta_ip;
    std::string ap_ip;
    bool isSTAConnected;
    bool isFirstStart;

    void initializeDualMode();
    bool tryConnectSTA(const char *ssid, const char *password, int maxAttempts = SMOOTH_CONNECT_ATTEMPTS);

public:
    WiFiManager();
    WiFiManager(const char *ssid, const char *password);
    ~WiFiManager();
    
    void setupAP();
    void connect();
    bool smoothConnect();
    void clear_var();
    void setNetwork(const char *ssid, const char *password);
    
    std::vector<std::string> scanNetworks();
    
    // Status methods
    const char *getSSID() const { return sta_ssid.empty() ? ap_ssid.c_str() : sta_ssid.c_str(); }
    const char *getAPSSID() const { return ap_ssid.c_str(); }
    const char *getSTASSID() const { return sta_ssid.c_str(); }
    std::string getIP() const { return isSTAConnected ? sta_ip : ap_ip; }
    std::string getAPIP() const { return ap_ip; }
    std::string getSTAIP() const { return sta_ip; }
    bool getIsAP() const { return true; } // AP è SEMPRE attivo in Dual Mode
    bool isWiFiConnected() const { return isSTAConnected && WiFi.status() == WL_CONNECTED; }
};

#endif // WIFIMANAGER_H