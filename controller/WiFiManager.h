#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <WiFi.h>
#include <vector>
#include <string>
#include "Config.h"

class WebServer;

// Dichiarazione della variabile globale my_webServer
extern WebServer *my_webServer;

class WiFiManager
{
private:
    const char *ssid;
    const char *password;
    std::string ip_address;
    bool isAP;
    bool isFirstStart = true;

public:
    WiFiManager();
    WiFiManager(const char *ssid, const char *password);
    ~WiFiManager();
    
    void setupAP();
    void connect();
    void smoothConnect();
    void clear_var();
    void setNetwork(const char *ssid, const char *password);
    
    std::vector<std::string> scanNetworks();
    
    // Status methods
    const char *getSSID() const { return ssid; }
    std::string getIP() const { return ip_address; }
    bool getIsAP() const { return isAP; }
    bool isWiFiConnected() const { return !isAP && WiFi.status() == WL_CONNECTED; }
};

#endif // WIFIMANAGER_H