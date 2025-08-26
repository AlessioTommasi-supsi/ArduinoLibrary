#include "NavigationManager.h"
#include "CssStyleManager.h"

String NavigationManager::getNavbar() {
    String navbar = R"(
        <!-- Navbar Responsive con Scorrimento Orizzontale -->
        <div class="navbar">
            <a href="/">
            <div class="icon">🏠</div>
            <span>Home</span>
            </a>
            
            <a href="/monitor">
            <div class="icon">🖥️</div>
            <span>Monitor</span>
            </a>
            <a href="/pinout">
            <div class="icon">🔌</div>
            <span>Pinout</span>
            </a>
            
            <a href="/history">
            <div class="icon">⏳</div>
            <span>History</span>
            </a>
            
            <a href="/services">
            <div class="icon">⚡</div>
            <span>Services</span>
            </a>
            <a href="/config">
            <div class="icon">📶</div>
            <span>WiFi Config</span>
            </a>
            
        </div>
        )";
    return navbar;
}

String NavigationManager::getNavbarCss() {
    return CssStyleManager::getNavbarCss();
}

String NavigationManager::getNavbarStyleCss() {
    return CssStyleManager::getNavbarStyleCss();
}