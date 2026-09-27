#ifndef VIEW_CONFIG_H
#define VIEW_CONFIG_H

#include <Arduino.h>
#include "esphome/components/wifi/wifi_component.h"

namespace smarthome {

class ViewConfig {
public:
    static String generateConfigPageHTML();
    static String generateWifiSwitchSuccessHTML(const String &ssid);
};

} // namespace smarthome

#endif // VIEW_CONFIG_H
