#ifndef VIEW_CONFIG_H
#define VIEW_CONFIG_H

#include <string>
#include "esphome/components/wifi/wifi_component.h"

namespace smarthome {

class ViewConfig {
public:
    static std::string generateConfigPageHTML();
    static std::string generateWifiSwitchSuccessHTML(const std::string &ssid, const std::string &ip = "");
};

} // namespace smarthome

#endif // VIEW_CONFIG_H
