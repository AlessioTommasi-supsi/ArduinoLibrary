#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <WiFi.h>
#include <vector>
#include <string>
#include <Preferences.h>
#include "Config.h"

class WebServer;

// ➕ Dichiarazione della variabile globale my_webServer
extern WebServer *my_webServer;

class WiFiManager
{
private:
    const char *ssid;
    const char *password;
    std::string ip_address;
    bool isConnected;
    bool isAP;
    bool isFirstStart = true;
    
    // ➕ NUOVA OPZIONE: AP sempre attivo
    bool alwaysEnableAP;
    std::string ap_ip_address;  // IP separato per l'AP
    
    // Variabili per riconnessione automatica
    bool autoReconnectEnabled;
    unsigned long lastConnectionCheck;
    unsigned long lastReconnectAttempt;
    
    // Gestione persistenza credenziali
    Preferences preferences;
    static const char* PREF_NAMESPACE;
    static const char* PREF_SSID_KEY;
    static const char* PREF_PASSWORD_KEY;
    static const char* PREF_SAVED_KEY;

    void saveCredentials(const char* ssid, const char* password);
    bool loadSavedCredentials();
    void clearSavedCredentials();

public:
    // ➕ NUOVO COSTRUTTORE con opzione AP sempre attivo
    WiFiManager(bool enableAPAlways = true);
    WiFiManager(const char *ssid, const char *password, bool enableAPAlways = true);
    ~WiFiManager();
    
    void setupAP();
    void connect();
    void smoothConnect();
    void clear_var();
    void setNetwork(const char *ssid, const char *password);
    
    // Metodi per riconnessione automatica
    void checkConnection();
    bool autoReconnect();
    void enableAutoReconnect(bool enable);
    bool isWiFiConnected() const;
    void updateConnectionStatus();
    
    // Metodi per persistenza
    bool tryConnectWithSavedCredentials();
    void saveCurrentCredentials();
    bool hasSavedCredentials();
    
    std::vector<std::string> scanNetworks();
    
    // ➕ NUOVI METODI per gestione dual mode
    void enableDualMode(bool enable = true);
    void setupDualMode();
    std::string getAPIP() const { return ap_ip_address; }
    bool isDualModeActive() const { return isConnected && isAP; }
    
    const char *getSSID() const { return ssid; }
    std::string getIP() const { return ip_address; }
    bool getIsConnected() const { return isConnected; }
    bool getIsAP() const { return isAP; }
};

#endif // WIFIMANAGER_H