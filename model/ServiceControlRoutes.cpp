#include "ServiceControlRoutes.h"
#include "Config.h"
#include "viewGeneric.h"
#include "CustomButtonRoutes.h"

void ServiceControlRoutes::defineRoutes(AsyncWebServer &server)
{
    // Route principale per la pagina services
    server.on("/services", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        try 
        {
            String content = generateServicesContent();
            request->send(200, "text/html", content);
        }
        catch (const std::exception &e)
        {
            request->send(500, "text/html", "Error: " + String(e.what()));
        }
        catch (...)
        {
            request->send(500, "text/html", "Unknown error occurred");
        }
    });

    // Route per solo i bottoni senza struttura pagina (per home page)
    server.on("/servicesButtonsOnly", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        try 
        {
            String content = "";
            
            // CSS per i bottoni emoji
            content += viewGeneric::addEmoji_buttonStyleCss();
            
            content += "  <div class=\"next_page\" id=\"aboutMeTitle\">";
            content += "    <div class=\"glass_container\">";
            content += "      <div class=\"emoji-button-container\">";
            
            // Bottoni predefiniti
            content += generateServiceControlsContent();
            
            // Bottoni personalizzati
            content += CustomButtonRoutes::generateCustomButtonsContent();
            
            // Bottone "+" per aggiungere nuovi bottoni (SEMPRE ULTIMO)
            content += "        <button class=\"emoji-button\" onclick=\"window.location.href='/addButton'\">";
            content += "          &#x2795;"; // ➕
            content += "          <label>Aggiungi Bottone</label>";
            content += "        </button>";
            
            content += "      </div>";
            content += "    </div>";
            content += "    <br><br><br><br>";
            content += "  </div>";
            
            request->send(200, "text/html", content);
        }
        catch (...)
        {
            request->send(500, "text/html", "Error loading services buttons");
        }
    });
}

String ServiceControlRoutes::generateServicesContent()
{
    String content = viewGeneric::defaultCssHeader("Smart Home Services");
    
    // Aggiungi script JavaScript per fetch dinamico
    content += generateServicesJavaScript();
    
    // Aggiungi CSS per i bottoni emoji
    content += viewGeneric::addEmoji_buttonStyleCss();
    
    content += "  <div class=\"next_page\" id=\"aboutMeTitle\">";
    content += "    <div class=\"glass_container\">";
    content += "      <div class=\"emoji-button-container\">";
    
    // Bottoni predefiniti
    content += generateServiceControlsContent();
    
    // Contenitore per bottoni personalizzati (caricamento dinamico)
    content += "        <div id=\"customButtonsContainer\">";
    content += "        </div>";
    
    // Bottone "+" per aggiungere nuovi bottoni
    content += "        <button class=\"emoji-button\" onclick=\"window.location.href='/addButton'\">";
    content += "          &#x2795;"; // ➕
    content += "          <label>Aggiungi Bottone</label>";
    content += "        </button>";
    
    content += "      </div>";
    content += "    </div>";
    content += "    <br><br><br><br>";
    content += "  </div>";
    
    // Script per caricare i bottoni personalizzati dinamicamente
    content += "<script>";
    content += "document.addEventListener('DOMContentLoaded', function() {";
    content += "  loadPageContent('/customButtons', 'customButtonsContainer', " + String(UI_AUTO_UPDATE_DISABLED) + ");";
    content += "});";
    content += "</script>";
    
    content += viewGeneric::defaultFooter();
    
    return content;
}

String ServiceControlRoutes::generateServiceControlsContent()
{
    String content = "";
    
    // Bottone Porta Studio
    content += "        <button class=\"emoji-button\" onclick=\"fetchData(this, 'pulsePin?pin=32')\">";
    content += "          &#x1F6AA;"; // 🚪
    content += "          <label>Porta studio</label>";
    content += "          <div class=\"loading-icon\">&#x23F3;</div>"; // ⏳
    content += "        </button>";
    
    // Bottone Luce Interna Studio
    content += "        <button class=\"emoji-button\" onclick=\"fetchData(this, 'pulsePin?pin=33')\">";
    content += "          &#x1F4A1;"; // 💡
    content += "          <label>Luce interna studio</label>";
    content += "          <div class=\"loading-icon\">&#x23F3;</div>";
    content += "        </button>";
    
    // Bottone Luce Esterna Studio
    content += "        <button class=\"emoji-button\" onclick=\"fetchData(this, 'pulsePin?pin=25')\">";
    content += "          &#x1F4A1;"; // 💡
    content += "          <label>Luce esterna studio</label>";
    content += "          <div class=\"loading-icon\">&#x23F3;</div>";
    content += "        </button>";
    
    // Bottone Luce Presepe
    content += "        <button class=\"emoji-button\" onclick=\"fetchData(this, 'pulsePin?pin=26')\">";
    content += "          &#x1F4A1;"; // 💡
    content += "          <label>Luce presepe</label>";
    content += "          <div class=\"loading-icon\">&#x23F3;</div>";
    content += "        </button>";
    
    // Bottone Luce Cantina
    content += "        <button class=\"emoji-button\" onclick=\"fetchData(this, 'pulsePin?pin=27')\">";
    content += "          &#x1F4A1;"; // 💡
    content += "          <label>Luce cantina</label>";
    content += "          <div class=\"loading-icon\">&#x23F3;</div>";
    content += "        </button>";
    
    // Bottone Luce Camino
    content += "        <button class=\"emoji-button\" onclick=\"fetchData(this, 'pulsePin?pin=14')\">";
    content += "          &#x1F4A1;"; // 💡
    content += "          <label>Luce camino</label>";
    content += "          <div class=\"loading-icon\">&#x23F3;</div>";
    content += "        </button>";
    
    return content;
}

String ServiceControlRoutes::generateServicesJavaScript()
{
    String script = R"(
        <script>
            // JavaScript per bottoni di servizio - fetchData è già definita globalmente in viewGeneric
            console.log('🔧 ServiceControlRoutes JavaScript caricato');
        </script>
    )";
    
    return script;
}