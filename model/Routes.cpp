#include "Routes.h"

void Routes::defineRoutes(AsyncWebServer &server)
{
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){ 
        String htmlContent = viewHome::generateHTML();
        const char *htmlContentPtr = htmlContent.c_str();
        request->send(200, "text/html", htmlContentPtr); 
    });

    // Inizializza tutte le route specializzate
    cssRoutes css;
    css.defineRoutes(server);

    pinoutRoutes pinout;
    pinout.defineRoutes(server);

    WifiRoutes wifi;
    wifi.defineRoutes(server);

    MonitorResource monitor;
    monitor.defineRoutes(server);

    // Nuove classi specializzate per i bottoni
    CustomButtonRoutes customButtons;
    customButtons.defineRoutes(server);

    ServiceControlRoutes serviceControl;
    serviceControl.defineRoutes(server);

    HomeRoutes home;
    home.defineRoutes(server);
}