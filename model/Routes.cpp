#include "Routes.h"


void Routes::defineRoutes(AsyncWebServer &server)
{
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){ 
        String htmlContent = viewHome::generateHTML();
        //String htmlContent = "Hello World!";
        const char *htmlContentPtr = htmlContent.c_str();
        request->send(200, "text/html", htmlContentPtr); 
    });
    
    

   
    /*

    -a chiamata interrupt quanlche attivita che viene svolta

    server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request)
              {
        String message;
        if (request->hasParam(PARAM_MESSAGE)) {
            message = request->getParam(PARAM_MESSAGE)->value();
        } else {
            message = "No message sent";
        }
        request->send(200, "text/plain", "Hello, GET: " + message); });

    server.on("/post", HTTP_POST, [](AsyncWebServerRequest *request)
              {
        String message;
        if (request->hasParam(PARAM_MESSAGE, true)) {
            message = request->getParam(PARAM_MESSAGE, true)->value();
        } else {
            message = "No message sent";
        }
        request->send(200, "text/plain", "Hello, POST: " + message); });

    

    */

    cssRoutes css;
    css.defineRoutes(server);

    pinoutRoutes pinout;
    pinout.defineRoutes(server);

    ModbusRoutes modbus;
    modbus.defineRoutes(server);

    WifiRoutes wifi;
    wifi.defineRoutes(server);

    MonitorResource monitor;
    monitor.defineRoutes(server);


    // MultiplexRoutes multiplex;
    // multiplex.defineRoutes(server);

    

}