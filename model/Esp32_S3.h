#ifndef ESP32_S3_H
#define ESP32_S3_H

#include "PinoutData.h"

class Esp32_S3 : public PinoutData
{
public:
    std::vector<Pin> pins;
    Esp32_S3();

    void printPinsOnSerial() override;
    void initializePins() override;
    void readPins() override;
    void addPin(const Pin &pin) override;
    Pin &getPin(int GPIOPin) override;
    std::vector<Pin> getPins() override;
    std::vector<int> getPinNumbers() override;
    std::vector<Pin>::iterator begin() override;
    std::vector<Pin>::iterator end() override;
    std::vector<Pin>::const_iterator begin() const override;
    std::vector<Pin>::const_iterator end() const override;

    std::string toString() const override;
};

#endif // ESP32_S3_H
