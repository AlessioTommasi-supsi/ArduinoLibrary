#ifndef NAVIGATION_MANAGER_H
#define NAVIGATION_MANAGER_H

#include <Arduino.h>

namespace smarthome {

class NavigationManager {
public:
    static String getNavbar();
    static String getNavbarCss();
};

} // namespace smarthome

#endif // NAVIGATION_MANAGER_H
