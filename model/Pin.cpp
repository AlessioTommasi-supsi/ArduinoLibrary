#include "Pin.h"
#include <cstring>

Pin::Pin(uint8_t num, PinType t, uint8_t input, const char* n, float volt)
    : number(num), type(t), voltage(volt), isInput(input) {
    strncpy(note, n, sizeof(note));  // Copia la nota nell'array
    note[sizeof(note) - 1] = '\0';   // Assicura la terminazione null
    pinMode(number, input ? INPUT : OUTPUT);
}

void Pin::setMode(uint8_t input) {
    isInput = input;
    pinMode(number, input ? INPUT : OUTPUT);
}

void Pin::setNote(const char* newNote) {
    strncpy(note, newNote, sizeof(note));
    note[sizeof(note) - 1] = '\0';   // Assicura la terminazione null
}

String Pin::toString() const {
    String result = "Pin number: " + String(number) + ", Type: " + String(static_cast<int>(type)) 
                    + ", Voltage: " + String(voltage) + ", Input: " + (isInput ? "Yes" : "No") 
                    + ", Note: " + String(note);
    return result;
}
