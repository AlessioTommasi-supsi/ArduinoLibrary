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
        String signalType = request->getParam("signalType")->value();
        String pin = request->getParam("pin")->value();
        String type = request->getParam("type")->value();
        String alarmLevel = "";
        if (request->hasParam("alarmLevel")) {
            alarmLevel = request->getParam("alarmLevel")->value();
        }

        // Background action: log the alarm setup
        Serial.println("Alarm set for signal: " + signalType + " on pin: " + pin + " type: " + type);
        if (type == "alarm" && alarmLevel != "") {
            Serial.println("Alarm threshold level: " + alarmLevel);
        }

        // Build popup message and URL-encode spaces
        String popupMessage = "Allarme impostato con successo per il segnale: " + signalType;
        String encoded = popupMessage;
        encoded.replace(" ", "%20");

        // Redirect to the dedicated popup page
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