#include "MultiplexRoutes.h"
#include <vector>
#include <set> 

void MultiplexRoutes::defineRoutes(AsyncWebServer &server){

    server.on("/multiplex_config", HTTP_GET, [](AsyncWebServerRequest *request){
        String htmlContent = viewMultiplex::Config();
        const char *htmlContentPtr = htmlContent.c_str();
        request->send(200, "text/html", htmlContentPtr); 
         
    });

    server.on("/multiplex_graph", HTTP_GET, [](AsyncWebServerRequest *request){
        

        String signalType;
        String action ;
        String milliseconds;

        Serial.println("selected Signal type: " + signalType);

        if (request->hasParam("signalType"))
        {
            signalType = request->getParam("signalType")->value();
            Serial.println("Signal type: " + signalType);
        }
        else
        {
        error_param:
            Serial.println("Errore parametri!");
            request->send(400, "text/plain", "Error: Missing parameter");
            return;
        }
        
        if (request->hasParam("action"))
        {
            action = request->getParam("action")->value();
            Serial.println("Action recived: " + action);
        }
        else
        {
            action = "first_entry_on_this_page";
        }
        if (request->hasParam("milliseconds"))
        {
            milliseconds = request->getParam("milliseconds")->value();
            Serial.println("Milliseconds: " + milliseconds);
        }
        else
        {
            milliseconds = "1000";
        }
        
        int PinNumber = 23; //verra ricornato da ADC Controller
        std::vector<int> pinNumbers;
        pinNumbers.push_back(PinNumber);

        if (action == "start_recording")
{
            SystemState::getInstance()->pinoutData->getPin(PinNumber).startRecording(milliseconds.toInt());
        }
        else if (action == "stop_recording")
        {
            SystemState::getInstance()->pinoutData->getPin(PinNumber).stopRecording();
        }
        else
        {
            // Azione di default 
        }

       
        Serial.println("Action completed, preparing response...");

    

        String htmlContent = viewGeneric::defaultCssHeader("Graph View");

        
        htmlContent+= viewMultiplex::pinStartAndStopForm( PinNumber,  signalType);
        
        htmlContent += viewGraph::generateGraph(pinNumbers, "getPinValues", "pin");
        const char *htmlContentPtr = htmlContent.c_str();
        request->send(200, "text/html", htmlContentPtr); 
         
    });
}