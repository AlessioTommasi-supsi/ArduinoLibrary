#ifndef ADS1115_MODEL_H
#define ADS1115_MODEL_H

#include <Arduino.h>

// Definizione dei pin del multiplexer (collegati all'ESP32 / ESP32-S3)
#if defined(CONFIG_IDF_TARGET_ESP32S3) || defined(ARDUINO_ESP32S3_DEV)
  #ifndef PIN_A
    #define PIN_A 4  // S0 MUX per ESP32-S3
  #endif
  #ifndef PIN_B
    #define PIN_B 5  // S1 MUX per ESP32-S3
  #endif
  #ifndef PIN_C
    #define PIN_C 6  // S2 MUX per ESP32-S3
  #endif
#else
  #ifndef PIN_A
    #define PIN_A 12 // S0 MUX per ESP32 Standard
  #endif
  #ifndef PIN_B
    #define PIN_B 13 // S1 MUX per ESP32 Standard
  #endif
  #ifndef PIN_C
    #define PIN_C 14 // S2 MUX per ESP32 Standard
  #endif
#endif

class ADS1115_model {
public:
    // Imposta il canale del multiplexer (mappa completa da 0 a 7)
    static void setChannel(int channel) {
        switch (channel)
        {
            case 0:
                digitalWrite(PIN_A, LOW);
                digitalWrite(PIN_B, LOW);
                digitalWrite(PIN_C, LOW);
                break;
            case 1:
                digitalWrite(PIN_A, HIGH);
                digitalWrite(PIN_B, LOW);
                digitalWrite(PIN_C, LOW);
                break;
            case 2:
                digitalWrite(PIN_A, LOW);
                digitalWrite(PIN_B, HIGH);
                digitalWrite(PIN_C, LOW);
                break;
            case 3:
                digitalWrite(PIN_A, HIGH);
                digitalWrite(PIN_B, HIGH);
                digitalWrite(PIN_C, LOW);
                break;
            case 4:
                digitalWrite(PIN_A, LOW);
                digitalWrite(PIN_B, LOW);
                digitalWrite(PIN_C, HIGH);
                break;
            case 5:
                digitalWrite(PIN_A, HIGH);
                digitalWrite(PIN_B, LOW);
                digitalWrite(PIN_C, HIGH);
                break;
            case 6:
                digitalWrite(PIN_A, LOW);
                digitalWrite(PIN_B, HIGH);
                digitalWrite(PIN_C, HIGH);
                break;
            case 7:
                digitalWrite(PIN_A, HIGH);
                digitalWrite(PIN_B, HIGH);
                digitalWrite(PIN_C, HIGH);
                break;
            default:
                // Se il canale non è valido, non eseguire nulla
                break;
        }
    }
};

#endif // ADS1115_MODEL_H
