#include "MultiplexRoutes.h"
#include <vector>
#include <set>
#include "viewADS.h"

void MultiplexRoutes::defineRoutes(AsyncWebServer &server) {

    server.on("/multiplex_config", HTTP_GET, [](AsyncWebServerRequest *request) {
        String htmlContent = viewMultiplex::Config();
        request->send(200, "text/html", htmlContent);
    });
   
     server.on("/getADSValues", HTTP_GET, [](AsyncWebServerRequest *request) {
        ADS1115_controller* adsCtrl = ADS1115_controller::getInstance();
        
        if (adsCtrl->isInitializationFailed()) {
            // Se l'inizializzazione è fallita, restituisce un messaggio d'errore
            request->send(500, "text/html", "<h1>Error: ADS1115 Initialization Failed.</h1>");
            return;
        }
        
        const std::vector<float>& values = adsCtrl->getRecordedValues();
        String json = "[";
        for (size_t i = 0; i < values.size(); i++) {
            if (i > 0)
                json += ",";
            json += String(values[i]);
        }
        json += "]";
        request->send(200, "application/json", json);
    });

    // New endpoint for real-time value
    server.on("/getMultiplexValue", HTTP_GET, [](AsyncWebServerRequest *request) {
        ADS1115_controller* adsCtrl = ADS1115_controller::getInstance();
        
        String signalType = "";
        if (request->hasParam("signalType")) {
            signalType = request->getParam("signalType")->value();
        }
        
        // TEST MODE - simulate random values
        if (signalType == "TEST") {
            float simulatedValue = random(100, 500) / 10.0; // Random value between 10.0 and 50.0
            String json = "{\"value\":" + String(simulatedValue, 2) + ",\"signalType\":\"TEST\",\"mode\":\"simulation\"}";
            request->send(200, "application/json", json);
            return;
        }
        
        // Check if ADS is initialized for real signals
        if (adsCtrl->isInitializationFailed()) {
            String errorJson = "{\"error\":\"ADS1115 not initialized\",\"value\":0}";
            request->send(500, "application/json", errorJson);
            return;
        }
        
        adsCtrl->setChannel(signalType);
        
        // Try to read current value safely
        float currentValue = 0.0;
        try {
            currentValue = adsCtrl->read();
        } catch (...) {
            Serial.println("Error reading ADS1115 value");
            String errorJson = "{\"error\":\"Failed to read ADS1115\",\"value\":0}";
            request->send(500, "application/json", errorJson);
            return;
        }
        
        String json = "{\"value\":" + String(currentValue, 4) + ",\"signalType\":\"" + signalType + "\"}";
        request->send(200, "application/json", json);
    });

    // Endpoint to retry ADS initialization
    server.on("/reinitialize_ads", HTTP_GET, [](AsyncWebServerRequest *request) {
        ADS1115_controller* adsCtrl = ADS1115_controller::getInstance();
        adsCtrl->reinitialize();
        
        if (adsCtrl->isInitializationFailed()) {
            String htmlContent = "<h1>Reinitializzazione Fallita</h1>";
            htmlContent += "<p>L'ADS1115 non riesce ancora a inizializzarsi. Controlla i collegamenti I2C.</p>";
            htmlContent += "<form action='/reinitialize_ads' method='get'><button type='submit'>Riprova</button></form>";
            htmlContent += "<br><a href='/multiplex_config'>Torna alla configurazione</a>";
            request->send(500, "text/html", htmlContent);
        } else {
            String htmlContent = "<h1>Reinizializzazione Riuscita!</h1>";
            htmlContent += "<p>L'ADS1115 è stato inizializzato correttamente.</p>";
            htmlContent += "<br><a href='/multiplex_config'>Torna alla configurazione</a>";
            request->send(200, "text/html", htmlContent);
        }
    });

    server.on("/ads", HTTP_GET, [](AsyncWebServerRequest *request) {
        String content = viewADS::generateHTML();
        request->send(200, "text/html", content);
    });

    // Endpoint for TEST mode - simulated graph data
    server.on("/getTestValues", HTTP_GET, [](AsyncWebServerRequest *request) {
        // Generate simulated data for the graph
        String json = "[";
        int numPoints = 20; // Generate 20 data points
        
        for (int i = 0; i < numPoints; i++) {
            if (i > 0) json += ",";
            
            // Create realistic looking data with some variation
            float baseValue = 25.0; // Base temperature value
            float variation = sin(i * 0.3) * 5.0; // Sine wave variation
            float noise = (random(-100, 100) / 100.0); // Small random noise
            float simulatedValue = baseValue + variation + noise;
            
            json += String(simulatedValue, 2);
        }
        
        json += "]";
        request->send(200, "application/json", json);
    });

    // Endpoint: /multiplex_graph
    // Gestisce l'input della configurazione e avvia/ferma la registrazione.
    // Genera la pagina HTML con il grafico, inserendo anche un eventuale messaggio d'errore
    // e un bottone "Riprova inizializzazione" se l'ADS1115 non è stato inizializzato correttamente.
    server.on("/multiplex_graph", HTTP_GET, [](AsyncWebServerRequest *request) {
        String signalType;
        String action = "";
        String milliseconds;
        String ErrorMessage = "";

        if (request->hasParam("signalType")) {
            signalType = request->getParam("signalType")->value();
            Serial.println("Signal type: " + signalType);
        } else {
            request->send(400, "text/plain", "Error: Missing parameter signalType");
            return;
        }
        
        if (request->hasParam("action")) {
            action = request->getParam("action")->value();
            Serial.println("Action received: " + action);
        } else {
            action = "first_entry_on_this_page";
        }
        
        if (request->hasParam("milliseconds")) {
            milliseconds = request->getParam("milliseconds")->value();
            Serial.println("Milliseconds: " + milliseconds);
        } else {
            milliseconds = "1000";
        }
        
        // TEST MODE - skip ADS initialization check BEFORE accessing ADS controller
        if (signalType == "TEST") {
            Serial.println("TEST mode activated - bypassing ADS1115");
            
            // In modalità TEST, non chiamare MAI l'ADS1115_controller
            // Gestisci solo le azioni che non richiedono hardware
            if (action == "start_recording" || action == "stop_recording" || 
                action == "start_monitor" || action == "stop_monitor" ||
                action == "start_monitor_alert" || action == "stop_monitor_alert") {
                Serial.println("TEST mode: Azione " + action + " simulata (nessun hardware coinvolto)");
            }
            
            // Generate test page without ADS operations
            String htmlContent = viewGeneric::defaultCssHeader("Graph View - TEST Mode");
            htmlContent += viewMultiplex::pinStartAndStopForm(999, signalType); // Use 999 as test channel
            
            // Create a test vector for the graph - use empty vector to avoid selector
            std::vector<int> testVector = {};  // Empty vector = no selector, direct graph
            htmlContent += "<script>";
            htmlContent += "document.addEventListener('DOMContentLoaded', function() {";
            htmlContent += "const graphContainer = document.getElementById('graphContainer');";
            htmlContent += "if (graphContainer) {";
            htmlContent += "graphContainer.innerHTML = `";
            htmlContent += "<canvas id='myChart' style='width: 100%; height: 100%; display: block; border: 1px solid #ddd; border-radius: 8px;'></canvas>";
            htmlContent += "`;";
            htmlContent += "}";
            htmlContent += "});";
            htmlContent += "</script>";
            htmlContent += viewGraph::generateBasicJavaScript();
            htmlContent += viewGraph::generateDrawFunctionJS();
            htmlContent += viewGraph::generateUpdateFunctionJS("getTestValues", "channel");
            htmlContent += viewGraph::generateInitializationJS();
            htmlContent += viewGeneric::defaultFooter();
            
            request->send(200, "text/html", htmlContent);
            return;
        }

        // Only access ADS controller for NON-TEST modes
        ADS1115_controller* adsCtrl = ADS1115_controller::getInstance();
        
        // Set recording interval only for real ADS operations
        if (request->hasParam("milliseconds")) {
            adsCtrl->recordingInterval = milliseconds.toInt();
        }
        
        // Check ADS initialization before proceeding (only for real signals)
        if (adsCtrl->isInitializationFailed()) {
            ErrorMessage = "<h1>Error: ADS1115 Initialization Failed.</h1>";
            ErrorMessage += "<p>Impossibile procedere senza ADS1115 funzionante.</p>";
            ErrorMessage += "<form action='/reinitialize_ads' method='get'><button type='submit'>Riprova inizializzazione</button></form>";
            ErrorMessage += "<br><a href='/multiplex_config'>Torna alla configurazione</a>";
            Serial.println("Initialization failed: " + ErrorMessage);
            request->send(500, "text/html", ErrorMessage);
            return;
        }
        
        // Imposta il canale del multiplexer in base al signalType (only if ADS is working)
        adsCtrl->setChannel(signalType);

        //gestione pin che monitora un altro pin! posso fare monitor di 1 solo pi di out per volta! e solo o monitor o solo alert!!
        if (action == "start_monitor") {
            int outputPinNumber = request->getParam("out_pin_number")->value().toInt();
            adsCtrl->startMonitorTask(outputPinNumber);
        } else if (action == "stop_monitor") {
            adsCtrl->stopMonitorTask();
        }
        if (action == "start_monitor_alert") {
            int outputPinNumber = request->getParam("out_pin_number")->value().toInt();
            float alertValue = request->getParam("alert_value")->value().toFloat();
            adsCtrl->startAlertMonitorTask(outputPinNumber, alertValue);
        } else if (action == "stop_monitor_alert") {
            adsCtrl->stopMonitorTask();
        }

        if (action == "start_recording") {
            adsCtrl->startRecording(signalType, milliseconds.toInt());
        } else if (action == "stop_recording") {
            adsCtrl->stopRecording();
        }
        
        Serial.println("Action completed, preparing response...");
        
        // Prepara un vettore contenente il canale selezionato (un singolo elemento)
        std::vector<int> channelVector = { adsCtrl->getCurrentChannel() };
        
        String htmlContent = viewGeneric::defaultCssHeader("Graph View");
        htmlContent += viewMultiplex::pinStartAndStopForm(adsCtrl->getCurrentChannel(), signalType);
        htmlContent += viewGraph::generateGraph(channelVector, "getADSValues", "channel");
        htmlContent += viewGeneric::defaultFooter();
        
        request->send(200, "text/html", htmlContent);
    });
}
