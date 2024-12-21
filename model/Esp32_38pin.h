#ifndef ESP32_38PIN_H
#define ESP32_38PIN_H

#include "PinoutData.h"

class Esp32_38pin : public PinoutData {
private:
    

public:
    std::vector<Pin> pins;
    Esp32_38pin();

    void initializePins();
    void printPinsOnSerial();
    
    
    void readPins() override;
    void addPin(const Pin& pin) override;
    Pin& getPin(int GPIOPin) override;
    std::vector<Pin>::iterator begin() override;
    std::vector<Pin>::iterator end() override;
    std::vector<Pin>::const_iterator begin() const override;
    std::vector<Pin>::const_iterator end() const override;

    std::string toString() const override;
};

#endif // ESP32_38PIN_H
