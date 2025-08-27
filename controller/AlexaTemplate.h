#ifndef ALEXA_TEMPLATE_H
#define ALEXA_TEMPLATE_H

#include <Arduino.h>
#include <WiFi.h>
#include "DeviceManager.h"

class AlexaTemplate {
private:
    String deviceMAC;
    IPAddress localIP;
    
    // Metodi privati per generazione template
    String escapeXML(const String& text);
    String escapeJSON(const String& text);
    
public:
    AlexaTemplate();
    ~AlexaTemplate();
    
    // Metodi per aggiornamento dati
    void updateNetworkInfo();
    void setIP(IPAddress ip) { localIP = ip; }
    void setMAC(const String& mac) { deviceMAC = mac; }
    
    // Template XML
    String generateDescriptionXML();
    
    // Template JSON per API Hue
    String generateDeviceListJSON(DeviceManager* deviceManager);
    String generateSingleDeviceJSON(const Device& device);
    String generateSuccessResponse(int deviceId, bool state);
    String generateErrorResponse(int errorType, const String& address, const String& description);
    
    // Template di autenticazione
    String generateAuthSuccessJSON();
    
    // Utility
    String getDeviceUUID() const;
    void printTemplateInfo();
};

#endif