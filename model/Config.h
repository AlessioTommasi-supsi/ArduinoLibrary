#ifndef CONFIG_H
#define CONFIG_H

// ===== CONFIGURAZIONE WIFI =====
const int WIFI_CONNECT_TIMEOUT = 20;                // Timeout connessione WiFi (secondi)
const int WIFI_RETRY_TIMEOUT = 15;                  // Timeout retry WiFi salvato (secondi)
const int WIFI_CHECK_INTERVAL = 30000;              // Controllo connessione WiFi (30 sec)
const int WIFI_AUTO_RECONNECT_INTERVAL = 30000;     // Riconnessione automatica ogni 30 secondi
const int WIFI_SCAN_DELAY = 100;                    // Delay dopo scan WiFi
const int WIFI_RECONNECT_ATTEMPTS = 10;             // Tentativi riconnessione
const int WIFI_AUTO_RECONNECT_ATTEMPTS = 6;         // Tentativi per riconnessione automatica (3 sec)

// ===== CONFIGURAZIONE ACCESS POINT =====
extern const char* DEFAULT_AP_SSID;
extern const char* DEFAULT_AP_PASSWORD;

// ===== CONFIGURAZIONE SISTEMA =====
const int SYSTEM_CHECK_INTERVAL = 5000;             // Controllo sistema ogni 5 secondi
const int CONNECTION_RETRY_DELAY = 1000;            // Delay tra tentativi connessione
const int SMOOTH_CONNECT_ATTEMPTS = 10;             // Tentativi per smoothConnect
const int SMOOTH_CONNECT_DELAY = 200;               // Delay per smoothConnect

// ===== CONFIGURAZIONE SERIALE =====
const int SERIAL_BAUD_RATE = 9600;                  // Velocità seriale
const int DEBUG_PRINT_INTERVAL = 30000;             // Intervallo debug WiFi

#endif