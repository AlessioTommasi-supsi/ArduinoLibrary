#include "MultiplexRoutes.h"
#include <vector>
#include <set> 


ADS1115_controller MultiplexRoutes::adsController;


void MultiplexRoutes::defineRoutes(AsyncWebServer &server){

    server.on("/multiplex_config", HTTP_GET, [](AsyncWebServerRequest *request){
        String htmlContent = viewMultiplex::Config();
        const char *htmlContentPtr = htmlContent.c_str();
        request->send(200, "text/html", htmlContentPtr); 
         
    });

    server.on("/getADSValues", HTTP_GET, [](AsyncWebServerRequest *request){
        if(adsController.initializationFailed) {
            request->send(500, "text/html", "<h1>Error: ADS1115 Initialization Failed.</h1>");
            return;
        }
        
        const std::vector<float>& values = adsController.getRecordedValues();
        String json = "[";
        for (size_t i = 0; i < values.size(); i++) {
            if (i > 0)
                json += ",";
            json += String(values[i]);
        }
        json += "]";
        
        request->send(200, "application/json", json);
    });
    

    server.on("/multiplex_graph", HTTP_GET, [](AsyncWebServerRequest *request){
        String signalType;
        String action;
        String milliseconds;

        String ErrorMessage = "";
        
        if (request->hasParam("signalType")) {
            signalType = request->getParam("signalType")->value();
            Serial.println("Signal type: " + signalType);
        } else {
            request->send(400, "text/plain", "Error: Missing parameter signalType");
            return;
        }
        
        if (request->hasParam("action")) {
            action = request->getParam("action")->value();
            Serial.println("Action received: " + action);
        } else {
            action = "first_entry_on_this_page";
        }
        
        if (request->hasParam("milliseconds")) {
            milliseconds = request->getParam("milliseconds")->value();
            Serial.println("Milliseconds: " + milliseconds);
        } else {
            milliseconds = "1000";
        }
        
        if (action == "start_recording") {
            adsController.startRecording(signalType, milliseconds.toInt());
        } else if (action == "stop_recording") {
            adsController.stopRecording();
        }
        
        if(adsController.initializationFailed) {
            // Nel caso in cui l'inizializzazione fallisca, rispondi subito con un messaggio di errore
            //request->send(500, "text/html", "<h1>Error: ADS1115 Initialization Failed.</h1>");
            ErrorMessage = "<h1>Error: ADS1115 Initialization Failed.</h1>";
            Serial.println(ErrorMessage);
        }
        
        Serial.println("Action completed, preparing response...");
        
        std::vector<int> channelVector = { adsController.getCurrentChannel() };
        String htmlContent = viewGeneric::defaultCssHeader("Graph View");
        htmlContent += viewMultiplex::pinStartAndStopForm(adsController.getCurrentChannel(), signalType);
        htmlContent += viewGraph::generateGraph(channelVector, "getADSValues", "ads1115");
        
        htmlContent += ErrorMessage;
        request->send(200, "text/html", htmlContent);
    });
}