#include "Esp32_38pin.h"
#include <Arduino.h>
#include <sstream>


Esp32_38pin::Esp32_38pin() {
    Serial.println("Costruttore Esp32_38pin");
    initializePins();
    Serial.println("Inizializzazione dei pin effettuata!");
}

std::vector<Pin> Esp32_38pin::getPins() {
    return pins;
}


std::vector<int> Esp32_38pin::getPinNumbers() {
    std::vector<int> pinNumbers;
    for (const auto& pin : pins) {
        pinNumbers.push_back(pin.number);
    }
    return pinNumbers;
}
/*
void Esp32_38pin::initializePins()
{
    try
    {
        pins.clear();
        
        Pin &defaultPin = getPin(0);
        sprintf(defaultPin.note, "default pin");
        addPin(defaultPin);


        //METODO FUNZIONANTE!
        /*
        Pin defaultPin(0, PinType::UNKNOWN, false, "default pin");
        addPin(defaultPin);
        Serial.println("Pin default aggiunto"); 
        

        addPin(Pin(-1, PinType::VOUT, false, "3.3V"));
        Serial.println("Pin 3.3V aggiunto");

        addPin(Pin(-2, PinType::GND, false, "0V"));
        addPin(Pin(-3, PinType::EN, false, "Pin Enable"));

        // Pin GPIO23 - VSPI MOSI
        addPin(Pin(23, PinType::SPI, true, "VSPI MOSI or input digital"));

        // Pin GPIO36 - ADC1 (SENSOR_VP)
        addPin(Pin(36, PinType::ADC, true, "ADC0 - SENSOR_VP"));
        
        // Pin GPIO22 - I2C SCL
        addPin(Pin(22, PinType::I2C, true, "I2C SCL"));
        /*
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
        addPin(Pin(9 , PinType::DIGITAL, true, "SD D2"));

        // Pin GPIO15 - ADC2, HSPI_CS0
        //addPin(Pin(15, PinType::ADC, true, "ADC13, HSPI_CS0  HS2 CMD ovvero per interfacciamento con memoria flash"));

        // Pin GPIO10 - SD3 SPIWP HS1 DATA 3
        addPin(Pin(10, PinType::DIGITAL, true, "SD D3"));
        
        //Pin GPIO8 - SD2 SPIWP HS1 DATA 1
        //addPin(Pin(8, PinType::DIGITAL, true, "Input digital SD2 SPIWP HS1 DATA 1 ovvero per interfacchiamento scheda sd"));

        // Pin GPIO11 SPICS0 HS1 CMD
        //addPin(Pin(11, PinType::DIGITAL, true, "SPICS0 CMD"));

        // Pin GPIO 7 SD0 SPIQ HS1 DATA 0
        //addPin(Pin(7, PinType::DIGITAL, true, "Input digital SD0 SPIQ HS1 DATA 0 ovvero per interfacchiamento scheda sd"));

        //addPin(Pin(-4, PinType::VIN, false, "tensione di alimentazione del dispositivo (5V)"));

        // Pin GPIO6 SD0 SPID HS1 CLK
        //addPin(Pin(6, PinType::DIGITAL, true, "segnale di clock temporizzato"));
        
    }
    catch(const std::exception& e)
    {
        Serial.println("Errore durante l'inizializzazione dei pin: " + String(e.what()));
    }
    
    

    printPinsOnSerial();
}
*/

void Esp32_38pin::initializePins()
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
        sprintf(adc0Pin.note, "ADC0 - SENSOR_VP");
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
        sprintf(adc3Pin.note, "ADC3 - SENSOR_VN");
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
        sprintf(adc6Pin.note, "ADC6 - Input only");
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

        Pin &uartRxd1Pin = getPin(17);
        uartRxd1Pin.number = 17;
        uartRxd1Pin.type = PinType::UART;
        uartRxd1Pin.isInput = true;
        sprintf(uartRxd1Pin.note, "UART RXD1");
        addPin(uartRxd1Pin);

        Pin &adc16Pin = getPin(14);
        adc16Pin.number = 14;
        adc16Pin.type = PinType::ADC;
        adc16Pin.isInput = false;
        sprintf(adc16Pin.note, "ADC2 is in use by Wi-Fi. Please see https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/adc.html#adc-limitations for more info");
        addPin(adc16Pin);

        Pin &uartTxd2Pin = getPin(16);
        uartTxd2Pin.number = 16;
        uartTxd2Pin.type = PinType::UART;
        uartTxd2Pin.isInput = true;
        sprintf(uartTxd2Pin.note, "UART TXD2");
        addPin(uartTxd2Pin);

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

        // Add pins using the new structure
        Pin &sdD2Pin = getPin(9);
        sdD2Pin.number = 9;
        sdD2Pin.type = PinType::DIGITAL;
        sdD2Pin.isInput = true;
        sprintf(sdD2Pin.note, "SD D2");
        addPin(sdD2Pin);

        Pin &sdD3Pin = getPin(10);
        sdD3Pin.number = 10;
        sdD3Pin.type = PinType::DIGITAL;
        sdD3Pin.isInput = true;
        sprintf(sdD3Pin.note, "SD D3");
        addPin(sdD3Pin);

        Pin &sd2SpiwpPin = getPin(8);
        sd2SpiwpPin.number = 8;
        sd2SpiwpPin.type = PinType::DIGITAL;
        sd2SpiwpPin.isInput = true;
        sprintf(sd2SpiwpPin.note, "SD2 SPIWP HS1 DATA 1");
        addPin(sd2SpiwpPin);

        Pin &spics0CmdPin = getPin(11);
        spics0CmdPin.number = 11;
        spics0CmdPin.type = PinType::DIGITAL;
        spics0CmdPin.isInput = true;
        sprintf(spics0CmdPin.note, "SPICS0 CMD");
        addPin(spics0CmdPin);

        Pin &sd0SpiqPin = getPin(7);
        sd0SpiqPin.number = 7;
        sd0SpiqPin.type = PinType::DIGITAL;
        sd0SpiqPin.isInput = true;
        sprintf(sd0SpiqPin.note, "SD0 SPIQ HS1 DATA 0");
        addPin(sd0SpiqPin);

        Pin &sd0SpidPin = getPin(6);
        sd0SpidPin.number = 6;
        sd0SpidPin.type = PinType::DIGITAL;
        sd0SpidPin.isInput = true;
        sprintf(sd0SpidPin.note, "segnale di clock temporizzato");
        addPin(sd0SpidPin);
    }
    catch (...)
    {
        Serial.println("Errore durante l'inizializzazione dei pin: ");
    }

    printPinsOnSerial();
}

void Esp32_38pin::addPin(const Pin& pin) {
    try
    {
        pins.push_back(pin);
    }
    catch(...)
    {
        Serial.println("Errore durante l'aggiunta del pin: " + pin.toString());
    }
    
}


void Esp32_38pin::printPinsOnSerial() {
    Serial.println("Pinout ESP32 38 pin:");

    try
    {
        for (const auto& pin : pins) {
            Serial.println(pin.toString());
        }
    }
    catch(const std::exception& e)
    {
        Serial.println("Errore durante la stampa dei pin: " + String(e.what()));
    }
    
}



void Esp32_38pin::readPins() {
    for (auto& pin : pins) {
        uint16_t value = pin.read();

        // Stampa il valore letto per il debug
        //Serial.println("readed value: "); 
        //Serial.println(pin.toString());
        
    }
}


Pin& Esp32_38pin::getPin(int GPIOPin) {
    for (auto& pin : pins) {
        if (pin.number == GPIOPin) {
            return pin;
        }
    }
    static Pin defaultPin(GPIOPin, PinType::UNKNOWN, true/*di default i pin sono in lettura, cosi non ho problemi se ci metto tensione all inizio*/, "Pin not found");
    Serial.println("Pin " + String(GPIOPin) + " not found");
    return defaultPin;
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
