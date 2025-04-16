#include "MultiplexRoutes.h"


void MultiplexRoutes::defineRoutes(AsyncWebServer &server){

    server.on("/multiplex_config", HTTP_GET, [](AsyncWebServerRequest *request){
        String htmlContent = viewMultiplex::Config();
        const char *htmlContentPtr = htmlContent.c_str();
        request->send(200, "text/html", htmlContentPtr); 
         
    });

    server.on("/multiplex_graph", HTTP_GET, [](AsyncWebServerRequest *request){
        String htmlContent = viewGraph::generateHTML();

        String signalType = request->getParam("signalType")->value();

        Serial.println("selected Signal type: " + signalType);



        int PinNumber = 0; //verra ricornato da ADC Controller

        htmlContent += viewMultiplex::pinStartAndStopForm( PinNumber,  signalType);
        const char *htmlContentPtr = htmlContent.c_str();
        request->send(200, "text/html", htmlContentPtr); 
         
    });
}