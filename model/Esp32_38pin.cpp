#include "Esp32_38pin.h"
#include <Arduino.h>
#include <sstream>

Esp32_38pin::Esp32_38pin() {
    initializePins();
}


void Esp32_38pin::initializePins() {
    addPin(Pin(-1, PinType::VOUT, false, "3.3V"));
    addPin(Pin(-2, PinType::GND, false, "0V"));
    addPin(Pin(-3, PinType::EN, false, "Pin Enable"));

    // Pin GPIO23 - VSPI MOSI
    addPin(Pin(23, PinType::SPI, true, "VSPI MOSI or input digital"));

    // Pin GPIO36 - ADC1 (SENSOR_VP)
    addPin(Pin(36, PinType::ADC, true, "ADC0 - SENSOR_VP"));

    // Pin GPIO22 - I2C SCL
    addPin(Pin(22, PinType::I2C, true, "I2C SCL"));

    // Pin GPIO39 - ADC1 (SENSOR_VN)
    addPin(Pin(39, PinType::ADC, true, "ADC3 - SENSOR_VN"));

    // Pin GPIO1 - UART TXD0
    addPin(Pin(1, PinType::UART, true, "UART TXD0 or input digital"));

    // Pin GPIO34 - ADC1 (Input only)
    addPin(Pin(34, PinType::ADC, true, "ADC6 - Input only"));
    
    // Pin GPIO3 - UART RXD0
    addPin(Pin(3, PinType::UART, true, "UART RXD0"));

    // Pin GPIO35 - ADC1 (Input only)
    addPin(Pin(35, PinType::ADC, true, "ADC7 - Input only"));

    // Pin GPIO21 - I2C SDA
    addPin(Pin(21, PinType::I2C, true, "I2C SDA"));

    // Pin GPIO32 - ADC1, TOUCH9
    addPin(Pin(32, PinType::ADC, true, "ADC4, TOUCH9"));

    addPin(Pin(-2, PinType::GND, false, "0V"));
    
    // Pin GPIO33 - ADC1, TOUCH8
    addPin(Pin(33, PinType::ADC, true, "ADC5, TOUCH8"));

    // Pin GPIO19 - VSPI MISO
    addPin(Pin(19, PinType::SPI, true, "VSPI MISO"));

    // Pin GPIO25 - DAC1
    addPin(Pin(25, PinType::ADC, true, "ADC18 or DAC1 "));

    // Pin GPIO18 - VSPI CLK
    addPin(Pin(18, PinType::SPI, true, "VSPI CLK or input digital"));

    // Pin GPIO26 - DAC2
    addPin(Pin(26, PinType::ADC, true, "ADC19 or DAC2 "));

    // Pin GPIO5 - VSPI CS0, PWM
    addPin(Pin(5, PinType::PWM, true, "input digital VSPI CS0, PWM"));

    
    // Pin GPIO27 - ADC2, TOUCH7
    addPin(Pin(27, PinType::ADC, true, "ADC17, TOUCH7"));

    // Pin GPIO17 - UART RXD1
    addPin(Pin(17, PinType::UART, true, "UART RXD1"));

    // Pin GPIO14 - ADC2, HSPI_CLK
    addPin(Pin(14, PinType::ADC, true, "ADC16, HSPI_CLK"));

    // Pin GPIO16 - UART TXD2
    addPin(Pin(16, PinType::UART, true, "UART TXD2"));

    
    // Pin GPIO12 - ADC2, TOUCH6, HSPIQ
    addPin(Pin(12, PinType::ADC, true, "ADC15, TOUCH6, HSPIQ"));

    // Pin GPIO4 - ADC2, TOUCH0
    addPin(Pin(4, PinType::ADC, true, "ADC10, TOUCH0"));


    addPin(Pin(-2, PinType::GND, false, "0V"));
    
    // Pin GPIO0 - ADC2, TOUCH1
    addPin(Pin(0, PinType::ADC, true, "ADC11, TOUCH1"));

    // Pin GPIO13 - ADC2, TOUCH5, HSPI_DATA2
    addPin(Pin(13, PinType::ADC, true, "ADC2, TOUCH5, HSPI_DATA2"));

    // Pin GPIO2 - ADC12 
    addPin(Pin(2, PinType::ADC, true, "ADC12"));

    // Pin GPIO9 SD2 SPIHD  HS1 DATA 2
    addPin(Pin(9 , PinType::DIGITAL, true, "Input digital SD2 SPIHD  HS1 DATA 2 ovvero per interfacchiamento scheda sd"));

    // Pin GPIO15 - ADC2, HSPI_CS0
    addPin(Pin(15, PinType::ADC, true, "ADC13, HSPI_CS0  HS2 CMD ovvero per interfacciamento con memoria flash"));

    // Pin GPIO10 - SD3 SPIWP HS1 DATA 3
    addPin(Pin(10, PinType::DIGITAL, true, "Input digital SD3 SPIWP HS1 DATA 3 ovvero per interfacchiamento scheda sd"));
    
    //Pin GPIO8 - SD2 SPIWP HS1 DATA 1
    addPin(Pin(8, PinType::DIGITAL, true, "Input digital SD2 SPIWP HS1 DATA 1 ovvero per interfacchiamento scheda sd"));

    // Pin GPIO11 SPICS0 HS1 CMD
    addPin(Pin(11, PinType::DIGITAL, true, "Input digital SPICS0 HS1 CMD ovvero per interfacchiamento scheda sd"));

    // Pin GPIO 7 SD0 SPIQ HS1 DATA 0
    addPin(Pin(7, PinType::DIGITAL, true, "Input digital SD0 SPIQ HS1 DATA 0 ovvero per interfacchiamento scheda sd"));

    addPin(Pin(-4, PinType::VIN, false, "tensione di alimentazione del dispositivo (5V)"));

    // Pin GPIO6 SD0 SPID HS1 CLK
    addPin(Pin(6, PinType::DIGITAL, true, "segnale di clock temporizzato"));

}




void Esp32_38pin::readPins() {
    for (auto& pin : pins) {
        if (!pin.isInput) {
            // Gestisci errore: il pin è configurato come output
            Serial.println("Il pin " + String(pin.number) + " è configurato come output.");
            continue;  // Ignora la lettura di questo pin
        }

        
        switch (pin.type) {
            case PinType::ADC:
            case PinType::ANALOGIC:
                pin.voltage = analogRead(pin.number) * (3.3 / 4095.0); // Conversione per ESP32
                break;
            case PinType::TOUCH:
                pin.voltage = touchRead(pin.number); // Valore grezzo da touchRead
                break;
            default:
                pin.voltage = digitalRead(pin.number); // 1 per HIGH, 0 per LOW
                break;
        }
    }
}


void Esp32_38pin::addPin(const Pin& pin) {
    pins.push_back(pin);
}

std::vector<Pin>::iterator Esp32_38pin::begin() {
    return pins.begin();
}

std::vector<Pin>::iterator Esp32_38pin::end() {
    return pins.end();
}

std::vector<Pin>::const_iterator Esp32_38pin::begin() const {
    return pins.begin();
}

std::vector<Pin>::const_iterator Esp32_38pin::end() const {
    return pins.end();
}

std::string Esp32_38pin::toString() const {
    std::ostringstream oss;
    for (const auto& pin : pins) {
        oss << pin.toString() << "\n";
    }
    return oss.str();
}
