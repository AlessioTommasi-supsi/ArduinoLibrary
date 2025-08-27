#include "AlexaTemplate.h"

AlexaTemplate::AlexaTemplate() {
    updateNetworkInfo();
}

AlexaTemplate::~AlexaTemplate() {
    // Nessuna pulizia necessaria
}

void AlexaTemplate::updateNetworkInfo() {
    localIP = WiFi.localIP();
    deviceMAC = WiFi.macAddress();
    deviceMAC.replace(":", "");
    deviceMAC.toLowerCase();
}

String AlexaTemplate::escapeXML(const String& text) {
    String escaped = text;
    escaped.replace("&", "&amp;");
    escaped.replace("<", "&lt;");
    escaped.replace(">", "&gt;");
    escaped.replace("\"", "&quot;");
    escaped.replace("'", "&apos;");
    return escaped;
}

String AlexaTemplate::escapeJSON(const String& text) {
    String escaped = text;
    escaped.replace("\\", "\\\\");
    escaped.replace("\"", "\\\"");
    escaped.replace("\n", "\\n");
    escaped.replace("\r", "\\r");
    escaped.replace("\t", "\\t");
    return escaped;
}

String AlexaTemplate::generateDescriptionXML() {
    updateNetworkInfo();
    
    String xml = "<?xml version=\"1.0\"?>\r\n";
    xml += "<root xmlns=\"urn:schemas-upnp-org:device-1-0\">\r\n";
    xml += "  <specVersion>\r\n";
    xml += "    <major>1</major>\r\n";
    xml += "    <minor>0</minor>\r\n";
    xml += "  </specVersion>\r\n";
    xml += "  <device>\r\n";
    xml += "    <deviceType>urn:schemas-upnp-org:device:Basic:1</deviceType>\r\n";
    xml += "    <friendlyName>Philips hue (" + localIP.toString() + ")</friendlyName>\r\n";
    xml += "    <manufacturer>Royal Philips Electronics</manufacturer>\r\n";
    xml += "    <manufacturerURL>http://www.philips.com</manufacturerURL>\r\n";
    xml += "    <modelDescription>Philips hue Personal Wireless Lighting</modelDescription>\r\n";
    xml += "    <modelName>Philips hue bridge 2012</modelName>\r\n";
    xml += "    <modelNumber>929000226503</modelNumber>\r\n";
    xml += "    <modelURL>http://www.meethue.com</modelURL>\r\n";
    xml += "    <serialNumber>" + deviceMAC + "</serialNumber>\r\n";
    xml += "    <UDN>uuid:2f402f80-da50-11e1-9b23-" + deviceMAC + "</UDN>\r\n";
    xml += "    <serviceList>\r\n";
    xml += "      <service>\r\n";
    xml += "        <serviceType>(null)</serviceType>\r\n";
    xml += "        <serviceId>(null)</serviceId>\r\n";
    xml += "        <controlURL>(null)</controlURL>\r\n";
    xml += "        <eventSubURL>(null)</eventSubURL>\r\n";
    xml += "        <SCPDURL>(null)</SCPDURL>\r\n";
    xml += "      </service>\r\n";
    xml += "    </serviceList>\r\n";
    xml += "    <presentationURL>http://" + localIP.toString() + ":80/</presentationURL>\r\n";
    xml += "  </device>\r\n";
    xml += "</root>\r\n";
    
    return xml;
}

String AlexaTemplate::generateDeviceListJSON(DeviceManager* deviceManager) {
    if (!deviceManager) return "{}";
    
    String json = "{";
    
    for (int i = 0; i < deviceManager->getDeviceCount(); i++) {
        if (i > 0) json += ",";
        
        const Device& device = deviceManager->getDevice(i);
        json += "\"" + String(i + 1) + "\":{";
        json += "\"state\":{";
        json += "\"on\":false,";
        json += "\"bri\":254,";
        json += "\"alert\":\"none\",";
        json += "\"reachable\":true";
        json += "},";
        json += "\"type\":\"Dimmable light\",";
        json += "\"name\":\"" + escapeJSON(device.name) + "\",";
        json += "\"modelid\":\"LWB004\",";
        json += "\"manufacturername\":\"Philips\",";
        json += "\"productname\":\"Hue white lamp\",";
        json += "\"uniqueid\":\"" + escapeJSON(device.uuid) + "\"";
        json += "}";
    }
    
    json += "}";
    return json;
}

String AlexaTemplate::generateSingleDeviceJSON(const Device& device) {
    String json = "{";
    json += "\"state\":{";
    json += "\"on\":false,";
    json += "\"bri\":254,";
    json += "\"alert\":\"none\",";
    json += "\"reachable\":true";
    json += "},";
    json += "\"type\":\"Dimmable light\",";
    json += "\"name\":\"" + escapeJSON(device.name) + "\",";
    json += "\"modelid\":\"LWB004\",";
    json += "\"manufacturername\":\"Philips\",";
    json += "\"productname\":\"Hue white lamp\",";
    json += "\"uniqueid\":\"" + escapeJSON(device.uuid) + "\"";
    json += "}";
    
    return json;
}

String AlexaTemplate::generateSuccessResponse(int deviceId, bool state) {
    String json = "[{\"success\":{\"/lights/" + String(deviceId) + "/state/on\":" + (state ? "true" : "false") + "}}]";
    return json;
}

String AlexaTemplate::generateErrorResponse(int errorType, const String& address, const String& description) {
    String json = "[{\"error\":{";
    json += "\"type\":" + String(errorType) + ",";
    json += "\"address\":\"" + escapeJSON(address) + "\",";
    json += "\"description\":\"" + escapeJSON(description) + "\"";
    json += "}}]";
    
    return json;
}

String AlexaTemplate::generateAuthSuccessJSON() {
    return "[{\"success\":{\"username\":\"2WLEDHardQrI3WHYTHoMcXHgEspsM8ZZRpSKtBQr\"}}]";
}

String AlexaTemplate::getDeviceUUID() const {
    return "2f402f80-da50-11e1-9b23-" + deviceMAC;
}

void AlexaTemplate::printTemplateInfo() {
    Serial.println("\n📄 AlexaTemplate Info:");
    Serial.println("======================");
    Serial.printf("📍 IP: %s\n", localIP.toString().c_str());
    Serial.printf("🔧 MAC: %s\n", deviceMAC.c_str());
    Serial.printf("🆔 UUID: %s\n", getDeviceUUID().c_str());
    Serial.printf("💾 Memoria template: ~%d bytes\n", sizeof(AlexaTemplate));
}