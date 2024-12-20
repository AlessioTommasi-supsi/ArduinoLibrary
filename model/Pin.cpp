#include "Pin.h"
#include <sstream>

Pin::Pin(int num, PinType t, float volt)
    : number(num), type(t), voltage(volt) {}

std::string Pin::toString() const {
    std::ostringstream oss;
    oss << "Pin number: " << number << ", Type: " << static_cast<int>(type) << ", Voltage: " << voltage;
    return oss.str();
}
