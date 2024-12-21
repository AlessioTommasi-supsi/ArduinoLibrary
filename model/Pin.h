#ifndef PIN_H
#define PIN_H

#include "PinType.h"
#include <Arduino.h>

class Pin {
public:
    int number;
    PinType type;
    float voltage;
    bool isInput;      // Attributo per indicare se il pin è usato come input se viene indicato da un compnente devo modificare direttamente attributo e non usare setmode!
    String note;       // Attributo per le note descrittive

    Pin(int num, PinType t, bool input, String n, float volt = 0.0f);

    void setMode(bool input);
    void setNote(const String& newNote);  // Metodo per cambiare la nota
    String toString() const;
};

#endif // PIN_H
