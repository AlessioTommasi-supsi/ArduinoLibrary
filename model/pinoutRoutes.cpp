
#include "pinoutRoutes.h"


void pinoutRoutes::defineRoutes(AsyncWebServer &server)
{

    server.on("/pinout", HTTP_GET, [](AsyncWebServerRequest *request)
              {
        String htmlContent =""; 
        
        try
        {
            if (request->hasParam("pin"))
            {
                String pinNumber = request->getParam("pin")->value();
                // arrivo da editPin! dunque devo aggiungere il pin!
                if (request->hasParam("pinType") && request->hasParam("isInput") && request->hasParam("outputValue") && request->hasParam("pinNote"))
                {
                    String pinType = request->getParam("pinType")->value();
                    bool isInput = request->getParam("isInput")->value() == "true" ? true : false;
                    uint8_t pinMode = isInput ? INPUT : OUTPUT;
                    float outputValue = request->getParam("outputValue")->value().toFloat();
                    bool goHigh = outputValue > 0;
                    String pinNote = request->getParam("pinNote")->value();

                    Pin *pin = &SystemState::getInstance()->pinoutData->getPin(pinNumber.toInt());

                    Serial.println("Saving pin " + pinNumber + " with type " + pinType + ", isInput " + String(isInput) + ", pinMode " + String(pinMode) + ", outputValue " + String(outputValue) + ", pinNote " + pinNote);
                    pin->isInput = isInput;
                    pin->setType(pinType);
                    pin->setMode(pinMode);
                    pin->setNote(pinNote.c_str());

                    if (!isInput)
                    {
                        pin->write(goHigh);
                    }
                }
                else
                {
                    Serial.println("Error: missing parameters");
                }
            }
            htmlContent = Pinout::generateHTML();
            const char *htmlContentPtr = htmlContent.c_str();
            request->send(200, "text/html", htmlContentPtr);
             
        }
        catch (const std::exception &e)
        {
            request->send(500, "text/html", "Error: " + String(e.what()));
        }
        catch (...)
        {
            request->send(500, "text/html", "Unknown error occurred");
        } });

    server.on("/pinoutContent", HTTP_GET, [](AsyncWebServerRequest *request)
              {
        try
        {  
            
            String content = "";
            PinoutData *pinoutData = SystemState::getInstance()->pinoutData;
            pinoutData->readPins();
            for (auto pin = pinoutData->begin(); pin != pinoutData->end(); ++pin)
            {
                content += "<div class=\"pin-container\">";
                content += "    <div class=\"pin-info\">";
                content += "        Pin number: " + String(pin->number) + ", Type: " + pin->getType() + ", Voltage: " + String(pin->voltage / 1000.0, 3) + " V, Input: " + (pin->isInput ? "Yes" : "No") + ", Note: " + String(pin->note);
                content += "    </div>";
                content += "    <div class=\"pin-actions\">";
                content += "        <form action=\"/startRecordingPin\" method=\"get\">";
                content += "            <input type=\"hidden\" name=\"pin\" value=\"" + String(pin->number) + "\">";
                content += "            <label for=\"milliseconds\">Milliseconds:</label>";
                content += "            <input type=\"text\" id=\"milliseconds\" name=\"milliseconds\" required value=\"1000\">";
                content += "            <button type=\"submit\" class=\"start\">Start Recording</button>";
                content += "        </form>";
                content += "        <form action=\"/stopRecordingPin\" method=\"get\">";
                content += "            <input type=\"hidden\" name=\"pin\" value=\"" + String(pin->number) + "\">";
                content += "            <button type=\"submit\" class=\"stop\">Stop Recording</button>";
                content += "        </form>";
                content += "        <form action=\"/editPin\" method=\"get\">";
                content += "            <input type=\"hidden\" name=\"pin\" value=\"" + String(pin->number) + "\">";
                content += "            <button type=\"submit\" class=\"edit\">Edit</button>";
                content += "        </form>";
                content += "    </div>";
                content += "</div>";
            }
            
            // Prepara la risposta includendo gli header e il contenuto
            AsyncWebServerResponse *response = request->beginResponse(200, "text/html", content);
            response->addHeader("Access-Control-Allow-Origin", "*"); // Aggiungi header CORS
            request->send(response); // Invia la risposta al client
             
        }
        catch(const std::exception& e)
        {
            request->send(200, "text/html", "Error: " + String(e.what()));
        }
        catch(...)
        {
            request->send(200, "text/html", "Unknown error occurred");
        } });

    server.on("/startRecordingPin", HTTP_GET, [](AsyncWebServerRequest *request)
              {
        try {
            String milliseconds = request->getParam("milliseconds")->value();
            String registerAddress = request->getParam("pin")->value();

            Serial.println("Start recording Pin " + registerAddress + " every " + milliseconds + " milliseconds");
            
            SystemState::getInstance()->pinoutData->getPin(registerAddress.toInt()).startRecording(milliseconds.toInt());

            String popupScript = "showPopup('Recording started for pin " + registerAddress + "');";
            String htmlContent = Pinout::generateHTML(popupScript);
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
    } });

    server.on("/stopRecordingPin", HTTP_GET, [](AsyncWebServerRequest *request)
              {
        try {
            String registerAddress = request->getParam("pin")->value();

            SystemState::getInstance()->pinoutData->getPin(registerAddress.toInt()).stopRecording();
            //SystemState::getInstance()->stopRecordingRegister(registerAddress.toInt());
            Serial.println("Stop recording Pin " + registerAddress);
            String popupScript = "showPopup('Recording stopped');";
            String htmlContent = Pinout::generateHTML(popupScript);
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
    } });


    
    server.on("/editPin", HTTP_GET, [](AsyncWebServerRequest *request){        
        if (!request->hasParam("pin")) {
            String htmlContent = viewEditPin::generateHTML();
            const char *htmlContentPtr = htmlContent.c_str();
            request->send(200, "text/html", htmlContentPtr);
             
        }else
        {
            String pinNumber = request->getParam("pin")->value();
            String htmlContent = "";

            if (request->hasParam("pinType") && request->hasParam("isInput") && request->hasParam("outputValue") && request->hasParam("pinNote")) {
                String pinType = request->getParam("pinType")->value();
                bool isInput = request->getParam("isInput")->value() == "true" ? true : false;
                uint8_t pinMode = isInput ? INPUT : OUTPUT;
                float outputValue = request->getParam("outputValue")->value().toFloat();
                bool goHigh = outputValue > 0;
                String pinNote = request->getParam("pinNote")->value();
                
                Pin *pin = &SystemState::getInstance()->pinoutData->getPin(pinNumber.toInt());

                Serial.println("Saving pin " + pinNumber + " with type " + pinType + ", isInput " + String(isInput) + ", pinMode " + String(pinMode) + ", outputValue " + String(outputValue) + ", pinNote " + pinNote);
                pin->isInput = isInput;
                pin->setType(pinType);
                pin->setMode(pinMode);
                pin->setNote(pinNote.c_str());
                
                if (!isInput) {
                    pin->write(goHigh);
                }

                //aggiungo popup script con scritto salvataggio avvenuto!
                String script = "Pin " + pinNumber + " saved!";
                script += viewEditPin::addDefaultScript();
                htmlContent = viewEditPin::generateHTML(pinNumber.toInt(), script);
            }else{
                htmlContent = viewEditPin::generateHTML(pinNumber.toInt());
            }

            
            const char *htmlContentPtr = htmlContent.c_str();

            request->send(200, "text/html", htmlContentPtr);
        }
    });
}