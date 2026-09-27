#include "NavigationManager.h"

namespace smarthome {

String NavigationManager::getNavbar() {
    String navbar = R"raw(
        <!-- Navbar Floating Glassmorphism (Home & WiFi Config) -->
        <div class="navbar">
            <a href="/home">
                <div class="icon">&#127968;</div>
                <span>Home</span>
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
      background: rgba(10, 5, 8, 0.85);
      backdrop-filter: blur(15px);
      -webkit-backdrop-filter: blur(15px);
      border: 1px solid rgba(255, 255, 255, 0.15);
      border-radius: 30px;
      padding: 10px 30px;
      display: flex;
      gap: 30px;
      z-index: 2000;
      box-shadow: 0 4px 20px rgba(0, 0, 0, 0.8);
    }
    .navbar a {
      text-decoration: none;
      color: #94a3b8;
      display: flex;
      flex-direction: column;
      align-items: center;
      font-size: 13px;
      font-weight: 600;
      transition: color 0.3s ease;
    }
    .navbar a:hover, .navbar a.active {
      color: #ff1a1a;
    }
    .navbar .icon {
      font-size: 24px;
      margin-bottom: 2px;
    }
    )raw";
    return css;
}

} // namespace smarthome
