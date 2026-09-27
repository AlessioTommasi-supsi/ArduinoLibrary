#ifndef VIEW_SERVICES_H
#define VIEW_SERVICES_H

#include <Arduino.h>
#include <vector>
#include "CustomButtonModel.h"

namespace smarthome {

class ViewServices {
public:
    static String generateServicesHTML(const std::vector<CustomButtonItem> &customButtons);
    static String generateCustomButtonsHTML(const std::vector<CustomButtonItem> &customButtons);
    static String generateAddButtonFormHTML();
};

} // namespace smarthome

#endif // VIEW_SERVICES_H
