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
    