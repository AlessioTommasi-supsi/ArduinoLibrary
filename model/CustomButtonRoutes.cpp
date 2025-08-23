#include "CustomButtonRoutes.h"
#include "Config.h"
#include "viewGeneric.h"

CustomButtonManager* CustomButtonRoutes::customButtonManager = nullptr;

void CustomButtonRoutes::defineRoutes(AsyncWebServer &server)
{
    // Inizializza il manager dei bottoni personalizzati
    initializeCustomButtonManager();
    
    // Route per caricamento dinamico dei bottoni personalizzati
    server.on("/customButtons", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        try 
        {
            String content = generateCustomButtonsContent();
            request->send(200, "text/html", content);
        }
        catch (...)
        {
            request->send(500, "text/html", "Error loading custom buttons");
        }
    });
    
    // Route per il form di aggiunta bottone
    server.on("/addButton", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        try 
        {
            String content = generateAddButtonContent();
            request->send(200, "text/html", content);
        }
        catch (...)
        {
            request->send(500, "text/html", "Error loading add button form");
        }
    });
    
    // Route per salvare un nuovo bottone
    server.on("/saveButton", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        try 
        {
            if (!request->hasParam("label") || !request->hasParam("emoji") || !request->hasParam("url")) {
                request->send(400, "text/html", "Missing required parameters");
                return;
            }
            
            String label = request->getParam("label")->value();
            String emoji = request->getParam("emoji")->value();
            String url = request->getParam("url")->value();
            
            if (label.length() == 0 || emoji.length() == 0 || url.length() == 0) {
                request->send(400, "text/html", "All fields are required");
                return;
            }
            
            if (customButtonManager->addButton(label, emoji, url)) {
                // Reindirizza alla home page con messaggio di successo
                request->redirect("/?success=1");
            } else {
                request->send(400, "text/html", "Failed to add button - may be full or duplicate label");
            }
        }
        catch (...)
        {
            request->send(500, "text/html", "Error saving button");
        }
    });
    
    // Route per rimuovere un bottone
    server.on("/removeButton", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        try 
        {
            if (!request->hasParam("label")) {
                request->send(400, "text/html", "Missing label parameter");
                return;
            }
            
            String label = request->getParam("label")->value();
            
            if (customButtonManager->removeButton(label)) {
                // Reindirizza alla home page con messaggio di rimozione
                request->redirect("/?removed=1");
            } else {
                request->send(400, "text/html", "Failed to remove button");
            }
        }
        catch (...)
        {
            request->send(500, "text/html", "Error removing button");
        }
    });
}

String CustomButtonRoutes::generateAddButtonContent()
{
    String content = viewGeneric::defaultCssHeader("Aggiungi Nuovo Bottone");
    
    // Aggiungi CSS per i form
    content += viewGeneric::addFormStyleCss();
    
    content += "  <div class=\"form-container\">";
    content += "    <h2>Aggiungi Nuovo Bottone</h2>";
    content += "    <form action=\"/saveButton\" method=\"get\">";
    
    content += "      <label for=\"label\">Etichetta Bottone:</label>";
    content += "      <input type=\"text\" id=\"label\" name=\"label\" required maxlength=\"" + String(MAX_BUTTON_LABEL_LENGTH) + "\" placeholder=\"Es: Luce Soggiorno\">";
    
    content += "      <label for=\"emoji\">Emoji (codice Unicode o emoji):</label>";
    content += "      <input type=\"text\" id=\"emoji\" name=\"emoji\" required maxlength=\"" + String(MAX_BUTTON_EMOJI_LENGTH) + "\" placeholder=\"Es: 💡 o &#x1F4A1;\">";
    
    content += "      <label for=\"url\">URL da chiamare:</label>";
    content += "      <input type=\"text\" id=\"url\" name=\"url\" required maxlength=\"" + String(MAX_BUTTON_URL_LENGTH) + "\" placeholder=\"Per ESP locale: pulsePin?pin=15 | Per siti esterni: http://example.com/api\">";
    content += "      <small style=\"color: #666; font-size: 12px;\">💡 Usa URL completi (http://...) per chiamate esterne, URL relativi per l'ESP locale</small><br><br>";
    
    content += "      <button type=\"submit\">Salva Bottone</button>";
    content += "    </form>";
    
    content += "    <br>";
    content += "    <button onclick=\"window.location.href='/services'\" style=\"background-color: #888;\">Annulla</button>";
    content += "  </div>";
    
    content += viewGeneric::defaultFooter();
    
    return content;
}

String CustomButtonRoutes::generateCustomButtonsContent()
{
    if (customButtonManager == nullptr) {
        initializeCustomButtonManager();
    }
    
    return customButtonManager->generateButtonsHTML();
}

void CustomButtonRoutes::initializeCustomButtonManager()
{
    if (customButtonManager == nullptr) {
        customButtonManager = new CustomButtonManager();
    }
}

CustomButtonManager* CustomButtonRoutes::getCustomButtonManager()
{
    if (customButtonManager == nullptr) {
        initializeCustomButtonManager();
    }
    return customButtonManager;
}