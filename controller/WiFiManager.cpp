#include "WiFiManager.h"
#include "WebServer.h"
#include <stdexcept>

WebServer *my_webServer = nullptr;

WiFiManager::WiFiManager()
{
    this->ap_ssid = DEFAULT_AP_SSID;
    this->ap_password = DEFAULT_AP_PASSWORD;
    this->sta_ssid = "";
    this->sta_password = "";
    this->sta_ip = "";
    this->ap_ip = "";
    this->isSTAConnected = false;
    this->isFirstStart = true;

    initializeDualMode();
}

void WiFiManager::initializeDualMode()
{
    try
    {
        // 1. Configura la modalità Dual Mode (WIFI_AP_STA)
        WiFi.mode(WIFI_AP_STA);
        delay(WIFI_MODE_DELAY);

        // 2. Avvia l'Access Point (SEMPRE ATTIVO)
        WiFi.softAP(ap_ssid.c_str(), ap_password.c_str());
        IPAddress apIP = WiFi.softAPIP();
        this->ap_ip = apIP.toString().c_str();

        Serial.print("ESP32 Dual Mode - AP attivato con IP: ");
        Serial.println(apIP);

        // 3. Inizializza il WebServer una sola volta (se non esiste)
        if (my_webServer == nullptr)
        {
            my_webServer = new WebServer(ap_ssid.c_str(), ap_password.c_str());
        }

        // 4. Carica credenziali da NVS e tenta la connessione STA in background
        String savedSsid, savedPass;
        if (CredentialsStorage::loadCredentials(savedSsid, savedPass))
        {
            Serial.print("Credenziali NVS trovate per SSID: ");
            Serial.println(savedSsid);
            if (tryConnectSTA(savedSsid.c_str(), savedPass.c_str(), SMOOTH_CONNECT_ATTEMPTS))
            {
                sta_ssid = savedSsid.c_str();
                sta_password = savedPass.c_str();
                Serial.print("Connessione STA riuscita! IP locale: ");
                Serial.println(sta_ip.c_str());
            }
            else
            {
                Serial.println("Connessione STA alle credenziali salvate fallita. L'AP rimane comunque attivo a 192.168.4.1.");
            }
        }
        else
        {
            Serial.println("Nessuna credenziale salvata in NVS. In attesa di configurazione tramite Web UI.");
        }
    }
    catch (...)
    {
        Serial.println("Errore durante l'inizializzazione della modalità Dual Mode AP+STA!");
    }
}

bool WiFiManager::tryConnectSTA(const char *ssid, const char *password, int maxAttempts)
{
    if (ssid == nullptr || strlen(ssid) == 0)
    {
        return false;
    }

    Serial.print("Tentativo di connessione STA a ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < maxAttempts)
    {
        delay(SMOOTH_CONNECT_DELAY);
        Serial.print(".");
        attempts++;
    }
    Serial.println();

    if (WiFi.status() == WL_CONNECTED)
    {
        this->sta_ip = std::string(WiFi.localIP().toString().c_str());
        this->isSTAConnected = true;
        return true;
    }
    else
    {
        WiFi.disconnect(false);
        this->isSTAConnected = false;
        this->sta_ip = "";
        return false;
    }
}

WiFiManager::WiFiManager(const char *ssid, const char *password)
{
    this->ap_ssid = DEFAULT_AP_SSID;
    this->ap_password = DEFAULT_AP_PASSWORD;
    this->isSTAConnected = false;
    this->isFirstStart = true;

    initializeDualMode();

    if (ssid != nullptr && strlen(ssid) > 0)
    {
        setNetwork(ssid, password);
    }
}

void WiFiManager::setupAP()
{
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(ap_ssid.c_str(), ap_password.c_str());
    this->ap_ip = WiFi.softAPIP().toString().c_str();
}

void WiFiManager::connect()
{
    if (!sta_ssid.empty())
    {
        tryConnectSTA(sta_ssid.c_str(), sta_password.c_str());
    }
}

bool WiFiManager::smoothConnect()
{
    if (sta_ssid.empty())
    {
        return false;
    }
    return tryConnectSTA(sta_ssid.c_str(), sta_password.c_str(), SMOOTH_CONNECT_ATTEMPTS);
}

void WiFiManager::clear_var()
{
    WiFi.disconnect(false);
    isSTAConnected = false;
    sta_ip = "";
}

void WiFiManager::setNetwork(const char *ssid_new, const char *password_new)
{
    std::string old_ssid = this->sta_ssid;
    std::string old_password = this->sta_password;

    Serial.print("Richiesto cambio rete STA a: ");
    Serial.println(ssid_new);

    bool success = tryConnectSTA(ssid_new, password_new, SMOOTH_CONNECT_ATTEMPTS);

    if (success)
    {
        this->sta_ssid = ssid_new;
        this->sta_password = password_new;

        if (CredentialsStorage::saveCredentials(ssid_new, password_new))
        {
            Serial.println("Credenziali salvate con successo in NVS!");
        }
        else
        {
            Serial.println("Errore durante il salvataggio delle credenziali in NVS.");
        }
    }
    else
    {
        Serial.println("Connessione alla nuova rete fallita! L'AP rimane attivo a 192.168.4.1.");

        if (!old_ssid.empty())
        {
            tryConnectSTA(old_ssid.c_str(), old_password.c_str(), 5);
        }

        throw std::runtime_error(std::string("Errore cambio rete Wi-Fi! Impossibile connettersi a: ") + ssid_new);
    }
}

std::vector<std::string> WiFiManager::scanNetworks()
{
    std::vector<std::string> networks;
    int n = WiFi.scanNetworks();
    for (int i = 0; i < n; ++i)
    {
        networks.push_back(std::string(WiFi.SSID(i).c_str()));
        Serial.print("Rete trovata: ");
        Serial.print(WiFi.SSID(i));
        Serial.print(" (RSSI: ");
        Serial.print(WiFi.RSSI(i));
        Serial.println(")");
    }
    return networks;
}

WiFiManager::~WiFiManager()
{
    if (my_webServer != nullptr)
    {
        delete my_webServer;
        my_webServer = nullptr;
    }
    Serial.println("WiFiManager distrutto.");
}