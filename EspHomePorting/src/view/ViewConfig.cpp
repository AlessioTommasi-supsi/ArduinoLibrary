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
        select, input { margin-bottom: 16px; padding: 12px; border-radius: 8px; border: 1px solid rgba(255,255,255,0.25); background: rgba(0,0,0,0.6); color: #fff; font-size: 16px; box-sizing: border-box; width: 100%; }
        .rescan-btn { background: rgba(255,255,255,0.12); color: #fff; border: 1px solid rgba(255,255,255,0.3); border-radius: 6px; padding: 6px 12px; font-size: 13px; font-weight: bold; cursor: pointer; transition: 0.2s; }
        .rescan-btn:hover { background: rgba(255,255,255,0.25); }
        .btn-submit { padding: 14px; background: #ff2222; color: white; border: none; border-radius: 8px; font-size: 18px; font-weight: bold; cursor: pointer; transition: 0.2s; margin-top: 8px; }
        .btn-submit:hover { background: #d61515; }
    </style>
    <script>
        function scanWifi() {
            var btn = document.getElementById('btn-scan');
            if (btn) {
                btn.innerText = '⏳ Ricerca in corso...';
                btn.disabled = true;
            }
            fetch('/rescan').finally(() => {
                setTimeout(fetchResults, 2000);
            });
        }

        function fetchResults() {
            fetch('/scan_results')
                .then(r => r.json())
                .then(data => {
                    var sel = document.getElementById('wifi_select');
                    var curr = sel.value;
                    sel.innerHTML = '<option value="">-- Seleziona una rete rilevata (' + data.length + ' trovate) --</option>';
                    data.forEach(item => {
                        var opt = document.createElement('option');
                        opt.value = item.ssid;
                        opt.text = item.ssid + ' (' + item.rssi + ' dBm)';
                        sel.appendChild(opt);
                    });
                    if (curr) sel.value = curr;
                    var btn = document.getElementById('btn-scan');
                    if (btn) {
                        btn.innerText = '🔄 Riscansiona';
                        btn.disabled = false;
                    }
                })
                .catch(err => {
                    var btn = document.getElementById('btn-scan');
                    if (btn) {
                        btn.innerText = '🔄 Riscansiona';
                        btn.disabled = false;
                    }
                });
        }

        window.addEventListener('load', function() {
            var sel = document.getElementById('wifi_select');
            if (sel && sel.options.length <= 1) {
                scanWifi();
            }
        });
    </script>
</head>
<body>
    <div class="logo_top_container">
        <img src="/logo.webp" alt="Logo" class="logo_top_image">
    </div>

    <div class="form-container">
        <h2 style="color: #ff2222;">&#128246; Wi-Fi Config (Panigale-Mel-AP)</h2>
        <form action="/switch_wifi" method="get">
            <div style="display:flex; justify-content:space-between; align-items:center; margin-bottom: 8px;">
                <label for="wifi_select" style="margin-bottom:0;">Reti Wi-Fi rilevate:</label>
                <button type="button" id="btn-scan" class="rescan-btn" onclick="scanWifi()">🔄 Riscansiona</button>
            </div>
            <select id="wifi_select" onchange="if(this.value) document.getElementById('ssid').value = this.value;">
                <option value="">-- Seleziona una rete rilevata --</option>
)raw";

    if (esphome::wifi::global_wifi_component != nullptr) {
        for (auto &scan_res : esphome::wifi::global_wifi_component->get_scan_result()) {
            if (scan_res.get_is_hidden()) continue;
            String ssidName = String(scan_res.get_ssid().c_str());
            if (ssidName.length() == 0) continue;
            html += "<option value='" + ssidName + "'>" + ssidName + " (" + String(scan_res.get_rssi()) + " dBm)</option>";
        }
    }

    html += R"raw(
            </select>

            <label for="ssid">Nome Rete Wi-Fi (SSID):</label>
            <input type="text" id="ssid" name="ssid" placeholder="Seleziona dal menu oppure scrivi qui" required>

            <label for="password">Password Wi-Fi:</label>
            <input type="password" id="password" name="password" placeholder="Inserisci Password" required>

            <button type="submit" class="btn-submit">Connetti</button>
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
        <a href="/home" style="color: #ff2222; font-weight: bold; text-decoration: none;">Torna alla Home</a>
    </div>
    )raw" + NavigationManager::getNavbar() + R"raw(
</body>
</html>
)raw";
    return html;
}

} // namespace smarthome
