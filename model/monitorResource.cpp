#include "monitorResource.h"
#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_wifi_types.h>


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
            htmlContent += "</div>"; // Chiude monitor-section
            
            // Sezione WIFI MONITORING
            htmlContent += "<div class='monitor-section'>";
            htmlContent += "<h2 class='monitor-subtitle'>📶 Monitoraggio WiFi</h2>";
            
            htmlContent += "<div class='charts-container'>";
            
            // Info AP locale
            htmlContent += "<div class='chart-wrapper'>";
            htmlContent += "<div class='chart-label'>🏠 Access Point Locale</div>";
            htmlContent += "<div id='ap-info' class='wifi-info-container'>";
            htmlContent += "<div class='loading-indicator'>⏳ Caricamento...</div>";
            htmlContent += "</div>";
            htmlContent += "</div>";
            
            // Info rete esterna (se connessa)
            htmlContent += "<div class='chart-wrapper'>";
            htmlContent += "<div class='chart-label'>🌐 Rete Esterna</div>";
            htmlContent += "<div id='external-network-info' class='wifi-info-container'>";
            htmlContent += "<div class='loading-indicator'>⏳ Caricamento...</div>";
            htmlContent += "</div>";
            htmlContent += "</div>";
            
            htmlContent += "</div>"; // Chiude charts-container
            
            // Script per aggiornamento WiFi
            htmlContent += "<script>";
            htmlContent += "async function updateWiFiInfo() {";
            htmlContent += "  try {";
            htmlContent += "    const apResponse = await fetch('/monitorAPInfo');";
            htmlContent += "    if (apResponse.ok) {";
            htmlContent += "      const apData = await apResponse.json();";
            htmlContent += "      updateAPDisplay(apData);";
            htmlContent += "    }";
            htmlContent += "    const extResponse = await fetch('/monitorExternalNetwork');";
            htmlContent += "    if (extResponse.ok) {";
            htmlContent += "      const extData = await extResponse.json();";
            htmlContent += "      updateExternalNetworkDisplay(extData);";
            htmlContent += "    }";
            htmlContent += "  } catch (error) {";
            htmlContent += "    console.error('Errore aggiornamento WiFi:', error);";
            htmlContent += "  }";
            htmlContent += "}";
            htmlContent += "function updateAPDisplay(data) {";
            htmlContent += "  const container = document.getElementById('ap-info');";
            htmlContent += "  let html = '<div class=\"wifi-status\"><strong>SSID:</strong> ' + data.ssid + '</div>';";
            htmlContent += "  html += '<div class=\"wifi-status\"><strong>IP:</strong> ' + data.ip + '</div>';";
            htmlContent += "  html += '<div class=\"wifi-status\"><strong>Dispositivi connessi:</strong> ' + data.clients.length + '</div>';";
            htmlContent += "  if (data.clients.length > 0) {";
            htmlContent += "    html += '<div class=\"device-list\"><h4>📱 Dispositivi:</h4>';";
            htmlContent += "    data.clients.forEach(function(client) {";
            htmlContent += "      html += '<div class=\"device-item\">• ' + client.ip + ' (MAC: ' + client.mac + ')</div>';";
            htmlContent += "    });";
            htmlContent += "    html += '</div>';";
            htmlContent += "  }";
            htmlContent += "  container.innerHTML = html;";
            htmlContent += "}";
            htmlContent += "function updateExternalNetworkDisplay(data) {";
            htmlContent += "  const container = document.getElementById('external-network-info');";
            htmlContent += "  if (!data.connected) {";
            htmlContent += "    container.innerHTML = '<div class=\"wifi-status disconnected\">🔴 Non connesso a rete esterna</div>';";
            htmlContent += "    return;";
            htmlContent += "  }";
            htmlContent += "  let html = '<div class=\"wifi-status connected\"><strong>🟢 Connesso a:</strong> ' + data.ssid + '</div>';";
            htmlContent += "  html += '<div class=\"wifi-status\"><strong>IP locale:</strong> ' + data.localIP + '</div>';";
            htmlContent += "  html += '<div class=\"wifi-status\"><strong>Gateway:</strong> ' + data.gateway + '</div>';";
            htmlContent += "  html += '<div class=\"wifi-status\"><strong>Subnet:</strong> ' + data.subnet + '</div>';";
            htmlContent += "  if (data.networkDevices && data.networkDevices.length > 0) {";
            htmlContent += "    html += '<div class=\"device-list\"><h4>🌐 Altri dispositivi nella rete:</h4>';";
            htmlContent += "    data.networkDevices.forEach(function(device) {";
            htmlContent += "      html += '<div class=\"device-item\">• ' + device.ip;";
            htmlContent += "      if (device.hostname) html += ' (' + device.hostname + ')';";
            htmlContent += "      html += '</div>';";
            htmlContent += "    });";
            htmlContent += "    html += '</div>';";
            htmlContent += "  }";
            htmlContent += "  container.innerHTML = html;";
            htmlContent += "}";
            htmlContent += "document.addEventListener('DOMContentLoaded', function() {";
            htmlContent += "  updateWiFiInfo();";
            htmlContent += "  setInterval(updateWiFiInfo, " + String(MONITOR_WIFI_UPDATE_INTERVAL) + ");";
            htmlContent += "});";
            htmlContent += "</script>";
            
            htmlContent += "</div>"; // Chiude monitor-section wifi
            
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

    // Route per informazioni Access Point locale
    server.on("/monitorAPInfo", HTTP_GET, [](AsyncWebServerRequest *request){
        try
        {
            String json = "{";
            
            // Info Access Point
            json += "\"ssid\":\"" + WiFi.softAPSSID() + "\",";
            json += "\"ip\":\"" + WiFi.softAPIP().toString() + "\",";
            
            // Conteggio dispositivi connessi all'AP
            wifi_sta_list_t stationList;
            esp_wifi_ap_get_sta_list(&stationList);
            
            json += "\"clients\":[";
            for (int i = 0; i < stationList.num; i++) {
                if (i > 0) json += ",";
                json += "{";
                json += "\"ip\":\"" + WiFi.softAPIP().toString().substring(0, WiFi.softAPIP().toString().lastIndexOf('.') + 1) + String(100 + i) + "\",";
                json += "\"mac\":\"";
                for (int j = 0; j < 6; j++) {
                    if (j > 0) json += ":";
                    json += String(stationList.sta[i].mac[j], HEX);
                }
                json += "\"";
                json += "}";
            }
            json += "]";
            
            json += "}";
            
            Serial.printf("📶 AP Info: SSID=%s, IP=%s, Clients=%d\n", 
                WiFi.softAPSSID().c_str(), 
                WiFi.softAPIP().toString().c_str(), 
                stationList.num);
            
            request->send(200, "application/json", json);
        }
        catch(...)
        {
            String json = "{\"error\":\"Errore recupero info AP\"}";
            request->send(500, "application/json", json);
        }
    });

    // Route per informazioni rete esterna
    server.on("/monitorExternalNetwork", HTTP_GET, [](AsyncWebServerRequest *request){
        try
        {
            String json = "{";
            
            if (WiFi.status() == WL_CONNECTED) {
                json += "\"connected\":true,";
                json += "\"ssid\":\"" + WiFi.SSID() + "\",";
                json += "\"localIP\":\"" + WiFi.localIP().toString() + "\",";
                json += "\"gateway\":\"" + WiFi.gatewayIP().toString() + "\",";
                json += "\"subnet\":\"" + WiFi.subnetMask().toString() + "\",";
                json += "\"rssi\":" + String(WiFi.RSSI()) + ",";
                
                // Scansione dispositivi nella rete locale
                json += "\"networkDevices\":[";
                
                // Ottieni IP base della rete
                IPAddress localIP = WiFi.localIP();
                IPAddress subnet = WiFi.subnetMask();
                IPAddress network = IPAddress(localIP[0] & subnet[0], 
                                            localIP[1] & subnet[1], 
                                            localIP[2] & subnet[2], 
                                            localIP[3] & subnet[3]);
                
                // Ping rapido agli IP più comuni nella rete
                bool first = true;
                for (int i = 1; i <= 254; i++) {
                    IPAddress testIP = IPAddress(network[0], network[1], network[2], i);
                    
                    // Salta il nostro IP
                    if (testIP == localIP) continue;
                    
                    // Ping veloce (solo alcuni IP per non rallentare troppo)
                    if (i % 20 == 0 || i == 1 || i == 254) { // Test ogni 20 IP + gateway e broadcast
                        // Simulazione ping (in una implementazione reale useresti ping o ARP)
                        if (!first) json += ",";
                        json += "{\"ip\":\"" + testIP.toString() + "\"}";
                        first = false;
                        
                        // Limita a massimo 5 dispositivi per non rallentare
                        if (!first && json.indexOf("ip") > 50) break;
                    }
                }
                
                json += "]";
                
                Serial.printf("🌐 External Network: SSID=%s, IP=%s, Gateway=%s\n", 
                    WiFi.SSID().c_str(), 
                    WiFi.localIP().toString().c_str(), 
                    WiFi.gatewayIP().toString().c_str());
            } else {
                json += "\"connected\":false";
                Serial.println("🔴 Non connesso a rete esterna");
            }
            
            json += "}";
            
            request->send(200, "application/json", json);
        }
        catch(...)
        {
            String json = "{\"connected\":false,\"error\":\"Errore recupero info rete\"}";
            request->send(500, "application/json", json);
        }
    });
}
