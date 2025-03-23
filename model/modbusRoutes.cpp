

#include "modbusRoutes.h"


void ModbusRoutes::defineRoutes(AsyncWebServer &server)
{
    server.on("/graph", HTTP_GET, [](AsyncWebServerRequest *request){
        String htmlContent = viewGraph::generateHTML();
        const char *htmlContentPtr = htmlContent.c_str();
        request->send(200, "text/html", htmlContentPtr); 
    });
    
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
                //String htmlContent = viewCurrentRegister::generateHTML(registerAddress, registerValue);
                const char *htmlContentPtr = viewCurrentRegister::generateHTML(registerAddress, registerValue).c_str();
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
}