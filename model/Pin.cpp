#include "Pin.h"
#include <cstring>

Pin::Pin(uint8_t num, PinType t, uint8_t input, const char* n, uint16_t volt)
    : number(num), type(t), voltage(volt), isInput(input) {
    strncpy(note, n, sizeof(note));  // Copia la nota nell'array
    note[sizeof(note) - 1] = '\0';   // Assicura la terminazione null
    if (number != static_cast<uint8_t>(-1)) {
        pinMode(number, input ? INPUT : OUTPUT);
    }
}

void Pin::setMode(uint8_t input) {
    isInput = input;
    if (number != static_cast<uint8_t>(-1)) {
        pinMode(number, input ? INPUT : OUTPUT);
    }
}

void Pin::setNote(const char* newNote) {
    strncpy(note, newNote, sizeof(note));
    note[sizeof(note) - 1] = '\0';   // Assicura la terminazione null
}

String Pin::toString() const {
    String result = "Pin number: " + String(number) + ", Type: " + String(static_cast<int>(type)) 
                    + ", Voltage: " + String(voltage / 1000.0, 3) + " V"  // Converte da mV a V per la stampa
                    + ", Input: " + (isInput ? "Yes" : "No") 
                    + ", Note: " + String(note);
    return result;
}

uint16_t Pin::read()
{
    if (!isInput) /*controlla se isInput != 0 allora entro nel true! */
    {
        // Gestisci errore: il pin è configurato come output
        Serial.println("Errore: Il pin " + String(number) + " è configurato come output.");
        voltage = -1;
        return voltage;
    }

    switch (type)
    {
    case PinType::ADC:
    case PinType::ANALOGIC:
        voltage = static_cast<uint16_t>(analogRead(number) * (3300.0 / 4095.0)); // Conversione in mV
        break;
    case PinType::TOUCH:
         voltage = static_cast<uint16_t>(touchRead(number)); // Valore grezzo da touchRead
        break;
    default:
         voltage = static_cast<uint16_t>(digitalRead(number) * 1000); // HIGH = 1000 mV, LOW = 0 mV
        break;
    }
    return  voltage;
}
