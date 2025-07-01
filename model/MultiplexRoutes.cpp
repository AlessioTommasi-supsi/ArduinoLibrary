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
        // Se non ci sono valori, restituisce un array vuoto (oppure puoi decidere di restituire [0])
        if(values.size() == 0) {
            json = "[]";
        } else {
            json += "]";
        }
        request->send(200, "application/json", json);
    });

    // Endpoint: /ADS_history
    // Mostra la pagina della cronologia delle letture ADS1115
    server.on("/ADS_history", HTTP_GET, [](AsyncWebServerRequest *request) {
        String htmlContent = viewADS::generateHTML();
        request->send(200, "text/html", htmlContent);
    });

    // Endpoint: /getADSContent
    // Restituisce il contenuto aggiornato della tabella ADS per l'aggiornamento dinamico
    server.on("/getADSContent", HTTP_GET, [](AsyncWebServerRequest *request) {
        String content = viewADS::adsContent();
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
        
        // Imposta il canale del multiplexer in base al signalType
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
        
        if (adsCtrl->isInitializationFailed()) {
            // Aggiunge un messaggio d'errore e un form con un bottone per tentare la re-inizializzazione.
            ErrorMessage = "<h1>Error: ADS1115 Initialization Failed.</h1>";
            ErrorMessage += "<form action='/reinitialize_ads' method='get'><button type='submit'>Riprova inizializzazione</button></form><br><br><br><br><br>";
            Serial.println("Initialization failed: " + ErrorMessage);
        }
        
        Serial.println("Action completed, preparing response...");
        
        // Prepara un vettore contenente il canale selezionato (un singolo elemento)
        std::vector<int> channelVector = { adsCtrl->getCurrentChannel() };
        
        String htmlContent = viewGeneric::defaultCssHeader("Graph View");
        htmlContent += viewMultiplex::pinStartAndStopForm(adsCtrl->getCurrentChannel(), signalType);
        htmlContent += viewGraph::generateGraph(channelVector, "getADSValues", "ads1115");
        htmlContent += ErrorMessage;
        
        request->send(200, "text/html", htmlContent);
    });

    // Endpoint: /reinitialize_ads
    // Tenta di re-inizializzare l'ADS1115 e restituisce il risultato (con un link per tornare al Graph)
    server.on("/reinitialize_ads", HTTP_GET, [](AsyncWebServerRequest *request) {
        ADS1115_controller* adsCtrl = ADS1115_controller::getInstance();
        adsCtrl->reinitialize();
        String Message = "";
        if (adsCtrl->isInitializationFailed()) {
            Message = "<h1>Riprova inizializzazione fallita.</h1>";
        } else {
            Message = "<h1>ADS1115 re-inizializzato con successo!</h1>";
        }
        Message += "<a href='/multiplex_config'>Torna alle configurazioni</a>";
        request->send(200, "text/html", Message);
    });
}
