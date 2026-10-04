#ifndef VIEW_SERVICES_H
#define VIEW_SERVICES_H

#include <string>
#include <vector>
#include "CustomButtonModel.h"

namespace smarthome {

class ViewServices {
public:
    static std::string generateServicesHTML(const std::vector<CustomButtonItem> &customButtons, bool posState = false, bool anabState = false);
    static std::string generateCustomButtonsHTML(const std::vector<CustomButtonItem> &customButtons);
    static std::string generateAddButtonFormHTML();
};

} // namespace smarthome

#endif // VIEW_SERVICES_H
