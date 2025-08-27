#include "AlexaUDP.h"

AlexaUDP::AlexaUDP() : isRunning(false), lastDiscovery(0) {
    deviceMAC = WiFi.macAddress();
    deviceMAC.replace(":", "");
    deviceMAC.toLowerCase();
    localIP = WiFi.localIP();
}

AlexaUDP::~AlexaUDP() {
    stop();
}

bool AlexaUDP::begin() {
    if (isRunning) return true;
    
    Serial.println("🌐 Avvio AlexaUDP Discovery Server...");
    
    // Aggiorna IP locale
    localIP = WiFi.localIP();
    if (localIP == IPAddress(0, 0, 0, 0)) {
        Serial.println("❌ WiFi non connesso - impossibile avviare UDP");
        return false;
    }
    
    // Avvia UDP multicast
    if (udp.beginMulticast(SSDP_MULTICAST_IP, SSDP_MULTICAST_PORT)) {
        isRunning = true;
        Serial.printf("✅ UDP Discovery attivo su %s:%d\n", 
                     SSDP_MULTICAST_IP.toString().c_str(), 
                     SSDP_MULTICAST_PORT);
        Serial.printf("📍 IP locale: %s\n", localIP.toString().c_str());
        Serial.printf("🔧 MAC: %s\n", deviceMAC.c_str());
        return true;
    } else {
        Serial.println("❌ Errore avvio UDP multicast");
        return false;
    }
}

void AlexaUDP::stop() {
    if (isRunning) {
        udp.stop();
        isRunning = false;
        Serial.println("🛑 AlexaUDP Discovery fermato");
    }
}

void AlexaUDP::handle() {
    if (!isRunning) return;
    
    // Verifica se ci sono pacchetti UDP in arrivo
    int packetSize = udp.parsePacket();
    if (packetSize > 0) {
        processUDPPacket();
    }
}

void AlexaUDP::processUDPPacket() {
    char packetBuffer[SSDP_BUFFER_SIZE];
    int len = udp.read(packetBuffer, SSDP_BUFFER_SIZE - 1);
    
    if (len <= 0) return;
    
    packetBuffer[len] = '\0';
    String packet = String(packetBuffer);
    
    // Verifica se è una richiesta M-SEARCH da Alexa
    if (isSearchRequest(packet)) {
        IPAddress remoteIP = udp.remoteIP();
        uint16_t remotePort = udp.remotePort();
        
        Serial.printf("🔍 Discovery request da %s:%d\n", 
                     remoteIP.toString().c_str(), remotePort);
        
        // Invia risposta SSDP
        sendSSDPResponse(remoteIP, remotePort);
        lastDiscovery = millis();
    }
}

bool AlexaUDP::isSearchRequest(const String& packet) {
    return packet.indexOf("M-SEARCH") >= 0 && 
           packet.indexOf("upnp:rootdevice") >= 0;
}

void AlexaUDP::sendSSDPResponse(IPAddress remoteIP, uint16_t remotePort) {
    String response = generateSSDPResponse();
    
    // Crea nuovo UDP per risposta
    WiFiUDP responseUdp;
    if (responseUdp.beginPacket(remoteIP, remotePort)) {
        responseUdp.print(response);
        responseUdp.endPacket();
        responseUdp.stop();
        
        Serial.printf("📤 Risposta SSDP inviata a %s:%d\n", 
                     remoteIP.toString().c_str(), remotePort);
    } else {
        Serial.println("❌ Errore invio risposta SSDP");
    }
}

String AlexaUDP::generateSSDPResponse() {
    String response = "HTTP/1.1 200 OK\r\n";
    response += "CACHE-CONTROL: max-age=86400\r\n";
    response += "DATE: " + String("Mon, 22 Jun 1998 09:55:21 GMT") + "\r\n";
    response += "EXT:\r\n";
    response += "LOCATION: http://" + localIP.toString() + ":80/description.xml\r\n";
    response += "OPT: \"http://schemas.upnp.org/upnp/1/0/\"; ns=01\r\n";
    response += "01-NLS: 1\r\n";
    response += "SERVER: Linux/3.14.0 UPnP/1.0 IpBridge/1.26.0\r\n";
    response += "ST: upnp:rootdevice\r\n";
    response += "USN: uuid:2f402f80-da50-11e1-9b23-" + deviceMAC + "::upnp:rootdevice\r\n";
    response += "\r\n";
    
    return response;
}

void AlexaUDP::printStatus() {
    Serial.println("\n📊 Status AlexaUDP:");
    Serial.println("==================");
    Serial.printf("🌐 Stato: %s\n", isRunning ? "✅ Attivo" : "❌ Fermato");
    if (isRunning) {
        Serial.printf("📍 IP: %s\n", localIP.toString().c_str());
        Serial.printf("🔧 MAC: %s\n", deviceMAC.c_str());
        Serial.printf("📡 Porta: %d\n", SSDP_MULTICAST_PORT);
        Serial.printf("🕐 Ultimo discovery: %lu ms fa\n", 
                     lastDiscovery > 0 ? millis() - lastDiscovery : 0);
    }
}