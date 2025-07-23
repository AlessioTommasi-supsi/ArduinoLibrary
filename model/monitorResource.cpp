#include "monitorResource.h"


void MonitorResource::defineRoutes(AsyncWebServer &server)
{
    

    server.on("/monitor", HTTP_GET, [](AsyncWebServerRequest *request){
        try
        {
            String htmlContent = viewGeneric::basicHeader("System Monitor");
            htmlContent += viewGeneric::addExportCSVScript();
            
            htmlContent += "<h1>🔍 System Monitor</h1>";
            
            // CSS per il layout unificato
            htmlContent += "<style>";
            htmlContent += ".monitor-section { margin: 30px 0; }";
            htmlContent += ".monitor-grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(300px, 1fr)); gap: 20px; margin: 20px 0; }";
            htmlContent += ".monitor-card { background: rgba(255, 255, 255, 0.9); border-radius: 15px; padding: 20px; box-shadow: 0 4px 8px rgba(0,0,0,0.1); }";
            htmlContent += ".monitor-value { font-size: 24px; font-weight: bold; color: #2196F3; }";
            htmlContent += ".monitor-label { font-size: 14px; color: #666; margin-bottom: 5px; }";
            htmlContent += ".status-ok { color: #4CAF50; }";
            htmlContent += ".status-warning { color: #FF9800; }";
            htmlContent += ".status-critical { color: #f44336; }";
            htmlContent += ".progress-bar { width: 100%; height: 20px; background: #e0e0e0; border-radius: 10px; overflow: hidden; margin: 10px 0; }";
            htmlContent += ".progress-fill { height: 100%; background: linear-gradient(90deg, #4CAF50, #FF9800, #f44336); transition: width 0.3s ease; }";
            htmlContent += ".section-divider { border-top: 2px solid #e0e0e0; margin: 40px 0 20px 0; padding-top: 20px; }";
            htmlContent += "</style>";
            
            // Bottoni di export
            htmlContent += "<div style='text-align: center; margin: 20px 0;'>";
            htmlContent += "<button onclick=\"exportSystemData()\" style='background-color: #2196F3; color: white; padding: 10px 20px; border: none; border-radius: 5px; cursor: pointer; margin: 0 5px;'>📊 Export System CSV</button>";
            htmlContent += "<a href='/taskInfo' style='background-color: #FF9800; color: white; padding: 10px 20px; border: none; border-radius: 5px; text-decoration: none; margin: 0 5px;'>📋 Task Details</a>";
            htmlContent += "</div>";
            
            // Sezione System Stats (cards dinamiche)
            htmlContent += "<div class='monitor-section'>";
            htmlContent += "<h2>📊 System Statistics</h2>";
            htmlContent += "<div class='monitor-grid' id='systemMonitorGrid'>";
            htmlContent += "<!-- I dati verranno caricati dinamicamente -->";
            htmlContent += "</div>";
            htmlContent += "</div>";
            
            // Sezione Heap Monitor (grafico circolare esistente)
            htmlContent += "<div class='monitor-section section-divider'>";
            htmlContent += "<h2>💾 Memory Usage</h2>";
            htmlContent += viewGraph::initCirularProgressBarGraph();
            
            // HEAP - manteniamo il grafico circolare esistente
            size_t heapFree = heap_caps_get_free_size(MALLOC_CAP_8BIT);
            size_t heapTotal = heap_caps_get_total_size(MALLOC_CAP_8BIT);
            heapTotal = heapTotal * 0.6; // fattore di correzione come nell'originale
            size_t heapUsed = heapTotal - heapFree;
            
            htmlContent += viewGraph::generateCirularProgressBarGraph("HEAP", heapUsed, heapTotal, "monitorHeapData", 1000);
            htmlContent += viewGraph::endCirularProgressBarGraph();
            htmlContent += "</div>";
            
            // Sezione Stack Usage (grafici circolari dei pin)
            htmlContent += "<div class='monitor-section section-divider'>";
            htmlContent += "<h2>⚙️ Task Stack Usage</h2>";
            PinoutData *pinoutData = SystemState::getInstance()->pinoutData;
            htmlContent += viewGraph::initCirularProgressBarGraph();
            
            // Loop through each pin in the pinout data
            for (auto pin = pinoutData->begin(); pin != pinoutData->end(); ++pin)
            {
                if (pin->getRecordingTask() != NULL)
                {
                    size_t stackUsed = pin->getUsedStackInWords() * 4;
                    size_t stackTotal = pin->getStackSizeInWords() * 4;
                    htmlContent += viewGraph::generateCirularProgressBarGraph("Pin" + String(pin->getNumber()) + "Stack", stackUsed, stackTotal, "/monitorPinStack?pin=" + String(pin->getNumber()), 1000);
                }
            }
            htmlContent += viewGraph::endCirularProgressBarGraph();
            htmlContent += "</div>";
            
            // Script JavaScript unificato
            htmlContent += "<script>";
            
            // Funzione per aggiornare le system stats
            htmlContent += "function updateSystemMonitor() {";
            htmlContent += "  fetch('/systemStats')";
            htmlContent += "    .then(response => response.json())";
            htmlContent += "    .then(data => {";
            htmlContent += "      const grid = document.getElementById('systemMonitorGrid');";
            htmlContent += "      grid.innerHTML = '';";
            
            // Card Heap Memory dettagliata
            htmlContent += "      grid.innerHTML += `<div class='monitor-card'>`;";
            htmlContent += "        grid.innerHTML += '<h3>💾 Heap Memory Details</h3>';";
            htmlContent += "        grid.innerHTML += '<div class=\"monitor-label\">Used / Total</div>';";
            htmlContent += "        grid.innerHTML += '<div class=\"monitor-value\">' + (data.heap.used/1024).toFixed(1) + ' KB / ' + (data.heap.total/1024).toFixed(1) + ' KB</div>';";
            htmlContent += "        const heapPercent = (data.heap.used / data.heap.total * 100);";
            htmlContent += "        grid.innerHTML += '<div class=\"progress-bar\"><div class=\"progress-fill\" style=\"width: ' + heapPercent + '%\"></div></div>';";
            htmlContent += "        grid.innerHTML += '<div class=\"monitor-label\">Min Free: ' + (data.heap.minFree/1024).toFixed(1) + ' KB</div>';";
            htmlContent += "        const minFreePercent = (data.heap.minFree / data.heap.total * 100);";
            htmlContent += "        const statusClass = minFreePercent > 10 ? 'status-ok' : (minFreePercent > 5 ? 'status-warning' : 'status-critical');";
            htmlContent += "        grid.innerHTML += '<div class=\"monitor-label ' + statusClass + '\">Status: ' + (minFreePercent > 10 ? 'OK' : (minFreePercent > 5 ? 'Warning' : 'Critical')) + '</div>';";
            htmlContent += "        grid.innerHTML += `</div>`;";
            
            // Card PSRAM (se disponibile)
            htmlContent += "      if (data.psram.total > 0) {";
            htmlContent += "        grid.innerHTML += `<div class='monitor-card'>`;";
            htmlContent += "          grid.innerHTML += '<h3>🧠 PSRAM</h3>';";
            htmlContent += "          grid.innerHTML += '<div class=\"monitor-label\">Used / Total</div>';";
            htmlContent += "          grid.innerHTML += '<div class=\"monitor-value\">' + (data.psram.used/1024).toFixed(1) + ' KB / ' + (data.psram.total/1024).toFixed(1) + ' KB</div>';";
            htmlContent += "          const psramPercent = (data.psram.used / data.psram.total * 100);";
            htmlContent += "          grid.innerHTML += '<div class=\"progress-bar\"><div class=\"progress-fill\" style=\"width: ' + psramPercent + '%\"></div></div>';";
            htmlContent += "        grid.innerHTML += `</div>`;";
            htmlContent += "      }";
            
            // Card FreeRTOS
            htmlContent += "      grid.innerHTML += `<div class='monitor-card'>`;";
            htmlContent += "        grid.innerHTML += '<h3>⚙️ FreeRTOS Tasks</h3>';";
            htmlContent += "        grid.innerHTML += '<div class=\"monitor-label\">Active Tasks</div>';";
            htmlContent += "        grid.innerHTML += '<div class=\"monitor-value\">' + data.freertos.taskCount + '</div>';";
            htmlContent += "        grid.innerHTML += '<div class=\"monitor-label\">Main Stack Free: ' + (data.freertos.mainStackFree/1024).toFixed(1) + ' KB</div>';";
            htmlContent += "        const stackPercent = (data.freertos.mainStackFree / 8192 * 100);"; // Assumo 8KB di stack
            htmlContent += "        const stackStatus = stackPercent > 25 ? 'status-ok' : (stackPercent > 10 ? 'status-warning' : 'status-critical');";
            htmlContent += "        grid.innerHTML += '<div class=\"monitor-label ' + stackStatus + '\">Stack Status: ' + (stackPercent > 25 ? 'OK' : (stackPercent > 10 ? 'Warning' : 'Critical')) + '</div>';";
            htmlContent += "      grid.innerHTML += `</div>`;";
            
            // Card System Info
            htmlContent += "      const uptimeHours = (data.system.uptime / 3600000).toFixed(1);";
            htmlContent += "      const uptimeDays = (data.system.uptime / 86400000).toFixed(1);";
            htmlContent += "      grid.innerHTML += `<div class='monitor-card'>`;";
            htmlContent += "        grid.innerHTML += '<h3>🖥️ System Info</h3>';";
            htmlContent += "        grid.innerHTML += '<div class=\"monitor-label\">Uptime</div>';";
            htmlContent += "        grid.innerHTML += '<div class=\"monitor-value\">' + uptimeHours + ' hours (' + uptimeDays + ' days)</div>';";
            htmlContent += "        grid.innerHTML += '<div class=\"monitor-label\">CPU Freq: ' + data.system.cpuFreq + ' MHz</div>';";
            htmlContent += "        grid.innerHTML += '<div class=\"monitor-label\">Flash Free: ' + (data.system.freeSketchSpace/1024).toFixed(0) + ' KB</div>';";
            htmlContent += "        grid.innerHTML += '<div class=\"monitor-label\">Chip ID: ' + data.system.chipId + '</div>';";
            htmlContent += "      grid.innerHTML += `</div>`;";
            
            // Card WiFi
            htmlContent += "      const wifiStatus = data.wifi.connected === 'true' ? 'Connected' : 'Disconnected';";
            htmlContent += "      const wifiClass = data.wifi.connected === 'true' ? 'status-ok' : 'status-critical';";
            htmlContent += "      grid.innerHTML += `<div class='monitor-card'>`;";
            htmlContent += "        grid.innerHTML += '<h3>📶 WiFi Status</h3>';";
            htmlContent += "        grid.innerHTML += '<div class=\"monitor-label\">Connection</div>';";
            htmlContent += "        grid.innerHTML += '<div class=\"monitor-value ' + wifiClass + '\">' + wifiStatus + '</div>';";
            htmlContent += "        if (data.wifi.connected === 'true') {";
            htmlContent += "          const rssiClass = data.wifi.rssi > -50 ? 'status-ok' : (data.wifi.rssi > -70 ? 'status-warning' : 'status-critical');";
            htmlContent += "          grid.innerHTML += '<div class=\"monitor-label\">RSSI: <span class=\"' + rssiClass + '\">' + data.wifi.rssi + ' dBm</span></div>';";
            htmlContent += "          grid.innerHTML += '<div class=\"monitor-label\">IP: ' + data.wifi.localIP + '</div>';";
            htmlContent += "        }";
            htmlContent += "      grid.innerHTML += `</div>`;";
            
            htmlContent += "    })";
            htmlContent += "    .catch(error => console.error('Error:', error));";
            htmlContent += "}";
            
            // Funzione per export CSV
            htmlContent += "function exportSystemData() {";
            htmlContent += "  fetch('/systemStats')";
            htmlContent += "    .then(response => response.json())";
            htmlContent += "    .then(data => {";
            htmlContent += "      const timestamp = new Date().toISOString();";
            htmlContent += "      let csv = 'Timestamp,Parameter,Value,Unit\\n';";
            htmlContent += "      csv += timestamp + ',Heap Used,' + data.heap.used + ',bytes\\n';";
            htmlContent += "      csv += timestamp + ',Heap Total,' + data.heap.total + ',bytes\\n';";
            htmlContent += "      csv += timestamp + ',Heap Min Free,' + data.heap.minFree + ',bytes\\n';";
            htmlContent += "      csv += timestamp + ',PSRAM Used,' + data.psram.used + ',bytes\\n';";
            htmlContent += "      csv += timestamp + ',PSRAM Total,' + data.psram.total + ',bytes\\n';";
            htmlContent += "      csv += timestamp + ',Task Count,' + data.freertos.taskCount + ',count\\n';";
            htmlContent += "      csv += timestamp + ',Main Stack Free,' + data.freertos.mainStackFree + ',bytes\\n';";
            htmlContent += "      csv += timestamp + ',Uptime,' + data.system.uptime + ',ms\\n';";
            htmlContent += "      csv += timestamp + ',CPU Frequency,' + data.system.cpuFreq + ',MHz\\n';";
            htmlContent += "      csv += timestamp + ',Flash Free,' + data.system.freeSketchSpace + ',bytes\\n';";
            htmlContent += "      csv += timestamp + ',WiFi RSSI,' + data.wifi.rssi + ',dBm\\n';";
            htmlContent += "      ";
            htmlContent += "      const csvFile = new Blob([csv], { type: 'text/csv' });";
            htmlContent += "      const downloadLink = document.createElement('a');";
            htmlContent += "      downloadLink.download = 'system_monitor_' + new Date().getTime() + '.csv';";
            htmlContent += "      downloadLink.href = window.URL.createObjectURL(csvFile);";
            htmlContent += "      downloadLink.style.display = 'none';";
            htmlContent += "      document.body.appendChild(downloadLink);";
            htmlContent += "      downloadLink.click();";
            htmlContent += "      document.body.removeChild(downloadLink);";
            htmlContent += "    });";
            htmlContent += "}";
            
            // Avvia aggiornamento automatico
            htmlContent += "document.addEventListener('DOMContentLoaded', function() {";
            htmlContent += "  updateSystemMonitor();";
            htmlContent += "  setInterval(updateSystemMonitor, 3000);"; // Ogni 3 secondi
            htmlContent += "});";
            htmlContent += "</script>";
            
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

    // Nuovo endpoint per monitoraggio completo del sistema
    server.on("/systemStats", HTTP_GET, [](AsyncWebServerRequest *request){
        try
        {
            String json = "{";
            
            // HEAP memoria interna
            size_t heapFree = heap_caps_get_free_size(MALLOC_CAP_8BIT);
            size_t heapTotal = heap_caps_get_total_size(MALLOC_CAP_8BIT);
            size_t heapUsed = heapTotal - heapFree;
            size_t heapMinFree = heap_caps_get_minimum_free_size(MALLOC_CAP_8BIT);
            
            json += "\"heap\":{";
            json += "\"used\":" + String(heapUsed) + ",";
            json += "\"total\":" + String(heapTotal) + ",";
            json += "\"free\":" + String(heapFree) + ",";
            json += "\"minFree\":" + String(heapMinFree);
            json += "},";

            // PSRAM (se disponibile)
            size_t psramFree = heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
            size_t psramTotal = heap_caps_get_total_size(MALLOC_CAP_SPIRAM);
            size_t psramUsed = psramTotal - psramFree;
            
            json += "\"psram\":{";
            json += "\"used\":" + String(psramUsed) + ",";
            json += "\"total\":" + String(psramTotal) + ",";
            json += "\"free\":" + String(psramFree);
            json += "},";

            // FreeRTOS task info
            UBaseType_t taskCount = uxTaskGetNumberOfTasks();
            size_t freeStackMain = uxTaskGetStackHighWaterMark(NULL);
            
            json += "\"freertos\":{";
            json += "\"taskCount\":" + String(taskCount) + ",";
            json += "\"mainStackFree\":" + String(freeStackMain * 4); // converti in bytes
            json += "},";

            // Sistema generale
            unsigned long uptime = millis();
            uint32_t chipId = ESP.getEfuseMac() >> 24;
            uint32_t freeSketchSpace = ESP.getFreeSketchSpace();
            uint32_t sketchSize = ESP.getSketchSize();
            
            json += "\"system\":{";
            json += "\"uptime\":" + String(uptime) + ",";
            json += "\"chipId\":" + String(chipId) + ",";
            json += "\"sketchSize\":" + String(sketchSize) + ",";
            json += "\"freeSketchSpace\":" + String(freeSketchSpace) + ",";
            json += "\"cpuFreq\":" + String(ESP.getCpuFreqMHz());
            json += "},";

            // WiFi status
            json += "\"wifi\":{";
            json += "\"connected\":" + String(WiFi.status() == WL_CONNECTED ? "true" : "false") + ",";
            json += "\"rssi\":" + String(WiFi.RSSI()) + ",";
            json += "\"localIP\":\"" + WiFi.localIP().toString() + "\"";
            json += "}";

            json += "}";

            request->send(200, "application/json", json);
        }
        catch(...)
        {
            String json = "{\"error\":\"An error occurred\"}";
            request->send(500, "application/json", json);
        }
    });

    // Endpoint per diagnostica task dettagliata
    server.on("/taskInfo", HTTP_GET, [](AsyncWebServerRequest *request){
        try
        {
            String response = "";
            
            response = "<!DOCTYPE html><html><head><title>Task Info</title>";
            response += "<style>body{font-family:Arial,sans-serif;margin:20px;} .info-section{margin:20px 0;padding:15px;background:#f5f5f5;border-radius:8px;}</style>";
            response += "</head><body>";
            response += "<h1>FreeRTOS Task Information</h1>";
            
            // Informazioni generali sui task
            response += "<div class='info-section'>";
            response += "<h2>Task Statistics</h2>";
            response += "<p><strong>Total Tasks:</strong> " + String(uxTaskGetNumberOfTasks()) + "</p>";
            response += "<p><strong>Main Task Stack Free:</strong> " + String(uxTaskGetStackHighWaterMark(NULL) * 4) + " bytes</p>";
            response += "<p><strong>Scheduler State:</strong> " + String(xTaskGetSchedulerState() == taskSCHEDULER_RUNNING ? "Running" : "Not Running") + "</p>";
            response += "</div>";
            
            // Informazioni di sistema
            response += "<div class='info-section'>";
            response += "<h2>System Memory Info</h2>";
            response += "<p><strong>Heap Free:</strong> " + String(heap_caps_get_free_size(MALLOC_CAP_8BIT)) + " bytes</p>";
            response += "<p><strong>Heap Min Free:</strong> " + String(heap_caps_get_minimum_free_size(MALLOC_CAP_8BIT)) + " bytes</p>";
            response += "<p><strong>Heap Total:</strong> " + String(heap_caps_get_total_size(MALLOC_CAP_8BIT)) + " bytes</p>";
            
            // PSRAM se disponibile
            size_t psramTotal = heap_caps_get_total_size(MALLOC_CAP_SPIRAM);
            if (psramTotal > 0) {
                response += "<p><strong>PSRAM Total:</strong> " + String(psramTotal) + " bytes</p>";
                response += "<p><strong>PSRAM Free:</strong> " + String(heap_caps_get_free_size(MALLOC_CAP_SPIRAM)) + " bytes</p>";
            }
            response += "</div>";
            
            // Informazioni sui pin task attivi
            response += "<div class='info-section'>";
            response += "<h2>Pin Task Information</h2>";
            PinoutData *pinoutData = SystemState::getInstance()->pinoutData;
            bool hasActiveTasks = false;
            
            for (auto pin = pinoutData->begin(); pin != pinoutData->end(); ++pin) {
                if (pin->getRecordingTask() != NULL) {
                    hasActiveTasks = true;
                    response += "<p><strong>Pin " + String(pin->getNumber()) + " Task:</strong></p>";
                    response += "<ul>";
                    response += "<li>Stack Size: " + String(pin->getStackSizeInWords() * 4) + " bytes</li>";
                    response += "<li>Stack Used: " + String(pin->getUsedStackInWords() * 4) + " bytes</li>";
                    response += "<li>Stack Free: " + String((pin->getStackSizeInWords() - pin->getUsedStackInWords()) * 4) + " bytes</li>";
                    float stackUsage = (float)pin->getUsedStackInWords() / pin->getStackSizeInWords() * 100;
                    response += "<li>Stack Usage: " + String(stackUsage, 1) + "%</li>";
                    response += "</ul>";
                }
            }
            
            if (!hasActiveTasks) {
                response += "<p>No active pin recording tasks</p>";
            }
            response += "</div>";
            
            // Informazioni CPU
            response += "<div class='info-section'>";
            response += "<h2>CPU Information</h2>";
            response += "<p><strong>CPU Frequency:</strong> " + String(ESP.getCpuFreqMHz()) + " MHz</p>";
            response += "<p><strong>Chip Model:</strong> " + String(ESP.getChipModel()) + "</p>";
            response += "<p><strong>Chip Revision:</strong> " + String(ESP.getChipRevision()) + "</p>";
            response += "<p><strong>Flash Size:</strong> " + String(ESP.getFlashChipSize()) + " bytes</p>";
            response += "<p><strong>Free Sketch Space:</strong> " + String(ESP.getFreeSketchSpace()) + " bytes</p>";
            response += "</div>";
            
            response += "<div style='margin-top:20px;'>";
            response += "<a href='/monitor' style='padding:10px 20px;background:#007cba;color:white;text-decoration:none;border-radius:5px;'>← Back to Monitor</a>";
            response += "</div>";
            
            response += "</body></html>";
            
            request->send(200, "text/html", response);
        }
        catch(...)
        {
            String response = "Error getting task information";
            request->send(500, "text/html", response);
        }
    });

    // Pagina di monitoraggio avanzata
    server.on("/systemMonitor", HTTP_GET, [](AsyncWebServerRequest *request){
        try
        {
            String html = viewGeneric::basicHeader("System Monitor");
            html += viewGeneric::addExportCSVScript();
            
            html += "<h1>🔍 System Monitor</h1>";
            
            // CSS per il layout dei cards
            html += "<style>";
            html += ".monitor-grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(300px, 1fr)); gap: 20px; margin: 20px 0; }";
            html += ".monitor-card { background: rgba(255, 255, 255, 0.9); border-radius: 15px; padding: 20px; box-shadow: 0 4px 8px rgba(0,0,0,0.1); }";
            html += ".monitor-value { font-size: 24px; font-weight: bold; color: #2196F3; }";
            html += ".monitor-label { font-size: 14px; color: #666; margin-bottom: 5px; }";
            html += ".status-ok { color: #4CAF50; }";
            html += ".status-warning { color: #FF9800; }";
            html += ".status-critical { color: #f44336; }";
            html += ".progress-bar { width: 100%; height: 20px; background: #e0e0e0; border-radius: 10px; overflow: hidden; margin: 10px 0; }";
            html += ".progress-fill { height: 100%; background: linear-gradient(90deg, #4CAF50, #FF9800, #f44336); transition: width 0.3s ease; }";
            html += "</style>";
            
            // Aggiungi bottone export
            html += "<div style='text-align: center; margin: 20px 0;'>";
            html += "<button onclick=\"exportSystemData()\" style='background-color: #2196F3; color: white; padding: 10px 20px; border: none; border-radius: 5px; cursor: pointer;'>📊 Export System Data CSV</button>";
            html += "</div>";
            
            html += "<div class='monitor-grid' id='systemMonitorGrid'>";
            html += "<!-- I dati verranno caricati dinamicamente -->";
            html += "</div>";
            
            // Script per aggiornamento automatico
            html += "<script>";
            html += "function updateSystemMonitor() {";
            html += "  fetch('/systemStats')";
            html += "    .then(response => response.json())";
            html += "    .then(data => {";
            html += "      const grid = document.getElementById('systemMonitorGrid');";
            html += "      grid.innerHTML = '';";
            
            // Card Heap Memory
            html += "      grid.innerHTML += `<div class='monitor-card'>`;";
            html += "        grid.innerHTML += '<h3>💾 Heap Memory</h3>';";
            html += "        grid.innerHTML += '<div class=\"monitor-label\">Used / Total</div>';";
            html += "        grid.innerHTML += '<div class=\"monitor-value\">' + (data.heap.used/1024).toFixed(1) + ' KB / ' + (data.heap.total/1024).toFixed(1) + ' KB</div>';";
            html += "        const heapPercent = (data.heap.used / data.heap.total * 100);";
            html += "        grid.innerHTML += '<div class=\"progress-bar\"><div class=\"progress-fill\" style=\"width: ' + heapPercent + '%\"></div></div>';";
            html += "        grid.innerHTML += '<div class=\"monitor-label\">Min Free: ' + (data.heap.minFree/1024).toFixed(1) + ' KB</div>';";
            html += "        grid.innerHTML += `</div>`;";
            
            // Card PSRAM (se disponibile)
            html += "      if (data.psram.total > 0) {";
            html += "        grid.innerHTML += `<div class='monitor-card'>`;";
            html += "          grid.innerHTML += '<h3>🧠 PSRAM</h3>';";
            html += "          grid.innerHTML += '<div class=\"monitor-label\">Used / Total</div>';";
            html += "          grid.innerHTML += '<div class=\"monitor-value\">' + (data.psram.used/1024).toFixed(1) + ' KB / ' + (data.psram.total/1024).toFixed(1) + ' KB</div>';";
            html += "          const psramPercent = (data.psram.used / data.psram.total * 100);";
            html += "          grid.innerHTML += '<div class=\"progress-bar\"><div class=\"progress-fill\" style=\"width: ' + psramPercent + '%\"></div></div>';";
            html += "        grid.innerHTML += `</div>`;";
            html += "      }";
            
            // Card FreeRTOS
            html += "      grid.innerHTML += `<div class='monitor-card'>`;";
            html += "        grid.innerHTML += '<h3>⚙️ FreeRTOS</h3>';";
            html += "        grid.innerHTML += '<div class=\"monitor-label\">Active Tasks</div>';";
            html += "        grid.innerHTML += '<div class=\"monitor-value\">' + data.freertos.taskCount + '</div>';";
            html += "        grid.innerHTML += '<div class=\"monitor-label\">Main Stack Free: ' + (data.freertos.mainStackFree/1024).toFixed(1) + ' KB</div>';";
            html += "      grid.innerHTML += `</div>`;";
            
            // Card System Info
            html += "      const uptimeHours = (data.system.uptime / 3600000).toFixed(1);";
            html += "      const uptimeDays = (data.system.uptime / 86400000).toFixed(1);";
            html += "      grid.innerHTML += `<div class='monitor-card'>`;";
            html += "        grid.innerHTML += '<h3>🖥️ System</h3>';";
            html += "        grid.innerHTML += '<div class=\"monitor-label\">Uptime</div>';";
            html += "        grid.innerHTML += '<div class=\"monitor-value\">' + uptimeHours + ' hours (' + uptimeDays + ' days)</div>';";
            html += "        grid.innerHTML += '<div class=\"monitor-label\">CPU Freq: ' + data.system.cpuFreq + ' MHz</div>';";
            html += "        grid.innerHTML += '<div class=\"monitor-label\">Flash Free: ' + (data.system.freeSketchSpace/1024).toFixed(0) + ' KB</div>';";
            html += "      grid.innerHTML += `</div>`;";
            
            // Card WiFi
            html += "      const wifiStatus = data.wifi.connected === 'true' ? 'Connected' : 'Disconnected';";
            html += "      const wifiClass = data.wifi.connected === 'true' ? 'status-ok' : 'status-critical';";
            html += "      grid.innerHTML += `<div class='monitor-card'>`;";
            html += "        grid.innerHTML += '<h3>📶 WiFi</h3>';";
            html += "        grid.innerHTML += '<div class=\"monitor-label\">Status</div>';";
            html += "        grid.innerHTML += '<div class=\"monitor-value ' + wifiClass + '\">' + wifiStatus + '</div>';";
            html += "        if (data.wifi.connected === 'true') {";
            html += "          grid.innerHTML += '<div class=\"monitor-label\">RSSI: ' + data.wifi.rssi + ' dBm</div>';";
            html += "          grid.innerHTML += '<div class=\"monitor-label\">IP: ' + data.wifi.localIP + '</div>';";
            html += "        }";
            html += "      grid.innerHTML += `</div>`;";
            
            html += "    })";
            html += "    .catch(error => console.error('Error:', error));";
            html += "}";
            
            // Funzione per export CSV
            html += "function exportSystemData() {";
            html += "  fetch('/systemStats')";
            html += "    .then(response => response.json())";
            html += "    .then(data => {";
            html += "      const timestamp = new Date().toISOString();";
            html += "      let csv = 'Timestamp,Parameter,Value,Unit\\n';";
            html += "      csv += timestamp + ',Heap Used,' + data.heap.used + ',bytes\\n';";
            html += "      csv += timestamp + ',Heap Total,' + data.heap.total + ',bytes\\n';";
            html += "      csv += timestamp + ',Heap Min Free,' + data.heap.minFree + ',bytes\\n';";
            html += "      csv += timestamp + ',PSRAM Used,' + data.psram.used + ',bytes\\n';";
            html += "      csv += timestamp + ',PSRAM Total,' + data.psram.total + ',bytes\\n';";
            html += "      csv += timestamp + ',Task Count,' + data.freertos.taskCount + ',count\\n';";
            html += "      csv += timestamp + ',Main Stack Free,' + data.freertos.mainStackFree + ',bytes\\n';";
            html += "      csv += timestamp + ',Uptime,' + data.system.uptime + ',ms\\n';";
            html += "      csv += timestamp + ',CPU Frequency,' + data.system.cpuFreq + ',MHz\\n';";
            html += "      csv += timestamp + ',Flash Free,' + data.system.freeSketchSpace + ',bytes\\n';";
            html += "      csv += timestamp + ',WiFi RSSI,' + data.wifi.rssi + ',dBm\\n';";
            html += "      ";
            html += "      const csvFile = new Blob([csv], { type: 'text/csv' });";
            html += "      const downloadLink = document.createElement('a');";
            html += "      downloadLink.download = 'system_monitor_' + new Date().getTime() + '.csv';";
            html += "      downloadLink.href = window.URL.createObjectURL(csvFile);";
            html += "      downloadLink.style.display = 'none';";
            html += "      document.body.appendChild(downloadLink);";
            html += "      downloadLink.click();";
            html += "      document.body.removeChild(downloadLink);";
            html += "    });";
            html += "}";
            
            // Avvia aggiornamento automatico
            html += "document.addEventListener('DOMContentLoaded', function() {";
            html += "  updateSystemMonitor();";
            html += "  setInterval(updateSystemMonitor, 3000);"; // Ogni 3 secondi
            html += "});";
            html += "</script>";
            
            html += viewGeneric::defaultFooter();
            
            request->send(200, "text/html", html);
        }
        catch(...)
        {
            String response = "Error loading system monitor";
            request->send(500, "text/html", response);
        }
    });
}
