#include "CustomWebController.h"

namespace smarthome {

CustomWebController::CustomWebController() {}
CustomWebController::~CustomWebController() {}

void CustomWebController::setup() {
    auto *server = esphome::web_server_base::global_web_server_base->get_server();
    if (server == nullptr) return;

    auto add_route = [server](const std::string &url, std::function<void(esphome::web_server_idf::AsyncWebServerRequest*)> func) {
        server->addHandler(new CustomUrlHandler(url, std::move(func)));
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

    // 3. Endpoint /config
    add_route("/config", [](esphome::web_server_idf::AsyncWebServerRequest *request) {
        String html = ViewConfig::generateConfigPageHTML();
        request->send(200, "text/html", html.c_str());
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

    // 5. Endpoint /services
    add_route("/services", [this](esphome::web_server_idf::AsyncWebServerRequest *request) {
        String html = ViewServices::generateServicesHTML(model_.getButtons());
        request->send(200, "text/html", html.c_str());
    });

    // 6. Endpoint /customButtons
    add_route("/customButtons", [this](esphome::web_server_idf::AsyncWebServerRequest *request) {
        String html = ViewServices::generateCustomButtonsHTML(model_.getButtons());
        request->send(200, "text/html", html.c_str());
    });

    // 7. Endpoint /addButton
    add_route("/addButton", [](esphome::web_server_idf::AsyncWebServerRequest *request) {
        String html = ViewServices::generateAddButtonFormHTML();
        request->send(200, "text/html", html.c_str());
    });

    // 8. Endpoint /saveButton
    add_route("/saveButton", [this](esphome::web_server_idf::AsyncWebServerRequest *request) {
        if (request->hasParam("label") && request->hasParam("emoji") && request->hasParam("url")) {
            std::string label = request->getParam("label")->value();
            std::string emoji = request->getParam("emoji")->value();
            std::string url = request->getParam("url")->value();
            
            model_.addButton(String(label.c_str()), String(emoji.c_str()), String(url.c_str()));
            request->redirect("/services");
        } else {
            request->send(400, "text/plain", "Parametri mancanti");
        }
    });

    // 9. Endpoint /removeButton
    add_route("/removeButton", [this](esphome::web_server_idf::AsyncWebServerRequest *request) {
        if (request->hasParam("label")) {
            std::string label = request->getParam("label")->value();
            model_.removeButton(String(label.c_str()));
            request->redirect("/services");
        } else {
            request->send(400, "text/plain", "Parametro label mancante");
        }
    });
}

} // namespace smarthome
