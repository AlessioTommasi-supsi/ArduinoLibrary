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
        
        // Check if ADS is initialized
        if (adsCtrl->isInitializationFailed()) {
            String errorJson = "{\"error\":\"ADS1115 not initialized\",\"value\":0}";
            request->send(500, "application/json", errorJson);
            return;
        }
        
        String signalType = "";
        if (request->hasParam("signalType")) {
            signalType = request->getParam("signalType")->value();
            adsCtrl->setChannel(signalType);
        }
        
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

    // Endpoint: /multiplex_graph
    // Gestisce l'input della configurazione e avvia/ferma la registrazione.
    // Genera la pagina HTML con il grafico, inserendo anche un eventuale messaggio d'errore
    // e un bottone "Riprova inizializzazione" se l'ADS1115 non è stato inizializzato correttamente.
    server.on("/multiplex_graph", HTTP_GET, [](AsyncWebServerRequest *request) {
        String signalType;
        String action;
        String milliseconds;
        String ErrorMessage = "";

        
        ADS1115_controller* adsCtrl = ADS1115_controller::getInstance();
        
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
            adsCtrl->recordingInterval = milliseconds.toInt();
            Serial.println("Milliseconds: " + milliseconds);
        } else {
            milliseconds = "1000";
        }
        
        // Check ADS initialization before proceeding
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
