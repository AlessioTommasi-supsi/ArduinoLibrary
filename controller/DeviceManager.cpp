#include "DeviceManager.h"
#include <WiFi.h>
#include <HTTPClient.h>

DeviceManager::DeviceManager(Preferences* prefs) : deviceCount(0), preferences(prefs) {}

bool DeviceManager::addDevice(const String& name, const String& url) {
    if (isFull() || deviceExists(name)) return false;
    
    // Verifica che l'URL sia valido
    if (url.length() == 0 || !url.startsWith("http")) {
        Serial.println("❌ URL non valido - deve iniziare con http");
        return false;
    }
    
    Device newDevice;
    newDevice.name = name;
    newDevice.url = url;
    
    devices[deviceCount++] = newDevice;
    saveDevices();
    
    Serial.printf("✅ Dispositivo '%s' aggiunto con URL: %s\n", name.c_str(), url.c_str());
    return true;
}

bool DeviceManager::removeDevice(const String& name) {
    for (int i = 0; i < deviceCount; i++) {
        if (devices[i].name.equalsIgnoreCase(name)) {
            // Sposta elementi indietro
            for (int j = i; j < deviceCount - 1; j++) {
                devices[j] = devices[j + 1];
            }
            deviceCount--;
            saveDevices();
            return true;
        }
    }
    return false;
}

bool DeviceManager::deviceExists(const String& name) {
    for (int i = 0; i < deviceCount; i++) {
        if (devices[i].name.equalsIgnoreCase(name)) {
            return true;
        }
    }
    return false;
}

Device* DeviceManager::findDevice(const String& name) {
    for (int i = 0; i < deviceCount; i++) {
        if (devices[i].name.equalsIgnoreCase(name)) {
            return &devices[i];
        }
    }
    return nullptr;
}

void DeviceManager::loadDevices() {
    deviceCount = preferences->getInt("device_count", 0);
    
    for (int i = 0; i < deviceCount && i < MAX_DEVICES; i++) {
        String prefix = "dev" + String(i) + "_";
        
        devices[i].name = preferences->getString((prefix + "name").c_str(), "");
        devices[i].url = preferences->getString((prefix + "url").c_str(), "");
        devices[i].uuid = preferences->getString((prefix + "uuid").c_str(), "");
    }
    
    Serial.printf("💾 Caricati %d dispositivi\n", deviceCount);
}

void DeviceManager::saveDevices() {
    preferences->putInt("device_count", deviceCount);
    
    for (int i = 0; i < deviceCount; i++) {
        String prefix = "dev" + String(i) + "_";
        
        preferences->putString((prefix + "name").c_str(), devices[i].name);
        preferences->putString((prefix + "url").c_str(), devices[i].url);
        preferences->putString((prefix + "uuid").c_str(), devices[i].uuid);
    }
    
    Serial.printf("💾 Salvati %d dispositivi\n", deviceCount);
}

void DeviceManager::printDevices() {
    Serial.println("\n📱 Dispositivi Alexa configurati:");
    Serial.println("=====================================");
    
    if (deviceCount == 0) {
        Serial.println("   ❌ Nessun dispositivo configurato");
        Serial.println("   💡 Usa 'a2' per aggiungere dispositivi con URL completo");
        return;
    }
    
    for (int i = 0; i < deviceCount; i++) {
        Serial.printf("%2d. %-20s -> 🌐 %s\n", i + 1, devices[i].name.c_str(), devices[i].url.c_str());
    }
    
    Serial.printf("\nTotale: %d/%d dispositivi\n", deviceCount, MAX_DEVICES);
}

void DeviceManager::clear() {
    deviceCount = 0;
    saveDevices();
}

bool DeviceManager::executeDeviceAction(const String& deviceName, bool state) {
    Device* device = findDevice(deviceName);
    if (!device) {
        Serial.printf("❌ Dispositivo '%s' non trovato\n", deviceName.c_str());
        return false;
    }
    
    Serial.printf("📡 Comando Alexa: %s -> %s\n", deviceName.c_str(), device->url.c_str());
    return sendHTTPRequest(device->url);
}

bool DeviceManager::sendHTTPRequest(const String& url) {
    if (url.length() == 0) {
        Serial.println("❌ URL vuoto!");
        return false;
    }
    
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("❌ WiFi non connesso!");
        return false;
    }
    
    HTTPClient http;
    http.begin(url);
    http.setTimeout(ALEXA_HTTP_TIMEOUT);
    
    int httpCode = http.GET();
    bool success = (httpCode == 200);
    
    if (success) {
        Serial.printf("✅ Richiesta OK: %s\n", url.c_str());
    } else {
        Serial.printf("❌ Errore HTTP %d: %s\n", httpCode, url.c_str());
    }
    
    http.end();
    return success;
}