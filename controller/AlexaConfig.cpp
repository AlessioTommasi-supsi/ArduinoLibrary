#include "AlexaConfig.h"
#include <Preferences.h>

// Inizializzazione variabili statiche
Preferences AlexaConfig::preferences;
bool AlexaConfig::initialized = false;

void AlexaConfig::initialize() {
    if (initialized) return;
    
    preferences.begin("alexa_config", false);
    initialized = true;
    
    Serial.println("🎤 AlexaConfig inizializzato");
    printConfig();
}

void AlexaConfig::printConfig() {
    Serial.println("\n📋 Configurazione Alexa:");
    Serial.printf("   🔌 Porta FauxmoESP: %d\n", FAUXMO_PORT);
    Serial.printf("   📱 Max dispositivi: %d\n", MAX_DEVICES);
    Serial.printf("   ⏱️ HTTP Timeout: %d ms\n", ALEXA_HTTP_TIMEOUT);
}