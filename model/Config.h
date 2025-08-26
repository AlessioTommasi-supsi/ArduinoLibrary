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
const int WIFI_MODE_DELAY = 100;                    // Delay dopo cambio modalità WiFi
const int WIFI_RECONNECT_DELAY = 500;               // Delay tra tentativi riconnessione automatica

// ===== CONFIGURAZIONE ACCESS POINT =====
extern const char* DEFAULT_AP_SSID;
extern const char* DEFAULT_AP_PASSWORD;

// ===== CONFIGURAZIONE SISTEMA =====
const int SYSTEM_CHECK_INTERVAL = 1000;             // Controllo sistema ogni 1 secondo
const int CONNECTION_RETRY_DELAY = 1000;            // Delay tra tentativi connessione
const int SMOOTH_CONNECT_ATTEMPTS = 10;             // Tentativi per smoothConnect
const int SMOOTH_CONNECT_DELAY = 200;               // Delay per smoothConnect

// ===== CONFIGURAZIONE SERIALE =====
const int SERIAL_BAUD_RATE = 9600;                  // Velocità seriale
const int DEBUG_PRINT_INTERVAL = 30000;             // Intervallo debug WiFi

// ===== CONFIGURAZIONE WEB SERVER =====
const int WEBSERVER_PORT = 80;                      // Porta del web server
const int HTTP_TIMEOUT = 5000;                      // Timeout richieste HTTP

// ===== CONFIGURAZIONE PIN ESP32 =====
const int DEFAULT_SD_PIN = 5;                       // Pin di default per SD card
const int DEFAULT_PIN_DELAY = 1000;                 // Delay di default per impulsi pin (ms)
const int DEFAULT_RECORDING_INTERVAL = 1000;        // Intervallo di default per registrazione pin (ms)

// ===== CONFIGURAZIONE AGGIORNAMENTI UI =====
const int UI_AUTO_UPDATE_DISABLED = -1;             // Valore per disabilitare aggiornamento automatico UI
const int UI_REFRESH_INTERVAL_FAST = 1000;          // Aggiornamento veloce UI (1 sec)
const int UI_REFRESH_INTERVAL_MEDIUM = 3000;        // Aggiornamento medio UI (3 sec)
const int UI_REFRESH_INTERVAL_SLOW = 5000;          // Aggiornamento lento UI (5 sec)

// ===== CONFIGURAZIONE GRAFICI MONITOR =====
const int MONITOR_HEAP_UPDATE_INTERVAL = 5000;      // Aggiornamento grafico HEAP (5 sec)
const int MONITOR_STACK_UPDATE_INTERVAL = 5000;     // Aggiornamento grafico STACK (5 sec)
const bool MONITOR_CHART_ANIMATIONS = false;        // Abilita/disabilita animazioni grafici monitor

// ===== CONFIGURAZIONE MONITOR WIFI =====
const int MONITOR_WIFI_UPDATE_INTERVAL = 10000;     // Aggiornamento info WiFi (10 sec)
const int WIFI_SCAN_TIMEOUT = 5000;                 // Timeout per scansione WiFi (5 sec)
const int NETWORK_SCAN_TIMEOUT = 3000;              // Timeout per scansione dispositivi rete (3 sec)
const int MAX_CONNECTED_DEVICES = 20;               // Massimo numero dispositivi connessi da tracciare

// ===== CONFIGURAZIONE CSS =====
const int CSS_NAVBAR_Z_INDEX = 2000;                // Z-index per navbar

// ===== CONFIGURAZIONE BOTTONI PERSONALIZZATI =====
const int MAX_CUSTOM_BUTTONS = 200;                  // Massimo numero di bottoni personalizzati
const int MAX_BUTTON_LABEL_LENGTH = 50;             // Lunghezza massima etichetta bottone
const int MAX_BUTTON_EMOJI_LENGTH = 10;             // Lunghezza massima emoji bottone
const int MAX_BUTTON_URL_LENGTH = 200;              // Lunghezza massima URL bottone
extern const char* CUSTOM_BUTTONS_PREF_NAMESPACE;   // Namespace per salvare bottoni

#endif