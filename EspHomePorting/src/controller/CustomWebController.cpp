#include "CustomWebController.h"
#include <esp_wifi.h>
#include <esp_netif.h>

namespace smarthome {

CustomWebController *global_custom_web_controller = nullptr;

CustomWebController::CustomWebController() {
    global_custom_web_controller = this;
}
CustomWebController::~CustomWebController() {
    if (global_custom_web_controller == this) {
        global_custom_web_controller = nullptr;
    }
}

void CustomWebController::setup() {
    // Garantisce attivazione immediata dell'AP in modalità Dual Mode (APSTA)
    this->enable_dual_mode_ap();

    auto *server = esphome::web_server_base::global_web_server_base->get_server();
    if (server == nullptr) return;

    // Registra i nostri handler in cima alla lista per avere priorità su web_server generico
    auto add_route = [server](const std::string &url, std::function<void(esphome::web_server_idf::AsyncWebServerRequest*)> func) {
        AsyncWebServerAccessor::prependHandler(server, new CustomUrlHandler(url, std::move(func)));
    };

    // Helper reindirizzamento:
    // Se la richiesta è per panigalemel, o verso un IP locale (STA router o AP), reindirizza alla home relativa "/"
    // Solo se la richiesta è verso un dominio internet esterno (Captive Portal check da OS) reindirizza a 192.168.4.1
    auto captive_redirect = [](esphome::web_server_idf::AsyncWebServerRequest *request) {
        auto host_opt = request->get_header("Host");
        std::string host = host_opt.value_or("");
        if (host.find("panigalemel") != std::string::npos ||
            host.find("192.168.") != std::string::npos ||
            host.find("10.") == 0 ||
            host.find("172.") == 0 ||
            host.find("localhost") != std::string::npos) {
            request->redirect("/");
        } else {
            request->redirect("http://192.168.4.1/");
        }
    };

    // 1. Dashboard Principale (Servita direttamente su / e /home per velocità istantanea)
    auto serve_dashboard = [this](esphome::web_server_idf::AsyncWebServerRequest *request) {
        bool pos = this->getRelayState(4);
        bool anab = this->getRelayState(5);
        std::string html = ViewServices::generateServicesHTML(model_.getButtons(), pos, anab);
        request->send(200, "text/html", html.c_str());
    };

    add_route("/", serve_dashboard);
    add_route("/home", serve_dashboard);
    add_route("/services", serve_dashboard);

    // 2. Endpoints Immagini Locali (Servite da Flash in binario con Content-Length)
    add_route("/logo.webp", [](esphome::web_server_idf::AsyncWebServerRequest *request) {
        request->send(request->beginResponse(200, "image/webp", LOGO_MEL_DATA, LOGO_MEL_SIZE));
    });

    add_route("/posizione.webp", [](esphome::web_server_idf::AsyncWebServerRequest *request) {
        request->send(request->beginResponse(200, "image/webp", POSIZIONE_DATA, POSIZIONE_SIZE));
    });

    add_route("/anabbagliante.webp", [](esphome::web_server_idf::AsyncWebServerRequest *request) {
        request->send(request->beginResponse(200, "image/webp", ANABBAGLIANTE_DATA, ANABBAGLIANTE_SIZE));
    });

    add_route("/abbagliante.webp", [](esphome::web_server_idf::AsyncWebServerRequest *request) {
        request->send(request->beginResponse(200, "image/webp", ABBAGLIANTE_DATA, ABBAGLIANTE_SIZE));
    });

    // 3. Endpoint /togglePin?pin=X (Toggle logico e ritorno stato aggiornato)
    add_route("/togglePin", [this](esphome::web_server_idf::AsyncWebServerRequest *request) {
        if (request->hasParam("pin")) {
            std::string pinStr = request->getParam("pin")->value();
            int pinNum = atoi(pinStr.c_str());
            this->toggleRelay(pinNum);
            bool newState = this->getRelayState(pinNum);
            char resp[64];
            snprintf(resp, sizeof(resp), "{\"pin\":%d,\"state\":%s}", pinNum, newState ? "true" : "false");
            request->send(200, "application/json", resp);
            return;
        }
        request->send(400, "text/plain", "Missing pin");
    });

    // Retrocompatibilità /pulsePin -> inoltrato su toggle
    add_route("/pulsePin", [this](esphome::web_server_idf::AsyncWebServerRequest *request) {
        if (request->hasParam("pin")) {
            std::string pinStr = request->getParam("pin")->value();
            int pinNum = atoi(pinStr.c_str());
            this->toggleRelay(pinNum);
            bool newState = this->getRelayState(pinNum);
            char resp[64];
            snprintf(resp, sizeof(resp), "{\"pin\":%d,\"state\":%s}", pinNum, newState ? "true" : "false");
            request->send(200, "application/json", resp);
            return;
        }
        request->send(400, "text/plain", "Missing pin");
    });

    // Endpoint /status (Polling stato in tempo reale per la Web UI)
    add_route("/status", [this](esphome::web_server_idf::AsyncWebServerRequest *request) {
        bool pos = this->getRelayState(4);
        bool anab = this->getRelayState(5);
        char resp[64];
        snprintf(resp, sizeof(resp), "{\"pos\":%s,\"anab\":%s}", pos ? "true" : "false", anab ? "true" : "false");
        request->send(200, "application/json", resp);
    });

    // 4. Configura Wi-Fi per mantenere i risultati di scansione su richiesta
    if (esphome::wifi::global_wifi_component != nullptr) {
        esphome::wifi::global_wifi_component->set_keep_scan_results(true);
    }

    // Endpoint /config (Configurazione Wi-Fi accessibile dalla navbar)
    add_route("/config", [](esphome::web_server_idf::AsyncWebServerRequest *request) {
        if (esphome::wifi::global_wifi_component != nullptr) {
            esphome::wifi::global_wifi_component->start_scanning();
        }
        std::string html = ViewConfig::generateConfigPageHTML();
        request->send(200, "text/html", html.c_str());
    });

    // Endpoint JSON /scan_results (Scansione asincrona usata da JS senza reload)
    add_route("/scan_results", [](esphome::web_server_idf::AsyncWebServerRequest *request) {
        std::string json = "[";
        if (esphome::wifi::global_wifi_component != nullptr) {
            bool first = true;
            for (auto &res : esphome::wifi::global_wifi_component->get_scan_result()) {
                if (res.get_is_hidden()) continue;
                std::string ssid = res.get_ssid();
                if (ssid.empty()) continue;
                if (!first) json += ",";
                first = false;
                json += "{\"ssid\":\"" + ssid + "\",\"rssi\":" + std::to_string(res.get_rssi()) + "}";
            }
        }
        json += "]";
        request->send(200, "application/json", json.c_str());
    });

    // Endpoint per forzare una nuova scansione immediata
    add_route("/rescan", [](esphome::web_server_idf::AsyncWebServerRequest *request) {
        if (esphome::wifi::global_wifi_component != nullptr) {
            esphome::wifi::global_wifi_component->start_scanning();
        }
        request->send(200, "text/plain", "OK");
    });

    // Endpoint /switch_wifi
    add_route("/switch_wifi", [](esphome::web_server_idf::AsyncWebServerRequest *request) {
        if (request->hasParam("ssid") && request->hasParam("password")) {
            std::string newSsid = request->getParam("ssid")->value();
            std::string newPass = request->getParam("password")->value();
            
            // 1. Assicura che l'ESP32 sia in Dual Mode AP+STA per non spegnere mai l'AP
            esp_wifi_set_mode(WIFI_MODE_APSTA);

            if (esphome::wifi::global_wifi_component != nullptr) {
                // Salva le credenziali in memoria NVS
                esphome::wifi::global_wifi_component->save_wifi_sta(newSsid.c_str(), newPass.c_str());

                // Forza l'inizio immediato della connessione alla nuova rete (anche da modalità AP)
                esphome::wifi::WiFiAP sta{};
                sta.set_ssid(newSsid);
                sta.set_password(newPass);
                esphome::wifi::global_wifi_component->set_sta(sta);
                esphome::wifi::global_wifi_component->start_connecting(sta);
            }

            // 2. Attesa non-bloccante fino a 4 secondi per catturare subito l'IP assegnato da DHCP
            std::string obtainedIp = "";
            for (int i = 0; i < 16; i++) {
                vTaskDelay(pdMS_TO_TICKS(250)); // 250ms * 16 = 4s max
                if (esphome::wifi::global_wifi_component != nullptr && esphome::wifi::global_wifi_component->is_connected()) {
                    auto ips = esphome::wifi::global_wifi_component->wifi_sta_ip_addresses();
                    for (auto &ip : ips) {
                        if (ip.is_set()) {
                            char ip_buf[32];
                            obtainedIp = ip.str_to(ip_buf);
                            break;
                        }
                    }
                    if (!obtainedIp.empty() && obtainedIp != "0.0.0.0") break;
                }
            }

            // Fallback diretto sullo stack di rete nativo ESP-IDF se non ancora aggiornato in ESPHome
            if (obtainedIp.empty() || obtainedIp == "0.0.0.0") {
                esp_netif_t *sta_netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
                if (sta_netif != nullptr) {
                    esp_netif_ip_info_t ip_info;
                    if (esp_netif_get_ip_info(sta_netif, &ip_info) == ESP_OK && ip_info.ip.addr != 0) {
                        char ip_buf[32];
                        esp_ip4addr_ntoa(&ip_info.ip, ip_buf, sizeof(ip_buf));
                        obtainedIp = ip_buf;
                    }
                }
            }

            std::string html = ViewConfig::generateWifiSwitchSuccessHTML(newSsid, obtainedIp);
            request->send(200, "text/html", html.c_str());
        } else {
            request->send(400, "text/html", "Parametri mancanti");
        }
    });

    // Endpoint /wifi_ip (Polling asincrono dello stato di connessione e dell'IP ottenuto)
    add_route("/wifi_ip", [](esphome::web_server_idf::AsyncWebServerRequest *request) {
        std::string ipStr = "";
        bool connected = false;
        if (esphome::wifi::global_wifi_component != nullptr && esphome::wifi::global_wifi_component->is_connected()) {
            connected = true;
            auto ips = esphome::wifi::global_wifi_component->wifi_sta_ip_addresses();
            for (auto &ip : ips) {
                if (ip.is_set()) {
                    char ip_buf[32];
                    ipStr = ip.str_to(ip_buf);
                    break;
                }
            }
        }
        if (ipStr.empty() || ipStr == "0.0.0.0") {
            esp_netif_t *sta_netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
            if (sta_netif != nullptr) {
                esp_netif_ip_info_t ip_info;
                if (esp_netif_get_ip_info(sta_netif, &ip_info) == ESP_OK && ip_info.ip.addr != 0) {
                    char ip_buf[32];
                    esp_ip4addr_ntoa(&ip_info.ip, ip_buf, sizeof(ip_buf));
                    ipStr = ip_buf;
                    connected = true;
                }
            }
        }
        char resp[128];
        snprintf(resp, sizeof(resp), "{\"connected\":%s,\"ip\":\"%s\"}", connected ? "true" : "false", ipStr.c_str());
        request->send(200, "application/json", resp);
    });

    // 5. Endpoint /customButtons
    add_route("/customButtons", [this](esphome::web_server_idf::AsyncWebServerRequest *request) {
        std::string html = ViewServices::generateCustomButtonsHTML(model_.getButtons());
        request->send(200, "text/html", html.c_str());
    });

    // 6. Endpoint /addButton
    add_route("/addButton", [](esphome::web_server_idf::AsyncWebServerRequest *request) {
        std::string html = ViewServices::generateAddButtonFormHTML();
        request->send(200, "text/html", html.c_str());
    });

    // 7. Endpoint /saveButton
    add_route("/saveButton", [this](esphome::web_server_idf::AsyncWebServerRequest *request) {
        if (request->hasParam("label") && request->hasParam("emoji") && request->hasParam("url")) {
            std::string label = request->getParam("label")->value();
            std::string emoji = request->getParam("emoji")->value();
            std::string url = request->getParam("url")->value();
            
            model_.addButton(label, emoji, url);
            request->redirect("/");
        } else {
            request->send(400, "text/plain", "Parametri mancanti");
        }
    });

    // 8. Endpoint /removeButton
    add_route("/removeButton", [this](esphome::web_server_idf::AsyncWebServerRequest *request) {
        if (request->hasParam("label")) {
            std::string label = request->getParam("label")->value();
            model_.removeButton(label);
            request->redirect("/");
        } else {
            request->send(400, "text/plain", "Parametro label mancante");
        }
    });

    // Fallback automatico per ogni altra richiesta ignota: reindirizza sempre alla home
    server->onNotFound(captive_redirect);
}

void CustomWebController::toggleRelay(int pin) {
    if (pin == 4 && relayPosizione_ != nullptr) {
        relayPosizione_->toggle();
    } else if (pin == 5 && relayAnabbagliante_ != nullptr) {
        relayAnabbagliante_->toggle();
    }
}

bool CustomWebController::getRelayState(int pin) const {
    if (pin == 4 && relayPosizione_ != nullptr) {
        return relayPosizione_->state;
    } else if (pin == 5 && relayAnabbagliante_ != nullptr) {
        return relayAnabbagliante_->state;
    }
    return false;
}

void CustomWebController::enable_dual_mode_ap() {
    wifi_mode_t mode = WIFI_MODE_NULL;
    if (esp_wifi_get_mode(&mode) == ESP_OK) {
        if (mode == WIFI_MODE_APSTA) {
            return; // Già in Dual Mode (AP+STA), nessun intervento necessario
        }
    }

    // Forza la modalità Dual Mode: sia Station che Access Point simultanei
    esp_wifi_set_mode(WIFI_MODE_APSTA);

    wifi_config_t conf;
    memset(&conf, 0, sizeof(conf));
    const char *ssid = "Panigale-Mel-AP";
    const char *pass = "mammamelap";
    memcpy(conf.ap.ssid, ssid, strlen(ssid));
    memcpy(conf.ap.password, pass, strlen(pass));
    conf.ap.ssid_len = strlen(ssid);
    conf.ap.channel = 0; // Eredita automaticamente il canale della rete Wi-Fi connessa
    conf.ap.authmode = WIFI_AUTH_WPA2_PSK;
    conf.ap.max_connection = 4;
    conf.ap.beacon_interval = 100;
    conf.ap.pairwise_cipher = WIFI_CIPHER_TYPE_CCMP;

    esp_wifi_set_config(WIFI_IF_AP, &conf);

    esp_netif_t *ap_netif = esp_netif_get_handle_from_ifkey("WIFI_AP_DEF");
    if (ap_netif != nullptr) {
        esp_netif_ip_info_t info;
        esp_netif_set_ip4_addr(&info.ip, 192, 168, 4, 1);
        esp_netif_set_ip4_addr(&info.gw, 192, 168, 4, 1);
        esp_netif_set_ip4_addr(&info.netmask, 255, 255, 255, 0);

        esp_netif_dhcps_stop(ap_netif);
        esp_netif_set_ip_info(ap_netif, &info);
        esp_netif_dhcps_start(ap_netif);
    }
}

} // namespace smarthome
