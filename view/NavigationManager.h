#ifndef NAVIGATION_MANAGER_H
#define NAVIGATION_MANAGER_H

#include <Arduino.h>

class NavigationManager
{
public:
    // Navbar HTML
    static String getNavbar();
    
    // CSS per navbar (riferimento al CssStyleManager)
    static String getNavbarCss();
    static String getNavbarStyleCss();
};

#endif // NAVIGATION_MANAGER_H