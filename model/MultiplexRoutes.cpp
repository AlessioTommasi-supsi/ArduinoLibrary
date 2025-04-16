#include "MultiplexRoutes.h"


void MultiplexRoutes::defineRoutes(AsyncWebServer &server){

    server.on("/multiplex_config", HTTP_GET, [](AsyncWebServerRequest *request){
        String htmlContent = viewMultiplex::Config();
        const char *htmlContentPtr = htmlContent.c_str();
        request->send(200, "text/html", htmlContentPtr); 
         
    });

    server.on("/multiplex_graph", HTTP_GET, [](AsyncWebServerRequest *request){
        String htmlContent = viewGraph::generateHTML();
        const char *htmlContentPtr = htmlContent.c_str();
        request->send(200, "text/html", htmlContentPtr); 
         
    });
}