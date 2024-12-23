#ifndef PIN_H
#define PIN_H

#include "PinType.h"
#include <Arduino.h>

class Pin {
public:
    uint8_t number;      // Usa uint8_t per ridurre la dimensione
    PinType type;
    uint16_t voltage;
    uint8_t isInput;     // Usa uint8_t per risparmiare spazio
    char note[50];       // Usa array di char per gestire le note

    Pin(uint8_t num, PinType t, uint8_t input, const char* n, uint16_t volt = 0);

    void setMode(uint8_t input);
    void setNote(const char* newNote);  // Metodo per cambiare la nota
    String toString() const;
    uint16_t read();
};

#endif // PIN_H
