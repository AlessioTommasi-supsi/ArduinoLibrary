#ifndef PIN_H
#define PIN_H

#include "PinType.h"
#include <vector>
#include <string>

class Pin {
public:
    int number;
    PinType type;
    float voltage;

    Pin(int num, PinType t, float volt = 0.0f);

    std::string toString() const;
};

#endif // PIN_H
