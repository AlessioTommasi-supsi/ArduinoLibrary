#include "CustomWebController.h"

namespace smarthome {

CustomWebController::CustomWebController() {}
CustomWebController::~CustomWebController() {
    if (dnsServer_) {
        dnsServer_->stop();
    }
}

void CustomWebController::setup() {
    auto *server = esphome::web_server_base::global_web_server_base->get_server();
    if (server == nullptr) return;

    // 0. Avvia DNSServer nativo ESP-IDF per il Captive Portal (* -> 192.168.4.1)
    dnsServer_ = std::make_unique<DnsServerEspIdf>();
    dnsServer_->start(53, "192.168.4.1");

    auto add_route = [server](const std::string &url, std::function<void(esphome::web_server_idf::AsyncWebServerRequest*)> func) {
        server->addHandler(new CustomUrlHandler(url, std::move(func)));
    };

    // Helper di reindirizzamento immediato a http://192.168.4.1/home
    auto captive_redirect = [](esphome::web_server_idf::AsyncWebServerRequest *request) {
        request->redirect("http://192.168.4.1/home");
    };

    // 1. Endpoints Immagini Locali (Servite da Flash in binario con Content-Length)
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

    // 2. Endpoint /pulsePin?pin=X (Impulso Hardware di 500 ms)
    add_route("/pulsePin", [this](esphome::web_server_idf::AsyncWebServerRequest *request) {
        if (request->hasParam("pin")) {
            std::string pinStr = request->getParam("pin")->value();
            int pinNum = atoi(pinStr.c_str());
            int targetGpio = model_.getValidGpio(pinNum);
            if (targetGpio >= 0) {
                pinMode(targetGpio, OUTPUT);
                digitalWrite(targetGpio, HIGH);
                this->set_timeout(500, [targetGpio]() {
                    digitalWrite(targetGpio, LOW);
                });
                request->send(200, "text/plain", "Pulsed pin for 500ms");
                return;
            }
        }
        request->send(400, "text/plain", "Invalid pin");
    });

    // 0. Configura Wi-Fi per mantenere i risultati di scansione e avvia prima scansione
    if (esphome::wifi::global_wifi_component != nullptr) {
        esphome::wifi::global_wifi_component->set_keep_scan_results(true);
        esphome::wifi::global_wifi_component->start_scanning();
    }

    // Scansione periodica di background ogni 25 secondi
    this->set_interval(25000, []() {
        if (esphome::wifi::global_wifi_component != nullptr) {
            esphome::wifi::global_wifi_component->start_scanning();
        }
    });

    // 3. Endpoint /config (Configurazione Wi-Fi accessibile dalla navbar)
    add_route("/config", [](esphome::web_server_idf::AsyncWebServerRequest *request) {
        if (esphome::wifi::global_wifi_component != nullptr) {
            esphome::wifi::global_wifi_component->start_scanning();
        }
        String html = ViewConfig::generateConfigPageHTML();
        request->send(200, "text/html", html.c_str());
    });

    // Endpoint JSON per ottenere i risultati della scansione in tempo reale
    add_route("/scan_results", [](esphome::web_server_idf::AsyncWebServerRequest *request) {
        String json = "[";
        if (esphome::wifi::global_wifi_component != nullptr) {
            auto results = esphome::wifi::global_wifi_component->get_scan_result();
            bool first = true;
            for (const auto &res : results) {
                if (res.get_is_hidden()) continue;
                std::string ssid = res.get_ssid();
                if (ssid.empty()) continue;
                if (!first) json += ",";
                first = false;
                json += "{\"ssid\":\"" + String(ssid.c_str()) + "\",\"rssi\":" + String(res.get_rssi()) + "}";
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

    // 4. Endpoint /switch_wifi
    add_route("/switch_wifi", [](esphome::web_server_idf::AsyncWebServerRequest *request) {
        if (request->hasParam("ssid") && request->hasParam("password")) {
            std::string newSsid = request->getParam("ssid")->value();
            std::string newPass = request->getParam("password")->value();
            
            if (esphome::wifi::global_wifi_component != nullptr) {
                esphome::wifi::global_wifi_component->save_wifi_sta(newSsid.c_str(), newPass.c_str());
            }
            String html = ViewConfig::generateWifiSwitchSuccessHTML(String(newSsid.c_str()));
            request->send(200, "text/html", html.c_str());
        } else {
            request->send(400, "text/html", "Parametri mancanti");
        }
    });

    // 5. Endpoint Principale /home (Dashboard Panigale Luci)
    add_route("/home", [this](esphome::web_server_idf::AsyncWebServerRequest *request) {
        String html = ViewServices::generateServicesHTML(model_.getButtons());
        request->send(200, "text/html", html.c_str());
    });

    // 6. Redirect automatici per Captive Portal e Root a /home
    add_route("/", captive_redirect);
    add_route("/services", captive_redirect);
    add_route("/generate_204", captive_redirect);       // Android Captive Check
    add_route("/gen_204", captive_redirect);            // Android Captive Check
    add_route("/hotspot-detect.html", captive_redirect);// Apple iOS / macOS Captive Check
    add_route("/canonical.html", captive_redirect);     // Apple Captive Check
    add_route("/connecttest.txt", captive_redirect);    // Windows Captive Check
    add_route("/ncsi.txt", captive_redirect);           // Windows Captive Check
    add_route("/redirect", captive_redirect);

    // Fallback automatico per ogni altra richiesta ignota: reindirizza sempre a /home
    server->onNotFound(captive_redirect);

    // 7. Endpoint /customButtons
    add_route("/customButtons", [this](esphome::web_server_idf::AsyncWebServerRequest *request) {
        String html = ViewServices::generateCustomButtonsHTML(model_.getButtons());
        request->send(200, "text/html", html.c_str());
    });

    // 8. Endpoint /addButton
    add_route("/addButton", [](esphome::web_server_idf::AsyncWebServerRequest *request) {
        String html = ViewServices::generateAddButtonFormHTML();
        request->send(200, "text/html", html.c_str());
    });

    // 9. Endpoint /saveButton
    add_route("/saveButton", [this](esphome::web_server_idf::AsyncWebServerRequest *request) {
        if (request->hasParam("label") && request->hasParam("emoji") && request->hasParam("url")) {
            std::string label = request->getParam("label")->value();
            std::string emoji = request->getParam("emoji")->value();
            std::string url = request->getParam("url")->value();
            
            model_.addButton(String(label.c_str()), String(emoji.c_str()), String(url.c_str()));
            request->redirect("/home");
        } else {
            request->send(400, "text/plain", "Parametri mancanti");
        }
    });

    // 10. Endpoint /removeButton
    add_route("/removeButton", [this](esphome::web_server_idf::AsyncWebServerRequest *request) {
        if (request->hasParam("label")) {
            std::string label = request->getParam("label")->value();
            model_.removeButton(String(label.c_str()));
            request->redirect("/home");
        } else {
            request->send(400, "text/plain", "Parametro label mancante");
        }
    });
}

} // namespace smarthome
