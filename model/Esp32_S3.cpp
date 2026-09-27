#include "Esp32_S3.h"
#include <Arduino.h>
#include <sstream>

Esp32_S3::Esp32_S3()
{
    Serial.println("Costruttore Esp32_S3");
    initializePins();
    Serial.println("Inizializzazione dei pin ESP32-S3 effettuata!");
}

std::vector<Pin> Esp32_S3::getPins()
{
    return pins;
}

std::vector<int> Esp32_S3::getPinNumbers()
{
    std::vector<int> pinNumbers;
    for (const auto &pin : pins)
    {
        pinNumbers.push_back(pin.number);
    }
    return pinNumbers;
}

void Esp32_S3::initializePins()
{
    try
    {
        pins.clear();

        // ADC1 Pins (GPIO 1 to 10 - Safe to read during Wi-Fi)
        for (uint8_t i = 1; i <= 10; ++i)
        {
            Pin p(i, PinType::ADC, true, "ESP32-S3 ADC1 / Touch / GPIO");
            addPin(p);
        }

        // ADC2 Pins (GPIO 11 to 18 - ADC2 shared with Wi-Fi)
        for (uint8_t i = 11; i <= 14; ++i)
        {
            Pin p(i, PinType::ADC, true, "ESP32-S3 ADC2 / GPIO");
            addPin(p);
        }

        // USB Native Pins
        Pin usbMinus(19, PinType::DIGITAL, true, "ESP32-S3 USB D-");
        addPin(usbMinus);

        Pin usbPlus(20, PinType::DIGITAL, true, "ESP32-S3 USB D+");
        addPin(usbPlus);

        // I2C Pins (Default S3 SDA/SCL)
        Pin i2cSda(8, PinType::I2C, true, "ESP32-S3 I2C SDA / General GPIO");
        addPin(i2cSda);

        Pin i2cScl(9, PinType::I2C, true, "ESP32-S3 I2C SCL / General GPIO");
        addPin(i2cScl);

        // General GPIOs for S3 (GPIO 21, 38 to 48)
        Pin g21(21, PinType::DIGITAL, true, "ESP32-S3 GPIO 21");
        addPin(g21);

        for (uint8_t i = 38; i <= 42; ++i)
        {
            Pin p(i, PinType::DIGITAL, true, "ESP32-S3 General GPIO");
            addPin(p);
        }

        // UART0 Pins
        Pin txd0(43, PinType::UART, true, "ESP32-S3 UART0 TXD");
        addPin(txd0);

        Pin rxd0(44, PinType::UART, true, "ESP32-S3 UART0 RXD");
        addPin(rxd0);

        // Strapping / General GPIOs
        Pin g45(45, PinType::DIGITAL, true, "ESP32-S3 GPIO 45");
        addPin(g45);

        Pin g46(46, PinType::DIGITAL, true, "ESP32-S3 GPIO 46 (Input/Boot)");
        addPin(g46);

        Pin g47(47, PinType::DIGITAL, true, "ESP32-S3 GPIO 47");
        addPin(g47);

        Pin g48(48, PinType::DIGITAL, true, "ESP32-S3 GPIO 48 (RGB LED / General)");
        addPin(g48);
    }
    catch (...)
    {
        Serial.println("Errore durante l'inizializzazione dei pin ESP32-S3");
    }

    printPinsOnSerial();
}

void Esp32_S3::addPin(const Pin &pin)
{
    try
    {
        pins.push_back(pin);
    }
    catch (...)
    {
        Serial.println("Errore durante l'aggiunta del pin ESP32-S3: " + pin.toString());
    }
}

void Esp32_S3::printPinsOnSerial()
{
    Serial.println("Pinout ESP32-S3:");

    try
    {
        for (const auto &pin : pins)
        {
            Serial.println(pin.toString());
        }
    }
    catch (const std::exception &e)
    {
        Serial.println("Errore durante la stampa dei pin ESP32-S3: " + String(e.what()));
    }
}

void Esp32_S3::readPins()
{
    for (auto &pin : pins)
    {
        pin.read();
    }
}

Pin &Esp32_S3::getPin(int GPIOPin)
{
    for (auto &pin : pins)
    {
        if (pin.number == GPIOPin)
        {
            return pin;
        }
    }
    static Pin defaultPin(GPIOPin, PinType::UNKNOWN, true, "Pin not found");
    Serial.println("Pin ESP32-S3 " + String(GPIOPin) + " non trovato");
    return defaultPin;
}

std::vector<Pin>::iterator Esp32_S3::begin()
{
    return pins.begin();
}

std::vector<Pin>::iterator Esp32_S3::end()
{
    return pins.end();
}

std::vector<Pin>::const_iterator Esp32_S3::begin() const
{
    return pins.begin();
}

std::vector<Pin>::const_iterator Esp32_S3::end() const
{
    return pins.end();
}

std::string Esp32_S3::toString() const
{
    std::ostringstream oss;
    for (const auto &pin : pins)
    {
        oss << pin.toString() << "\n";
    }
    return oss.str();
}
