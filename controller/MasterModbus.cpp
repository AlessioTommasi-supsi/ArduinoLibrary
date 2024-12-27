#include "MasterModbus.h"
#include <HardwareSerial.h>  // Includi la libreria per Serial


MasterModbus::MasterModbus(): modbus(Serial1, dePin)
{
    begin();
}

void MasterModbus::processError()
{
    // Se la libreria non supporta getTimeoutFlag(), rimuovi questa parte.
    Serial.println("Error occurred while communicating with Modbus device.");
    // Puoi includere qui eventuali controlli di timeout o altre eccezioni, se supportati.
}

void MasterModbus::registerPins(PinoutData *pinoutData)
{
    Pin *rxPin = pinoutData->getPin(this->rxPin);
    if (rxPin)//Check if pointer is not null
    {
        rxPin->isInput = false;
        rxPin->setNote("rxPin pin for Modbus communication");
    }

    Pin *txPin = pinoutData->getPin(this->txPin);
    if (txPin)
    {
        txPin->isInput = false;
        txPin->setNote("txPin pin for Modbus communication");
    }

    Pin *dePin = pinoutData->getPin(this->dePin);
    if (dePin)
    {
        dePin->isInput = false;
        dePin->setNote("dePin pin for Modbus communication");
    }
}

void MasterModbus::begin()
{
    modbus.begin(115200, SERIAL_8N1); // Usa solo i parametri richiesti
    modbus.setTimeout(100);
    Serial.println(F("Modbus serial port configuration: 9600-8-N-1"));
    Serial.println(F("Modbus master pin: "));
    Serial.println(dePin);

}

uint32_t MasterModbus::readHoldingRegisters(uint16_t address)
{
    uint16_t holdingRegisters[2];
    uint8_t success = modbus.readHoldingRegisters(1, address - 1, holdingRegisters, 2);
    uint32_t value = 0;
    if (success)
    {
        value = holdingRegisters[0] << 16 | holdingRegisters[1];
        Serial.print(F("Received value: "));
        Serial.println(value);
    }
    else
    {
        processError();
    }
    return value;
}

float MasterModbus::readHoldingFloatRegisters(uint16_t address)
{
    uint16_t holdingRegisters[2];
    uint8_t success = modbus.readHoldingRegisters(1, address - 1, holdingRegisters, 2);
    float value = 0;
    if (success)
    {
        uint32_t combined = ((uint32_t)holdingRegisters[0] << 16) | holdingRegisters[1];
        memcpy(&value, &combined, sizeof(value));

        Serial.print(F("Read value of "));
        Serial.print(value, 6);
        Serial.print(F(" from holding register address "));
        Serial.println(address);
    }
    else
    {
        processError();
    }
    return value;
}

int MasterModbus::readHoldingIntRegisters(uint16_t address)
{
    uint16_t holdingRegister;
    uint8_t success = modbus.readHoldingRegisters(1, address - 1, &holdingRegister, 1);
    if (success)
    {
        Serial.print(F("Read value of "));
        Serial.print(holdingRegister);
        Serial.print(F(" from holding register address "));
        Serial.println(address);
    }
    else
    {
        processError();
    }
    return holdingRegister;
}

void MasterModbus::task()
{
    readHoldingFloatRegisters(122);
    readHoldingIntRegisters(500);
}
