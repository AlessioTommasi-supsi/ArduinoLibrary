#include "NavigationManager.h"

namespace smarthome {

String NavigationManager::getNavbar() {
    String navbar = R"raw(
        <!-- Navbar Responsive (2 Voci Attive: Services & WiFi Config) -->
        <div class="navbar">
            <a href="/services">
                <div class="icon">&#9889;</div>
                <span>Services</span>
            </a>
            <a href="/config">
                <div class="icon">&#128246;</div>
                <span>WiFi Config</span>
            </a>
        </div>
    )raw";
    return navbar;
}

String NavigationManager::getNavbarCss() {
    String css = R"raw(
    .navbar {
      position: fixed;
      bottom: 20px;
      left: 50%;
      transform: translateX(-50%);
      background-color: rgba(255, 255, 255, 0.4);
      backdrop-filter: blur(10px);
      -webkit-backdrop-filter: blur(10px);
      border-radius: 12px;
      display: flex;
      justify-content: space-around;
      align-items: center;
      padding: 10px 20px;
      width: 90%;
      max-width: 400px;
      box-shadow: 0 4px 10px rgba(0, 0, 0, 0.1);
      z-index: 2000;
    }
    .navbar a {
      text-decoration: none;
      color: #333;
      display: flex;
      flex-direction: column;
      align-items: center;
      font-size: 14px;
      transition: color 0.3s ease;
    }
    .navbar a:hover {
      color: #4CAF50;
    }
    .navbar .icon {
      font-size: 24px;
    }
    )raw";
    return css;
}

} // namespace smarthome
