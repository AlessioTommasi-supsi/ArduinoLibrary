#include "Routes.h"


void Routes::defineRoutes(AsyncWebServer &server)
{
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){ 
        String htmlContent = viewHome::generateHTML();
        //String htmlContent = "Hello World!";
        const char *htmlContentPtr = htmlContent.c_str();
        request->send(200, "text/html", htmlContentPtr); 
    });
    
    // **🔧 FIX: Route per Apply/Save EditPin che non funzionavano**
    server.on("/applyPinChanges", HTTP_POST, [](AsyncWebServerRequest *request){
        try {
            if (request->hasParam("pin", true) && request->hasParam("pinType", true) && 
                request->hasParam("isInput", true) && request->hasParam("outputValue", true) && 
                request->hasParam("pinNote", true)) {
                
                String pinNumber = request->getParam("pin", true)->value();
                String pinType = request->getParam("pinType", true)->value();
                bool isInput = request->getParam("isInput", true)->value() == "true";
                float outputValue = request->getParam("outputValue", true)->value().toFloat();
                String pinNote = request->getParam("pinNote", true)->value();
                
                // Applica le modifiche al pin
                Pin &pin = SystemState::getInstance()->pinoutData->getPin(pinNumber.toInt());
                pin.setType(pinType);
                pin.setIsInput(isInput);
                pin.setNote(pinNote.c_str());
                
                if (!isInput && outputValue > 0) {
                    pin.write(outputValue > 0);
                }
                
                Serial.println("Pin " + pinNumber + " modificato con successo");
                request->send(200, "text/plain", "Pin modifications applied successfully");
            } else {
                request->send(400, "text/plain", "Missing required parameters");
            }
        } catch (...) {
            request->send(500, "text/plain", "Error applying pin changes");
        }
    });
    
    server.on("/savePinChanges", HTTP_POST, [](AsyncWebServerRequest *request){
        try {
            // Salva tutte le modifiche ai pin nella memoria persistente
            SystemState::getInstance()->pinoutData->savePinsToMemory();
            Serial.println("Pin changes saved to memory");
            request->send(200, "text/plain", "Pin changes saved successfully");
        } catch (...) {
            request->send(500, "text/plain", "Error saving pin changes");
        }
    });
    
    // Nuove rotte per caricamento dinamico dei contenuti frammentati
    server.on("/homePageContent", HTTP_GET, [](AsyncWebServerRequest *request){
        try {
            String content = viewHome::pageContent();
            AsyncWebServerResponse *response = request->beginResponse(200, "text/html", content);
            response->addHeader("Access-Control-Allow-Origin", "*");
            response->addHeader("Cache-Control", "no-cache");
            request->send(response);
        } catch (...) {
            request->send(500, "text/html", "Error loading home content");
        }
    });

    server.on("/pinoutPageContent", HTTP_GET, [](AsyncWebServerRequest *request){
        try {
            String content = Pinout::pageContent();
            AsyncWebServerResponse *response = request->beginResponse(200, "text/html", content);
            response->addHeader("Access-Control-Allow-Origin", "*");
            response->addHeader("Cache-Control", "no-cache");
            request->send(response);
        } catch (...) {
            request->send(500, "text/html", "Error loading pinout content");
        }
    });

    server.on("/editPinPageContent", HTTP_GET, [](AsyncWebServerRequest *request){
        try {
            String content = "";
            if (request->hasParam("pinNumber")) {
                int pinNumber = request->getParam("pinNumber")->value().toInt();
                
                // CSS inline minimalista per il form
                content += "<style>";
                content += ".form-container{background:rgba(255,255,255,0.95);border-radius:10px;padding:20px;margin:20px auto;max-width:500px}";
                content += "form{display:flex;flex-direction:column}label{font-weight:bold;margin-bottom:5px;font-size:14px}";
                content += "input,select{margin-bottom:10px;padding:8px;border:1px solid #ccc;border-radius:4px;font-size:14px}";
                content += "button{padding:10px;background:#4CAF50;color:white;border:none;border-radius:4px;cursor:pointer;font-size:14px;margin:5px 0}";
                content += "button:hover{background:#45a049}";
                content += "</style>";
                
                content += "<h1 style='text-align:center;margin:20px 0'>✏ Edit Pin " + String(pinNumber) + "</h1>";
                content += viewEditPin::generateForm(pinNumber);
                content += viewEditPin::addDefaultScript();
            } else {
                content = "<h1>Error: Pin number required</h1>";
            }
            
            AsyncWebServerResponse *response = request->beginResponse(200, "text/html", content);
            response->addHeader("Access-Control-Allow-Origin", "*");
            response->addHeader("Cache-Control", "no-cache");
            request->send(response);
        } catch (...) {
            request->send(500, "text/html", "Error loading edit pin content");
        }
    });

    server.on("/currentRegisterPageContent", HTTP_GET, [](AsyncWebServerRequest *request){
        try {
            String content = "";
            String registerAddress = request->hasParam("registerAddress") ? request->getParam("registerAddress")->value() : "";
            float registerValue = request->hasParam("registerValue") ? request->getParam("registerValue")->value().toFloat() : 0.0;
            
            content = viewCurrentRegister::pageContent(registerAddress, registerValue);
            
            AsyncWebServerResponse *response = request->beginResponse(200, "text/html", content);
            response->addHeader("Access-Control-Allow-Origin", "*");
            response->addHeader("Cache-Control", "no-cache");
            request->send(response);
        } catch (...) {
            request->send(500, "text/html", "Error loading register content");
        }
    });

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

    MultiplexRoutes multiplex;
    multiplex.defineRoutes(server);

    

}