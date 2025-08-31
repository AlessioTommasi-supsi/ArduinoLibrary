#include "outRoutes.h"
#include "outView.h"

void OutRoutes::defineRoutes(AsyncWebServer &server) {
    server.on("/uscite", HTTP_GET, [](AsyncWebServerRequest *request){
        String signalType = "";
        if (request->hasParam("signalType")) {
            signalType = request->getParam("signalType")->value();
        }
        String popup = "";
        if (request->hasParam("popup")) {
            popup = request->getParam("popup")->value();
            popup.replace("%20", " ");
        }
        String htmlContent = outView::generateHTML(signalType, popup);
        const char *htmlContentPtr = htmlContent.c_str();
        request->send(200, "text/html", htmlContentPtr);
    });

    server.on("/setAlarm", HTTP_GET, [](AsyncWebServerRequest *request){
        // Parse parameters with safe defaults
        String signalType = request->hasParam("signalType") ? request->getParam("signalType")->value() : "";
        String pin = request->hasParam("pin") ? request->getParam("pin")->value() : "";
        String type = request->hasParam("type") ? request->getParam("type")->value() : "";
        String alarmLevel = request->hasParam("alarmLevel") ? request->getParam("alarmLevel")->value() : "0.0";

        // Validate required parameters and redirect to popup page on error
        if (signalType.length() == 0) {
            String msg = "Error: Missing signalType";
            String encoded = msg;
            encoded.replace(" ", "%20");
            request->redirect(String("/uscitePopup?signalType=") + "" + "&message=" + encoded);
            return;
        }
        if (pin.length() == 0) {
            String msg = "Error: Missing pin";
            String encoded = msg;
            encoded.replace(" ", "%20");
            request->redirect(String("/uscitePopup?signalType=") + signalType + "&message=" + encoded);
            return;
        }
        if (type.length() == 0) {
            String msg = "Error: Missing type";
            String encoded = msg;
            encoded.replace(" ", "%20");
            request->redirect(String("/uscitePopup?signalType=") + signalType + "&message=" + encoded);
            return;
        }

        // Ensure alarmLevel has a numeric default
        if (type != "alarm") {
            alarmLevel = "0.0";
        }

        // Now it's safe to initialize ADS controller and start tasks
        ADS1115_controller* adsCtrl = ADS1115_controller::getInstance();
        adsCtrl->setChannel(signalType);
        if (adsCtrl->isInitializationFailed()) {
            String html = "<!DOCTYPE html><html><head><title>ADS Error</title></head><body style='text-align:center;margin:50px;'>";
            html += "<h1>❌ ADS1115 Error</h1>";
            html += "<p>L'ADS1115 non è inizializzato correttamente.</p>";
            html += "<a href='/reinitialize_ads' style='padding:10px 20px;background:#f44336;color:white;text-decoration:none;border-radius:5px;'>Reinitializza ADS</a><br><br>";
            html += "<a href='/multiplex_config'>← Torna alla configurazione</a>";
            html += "</body></html>";
            request->send(500, "text/html", html);
            return;
        }

        if (type == "alarm") {
            adsCtrl->startAlertMonitorTask(pin.toInt(), alarmLevel.toFloat());
            Serial.println("Started alert monitor on pin " + pin + " with threshold " + alarmLevel);
        } else if (type == "monitor") {
            adsCtrl->startMonitorTask(pin.toInt());
            Serial.println("Started monitor on pin " + pin);
        }

        // Background action: log the setup
        Serial.println("OUT set for signal: " + signalType + " on pin: " + pin + " type: " + type);
        if (type == "alarm") {
            Serial.println("Alarm threshold level: " + alarmLevel);
        }

        // Build success popup message and redirect to the popup page
        String popupMessage = "Allarme impostato con successo per il segnale: " + signalType;
        String encoded = popupMessage;
        encoded.replace(" ", "%20");
        request->redirect("/uscitePopup?signalType=" + signalType + "&message=" + encoded);
    });

    // Route that serves the standalone popup page with OK button
    server.on("/uscitePopup", HTTP_GET, [](AsyncWebServerRequest *request){
        String signalType = "";
        String message = "";
        if (request->hasParam("signalType")) {
            signalType = request->getParam("signalType")->value();
        }
        if (request->hasParam("message")) {
            message = request->getParam("message")->value();
            message.replace("%20", " ");
        }
        String encodedSignal = signalType;
        encodedSignal.replace(" ", "%20");
        String redirectPage = "/multiplex_graph?signalType=" + encodedSignal;
        String htmlContent = outView::generatePopupPage(signalType, message, redirectPage);
        const char *htmlContentPtr = htmlContent.c_str();
        request->send(200, "text/html", htmlContentPtr);
    });
}