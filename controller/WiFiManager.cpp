#include "WiFiManager.h"
#include "WebServer.h"

WebServer *my_webServer = nullptr;

WiFiManager::WiFiManager()
{
    /*qui creo Esp32 funziona come AP!*/
    this->ssid = DEFAULT_AP_SSID;
    this->password = DEFAULT_AP_PASSWORD;
    this->setupAP();
    isAP = true;
    isConnected = false;
    autoReconnectEnabled = true;
    lastConnectionCheck = 0;
    lastReconnectAttempt = 0;
}

void WiFiManager::setupAP()
{
    try
    {
        clear_var();
       
        WiFi.mode(WIFI_AP);
        delay(100);
        WiFi.softAP(ssid, password);
        IPAddress IP = WiFi.softAPIP();
        Serial.print("ESP32 AP IP address: ");
        Serial.println(IP);

        this->ip_address = IP.toString().c_str();

        my_webServer = new WebServer(ssid, password);
    }
    catch(...)
    {
        Serial.println("Errore durante la creazione dell'access point!");
    }
    isAP = true;
    isConnected = false;
}

void WiFiManager::clear_var()
{
    if (my_webServer != nullptr)
    {
        try
        {
            if (!isFirstStart)
            {
                delete my_webServer;
            }
        }
        catch (const std::exception &e)
        {
            Serial.println("Errore durante la distruzione del web server!");
        }
        my_webServer = nullptr;
    }
    WiFi.disconnect();
    isConnected = false;
}

WiFiManager::WiFiManager(const char *ssid, const char *password)
{
    this->ssid = ssid;
    this->password = password;
    this->isConnected = false;
    this->autoReconnectEnabled = true;
    this->lastConnectionCheck = 0;
    this->lastReconnectAttempt = 0;
    this->connect();
}

void WiFiManager::connect()
{
    try
    {
        clear_var();
        WiFi.mode(WIFI_STA);
        
        delay(100);

        Serial.println("Connessione alla rete Wi-Fi...");

        WiFi.begin(ssid, password);

        int attempts = 0;
        while (WiFi.status() != WL_CONNECTED && attempts < WIFI_CONNECT_TIMEOUT)
        {
            delay(CONNECTION_RETRY_DELAY);
            Serial.println("Connessione in corso...");
            attempts++;
        }

        if (WiFi.status() == WL_CONNECTED)
        {
            my_webServer = new WebServer(ssid, password);

            Serial.println("Connessione Wi-Fi stabilita!");
            Serial.print("Indirizzo IP: ");
            Serial.println(WiFi.localIP());
            this->ip_address = std::string(WiFi.localIP().toString().c_str());
            this->isConnected = true;
            this->isAP = false;
        }
        else
        {
            throw std::runtime_error("Timeout connessione WiFi");
        }
    }
    catch(...)
    {
        Serial.println("Errore durante la connessione alla rete Wi-Fi!");
        this->ssid = DEFAULT_AP_SSID;
        this->password = DEFAULT_AP_PASSWORD;
        this->setupAP();
        isAP = true;
        isConnected = false;
    }
}

void WiFiManager::smoothConnect()
{
    try
    {
        delay(100);

        Serial.println("Connessione alla rete Wi-Fi...");

        WiFi.begin(ssid, password);

        int attempts = 0;
        while (WiFi.status() != WL_CONNECTED && attempts < SMOOTH_CONNECT_ATTEMPTS)
        {
            Serial.println("Connessione in corso...");
            attempts++;
            delay(SMOOTH_CONNECT_DELAY);
        }

        if (WiFi.status() != WL_CONNECTED)
        {
            throw std::runtime_error("Errore durante la connessione alla rete Wi-Fi dopo tentativi!");
        }

        Serial.println("smooth Connessione Wi-Fi stabilita!");
        Serial.print("Indirizzo IP: ");
        Serial.println(WiFi.localIP());
        this->ip_address = std::string(WiFi.localIP().toString().c_str());
        this->isConnected = true;
        Serial.println("ip_address: ");
        Serial.println(this->ip_address.c_str());
    }
    catch(const std::exception& e)
    {
        Serial.println("Errore non sono riuscito a connettermi, password probabilmente errata o potenza segnale troppo debole!");
        this->isConnected = false;
        throw new std::runtime_error("Errore durante la connessione alla rete Wi-Fi!");
    }
    isAP = false;
}

// ===== NUOVE FUNZIONI PER RICONNESSIONE AUTOMATICA =====

void WiFiManager::checkConnection()
{
    if (millis() - lastConnectionCheck < WIFI_CHECK_INTERVAL) return;
    lastConnectionCheck = millis();
    
    updateConnectionStatus();
    
    if (!isConnected && autoReconnectEnabled && !isAP)
    {
        if (millis() - lastReconnectAttempt >= WIFI_AUTO_RECONNECT_INTERVAL)
        {
            Serial.println("⚠️ WiFi disconnesso, tentativo di riconnessione automatica...");
            autoReconnect();
            lastReconnectAttempt = millis();
        }
    }
}

bool WiFiManager::autoReconnect()
{
    if (isAP || !autoReconnectEnabled) return false;
    
    Serial.println("🔄 Tentativo riconnessione automatica...");
    
    WiFi.begin(ssid, password);
    
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < WIFI_AUTO_RECONNECT_ATTEMPTS)
    {
        delay(500);
        Serial.print(".");
        attempts++;
    }
    
    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("\n✅ WiFi riconnesso automaticamente!");
        this->ip_address = std::string(WiFi.localIP().toString().c_str());
        this->isConnected = true;
        
        // Ricrea il web server se necessario
        if (my_webServer == nullptr)
        {
            my_webServer = new WebServer(ssid, password);
        }
        
        return true;
    }
    else
    {
        Serial.println("\n❌ Riconnessione automatica fallita");
        this->isConnected = false;
        return false;
    }
}

void WiFiManager::enableAutoReconnect(bool enable)
{
    autoReconnectEnabled = enable;
    Serial.println(enable ? "✅ Riconnessione automatica abilitata" : "❌ Riconnessione automatica disabilitata");
}

bool WiFiManager::isWiFiConnected() const
{
    return isConnected && WiFi.status() == WL_CONNECTED;
}

void WiFiManager::updateConnectionStatus()
{
    bool wasConnected = isConnected;
    isConnected = (WiFi.status() == WL_CONNECTED && !isAP);
    
    if (wasConnected && !isConnected)
    {
        Serial.println("⚠️ WiFi disconnesso rilevato!");
    }
    else if (!wasConnected && isConnected)
    {
        Serial.println("✅ WiFi connesso rilevato!");
    }
}

// ===== FUNZIONI ESISTENTI =====

std::vector<std::string> WiFiManager::scanNetworks()
{
    std::vector<std::string> networks;
    int n = WiFi.scanNetworks();
    for (int i = 0; i < n; ++i)
    {
        networks.push_back(std::string(WiFi.SSID(i).c_str()));
        Serial.println(WiFi.SSID(i));
        Serial.println("potenza segnale: "+String(WiFi.RSSI(i)));
    }
    return networks;
}

void WiFiManager::setNetwork(const char *ssid_new, const char *password_new)
{
    const char * old_ssid = this->ssid;
    const char * old_password = this->password;

    this->ssid = ssid_new;
    this->password = password_new;
    try
    {
        this->smoothConnect();
        isFirstStart = false;
    }
    catch(...)
    {
        this->ssid = old_ssid;
        this->password = old_password;

        if (isAP)
        {
            this->setupAP();
        }
        else
        {
            this->clear_var();
            this->setNetwork(old_ssid, old_password);
        }
        throw std::runtime_error(std::string("Errore cambio rete Wi-Fi!, attivo la vecchia rete: ssid: ") + old_ssid + ", password: " + old_password);
    }
}

WiFiManager::~WiFiManager()
{
    if (my_webServer != nullptr)
    {
        delete my_webServer;
        my_webServer = nullptr;
    }
    Serial.println("Oggetto WiFiManager distrutto, liberazione della memoria...");
}