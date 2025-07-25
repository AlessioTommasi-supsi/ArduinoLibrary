#include "pinoutRoutes.h"


void pinoutRoutes::defineRoutes(AsyncWebServer &server)
{

    server.on("/pinout", HTTP_GET, [](AsyncWebServerRequest *request)
              {
        String htmlContent =""; 
        
        try
        {
            // Gestione parametri popup
            String popupType = "";
            String popupMessage = "";
            if (request->hasParam("popup")) {
                popupType = request->getParam("popup")->value();
            }
            if (request->hasParam("message")) {
                popupMessage = request->getParam("message")->value();
                // URL decode del messaggio
                popupMessage.replace("%20", " ");
                popupMessage.replace("%C2%A0", " ");
                popupMessage.replace(":", ": ");
            }
            
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
                    pin->setIsInput(isInput);
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
            
            // Genera HTML con popup se necessario
            if (popupType != "" && popupMessage != "") {
                htmlContent = Pinout::generateHTMLWithPopup(popupType, popupMessage);
            } else {
                htmlContent = Pinout::generateHTML();
            }
            
            const char *htmlContentPtr = htmlContent.c_str();
            request->send(200, "text/html", htmlContentPtr);
             
        }
        catch (const std::exception &e)
        {
            Serial.println("Error: " + String(e.what()));
            request->send(500, "text/plain", "Internal Server Error");
        } });

    // Nuova rotta per la cronologia separata dei pin
    server.on("/pin_history", HTTP_GET, [](AsyncWebServerRequest *request) {
        try {
            String htmlContent = viewPinHistory::generateHTML();
            const char *htmlContentPtr = htmlContent.c_str();
            request->send(200, "text/html", htmlContentPtr);
        } catch (const std::exception& e) {
            Serial.println("Error in /pin_history route: " + String(e.what()));
            request->send(500, "text/html", "Error loading pin history page");
        } catch (...) {
            Serial.println("Unknown error in /pin_history route");
            request->send(500, "text/html", "Unknown error occurred");
        }
    });

    server.on("/pinoutContent", HTTP_GET, [](AsyncWebServerRequest *request)
              {
        
            
            try
            {  
                
                String content = "";
                PinoutData *pinoutData = SystemState::getInstance()->pinoutData;
                
                if (pinoutData) {
                    pinoutData->readPins();
                    
                    for (auto pin = pinoutData->begin(); pin != pinoutData->end(); ++pin)
                    {
                        content += "<div class=\"pin-container\">";
                        content += "    <div class=\"pin-info\">";
                        char noteBuffer[200];
                        pin->getNote(noteBuffer, sizeof(noteBuffer));
                        content += "        Pin number: " + String(pin->getNumber()) + ", Type: " + pin->getType() + ", Voltage: " + String(pin->getVoltage() / 1000.0, 3) + " V, Input: " + (pin->getIsInput() ? "Yes" : "No") + ", Note: " + String(noteBuffer);
                        content += "    </div>";
                        content += "    <div class=\"pin-actions\">";
                        
                        // Solo per pin input, aggiungo form con JavaScript AJAX
                        if (pin->getIsInput()) {
                            content += "        <form action=\"/startRecordingPin\" method=\"get\" class=\"start-form\" data-pin=\"" + String(pin->getNumber()) + "\" style=\"display:inline-block;margin-right:5px\">";
                            content += "            <input type=\"hidden\" name=\"pin\" value=\"" + String(pin->getNumber()) + "\">";
                            content += "            <label for=\"milliseconds\">ms:</label>";
                            content += "            <input type=\"text\" name=\"milliseconds\" value=\"1000\" style=\"width:60px;margin-right:5px\">";
                            content += "            <button type=\"submit\" class=\"start\">Start Recording</button>";
                            content += "        </form>";
                            content += "        <form action=\"/stopRecordingPin\" method=\"get\" class=\"stop-form\" data-pin=\"" + String(pin->getNumber()) + "\" style=\"display:inline-block;margin-right:5px\">";
                            content += "            <input type=\"hidden\" name=\"pin\" value=\"" + String(pin->getNumber()) + "\">";
                            content += "            <button type=\"submit\" class=\"stop\">Stop Recording</button>";
                            content += "        </form>";
                        }
                        
                        content += "        <form action=\"/editPin\" method=\"get\" style=\"display:inline-block\">";
                        content += "            <input type=\"hidden\" name=\"pin\" value=\"" + String(pin->getNumber()) + "\">";
                        content += "            <button type=\"submit\" class=\"edit\">Edit</button>";
                        content += "        </form>";
                        content += "    </div>";
                        content += "</div>";
                    }
                } else {
                    content = "Error: Pinout data not available";
                }
                
                // Prepara la risposta includendo gli header e il contenuto
                AsyncWebServerResponse *response = request->beginResponse(200, "text/html", content);
                response->addHeader("Access-Control-Allow-Origin", "*");
                request->send(response);
                
            }
            catch (const std::exception& e)
            {
                Serial.println("Error in /pinoutContent route: " + String(e.what()));
                request->send(500, "text/html", "Error: " + String(e.what()));
            }
            catch (...)
            {
                Serial.println("Unknown error in /pinoutContent route");
                request->send(500, "text/html", "Unknown error occurred");
            } 
        
    });

    server.on("/startRecordingPin", HTTP_GET, [](AsyncWebServerRequest *request)
              {
        try {
            String milliseconds = request->getParam("milliseconds")->value();
            String registerAddress = request->getParam("pin")->value();

            Serial.println("Start recording Pin " + registerAddress + " every " + milliseconds + " milliseconds");
            
            SystemState::getInstance()->pinoutData->getPin(registerAddress.toInt()).startRecording(milliseconds.toInt());
            
            // REDIRECT con popup invece di JSON
            String redirectUrl = "/pinout?popup=success&message=🟢%20Registrazione%20avviata%20per%20Pin%20" + registerAddress + "%20ogni%20" + milliseconds + "ms";
            
            AsyncWebServerResponse *response = request->beginResponse(302);
            response->addHeader("Location", redirectUrl);
            request->send(response);
             
        } catch (const std::exception &e) {
            Serial.println("Error in /startRecordingPin route: " + String(e.what()));
            String redirectUrl = "/pinout?popup=error&message=Errore:%20" + String(e.what());
            AsyncWebServerResponse *response = request->beginResponse(302);
            response->addHeader("Location", redirectUrl);
            request->send(response);
        } catch (...) {
            Serial.println("Unknown error in /startRecordingPin route");
            String redirectUrl = "/pinout?popup=error&message=Errore%20sconosciuto%20durante%20avvio%20registrazione";
            AsyncWebServerResponse *response = request->beginResponse(302);
            response->addHeader("Location", redirectUrl);
            request->send(response);
        }
    });

    server.on("/stopRecordingPin", HTTP_GET, [](AsyncWebServerRequest *request)
              {
        try {
            String registerAddress = request->getParam("pin")->value();
            SystemState::getInstance()->pinoutData->getPin(registerAddress.toInt()).stopRecording();

            // REDIRECT con popup invece di JSON  
            String redirectUrl = "/pinout?popup=success&message=🔴%20Registrazione%20fermata%20per%20Pin%20" + registerAddress;
            
            AsyncWebServerResponse *response = request->beginResponse(302);
            response->addHeader("Location", redirectUrl);
            request->send(response);
            
        } catch (const std::exception &e) {
            Serial.println("Error in /stopRecordingPin route: " + String(e.what()));
            String redirectUrl = "/pinout?popup=error&message=Errore:%20" + String(e.what());
            AsyncWebServerResponse *response = request->beginResponse(302);
            response->addHeader("Location", redirectUrl);
            request->send(response);
        } catch (...) {
            Serial.println("Unknown error in /stopRecordingPin route");
            String redirectUrl = "/pinout?popup=error&message=Errore%20sconosciuto%20durante%20arresto%20registrazione";
            AsyncWebServerResponse *response = request->beginResponse(302);
            response->addHeader("Location", redirectUrl);
            request->send(response);
        }
    });


    
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
                pin->setIsInput(isInput);
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


    server.on("/savePin", HTTP_POST, [](AsyncWebServerRequest *request) {
        try {
            String htmlContent = "";
            
            if (request->hasParam("pinNumber", true)) {
                String pinNumber = request->getParam("pinNumber", true)->value();
                String pinType = request->getParam("pinType", true)->value();
                String pinInputCheck = request->getParam("pinInputCheck", true) ? request->getParam("pinInputCheck", true)->value() : "";
                String pinModeSelect = request->getParam("pinModeSelect", true)->value();
                String pinHighValue = request->getParam("pinHighValue", true) ? request->getParam("pinHighValue", true)->value() : "";
                String pinNote = request->getParam("pinNote", true)->value();
                
                bool isInput = (pinInputCheck == "on");
                int pinMode = pinModeSelect.toInt();
                int outputValue = pinHighValue.toInt();
                bool goHigh = (outputValue == 1);
                
                Pin *pin = &SystemState::getInstance()->pinoutData->getPin(pinNumber.toInt());
                
                Serial.println("Saving pin " + pinNumber + " with type " + pinType + ", isInput " + String(isInput) + ", pinMode " + String(pinMode) + ", outputValue " + String(outputValue) + ", pinNote " + pinNote);
                pin->setIsInput(isInput);
                pin->setType(pinType);
                pin->setMode(pinMode);
                pin->setNote(pinNote.c_str());
                
                if (!isInput) {
                    pin->write(goHigh);
                }

                String script = "Pin " + pinNumber + " saved!";
                script += viewEditPin::addDefaultScript();
                htmlContent = viewEditPin::generateHTML(pinNumber.toInt(), script);
            } else {
                htmlContent = viewEditPin::generateHTML();
            }
            
            const char *htmlContentPtr = htmlContent.c_str();
            request->send(200, "text/html", htmlContentPtr);
        } catch (const std::exception& e) {
            Serial.println("Error in /savePin route: " + String(e.what()));
            request->send(500, "text/html", "Error: " + String(e.what()));
        } catch (...) {
            Serial.println("Unknown error in /savePin route");
            request->send(500, "text/html", "Unknown error occurred");
        }
    });

    server.on("/getPinValues", HTTP_GET, [](AsyncWebServerRequest *request){
        if (request->hasParam("pin")) {
            try
            {
                String pin = request->getParam("pin")->value();
                String json = "[]"; // Default empty JSON array
                
                std::vector<float> values = SystemState::getInstance()->pinoutData->getPin(pin.toInt()).getValuesVoltage();
                
                if (values.size() == 0) {
                    json = "[]";
                } else {
                    json = "[";
                    for (size_t i = 0; i < values.size(); ++i) {
                        if (i > 0)
                            json += ",";
                        json += String(values[i]);
                    }
                    json += "]";
                }
                
                request->send(200, "application/json", json);
            } catch (...) {
                request->send(200, "application/json", "[]");
            }
        } else {
            request->send(200, "application/json", "{\"error\":\"Pin parameter missing\"}");
        }
    });

    server.on("/getPinValuesHistory", HTTP_GET, [](AsyncWebServerRequest *request) {
        try {
            String content = viewPinHistory::pinContent();
            request->send(200, "text/html", content);
        } catch (const std::exception& e) {
            Serial.println("Error in /getPinValuesHistory route: " + String(e.what()));
            request->send(500, "text/html", "Error loading pin history");
        } catch (...) {
            Serial.println("Unknown error in /getPinValuesHistory route");
            request->send(500, "text/html", "Unknown error occurred");
        }
    });

    server.on("/editPinValue", HTTP_GET, [](AsyncWebServerRequest *request) {
        try {
            String pinNumber = request->getParam("pin")->value();
            String index = request->getParam("index")->value();
            String value = request->getParam("value")->value();
            
            SystemState::getInstance()->pinoutData->getPin(pinNumber.toInt()).editValue(index.toInt(), value.toFloat());
            
            String htmlContent = viewPinHistory::generateHTML();
            request->send(200, "text/html", htmlContent);
        } catch (const std::exception& e) {
            Serial.println("Error in /editPinValue route: " + String(e.what()));
            request->send(500, "text/html", "Error editing pin value");
        } catch (...) {
            Serial.println("Unknown error in /editPinValue route");
            request->send(500, "text/html", "Unknown error occurred");
        }
    });

    server.on("/deletePinValue", HTTP_GET, [](AsyncWebServerRequest *request) {
        try {
            String pinNumber = request->getParam("pin")->value();
            String index = request->getParam("index")->value();
            
            SystemState::getInstance()->pinoutData->getPin(pinNumber.toInt()).deleteValue(index.toInt());
            
            String htmlContent = viewPinHistory::generateHTML();
            request->send(200, "text/html", htmlContent);
        } catch (const std::exception& e) {
            Serial.println("Error in /deletePinValue route: " + String(e.what()));
            request->send(500, "text/html", "Error deleting pin value");
        } catch (...) {
            Serial.println("Unknown error in /deletePinValue route");
            request->send(500, "text/html", "Unknown error occurred");
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

}