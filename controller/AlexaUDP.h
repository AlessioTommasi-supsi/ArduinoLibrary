#ifndef ALEXA_UDP_H
#define ALEXA_UDP_H

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <IPAddress.h>

// ➕ COSTANTI UDP SSDP PER ALEXA DISCOVERY
#define SSDP_MULTICAST_IP    IPAddress(239, 255, 255, 250)
#define SSDP_MULTICAST_PORT  1900
#define SSDP_RESPONSE_PORT   1900
#define SSDP_BUFFER_SIZE     1024

class AlexaUDP {
private:
    WiFiUDP udp;
    bool isRunning;
    unsigned long lastDiscovery;
    String deviceMAC;
    IPAddress localIP;
    
    // Metodi privati per gestione UDP
    void processUDPPacket();
    void sendSSDPResponse(IPAddress remoteIP, uint16_t remotePort);
    String generateSSDPResponse();
    bool isSearchRequest(const String& packet);
    
public:
    AlexaUDP();
    ~AlexaUDP();
    
    // Metodi pubblici
    bool begin();
    void stop();
    void handle();
    bool isActive() const { return isRunning; }
    
    // Utility
    void setLocalIP(IPAddress ip) { localIP = ip; }
    String getMAC() const { return deviceMAC; }
    
    // Debug
    void printStatus();
};

#endif