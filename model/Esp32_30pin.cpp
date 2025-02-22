#include "Esp32_30pin.h"
#include <Arduino.h>
#include <sstream>

Esp32_30pin::Esp32_30pin()
{
    Serial.println("Costruttore Esp32_30pin");
    initializePins();
    Serial.println("Inizializzazione dei pin effettuata!");
}

std::vector<Pin> Esp32_30pin::getPins()
{
    return pins;
}

std::vector<int> Esp32_30pin::getPinNumbers()
{
    std::vector<int> pinNumbers;
    for (const auto &pin : pins)
    {
        pinNumbers.push_back(pin.number);
    }
    return pinNumbers;
}

void Esp32_30pin::initializePins()
{
    try
    {
        pins.clear();

        Pin &vspiMosiPin = getPin(23);
        vspiMosiPin.number = 23;
        vspiMosiPin.type = PinType::SPI;
        vspiMosiPin.isInput = true;
        sprintf(vspiMosiPin.note, "VSPI MOSI or input digital");
        addPin(vspiMosiPin);

        Pin &adc0Pin = getPin(36);
        adc0Pin.number = 36;
        adc0Pin.type = PinType::ADC;
        adc0Pin.isInput = true;
        sprintf(adc0Pin.note, "ADC0 - SENSOR_VP input only");
        addPin(adc0Pin);

        Pin &i2cSclPin = getPin(22);
        i2cSclPin.number = 22;
        i2cSclPin.type = PinType::I2C;
        i2cSclPin.isInput = true;
        sprintf(i2cSclPin.note, "I2C SCL");
        addPin(i2cSclPin);

        Pin &adc3Pin = getPin(39);
        adc3Pin.number = 39;
        adc3Pin.type = PinType::ADC;
        adc3Pin.isInput = true;
        sprintf(adc3Pin.note, "ADC3 - SENSOR_VN input only");
        addPin(adc3Pin);

        Pin &uartTxd0Pin = getPin(1);
        uartTxd0Pin.number = 1;
        uartTxd0Pin.type = PinType::UART;
        uartTxd0Pin.isInput = true;
        sprintf(uartTxd0Pin.note, "UART TXD0 or input digital");
        addPin(uartTxd0Pin);

        Pin &adc6Pin = getPin(34);
        adc6Pin.number = 34;
        adc6Pin.type = PinType::ADC;
        adc6Pin.isInput = true;
        sprintf(adc6Pin.note, "ADC6 - Input only input only");
        addPin(adc6Pin);

        Pin &uartRxd0Pin = getPin(3);
        uartRxd0Pin.number = 3;
        uartRxd0Pin.type = PinType::UART;
        uartRxd0Pin.isInput = true;
        sprintf(uartRxd0Pin.note, "UART RXD0");
        addPin(uartRxd0Pin);

        Pin &adc7Pin = getPin(35);
        adc7Pin.number = 35;
        adc7Pin.type = PinType::ADC;
        adc7Pin.isInput = true;
        sprintf(adc7Pin.note, "ADC7 - Input only");
        addPin(adc7Pin);

        Pin &i2cSdaPin = getPin(21);
        i2cSdaPin.number = 21;
        i2cSdaPin.type = PinType::I2C;
        i2cSdaPin.isInput = true;
        sprintf(i2cSdaPin.note, "I2C SDA");
        addPin(i2cSdaPin);

        Pin &adc4Pin = getPin(32);
        adc4Pin.number = 32;
        adc4Pin.type = PinType::ADC;
        adc4Pin.isInput = true;
        sprintf(adc4Pin.note, "ADC4, TOUCH9");
        addPin(adc4Pin);

        Pin &adc5Pin = getPin(33);
        adc5Pin.number = 33;
        adc5Pin.type = PinType::ADC;
        adc5Pin.isInput = true;
        sprintf(adc5Pin.note, "ADC5, TOUCH8");
        addPin(adc5Pin);

        Pin &vspiMisoPin = getPin(19);
        vspiMisoPin.number = 19;
        vspiMisoPin.type = PinType::SPI;
        vspiMisoPin.isInput = true;
        sprintf(vspiMisoPin.note, "VSPI MISO");
        addPin(vspiMisoPin);

        Pin &dac1Pin = getPin(25);
        dac1Pin.number = 25;
        dac1Pin.type = PinType::ADC;
        dac1Pin.isInput = false;
        dac1Pin.voltage = 3300;
        sprintf(dac1Pin.note, "ADC2 is in use by Wi-Fi. Please see https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/adc.html#adc-limitations for more info");
        addPin(dac1Pin);

        Pin &vspiClkPin = getPin(18);
        vspiClkPin.number = 18;
        vspiClkPin.type = PinType::SPI;
        vspiClkPin.isInput = true;
        sprintf(vspiClkPin.note, "VSPI CLK or input digital");
        addPin(vspiClkPin);

        Pin &dac2Pin = getPin(26);
        dac2Pin.number = 26;
        dac2Pin.type = PinType::ADC;
        dac2Pin.isInput = false;
        dac2Pin.voltage = 3300;
        sprintf(dac2Pin.note, "ADC2 is in use by Wi-Fi. Please see https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/adc.html#adc-limitations for more info");
        addPin(dac2Pin);

        Pin &pwmPin = getPin(5);
        pwmPin.number = 5;
        pwmPin.type = PinType::PWM;
        pwmPin.isInput = true;
        sprintf(pwmPin.note, "input digital VSPI CS0, PWM");
        addPin(pwmPin);

        Pin &adc17Pin = getPin(27);
        adc17Pin.number = 27;
        adc17Pin.type = PinType::ADC;
        adc17Pin.isInput = false;
        sprintf(adc17Pin.note, "ADC2 is in use by Wi-Fi. Please see https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/adc.html#adc-limitations for more info");
        addPin(adc17Pin);

        Pin &adc16Pin = getPin(14);
        adc16Pin.number = 14;
        adc16Pin.type = PinType::ADC;
        adc16Pin.isInput = false;
        sprintf(adc16Pin.note, "ADC2 is in use by Wi-Fi. Please see https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/adc.html#adc-limitations for more info");
        addPin(adc16Pin);

        Pin &adc15Pin = getPin(12);
        adc15Pin.number = 12;
        adc15Pin.type = PinType::ADC;
        adc15Pin.isInput = false;
        sprintf(adc15Pin.note, "ADC2 is in use by Wi-Fi. Please see https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/adc.html#adc-limitations for more info");
        addPin(adc15Pin);

        Pin &adc10Pin = getPin(4);
        adc10Pin.number = 4;
        adc10Pin.type = PinType::ADC;
        adc10Pin.isInput = true;
        sprintf(adc10Pin.note, "ADC10, TOUCH0");
        addPin(adc10Pin);

        Pin &adc11Pin = getPin(0);
        adc11Pin.number = 0;
        adc11Pin.type = PinType::ADC;
        adc11Pin.isInput = false;
        sprintf(adc11Pin.note, "ADC2 is in use by Wi-Fi. Please see https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/adc.html#adc-limitations for more info");
        addPin(adc11Pin);

        Pin &adc2Pin = getPin(13);
        adc2Pin.number = 13;
        adc2Pin.type = PinType::ADC;
        adc2Pin.isInput = false;
        sprintf(adc2Pin.note, "ADC2 is in use by Wi-Fi. Please see https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/adc.html#adc-limitations for more info");
        addPin(adc2Pin);

        Pin &adc12Pin = getPin(2);
        adc12Pin.number = 2;
        adc12Pin.type = PinType::ADC;
        adc12Pin.isInput = false;
        sprintf(adc12Pin.note, "ADC2 is in use by Wi-Fi. Please see https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/adc.html#adc-limitations for more info");
        addPin(adc12Pin);


    }
    catch (...)
    {
        Serial.println("Errore durante l'inizializzazione dei pin: ");
    }

    printPinsOnSerial();
}

void Esp32_30pin::addPin(const Pin &pin)
{
    try
    {
        pins.push_back(pin);
    }
    catch (...)
    {
        Serial.println("Errore durante l'aggiunta del pin: " + pin.toString());
    }
}

void Esp32_30pin::printPinsOnSerial()
{
    Serial.println("Pinout ESP32 38 pin:");

    try
    {
        for (const auto &pin : pins)
        {
            Serial.println(pin.toString());
        }
    }
    catch (const std::exception &e)
    {
        Serial.println("Errore durante la stampa dei pin: " + String(e.what()));
    }
}

void Esp32_30pin::readPins()
{
    for (auto &pin : pins)
    {
        uint16_t value = pin.read();

        // Stampa il valore letto per il debug
        // Serial.println("readed value: ");
        // Serial.println(pin.toString());
    }
}

Pin &Esp32_30pin::getPin(int GPIOPin)
{
    for (auto &pin : pins)
    {
        if (pin.number == GPIOPin)
        {
            return pin;
        }
    }
    static Pin defaultPin(GPIOPin, PinType::UNKNOWN, true /*di default i pin sono in lettura, cosi non ho problemi se ci metto tensione all inizio*/, "Pin not found");
    Serial.println("Pin " + String(GPIOPin) + " not found");
    return defaultPin;
}

std::vector<Pin>::iterator Esp32_30pin::begin()
{
    return pins.begin();
}

std::vector<Pin>::iterator Esp32_30pin::end()
{
    return pins.end();
}

std::vector<Pin>::const_iterator Esp32_30pin::begin() const
{
    return pins.begin();
}

std::vector<Pin>::const_iterator Esp32_30pin::end() const
{
    return pins.end();
}

std::string Esp32_30pin::toString() const
{
    std::ostringstream oss;
    for (const auto &pin : pins)
    {
        oss << pin.toString() << "\n";
    }
    return oss.str();
}
