#ifndef CUSTOM_WEB_CONTROLLER_H
#define CUSTOM_WEB_CONTROLLER_H

#include "esphome.h"
#include "esphome/core/component.h"
#include "esphome/components/web_server_base/web_server_base.h"
#include <memory>
#include "DnsServerEspIdf.h"
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
        return req_url == url_;
    }

    void handleRequest(esphome::web_server_idf::AsyncWebServerRequest *request) override {
        handler_(request);
    }
};

class CustomWebController : public esphome::Component {
private:
    CustomButtonModel model_;
    std::unique_ptr<DnsServerEspIdf> dnsServer_;

public:
    CustomWebController();
    ~CustomWebController();

    void setup() override;
};

} // namespace smarthome

#endif // CUSTOM_WEB_CONTROLLER_H
