#include "monitorResource.h"


void MonitorResource::defineRoutes(AsyncWebServer &server)
{
    

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
            heapTotal = heapTotal * 0.6; // ho visto da test che sopra il 70% da errore dunque mantengo questo fattore di correzione!
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
                if (pin->getRecordingTask() != NULL)
                {
                    // STANPO GRAFICO CON LO STACK LIB
                    size_t stackUsed = pin->getUsedStackInWords() * 4; // trasformo da parole a byte
                    size_t stackTotal = pin->getStackSizeInWords() * 4;

                    htmlContent += viewGraph::generateCirularProgressBarGraph("Pin" + String(pin->getNumber()) + "Stack", stackUsed, stackTotal, "/monitorPinStack?pin=" + String(pin->getNumber()), 1000);
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
                if (pin->getRecordingTask() != NULL)
                {
                    // STANPO GRAFICO CON LO STACK LIB
                    size_t stackUsed = pin->getUsedStackInWords() * 4; // trasformo da parole a byte
                    size_t stackTotal = pin->getStackSizeInWords() * 4;

                    htmlContent += viewGraph::generateCirularProgressBarGraph("Pin" + String(pin->getNumber()) + "Stack", stackUsed, stackTotal, "/monitorPinStack?pin=" + String(pin->getNumber()), 1000);
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
            String htmlContent = "";

            // Generate monitor content for all pins
            for (auto pin = SystemState::getInstance()->pinoutData->begin(); pin != SystemState::getInstance()->pinoutData->end(); ++pin)
            {
                if (pin->getRecordingTask() != NULL)
                {
                    size_t stackUsed = pin->getUsedStackInWords();
                    size_t stackTotal = pin->getStackSizeInWords();

                    htmlContent += viewGraph::generateCirularProgressBarGraph("Pin" + String(pin->getNumber()) + "Stack", stackUsed, stackTotal, "/monitorPinStack?pin=" + String(pin->getNumber()), 1000);
                }
            }

            if (request->hasParam("pin"))
            {
                String pin = request->getParam("pin")->value();
                String json = "[]";

                Pin &selectedPin = SystemState::getInstance()->pinoutData->getPin(pin.toInt());
                size_t stackUsed = selectedPin.getUsedStackInWords();
                size_t stackTotal = selectedPin.getStackSizeInWords();

                json = "[" + String(stackUsed) + "," + String(stackTotal) + "]";
                request->send(200, "application/json", json);
            }
            else
            {
                // Use viewGeneric instead of monitorGeneric
                String fullHtml = viewGeneric::defaultCssHeader("Monitor Pin Stack");
                fullHtml += htmlContent;
                fullHtml += viewGeneric::defaultFooter();
                
                const char *htmlContentPtr = fullHtml.c_str();
                request->send(200, "text/html", htmlContentPtr);
            }
        }
        catch (const std::exception &e)
        {
            Serial.println("Error: " + String(e.what()));
            request->send(500, "text/plain", "Internal Server Error");
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
