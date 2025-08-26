#ifndef DEVICE_MANAGER_H
#define DEVICE_MANAGER_H

#include <Arduino.h>
#include <Preferences.h>
#include "AlexaConfig.h"

// Struttura per dispositivi - SOLO URL COMPLETI
struct Device {
    String name;
    String url;      // Sempre URL completo
    String uuid;     // UUID persistente
    
    Device() {}
    Device(const String& n, const String& deviceUrl) : name(n), url(deviceUrl) {}
};

class DeviceManager {
private:
    Device devices[MAX_DEVICES];
    int deviceCount;
    Preferences* preferences;
    
public:
    DeviceManager(Preferences* prefs);
    
    // Gestione dispositivi - SOLO URL
    bool addDevice(const String& name, const String& url);
    bool removeDevice(const String& name);
    bool deviceExists(const String& name);
    
    // Accesso dati
    int getDeviceCount() const { return deviceCount; }
    const Device& getDevice(int index) const { return devices[index]; }
    Device* findDevice(const String& name);
    
    // Persistenza
    void loadDevices();
    void saveDevices();
    
    // Utility
    void printDevices();
    bool isFull() const { return deviceCount >= MAX_DEVICES; }
    void clear();
    
    // Esecuzione comandi
    bool executeDeviceAction(const String& deviceName, bool state);
    bool sendHTTPRequest(const String& url);
};

#endif