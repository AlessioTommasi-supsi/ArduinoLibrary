#include "Esp32_38pin.h"
#include <Arduino.h>
#include <sstream>

Esp32_38pin::Esp32_38pin() {
    initializePins();
}

void Esp32_38pin::initializePins() {
    // Inizializza i pin secondo il pinout fornito nella tua immagine

    addPin(Pin(36, PinType::ADC));   // GPIO36 - ADC1
    addPin(Pin(39, PinType::ADC));   // GPIO39 - ADC1
    addPin(Pin(34, PinType::ADC));   // GPIO34 - ADC1
    addPin(Pin(35, PinType::ADC));   // GPIO35 - ADC1
    addPin(Pin(32, PinType::DAC));   // GPIO32 - DAC1
    addPin(Pin(33, PinType::DAC));   // GPIO33 - DAC2
    addPin(Pin(25, PinType::DAC));   // GPIO25 - DAC1
    addPin(Pin(26, PinType::DAC));   // GPIO26 - DAC2
    addPin(Pin(14, PinType::PWM));   // GPIO14 - PWM
    addPin(Pin(27, PinType::PWM));   // GPIO27 - PWM
    addPin(Pin(12, PinType::TOUCH)); // GPIO12 - Touch
    addPin(Pin(13, PinType::TOUCH)); // GPIO13 - Touch
    addPin(Pin(2, PinType::UART));   // GPIO2 - UART0
    addPin(Pin(15, PinType::SPI));   // GPIO15 - SPI
    addPin(Pin(21, PinType::I2C));   // GPIO21 - I2C SDA
    addPin(Pin(22, PinType::I2C));   // GPIO22 - I2C SCL
    // Continua l'inizializzazione per gli altri pin...

    // Aggiungi altri pin e le loro funzionalità secondo il tuo pinout
}

void Esp32_38pin::readPins() {
    for (auto& pin : pins) {
        if (pin.type == PinType::ADC || pin.type == PinType::ANALOGIC) {
            pin.voltage = analogRead(pin.number) * (3.3 / 4095.0); // Conversione per ESP32
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
