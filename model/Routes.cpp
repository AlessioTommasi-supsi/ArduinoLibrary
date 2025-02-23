#include "Routes.h"


void Routes::defineRoutes(AsyncWebServer &server)
{
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "text/plain", "Hello, world"); }
    );
    
    

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
    server.on("/pinout", HTTP_GET, [](AsyncWebServerRequest *request){
        String htmlContent = Pinout::generateHTML();
        const char *htmlContentPtr = htmlContent.c_str();
        request->send(200, "text/html", htmlContentPtr); });

    server.on("/pinoutContent", HTTP_GET, [](AsyncWebServerRequest *request){
        String content = "";
        PinoutData *pinoutData = SystemState::getInstance()->pinoutData;
        pinoutData->readPins();
        for (auto pin = pinoutData->begin(); pin != pinoutData->end(); ++pin)
        {
            content += pin->toString();
            content += "<br>";
            content += "<form action='/startRecordingPin' method='get' style='display: inline;'>";
            content += "  <input type='hidden' name='pin' value='" + String(pin->number) + "'>";
            content += "  <label for='milliseconds'>Milliseconds:</label>";
            content += "  <input type='text' id='milliseconds' name='milliseconds' required value='1000'>";
            content += "  <button type='submit' style='padding: 5px; background-color: red; color: white; border: none; border-radius: 4px; cursor: pointer;'>Start Recording</button>";
            content += "</form>";
            content += "<form action='/stopRecordingPin' method='get' style='display: inline; margin-left: 10px;'>";
            content += "  <input type='hidden' name='pin' value='" + String(pin->number ) + "'>";
            content += "  <button type='submit' style='padding: 5px; background-color: grey; color: white; border: none; border-radius: 4px; cursor: pointer;'>Stop Recording</button>";
            content += "</form>";
            content += "<form action='/editPin' method='get' style='display: inline; margin-left: 10px;'>";
            content += "  <input type='hidden' name='pin' value='" + String(pin->number) + "'>";
            content += "  <button type='submit' style='padding: 5px; background-color: blue; color: white; border: none; border-radius: 4px; cursor: pointer;'>Edit</button>";
            content += "</form>";
            content += "<br><br>";
        }
        request->send(200, "text/html", content);
    });

    server.on("/startRecordingPin", HTTP_GET, [](AsyncWebServerRequest *request){
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

    server.on("/history", HTTP_GET, [](AsyncWebServerRequest *request){
                String htmlContent = viewHistory::generateHTML();
                const char *htmlContentPtr = htmlContent.c_str();
                request->send(200, "text/html", htmlContentPtr); 
    });
    
    server.on("/getHistoryContent", HTTP_GET, [](AsyncWebServerRequest *request){
        String content = "";
        std::vector<float> valuesVector = SystemState::getInstance()->getAllRegisterValue();
        std::vector<int> addresses = SystemState::getInstance()->getAllRegisterAddress();

        // Loop through each value in the vector in reverse order
        for (size_t i = valuesVector.size(); i > 0; i--)
        {
            size_t index = i - 1;
            content += "<tr>";
            content += "<td>" + String(addresses[index]) + "</td>"; // Display the address

            // Display the value with an input field for editing inside a form
            content += "<td>";
            content += "<form action='/editRegister' method='GET'>";
            content += "<input type='hidden' name='index' value='" + String(index) + "'>";
            content += "<input type='text' class='edit-input' name='value' value='" + String(valuesVector[index]) + "'>";
            content += "<input type='submit' value='Edit' class='action-link edit-link'>";
            content += "</form>";
            content += "</td>";

            // Add delete button with link
            content += "<td>";
            content += "<a href='/deleteRegister?index=" + String(index) + "' class='action-link delete-link'>Delete</a>"; // Delete link
            content += "</td>";

            content += "</tr>";
        }

        request->send(200, "text/html", content);
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

    server.on("/getPinValues", HTTP_GET, [](AsyncWebServerRequest *request){
        if (request->hasParam("pin")) {
            String pin = request->getParam("pin")->value();
            std::vector<float> values = SystemState::getInstance()->pinoutData->getPin(pin.toInt()).getValuesVoltage();

            String json = "[";
            for (size_t i = 0; i < values.size(); ++i) {
                if (i > 0) json += ",";
                json += String(values[i]);
            }
            json += "]";

            request->send(200, "application/json", json);
        } else {
            request->send(400, "application/json", "{\"error\":\"Pin parameter missing\"}");
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

    server.on("/monitor", HTTP_GET, [](AsyncWebServerRequest *request){
        try
        {
            String htmlContent = "";
            htmlContent += viewGeneric::defaultCssHeader("Monitor");
            
            htmlContent += viewGraph::initCirularProgressBarGraph();

            // Ottieni le metriche di sistema

            // HEAP
            
            size_t heapFree = heap_caps_get_free_size(MALLOC_CAP_8BIT);
            size_t heapTotal = heap_caps_get_total_size(MALLOC_CAP_8BIT);
            size_t heapUsed = heapTotal - heapFree;

            //htmlContent += viewGraph::generateCirularProgressBarGraph("HEAP", heapUsed, heapTotal);
            htmlContent += viewGraph::generateCirularProgressBarGraph("HEAP", heapUsed, heapTotal, "monitorHeapData", 1000);

           


            htmlContent += viewGraph::endCirularProgressBarGraph();

            htmlContent += "<br><br><h2> STACK USAGE: </h2> <br><br>";
            PinoutData *pinoutData = SystemState::getInstance()->pinoutData;


            htmlContent += viewGraph::initCirularProgressBarGraph();

            // Loop through each pin in the pinout data
            for (auto pin = pinoutData->begin(); pin != pinoutData->end(); ++pin)
            {
                if (pin->recordingTask != NULL)
                {
                    // STANPO GRAFICO CON LO STACK LIB
                    size_t stackUsed = pin->getUsedStackInWords() * 4; // trasformo da parole a byte
                    size_t stackTotal = pin->getStackSizeInWords() * 4;

                    htmlContent += viewGraph::generateCirularProgressBarGraph("Pin" + String(pin->number) + "Stack", stackUsed, stackTotal, "/monitorPinStack?pin=" + String(pin->number), 1000);
                }
            }
            htmlContent += viewGraph::endCirularProgressBarGraph();

            //non funziona grafici non si aggiornano
            //htmlContent += viewGeneric::dynamicUpdateContent("divMonitorHeapData"/*deve semplicemente essere id univoco*/, "/monitorPinStackDataContent", 10000 /*10 secondi*/);
            
            htmlContent += viewGeneric::defaultFooter();

            // Invio della risposta HTTP
            request->send(200, "text/html", htmlContent);
        }
        catch(...)
        {
            String htmlContent = "error";
            const char *htmlContentPtr = htmlContent.c_str();
            request->send(500, "text/html", htmlContentPtr);
        }
    });

    server.on("/monitorPinStackDataContent", HTTP_GET, [](AsyncWebServerRequest *request){ /* PER ORA NON USATO*/
        
        try
        {
            String htmlContent = "";
            PinoutData *pinoutData = SystemState::getInstance()->pinoutData;

            //htmlContent += " <div class='circle_progressbar_chart-container' style='font-family: Raleway, sans-serif; display: flex; flex-wrap: wrap; justify-content: space-around; gap: 20px; padding: 20px;'>";
            htmlContent += viewGraph::initCirularProgressBarGraph();

           
            // Loop through each pin in the pinout data
            for (auto pin = pinoutData->begin(); pin != pinoutData->end(); ++pin)
            {
                if (pin->recordingTask != NULL)
                {
                    // STANPO GRAFICO CON LO STACK LIB
                    size_t stackUsed = pin->getUsedStackInWords() * 4; // trasformo da parole a byte
                    size_t stackTotal = pin->getStackSizeInWords() * 4;

                    htmlContent += viewGraph::generateCirularProgressBarGraph("Pin" + String(pin->number) + "Stack", stackUsed, stackTotal, "/monitorPinStack?pin=" + String(pin->number), 1000);
                }
            }
            htmlContent += "</div>";
            htmlContent += viewGraph::endCirularProgressBarGraph();

            const char *htmlContentPtr = htmlContent.c_str();
            request->send(200, "text/html", htmlContentPtr);
        }
        catch(...)
        {
            String htmlContent = "error";
            const char *htmlContentPtr = htmlContent.c_str();
            request->send(500, "text/html", htmlContentPtr);
        }
    });

    server.on("/monitorPinStack", HTTP_GET, [](AsyncWebServerRequest *request){
        try
        {
            String json = "{";
            if (request->hasParam("pin"))
            {
                String pin = request->getParam("pin")->value();
                PinoutData *pinoutData = SystemState::getInstance()->pinoutData;
                Pin *pinData = &pinoutData->getPin(pin.toInt());

                // STANPO GRAFICO CON LO STACK LIB
                size_t stackUsed = pinData->getUsedStackInWords() * 4; // trasformo da parole a byte
                size_t stackTotal = pinData->getStackSizeInWords() * 4;

                json += "\"used\":" + String(stackUsed) + ",";
                json += "\"total\":" + String(stackTotal);
            }
            else
            {
                json += "\"error\":\"Pin parameter missing\"";
            }
            json += "}";

            request->send(200, "application/json", json);
        }
        catch(...)
        {
            String json = "{\"error\":\"An error occurred\"}";
            request->send(500, "application/json", json);
        }
    });

    server.on("/monitorHeapData", HTTP_GET, [](AsyncWebServerRequest *request){
        //Serial.println("GET /monitorHeapData");

        //todo: implementare qualcosa di simile per ogni pin: potrei monitorare task con Pin.thread!
        try
        {
            String json = "{";
            // Ottieni le metriche di sistema

            // HEAP
            size_t heapFree = heap_caps_get_free_size(MALLOC_CAP_8BIT);
            size_t heapTotal = heap_caps_get_total_size(MALLOC_CAP_8BIT);
            size_t heapUsed = heapTotal - heapFree;

            json += "\"used\":" + String(heapUsed) + ",";
            json += "\"total\":" + String(heapTotal);

            json += "}";

            request->send(200, "application/json", json);
        }
        catch(...)
        {
            String json = "{\"error\":\"An error occurred\"}";
            request->send(500, "application/json", json);
        }
    });
}