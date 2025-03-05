#include "WebServer.h"



WebServer::WebServer(const char *ssid, const char *password) : ssid(ssid), password(password) {}

void WebServer::begin()
{
    /* se non si vuole usare il wifi manager
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED)
    {
        Serial.println("WiFi Failed!");
        //return;
    }
    */
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    
   
    server.onNotFound([this](AsyncWebServerRequest *request)
                      { this->notFound(request); });

lbl_defineRoutes:
    try
    {
        Routes::defineRoutes(server);
    }
    catch(...)
    {
        Serial.println("Errore nella definizione delle rotte");
        //in tutte le eorre ritorno messaggui du errore
        server.reset();//non sicuro che sia corretta
        goto lbl_defineRoutes;
    }
    
    
    server.begin();
    Serial.println("Server started");
}

void WebServer::notFound(AsyncWebServerRequest *request)
{
    request->send(404, "text/plain", "Not found");
}