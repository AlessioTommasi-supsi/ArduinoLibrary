#include "ViewServices.h"
#include "NavigationManager.h"

namespace smarthome {

String ViewServices::generateCustomButtonsHTML(const std::vector<CustomButtonItem> &customButtons) {
    String html = "";
    for (const auto &btn : customButtons) {
        html += "<div class='emoji-button-wrapper'>";
        html += "  <button class='emoji-button' onclick=\"fetchData(this, '" + btn.url + "')\">";
        html += "    " + btn.emoji;
        html += "    <label>" + btn.label + "</label>";
        html += "    <div class='loading-icon'>&#8987;</div>";
        html += "  </button>";
        html += "  <button class='delete-btn' onclick=\"window.location.href='/removeButton?label=" + btn.label + "'\">&#10060;</button>";
        html += "</div>";
    }
    return html;
}

String ViewServices::generateServicesHTML(const std::vector<CustomButtonItem> &customButtons) {
    String html = R"raw(
<!DOCTYPE html>
<html>
<head>
    <title>Panigale - Controllo Luci</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="stylesheet" href="/glassmorphism.css">
    <style>
        )raw" + NavigationManager::getNavbarCss() + R"raw(
        body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; background-color: #020001; color: #fff; margin: 0; padding: 20px; text-align: center; padding-bottom: 120px; }
        .logo_top_container { text-align: center; margin-top: 10px; margin-bottom: 25px; }
        .logo_top_image { max-width: 250px; height: auto; }
        .glass_container { background: rgba(255, 255, 255, 0.04); backdrop-filter: blur(12px); border-radius: 20px; padding: 25px; border: 1px solid rgba(255, 255, 255, 0.12); box-shadow: 0 8px 32px 0 rgba(0, 0, 0, 0.8); margin: 0 auto; max-width: 1000px; }
        .emoji-button-container { display: flex; flex-wrap: wrap; justify-content: center; gap: 20px; }
        .emoji-button-wrapper { position: relative; flex: 0 1 calc(33.333% - 20px); max-width: calc(33.333% - 20px); min-width: 240px; }
        .image-button { position: relative; width: 100%; height: 220px; border: 2px solid rgba(255, 255, 255, 0.2); border-radius: 16px; overflow: hidden; padding: 0; cursor: pointer; background: #000; box-shadow: 0 8px 20px rgba(0, 0, 0, 0.6); transition: transform 0.2s ease, box-shadow 0.2s ease, border-color 0.2s ease; }
        .image-button:hover { transform: translateY(-4px) scale(1.02); border-color: #ff1a1a; box-shadow: 0 12px 25px rgba(255, 26, 26, 0.35); }
        .image-button:active, .image-button.loading-state { transform: scale(0.97); opacity: 0.85; }
        .image-button .btn-img { width: 100%; height: 100%; object-fit: cover; display: block; }
        .loading-icon { display: none; position: absolute; top: 10px; right: 12px; font-size: 28px; z-index: 10; filter: drop-shadow(0 0 6px rgba(0,0,0,0.9)); }
        .loading-state .loading-icon { display: block; animation: pulse-spin 0.5s ease-in-out; }
        @keyframes pulse-spin { 0% { transform: scale(0.8) rotate(0deg); } 50% { transform: scale(1.2) rotate(90deg); } 100% { transform: scale(1.0) rotate(180deg); } }
        .emoji-button { background: rgba(255, 255, 255, 0.08); border: 1px solid rgba(255, 255, 255, 0.15); border-radius: 16px; width: 100%; height: 220px; display: flex; flex-direction: column; align-items: center; justify-content: center; font-size: 70px; color: #fff; cursor: pointer; transition: 0.2s ease; position: relative; }
        .emoji-button:hover { transform: translateY(-4px) scale(1.02); background: rgba(255, 255, 255, 0.18); border-color: #ff1a1a; }
        .emoji-button label { display: block; margin-top: 10px; font-size: 18px; font-weight: 700; color: #f1f5f9; }
        .delete-btn { position: absolute; top: 8px; right: 8px; background: rgba(255, 0, 0, 0.85); border: none; color: white; border-radius: 50%; width: 32px; height: 32px; cursor: pointer; z-index: 12; font-size: 14px; }
    </style>
    <script>
        function fetchData(button, apiUrl) {
            button.classList.add('loading-state');
            fetch(apiUrl)
                .then(r => r.text())
                .catch(err => console.error(err));
            setTimeout(() => {
                button.classList.remove('loading-state');
            }, 500);
        }
    </script>
</head>
<body>
    <!-- Logo Locale in Alto al Centro -->
    <div class="logo_top_container">
        <img src="/logo.webp" alt="Logo" class="logo_top_image">
    </div>

    <div class="glass_container">
        <h2 style="color: #ff2222; text-transform: uppercase; letter-spacing: 2px;">Controllo Luci</h2>
        <div class="emoji-button-container">
            <!-- 1. Posizione (GPIO 4) -->
            <div class="emoji-button-wrapper">
                <button class="image-button" onclick="fetchData(this, 'pulsePin?pin=4')">
                    <img src="/posizione.webp" alt="Posizione" class="btn-img">
                    <div class="loading-icon">&#8987;</div>
                </button>
            </div>

            <!-- 2. Anabbagliante (GPIO 5) -->
            <div class="emoji-button-wrapper">
                <button class="image-button" onclick="fetchData(this, 'pulsePin?pin=5')">
                    <img src="/anabbagliante.webp" alt="Anabbagliante" class="btn-img">
                    <div class="loading-icon">&#8987;</div>
                </button>
            </div>

            <!-- 3. Abbagliante (GPIO 6) -->
            <div class="emoji-button-wrapper">
                <button class="image-button" onclick="fetchData(this, 'pulsePin?pin=6')">
                    <img src="/abbagliante.webp" alt="Abbagliante" class="btn-img">
                    <div class="loading-icon">&#8987;</div>
                </button>
            </div>

            <!-- Bottoni Dinamici Custom NVS -->
)raw" + generateCustomButtonsHTML(customButtons) + R"raw(
            <!-- Tasto Aggiungi Bottone -->
            <div class="emoji-button-wrapper">
                <button class="emoji-button" onclick="window.location.href='/addButton'">
                    &#10133;
                    <label style="color: #ff2222;">Aggiungi Bottone</label>
                </button>
            </div>
        </div>
    </div>
    )raw" + NavigationManager::getNavbar() + R"raw(
</body>
</html>
)raw";
    return html;
}

String ViewServices::generateAddButtonFormHTML() {
    String html = R"raw(
<!DOCTYPE html>
<html>
<head>
    <title>Aggiungi Bottone</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        )raw" + NavigationManager::getNavbarCss() + R"raw(
        body { font-family: 'Segoe UI', Arial; background-color: #020001; color: #fff; padding: 20px; display: flex; flex-direction: column; align-items: center; padding-bottom: 120px; }
        .logo_top_container { text-align: center; margin-top: 10px; margin-bottom: 25px; }
        .logo_top_image { max-width: 200px; height: auto; }
        .form-container { width: 100%; max-width: 450px; background: rgba(255, 255, 255, 0.05); backdrop-filter: blur(10px); padding: 25px; border-radius: 16px; border: 1px solid rgba(255, 255, 255, 0.15); box-shadow: 0 8px 32px rgba(0,0,0,0.8); }
        form { display: flex; flex-direction: column; }
        label { margin: 10px 0 5px; font-weight: bold; color: #ff2222; }
        input { padding: 12px; border-radius: 8px; border: 1px solid rgba(255,255,255,0.25); background: rgba(0,0,0,0.6); color: #fff; font-size: 16px; }
        button { margin-top: 20px; padding: 14px; background: #ff2222; color: white; border: none; border-radius: 8px; font-size: 18px; font-weight: bold; cursor: pointer; transition: 0.2s; }
        button:hover { background: #d61515; }
    </style>
</head>
<body>
    <div class="logo_top_container">
        <img src="/logo.webp" alt="Logo" class="logo_top_image">
    </div>
    <div class="form-container">
        <h2 style="color: #ff2222;">&#10133; Aggiungi Nuovo Bottone</h2>
        <form action="/saveButton" method="get">
            <label for="label">Etichetta Bottone:</label>
            <input type="text" id="label" name="label" placeholder="Es. Fendinebbia" required>
            
            <label for="emoji">Emoji (Icona):</label>
            <input type="text" id="emoji" name="emoji" placeholder="Es. 💡" required>
            
            <label for="url">URL / API Route:</label>
            <input type="text" id="url" name="url" placeholder="Es. pulsePin?pin=7" required>
            
            <button type="submit">Salva Bottone</button>
        </form>
    </div>
    )raw" + NavigationManager::getNavbar() + R"raw(
</body>
</html>
)raw";
    return html;
}

} // namespace smarthome
