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
                
                if (pinoutData) {
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

            String popupScript = "showPopup('Recording started for pin " + registerAddress + "');";
            String htmlContent = Pinout::generateHTML(popupScript);
            const char *htmlContentPtr = htmlContent.c_str();
            request->send(200, "text/html", htmlContentPtr);
             
        } catch (const std::exception &e) {
            Serial.println("Error in /startRecordingPin route: " + String(e.what()));
            request->send(500, "text/html", "Error: " + String(e.what()));
        } catch (...) {
            Serial.println("Unknown error in /startRecordingPin route");
            request->send(500, "text/html", "Unknown error occurred");
        }
    });

    server.on("/stopRecordingPin", HTTP_GET, [](AsyncWebServerRequest *request)
              {
        try {
            String registerAddress = request->getParam("pin")->value();
            SystemState::getInstance()->pinoutData->getPin(registerAddress.toInt()).stopRecording();

            String popupScript = "showPopup('Recording stopped for pin " + registerAddress + "');";
            String htmlContent = Pinout::generateHTML(popupScript);
            const char *htmlContentPtr = htmlContent.c_str();
            request->send(200, "text/html", htmlContentPtr);
             
        } catch (const std::exception &e) {
            Serial.println("Error in /stopRecordingPin route: " + String(e.what()));
            request->send(500, "text/html", "Error: " + String(e.what()));
        } catch (...) {
            Serial.println("Unknown error in /stopRecordingPin route");
            request->send(500, "text/html", "Unknown error occurred");
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
                pin->isInput = isInput;
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
            String content = viewHistory::pinoutContent();
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
            
            String content = viewHistory::pinoutContent();
            request->send(200, "text/html", content);
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
            
            String content = viewHistory::pinoutContent();
            request->send(200, "text/html", content);
        } catch (const std::exception& e) {
            Serial.println("Error in /deletePinValue route: " + String(e.what()));
            request->send(500, "text/html", "Error deleting pin value");
        } catch (...) {
            Serial.println("Unknown error in /deletePinValue route");
            request->send(500, "text/html", "Unknown error occurred");
        }
    });
}