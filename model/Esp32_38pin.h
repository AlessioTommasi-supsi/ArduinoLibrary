#ifndef ESP32_38PIN_H
#define ESP32_38PIN_H

#include "PinoutData.h"

class Esp32_38pin : public PinoutData {
private:
    std::vector<Pin> pins;

public:
    Esp32_38pin();

    void initializePins();
    void readPins();

    void addPin(const Pin& pin) override;
    std::vector<Pin>::iterator begin() override;
    std::vector<Pin>::iterator end() override;
    std::vector<Pin>::const_iterator begin() const override;
    std::vector<Pin>::const_iterator end() const override;

    std::string toString() const override;
};

#endif // ESP32_38PIN_H
