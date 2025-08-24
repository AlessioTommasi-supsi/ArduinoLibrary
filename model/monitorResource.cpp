#include "monitorResource.h"


void MonitorResource::defineRoutes(AsyncWebServer &server)
{
    server.on("/monitor", HTTP_GET, [](AsyncWebServerRequest *request){
        try
        {
            String htmlContent = "";
            htmlContent += viewGeneric::defaultCssHeader("System Monitor");
            
            // Carica CSS specifico per i grafici monitor dinamicamente
            htmlContent += viewGeneric::dynamicUpdateContent("", "/monitorGraphStyle", UI_AUTO_UPDATE_DISABLED);
            
            // Container principale con design moderno
            htmlContent += "<div class='monitor-container'>";
            
            // Sezione HEAP
            htmlContent += "<div class='monitor-section'>";
            htmlContent += "<h1 class='monitor-title'>📊 System Monitor</h1>";
            htmlContent += "<h2 class='monitor-subtitle'>💾 Memoria Heap</h2>";

            // Ottieni le metriche di sistema HEAP
            size_t heapFree = heap_caps_get_free_size(MALLOC_CAP_8BIT);
            size_t heapTotal = heap_caps_get_total_size(MALLOC_CAP_8BIT);
            size_t heapUsed = heapTotal - heapFree;

            htmlContent += viewGraph::initCirularProgressBarGraph();
            htmlContent += viewGraph::generateCirularProgressBarGraph("HEAP", heapUsed, heapTotal, "/monitorHeapData", MONITOR_HEAP_UPDATE_INTERVAL);
            htmlContent += viewGraph::endCirularProgressBarGraph();
            htmlContent += "</div>"; // Chiude monitor-section

            // Sezione STACK USAGE
            htmlContent += "<div class='monitor-section'>";
            htmlContent += "<h2 class='monitor-subtitle'>🔧 Stack Usage per Pin Attivi</h2>";
            
            PinoutData *pinoutData = SystemState::getInstance()->pinoutData;
            
            // Conta pin attivi
            int activePins = 0;
            for (auto pin = pinoutData->begin(); pin != pinoutData->end(); ++pin)
            {
                if (pin->recordingTask != NULL) activePins++;
            }
            
            if (activePins > 0) {
                htmlContent += viewGraph::initCirularProgressBarGraph();

                // Loop through each pin in the pinout data
                for (auto pin = pinoutData->begin(); pin != pinoutData->end(); ++pin)
                {
                    if (pin->recordingTask != NULL)
                    {
                        // GRAFICO STACK per ogni pin attivo
                        size_t stackUsed = pin->getUsedStackInWords() * 4; // trasforma da parole a byte
                        size_t stackTotal = pin->getStackSizeInWords() * 4;

                        htmlContent += viewGraph::generateCirularProgressBarGraph(
                            "Pin" + String(pin->number) + " Stack", 
                            stackUsed, 
                            stackTotal, 
                            "/monitorPinStack?pin=" + String(pin->number), 
                            MONITOR_STACK_UPDATE_INTERVAL
                        );
                    }
                }
                htmlContent += viewGraph::endCirularProgressBarGraph();
            } else {
                htmlContent += "<div style='text-align: center; padding: 40px; color: #7f8c8d;'>";
                htmlContent += "<h3>ℹ️ Nessun pin in registrazione attiva</h3>";
                htmlContent += "<p>Vai alla sezione <a href='/pinout' style='color: #3498db;'>Pinout</a> per avviare la registrazione di un pin.</p>";
                htmlContent += "</div>";
            }
            
            htmlContent += "</div>"; // Chiude monitor-section
            htmlContent += "</div>"; // Chiude monitor-container
            
            htmlContent += viewGeneric::defaultFooter();

            // Invio della risposta HTTP
            request->send(200, "text/html", htmlContent);
        }
        catch(...)
        {
            String htmlContent = "<h1>❌ Errore nel caricamento del monitor</h1>";
            request->send(500, "text/html", htmlContent);
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
