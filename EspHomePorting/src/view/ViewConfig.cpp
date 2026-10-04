#include "ViewConfig.h"
#include "NavigationManager.h"

namespace smarthome {

std::string ViewConfig::generateConfigPageHTML() {
    std::string html = R"raw(
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
            std::string ssidName = scan_res.get_ssid();
            if (ssidName.empty()) continue;
            html += "<option value='" + ssidName + "'>" + ssidName + " (" + std::to_string(scan_res.get_rssi()) + " dBm)</option>";
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

std::string ViewConfig::generateWifiSwitchSuccessHTML(const std::string &ssid, const std::string &ip) {
    bool hasIp = (!ip.empty() && ip != "0.0.0.0");
    std::string html = R"raw(
<!DOCTYPE html>
<html>
<head>
    <title>Wi-Fi Connesso</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="stylesheet" href="/glassmorphism.css">
    <style>
        )raw" + NavigationManager::getNavbarCss() + R"raw(
        body { font-family: 'Segoe UI', Arial, sans-serif; background-color: #020001; color: #fff; display: flex; flex-direction: column; align-items: center; justify-content: center; min-height: 100vh; margin: 0; padding: 20px; box-sizing: border-box; text-align: center; }
        .logo_top_container { text-align: center; margin-bottom: 25px; }
        .logo_top_image { max-width: 250px; height: auto; }
        .card { width: 90%; max-width: 480px; background: rgba(255,255,255,0.05); padding: 30px; border-radius: 20px; border: 1px solid rgba(255,255,255,0.15); box-shadow: 0 8px 32px rgba(0,0,0,0.8); backdrop-filter: blur(12px); }
        .ip-badge { background: rgba(0, 255, 102, 0.12); border: 1px solid #00ff66; padding: 14px 20px; border-radius: 12px; margin: 20px 0; }
        .ip-value { font-size: 26px; font-weight: bold; color: #00ff66; letter-spacing: 1px; margin: 6px 0; }
        .link-btn { display: inline-block; padding: 12px 24px; background: #ff2222; color: #fff; font-weight: bold; border-radius: 8px; text-decoration: none; margin-top: 15px; transition: 0.2s; font-size: 16px; }
        .link-btn:hover { background: #d61515; }
        .access-list { text-align: left; background: rgba(0,0,0,0.4); padding: 14px 20px; border-radius: 10px; margin: 15px 0; font-size: 15px; }
        .access-list a { color: #00e1ff; font-weight: bold; text-decoration: none; }
        .access-list a:hover { text-decoration: underline; }
    </style>
    <script>
        function checkIp() {
            fetch('/wifi_ip')
                .then(r => r.json())
                .then(data => {
                    if (data && data.ip && data.ip !== '0.0.0.0' && data.ip !== '') {
                        document.getElementById('status-title').innerHTML = '&#9989; Wi-Fi Connesso con Successo!';
                        document.getElementById('status-title').style.color = '#00ff66';
                        document.getElementById('ip-text').innerText = data.ip;
                        document.getElementById('ip-box').style.display = 'block';
                        document.getElementById('waiting-box').style.display = 'none';
                        var linkEl = document.getElementById('link-ip');
                        if (linkEl) {
                            linkEl.href = 'http://' + data.ip + '/';
                            linkEl.innerText = 'http://' + data.ip + '/';
                        }
                    }
                })
                .catch(e => console.log(e));
        }
        window.addEventListener('load', function() {
            var currIp = document.getElementById('ip-text').innerText.trim();
            if (!currIp || currIp === 'In attesa...' || currIp === '0.0.0.0') {
                setInterval(checkIp, 1000);
            }
        });
    </script>
</head>
<body>
    <div class="logo_top_container">
        <img src="/logo.webp" alt="Logo" class="logo_top_image">
    </div>
    <div class="card">
        <h2 id="status-title" style="color: )raw" + (hasIp ? "#00ff66" : "#ffcc00") + R"raw(;">
            )raw" + (hasIp ? "&#9989; Wi-Fi Connesso con Successo!" : "&#9203; Connessione in corso...") + R"raw(
        </h2>
        <p style="font-size: 17px; margin-bottom: 5px;">Rete Wi-Fi: <b>)raw" + ssid + R"raw(</b></p>

        <div id="ip-box" class="ip-badge" style="display: )raw" + (hasIp ? "block" : "none") + R"raw(;">
            <div style="font-size: 13px; text-transform: uppercase; color: #a0aec0; letter-spacing: 1px;">Indirizzo IP Ottenuto:</div>
            <div class="ip-value" id="ip-text">)raw" + (hasIp ? ip : "In attesa...") + R"raw(</div>
        </div>

        <div id="waiting-box" style="display: )raw" + (hasIp ? "none" : "block") + R"raw(; margin: 20px 0; color: #ffcc00;">
            <p>&#9203; Ricezione indirizzo IP dal router in corso...</p>
        </div>

        <div class="access-list">
            <p style="margin: 0 0 8px 0; font-weight: bold; color: #fff;">Indirizzi di accesso al pannello:</p>
            <div>&#127760; Hostname: <a href="http://panigalemel.local" target="_blank">http://panigalemel.local</a></div>
            <div style="margin-top: 6px;">&#128290; Indirizzo IP: <a id="link-ip" href=")raw" + (hasIp ? ("http://" + ip + "/") : "#") + R"raw(" target="_blank">)raw" + (hasIp ? ("http://" + ip + "/") : "In attesa IP...") + R"raw(</a></div>
        </div>

        <p style="font-size: 13px; color: #888; margin-top: 15px;">
            L'Access Point d'emergenza (<b>Panigale-Mel-AP</b> su <b>192.168.4.1</b>) rimane sempre attivo in parallelo.
        </p>
        <a href="/home" class="link-btn">Vai alla Dashboard</a>
    </div>
    )raw" + NavigationManager::getNavbar() + R"raw(
</body>
</html>
)raw";
    return html;
}

} // namespace smarthome
