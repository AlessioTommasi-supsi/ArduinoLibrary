#include "pinoutRoutes.h"
#include "esp_task_wdt.h"  // Include per il watchdog timer ESP32

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
        if(SystemState::getInstance()->getPinoutLock())
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
                    content += "            <input type=\"text\" id=\"milliseconds\" name=\"milliseconds\" required value=\"" + String(DEFAULT_RECORDING_INTERVAL) + "\">";
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
                SystemState::getInstance()->releasePinoutLock();
                
                // Prepara la risposta includendo gli header e il contenuto
                AsyncWebServerResponse *response = request->beginResponse(200, "text/html", content);
                response->addHeader("Access-Control-Allow-Origin", "*"); // Aggiungi header CORS
                request->send(response); // Invia la risposta al client
                
            }
            catch(const std::exception& e)
            {
                SystemState::getInstance()->releasePinoutLock();
                request->send(200, "text/html", "Error: " + String(e.what()));
            }
            catch(...)
            {
                SystemState::getInstance()->releasePinoutLock();
                request->send(200, "text/html", "Unknown error occurred");
            } 
        }
    });

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
            String htmlContent = Pinout::generateHTML(popupScript);
            const char *htmlContentPtr = htmlContent.c_str();
            request->send(500, "text/html", htmlContentPtr);
        } catch (...) {
            String popupScript = "showPopup('Unknown error occurred');";
            String htmlContent = Pinout::generateHTML(popupScript);
            const char *htmlContentPtr = htmlContent.c_str();
            request->send(500, "text/html", htmlContentPtr);
    } });

    server.on("/stopRecordingPin", HTTP_GET, [](AsyncWebServerRequest *request)
              {
        try {
            String registerAddress = request->getParam("pin")->value();

            SystemState::getInstance()->pinoutData->getPin(registerAddress.toInt()).stopRecording();
            Serial.println("Stop recording Pin " + registerAddress);
            String popupScript = "showPopup('Recording stopped');";
            String htmlContent = Pinout::generateHTML(popupScript);
            const char *htmlContentPtr = htmlContent.c_str();
            request->send(200, "text/html", htmlContentPtr);
             
        } catch (const std::exception &e) {
            String errorMessage = "Error: ";
            errorMessage += e.what();
            String popupScript = "showPopup('" + errorMessage + "');";
            String htmlContent = Pinout::generateHTML(popupScript);
            const char *htmlContentPtr = htmlContent.c_str();
            request->send(500, "text/html", htmlContentPtr);
        } catch (...) {
            String popupScript = "showPopup('Unknown error occurred');";
            String htmlContent = Pinout::generateHTML(popupScript);
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

    server.on("/pulsePin", HTTP_GET, [](AsyncWebServerRequest *request){   
        int PinNumber = -1;
        int pin_delay = DEFAULT_PIN_DELAY;
        bool rise_direction = false;  //di default se non specificato parametri tiro a pin a gnd per 1 secondo!   
        if (!request->hasParam("pin")) {
            request->send(500, "text/html", "Error: Pin parameter missing");
            return;
        }
        PinNumber = request->getParam("pin")->value().toInt();
        if (request->hasParam("rise_direction"))
        {
            rise_direction = request->getParam("rise_direction")->value() == "up";
        }

        if (request->hasParam("delay"))
        {
            pin_delay = request->getParam("delay")->value().toInt();
        }

        // Attiva il pin
        SystemState::getInstance()->pinoutData->getPin(PinNumber).write(rise_direction);
        Serial.println("Starting pulse on pin " + String(PinNumber) + " for " + String(pin_delay) + "ms"); 
        
        // Delay non bloccante con reset esplicito del watchdog
        unsigned long startTime = millis();
        int i = 0;
        while (millis() - startTime < pin_delay) {
            i++;
            Serial.print(".");
            esp_task_wdt_reset(); // Reset esplicito del watchdog timer
            delay(100); // delay per evitare loop troppo veloce
        }
        Serial.println("");
        Serial.println("Pin " + String(PinNumber) + " pulse completed after " + String(pin_delay) + " milliseconds");
        
        // Disattiva il pin
        SystemState::getInstance()->pinoutData->getPin(PinNumber).write(!rise_direction);

        String htmlContent = "Pin " + String(PinNumber) + " pulsed for " + String(pin_delay) + " milliseconds";
        const char *htmlContentPtr = htmlContent.c_str();
        request->send(200, "text/html", htmlContentPtr);
    });


    server.on("/getPinValues", HTTP_GET, [](AsyncWebServerRequest *request){
        if (request->hasParam("pin")) { 
            try
            {
                String pin = request->getParam("pin")->value();
    
                // SystemState::getInstance()->pinoutData->getPin(pin.toInt())  ritrna sempre qualcosa al massimo default p[in con pinnumber= -1!!]
                std::vector<float> values = SystemState::getInstance()->pinoutData->getPin(pin.toInt()).getValuesVoltage();
    
                if (values.size() == 0 ) 
                {
                    return request->send(200, "application/json", "[]");
                }
                
    
                String json = "[";
                for (size_t i = 0; i < values.size(); ++i)
                {
                    if (i > 0)
                        json += ",";
                    json += String(values[i]);
                }
                json += "]";
    
                request->send(200, "application/json", json);
                 
            }
            catch(...)
            {
                request->send(200, "application/json", "[]" );
                 
            }
            
            
        } else {
            request->send(200, "application/json", "{\"error\":\"Pin parameter missing\"}");
             
        } 
        });

        server.on("/history", HTTP_GET, [](AsyncWebServerRequest *request){
            String htmlContent = viewHistory::generateHTML();
            const char *htmlContentPtr = htmlContent.c_str();
            request->send(200, "text/html", htmlContentPtr);
        });

        server.on("/getPinValuesHistory", HTTP_GET, [](AsyncWebServerRequest *request) {
            String content = viewHistory::pinoutContent();
            request->send(200, "text/html", content);
        });
        
}