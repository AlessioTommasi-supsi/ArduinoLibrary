#ifndef NAVIGATION_MANAGER_H
#define NAVIGATION_MANAGER_H

#include <string>

namespace smarthome {

class NavigationManager {
public:
    static std::string getNavbar();
    static std::string getNavbarCss();
};

} // namespace smarthome

#endif // NAVIGATION_MANAGER_H
