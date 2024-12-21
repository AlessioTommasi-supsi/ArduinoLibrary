#include "Pin.h"

Pin::Pin(uint8_t num, PinType t, bool input, char*  n, float volt)
    : number(num), type(t), voltage(volt), isInput(input), note(n) {
    pinMode(number, input ? INPUT : OUTPUT);
}

void Pin::setMode(bool input) {
    isInput = input;
    pinMode(number, input ? INPUT : OUTPUT);
}

void Pin::setNote(const String& newNote) {
    note = newNote;
}

String Pin::toString() const {
    String result = "Pin number: " + String(number) + ", Type: " + String(static_cast<int>(type)) 
                    + ", Voltage: " + String(voltage) + ", Input: " + (isInput ? "Yes" : "No") 
                    + ", Note: " + note;
    return result;
}
