#ifndef ALEXA_CONFIG_H
#define ALEXA_CONFIG_H

#include <Arduino.h>
#include <Preferences.h>

// ===== CONFIGURAZIONE ALEXA =====
#define MAX_DEVICES 10
#define FAUXMO_PORT 80
#define FAUXMO_DISABLE_DELAY 500
#define ALEXA_RESTART_DELAY 1000

// ===== CONFIGURAZIONE HTTP ALEXA =====
#define ALEXA_HTTP_TIMEOUT 5000
#define ALEXA_HTTP_RESPONSE_MAX_LENGTH 500

class AlexaConfig {
private:
    static Preferences preferences;
    static bool initialized;
    
public:
    // Inizializzazione sistema
    static void initialize();
    
    // Configurazione di base
    static void printConfig();
};

#endif