#include "ViewConfig.h"
#include "NavigationManager.h"

namespace smarthome {

String ViewConfig::generateConfigPageHTML() {
    String html = R"raw(
<!DOCTYPE html>
<html>
<head>
    <title>Configurazione Wi-Fi</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="stylesheet" href="/glassmorphism.css">
    <style>
        )raw" + NavigationManager::getNavbarCss() + R"raw(
        body { font-family: 'Segoe UI', Arial, sans-serif; background-color: #020001; color: #fff; padding: 20px; display: flex; flex-direction: column; align-items: center; padding-bottom: 120px; }
        .logo_top_container { text-align: center; margin-top: 10px; margin-bottom: 25px; }
        .logo_top_image { max-width: 250px; height: auto; }
        .form-container { width: 90%; max-width: 500px; background: rgba(255, 255, 255, 0.04); backdrop-filter: blur(12px); padding: 25px; border-radius: 20px; border: 1px solid rgba(255, 255, 255, 0.12); box-shadow: 0 8px 32px 0 rgba(0, 0, 0, 0.8); }
        form { display: flex; flex-direction: column; }
        label { margin-bottom: 8px; font-weight: bold; color: #ff2222; }
        select, input { margin-bottom: 20px; padding: 12px; border-radius: 8px; border: 1px solid rgba(255,255,255,0.25); background: rgba(0,0,0,0.6); color: #fff; font-size: 16px; }
        button { padding: 14px; background: #ff2222; color: white; border: none; border-radius: 8px; font-size: 18px; font-weight: bold; cursor: pointer; transition: 0.2s; }
        button:hover { background: #d61515; }
    </style>
</head>
<body>
    <div class="logo_top_container">
        <img src="/logo.webp" alt="Logo" class="logo_top_image">
    </div>

    <div class="form-container">
        <h2 style="color: #ff2222;">&#128246; Wi-Fi Config (Panigale-Mel-AP)</h2>
        <form action="/switch_wifi" method="get">
            <label for="ssid">Seleziona Rete Wi-Fi:</label>
            <select id="ssid" name="ssid" required>
)raw";

    if (esphome::wifi::global_wifi_component != nullptr) {
        for (auto &scan_res : esphome::wifi::global_wifi_component->get_scan_result()) {
            String ssidName = String(scan_res.get_ssid().c_str());
            html += "<option value='" + ssidName + "'>" + ssidName + " (" + String(scan_res.get_rssi()) + " dBm)</option>";
        }
    }

    html += R"raw(
            </select>
            <label for="password">Password:</label>
            <input type="password" id="password" name="password" placeholder="Inserisci Password" required>
            <button type="submit">Connetti</button>
        </form>
    </div>
    )raw" + NavigationManager::getNavbar() + R"raw(
</body>
</html>
)raw";
    return html;
}

String ViewConfig::generateWifiSwitchSuccessHTML(const String &ssid) {
    String html = R"raw(
<!DOCTYPE html>
<html>
<head>
    <title>Wi-Fi Switching</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        )raw" + NavigationManager::getNavbarCss() + R"raw(
        body { font-family: 'Segoe UI', Arial; background-color: #020001; color: #fff; display: flex; flex-direction: column; align-items: center; justify-content: center; height: 100vh; margin: 0; text-align: center; }
        .logo_top_container { text-align: center; margin-bottom: 25px; }
        .logo_top_image { max-width: 250px; height: auto; }
        .card { background: rgba(255,255,255,0.05); padding: 30px; border-radius: 20px; border: 1px solid rgba(255,255,255,0.15); box-shadow: 0 8px 32px rgba(0,0,0,0.8); }
    </style>
</head>
<body>
    <div class="logo_top_container">
        <img src="/logo.webp" alt="Logo" class="logo_top_image">
    </div>
    <div class="card">
        <h2 style="color: #ff2222;">&#9989; Wi-Fi impostato con successo!</h2>
        <p>Riconnessione a: <b>)raw" + ssid + R"raw(</b></p>
        <p>L'Access Point (Panigale-Mel-AP) rimane SEMPRE ATTIVO per connessioni dirette.</p>
        <a href="/services" style="color: #ff2222; font-weight: bold; text-decoration: none;">Torna ai Servizi</a>
    </div>
    )raw" + NavigationManager::getNavbar() + R"raw(
</body>
</html>
)raw";
    return html;
}

} // namespace smarthome
