#include "servicesRoutes.h"
#include "Config.h"

void servicesRoutes::defineRoutes(AsyncWebServer &server)
{
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
}

String servicesRoutes::generateServicesContent()
{
    String content = "";
    
    // Sezione servizi smart home
    content += "  <div class=\"next_page\" id=\"aboutMeTitle\">";
    content += "    <div class=\"glass_container\">";
    content += "      <div class=\"emoji-button-container\">";
    
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
    
    content += "      </div>";
    content += "    </div>";
    content += "    <br><br><br><br>";
    content += "  </div>";
    
    return content;
}