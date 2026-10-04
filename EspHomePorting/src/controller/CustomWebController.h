#ifndef CUSTOM_WEB_CONTROLLER_H
#define CUSTOM_WEB_CONTROLLER_H

#include "esphome.h"
#include "esphome/core/component.h"
#include "esphome/components/web_server_base/web_server_base.h"
#include "esphome/components/switch/switch.h"
#include <memory>
#include "CustomButtonModel.h"
#include "ViewConfig.h"
#include "ViewServices.h"
#include "ImagesData.h"

namespace smarthome {

class CustomUrlHandler : public esphome::web_server_idf::AsyncWebHandler {
private:
    std::string url_;
    std::function<void(esphome::web_server_idf::AsyncWebServerRequest*)> handler_;

public:
    CustomUrlHandler(std::string url, std::function<void(esphome::web_server_idf::AsyncWebServerRequest*)> handler)
        : url_(std::move(url)), handler_(std::move(handler)) {}

    bool canHandle(esphome::web_server_idf::AsyncWebServerRequest *request) const override {
        char buffer[esphome::web_server_idf::AsyncWebServerRequest::URL_BUF_SIZE];
        std::string req_url = std::string(request->url_to(buffer));
        if (req_url == url_) return true;
        if (url_ == "/" && req_url.empty()) return true;
        if (url_ == "" && req_url == "/") return true;
        if (req_url == url_ + "/") return true;
        if (url_ == req_url + "/") return true;
        return false;
    }

    void handleRequest(esphome::web_server_idf::AsyncWebServerRequest *request) override {
        handler_(request);
    }
};

class AsyncWebServerAccessor : public esphome::web_server_idf::AsyncWebServer {
public:
    static void prependHandler(esphome::web_server_idf::AsyncWebServer *server, esphome::web_server_idf::AsyncWebHandler *handler) {
        if (server == nullptr || handler == nullptr) return;
        auto *acc = static_cast<AsyncWebServerAccessor*>(server);
        acc->handlers_.insert(acc->handlers_.begin(), handler);
    }
};

class CustomWebController : public esphome::Component {
private:
    CustomButtonModel model_;

    esphome::switch_::Switch *relayPosizione_{nullptr};
    esphome::switch_::Switch *relayAnabbagliante_{nullptr};

public:
    CustomWebController();
    ~CustomWebController();

    void set_relays(esphome::switch_::Switch *pos, esphome::switch_::Switch *anab) {
        relayPosizione_ = pos;
        relayAnabbagliante_ = anab;
    }

    void setup() override;

    void toggleRelay(int pin);
    bool getRelayState(int pin) const;

    void enable_dual_mode_ap();
};

extern CustomWebController *global_custom_web_controller;

} // namespace smarthome

#endif // CUSTOM_WEB_CONTROLLER_H
