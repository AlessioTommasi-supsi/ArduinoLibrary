#include "Routes.h"


void Routes::defineRoutes(AsyncWebServer &server)
{
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "text/plain", "Hello, world"); });

    

    /*
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

    server.on("/history", HTTP_GET, [](AsyncWebServerRequest *request){
                String htmlContent = viewHistory::generateHTML();
                const char *htmlContentPtr = htmlContent.c_str();
                request->send(200, "text/html", htmlContentPtr); 
    });

    server.on("/deleteRegister", HTTP_GET, [](AsyncWebServerRequest *request){
                String registerAddress = request->getParam("index")->value();
                SystemState::getInstance()->deleteValue(registerAddress.toInt());
                String htmlContent = viewHistory::generateHTML();
                const char *htmlContentPtr = htmlContent.c_str();
                request->send(200, "text/html", htmlContentPtr); 
    });

    server.on("/editRegister", HTTP_GET, [](AsyncWebServerRequest *request){
                String registerAddress = request->getParam("index")->value();
                String registerValue = request->getParam("value")->value();
                SystemState::getInstance()->editValue(registerAddress.toInt(), registerValue.toFloat());
                String htmlContent = viewHistory::generateHTML();
                const char *htmlContentPtr = htmlContent.c_str();
                request->send(200, "text/html", htmlContentPtr); 
    });

    server.on("/currentregister", HTTP_GET, [](AsyncWebServerRequest *request){
                String htmlContent = viewCurrentRegister::generateHTML();
                const char *htmlContentPtr = htmlContent.c_str();
                request->send(200, "text/html", htmlContentPtr); 
    });

    server.on("/modbusMaster", HTTP_GET, [](AsyncWebServerRequest *request){
                String registerAddress = request->getParam("registerAddress")->value();
                String registerType = request->getParam("registerType")->value();

                if(registerType == "int")
                {
                    float registerValue = SystemState::masterModbus->readHoldingIntRegisters(registerAddress.toInt());
                    String htmlContent = viewCurrentRegister::generateHTML(registerAddress, registerValue);
                    const char *htmlContentPtr = htmlContent.c_str();
                    request->send(200, "text/html", htmlContentPtr); 
                }
                else if(registerType == "float")
                {
                    float registerValue = SystemState::masterModbus->readHoldingFloatRegisters(registerAddress.toInt());
                    String htmlContent = viewCurrentRegister::generateHTML(registerAddress, registerValue);
                    const char *htmlContentPtr = htmlContent.c_str();
                    request->send(200, "text/html", htmlContentPtr); 
                }
                else
                {
                    request->send(400, "text/plain", "Invalid register type");
                }

                String htmlContent = viewCurrentRegister::generateHTML();
                const char *htmlContentPtr = htmlContent.c_str();
                request->send(200, "text/html", htmlContentPtr); 
    });

    server.on("/storevalue", HTTP_GET, [](AsyncWebServerRequest *request){
                String registerValue = request->getParam("registerValue")->value();
                String registerAddress = request->getParam("registerAddress")->value();

                SystemState::getInstance()->pushRegister(registerAddress.toInt(), registerValue.toFloat());

                String htmlContent = viewCurrentRegister::generateHTMLConfirm(registerAddress, registerValue.toFloat());
                const char *htmlContentPtr = htmlContent.c_str();
                request->send(200, "text/html", htmlContentPtr); 
    });

    server.on("/startRecording", HTTP_GET, [](AsyncWebServerRequest *request){
        try {
            String milliseconds = request->getParam("milliseconds")->value();
            String registerAddress = request->getParam("registerAddress")->value();

            Serial.println("Start recording register " + registerAddress + " every " + milliseconds + " milliseconds");
            //Serial.println("integer milliseconds: " + milliseconds.toInt());
            
            SystemState::getInstance()->startRecordingRegister(registerAddress.toInt(), milliseconds.toInt());

            String popupScript = "showPopup('Recording started');";
            String htmlContent = viewCurrentRegister::generateHTML(registerAddress, 0.0, popupScript);
            const char *htmlContentPtr = htmlContent.c_str();
            request->send(200, "text/html", htmlContentPtr);
        } catch (const std::exception &e) {
            String errorMessage = "Error: ";
            errorMessage += e.what();
            String popupScript = "showPopup('" + errorMessage + "');";
            String htmlContent = viewCurrentRegister::generateHTML("", 0.0, popupScript);
            const char *htmlContentPtr = htmlContent.c_str();
            request->send(500, "text/html", htmlContentPtr);
        } catch (...) {
            String popupScript = "showPopup('Unknown error occurred');";
            String htmlContent = viewCurrentRegister::generateHTML("", 0.0, popupScript);
            const char *htmlContentPtr = htmlContent.c_str();
            request->send(500, "text/html", htmlContentPtr);
        }
    });

    server.on("/stopRecording", HTTP_GET, [](AsyncWebServerRequest *request){
        try {
            String registerAddress = request->getParam("registerAddress")->value();
            SystemState::getInstance()->stopRecordingRegister(registerAddress.toInt());

            String popupScript = "showPopup('Recording stopped');";
            String htmlContent = viewCurrentRegister::generateHTML(registerAddress, 0.0, popupScript);
            const char *htmlContentPtr = htmlContent.c_str();
            request->send(200, "text/html", htmlContentPtr);
        } catch (const std::exception &e) {
            String errorMessage = "Error: ";
            errorMessage += e.what();
            String popupScript = "showPopup('" + errorMessage + "');";
            String htmlContent = viewCurrentRegister::generateHTML("", 0.0, popupScript);
            const char *htmlContentPtr = htmlContent.c_str();
            request->send(500, "text/html", htmlContentPtr);
        } catch (...) {
            String popupScript = "showPopup('Unknown error occurred');";
            String htmlContent = viewCurrentRegister::generateHTML("", 0.0, popupScript);
            const char *htmlContentPtr = htmlContent.c_str();
            request->send(500, "text/html", htmlContentPtr);
        }
    });


    // update del grafico!
    server.on("/getRegisterValues", HTTP_GET, [](AsyncWebServerRequest *request){
        if (request->hasParam("address")) {
            String address = request->getParam("address")->value();
            std::vector<float> values = SystemState::getInstance()->getAllRegisterValue(address.toInt());

            String json = "[";
            for (size_t i = 0; i < values.size(); ++i) {
                if (i > 0) json += ",";
                json += String(values[i]);
            }
            json += "]";

            request->send(200, "application/json", json);
        } else {
            request->send(400, "application/json", "{\"error\":\"Address parameter missing\"}");
        } 
    });

    server.on("/graph", HTTP_GET, [](AsyncWebServerRequest *request){
                String htmlContent = viewGraph::generateHTML();
                const char *htmlContentPtr = htmlContent.c_str();
                request->send(200, "text/html", htmlContentPtr); 
    });

    server.on("/config", HTTP_GET, [](AsyncWebServerRequest *request){
                //String htmlContent = viewGraph::generateHTMLData();
                //const char *htmlContentPtr = htmlContent.c_str();
                //qui devo modificare stato di systemState
                String htmlContent = viewConfig::generateHTML(SystemState::getInstance()->wifiManager->scanNetworks());
                const char *htmlContentPtr = htmlContent.c_str();
                request->send(200, "text/html", htmlContentPtr);
    });

    server.on("/switch_wifi", HTTP_GET, [](AsyncWebServerRequest *request){
                String ssid_new = request->getParam("ssid")->value();
                String password_new = request->getParam("password")->value();
                try
                {

                    SystemState::getInstance()->switchNetwork(ssid_new.c_str(), password_new.c_str());
                    //delay(1000);
                    String htmlContent = viewWifiSuccessChange::generateHTML(SystemState::getInstance()->wifiManager->ip_address);
                    const char *htmlContentPtr = htmlContent.c_str();
                    request->send(200, "text/html", htmlContentPtr);
                    //request->send(200, "text/html", "success Switch wifi page");
                }
                catch(...)
                {
                    String htmlContent = viewWifiSuccessChange::generateErrorPage(SystemState::getInstance()->wifiManager->ssid, SystemState::getInstance()->wifiManager->ip_address);
                    const char *htmlContentPtr = htmlContent.c_str();
                    request->send(500, "text/html", htmlContentPtr);

                    //request->send(500, "text/html", "error Switch wifi page");
                }
                
    });
}