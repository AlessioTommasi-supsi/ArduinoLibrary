#include "Pin.h"
#include <cstring>

// Costruttore
Pin::Pin(uint8_t num, PinType t, uint8_t input, const char *n, uint16_t volt)
    : number(num), type(t), voltage(volt), isInput(input)
{
    strncpy(note, n, sizeof(note));
    note[sizeof(note) - 1] = '\0';

    if (number != static_cast<uint8_t>(-1))
    {
        pinMode(number, input ? INPUT : OUTPUT);
    }
}

// Imposta la modalità del pin
void Pin::setMode(uint8_t mode)
{
    isInput = mode == INPUT;
    if (number != static_cast<uint8_t>(-1))
    {
        pinMode(number, mode);
    }
}

// Scrive sul pin

/**
 * true = HIGH
 * false = LOW
 */
bool Pin::write(bool goHigh)
{
    isInput = false;
    voltage = goHigh ? 3300 : 0;
    setMode(OUTPUT);
    digitalWrite(number, goHigh ? HIGH : LOW);
    return true;
}

void Pin::setType(String type)
{
    this->type = StringToPinType(type); // Converte la stringa in PinType e la assegna
}

String Pin::getType()
{
    return pinTypeToString(this->type); // Converte PinType in stringa
}

// Restituisce se il pin è input
bool Pin::getIsInput()
{
    return isInput;
}

// Imposta una nota
void Pin::setNote(const char *newNote)
{
    strncpy(note, newNote, sizeof(note));
    note[sizeof(note) - 1] = '\0';
}

// Conversione tipo pin in stringa
String Pin::pinTypeToString(PinType type)
{
    switch (type)
    {
    case PinType::ANALOGIC:
        return "ANALOGIC";
    case PinType::DIGITAL:
        return "DIGITAL";
    case PinType::VIN:
        return "VIN";
    case PinType::VOUT:
        return "VOUT";
    case PinType::GND:
        return "GND";
    case PinType::EN:
        return "EN";
    case PinType::ADC:
        return "ADC";
    case PinType::DAC:
        return "DAC";
    case PinType::PWM:
        return "PWM";
    case PinType::TOUCH:
        return "TOUCH";
    case PinType::UART:
        return "UART";
    case PinType::SPI:
        return "SPI";
    case PinType::I2C:
        return "I2C";
    default:
        return "UNKNOWN";
    }
}

// Conversione stringa in tipo pin
PinType Pin::StringToPinType(String type)
{
    if (type == "ANALOGIC")
        return PinType::ANALOGIC;
    if (type == "DIGITAL")
        return PinType::DIGITAL;
    if (type == "VIN")
        return PinType::VIN;
    if (type == "VOUT")
        return PinType::VOUT;
    if (type == "GND")
        return PinType::GND;
    if (type == "EN")
        return PinType::EN;
    if (type == "ADC")
        return PinType::ADC;
    if (type == "DAC")
        return PinType::DAC;
    if (type == "PWM")
        return PinType::PWM;
    if (type == "TOUCH")
        return PinType::TOUCH;
    if (type == "UART")
        return PinType::UART;
    if (type == "SPI")
        return PinType::SPI;
    if (type == "I2C")
        return PinType::I2C;
    return PinType::UNKNOWN;
}

// Legge il valore del pin
uint16_t Pin::read()
{
    uint16_t result = voltage;
    if (isInput)
    {
        result = (type == PinType::ANALOGIC) ? analogRead(number) * (3300.0 / 4095.0)
                                             : digitalRead(number) * 1000;
    }
    voltage = result;
    return result;
}

// Funzione per registrare valori dal pin
void Pin::recordingFunction()
{
    try
    {
        while (true)
        {
            uint16_t value = read();
            Serial.println("Recording value: " + String(value) + " at pin " + String(number));
            valuesVoltage.push_back(value);
            delay(timeToRecord);
        }
    }
    catch (...)
    {
        Serial.println("Errore nella registrazione del pin " + String(number));
        stopRecording();
    }
}

// Inizia la registrazione
void Pin::startRecording(int milliseconds)
{
    timeToRecord = milliseconds;
    if (recordingTask == NULL)
    {
        xTaskCreatePinnedToCore(
            [](void *parameter)
            {
                Pin *pin = static_cast<Pin *>(parameter);
                pin->recordingFunction();
            },
            "recordingTask",
            stackSize,
            this,
            1,
            &recordingTask,
            0);
    }
    else
    {
        /** Do nothing registration alredy in progress! */
        //stopRecording();
        //startRecording(milliseconds);
    }
}

// Ferma la registrazione
void Pin::stopRecording()
{
    if (recordingTask != NULL)
    {
        vTaskDelete(recordingTask);
        recordingTask = NULL;
        Serial.println("Recording stopped!");
    }
}

// Ottiene i valori registrati
std::vector<float> Pin::getValuesVoltage()
{
    std::vector<float> copyValues(valuesVoltage.begin(), valuesVoltage.end());
    return copyValues;
}

// Dimensione stack usato
size_t Pin::getUsedStackInWords()
{
    size_t size = valuesVoltage.size() * sizeof(valuesVoltage[0]) / 4;
    return size;
}

// Dimensione stack totale
size_t Pin::getStackSizeInWords()
{
    return stackSize;
}

// Ritorna descrizione del pin
String Pin::toString() const
{
    String result = "Pin number: " + String(number) +
                    ", Type: " + pinTypeToString(type) +
                    ", Voltage: " + String(voltage / 1000.0, 3) + " V" +
                    ", Input: " + (isInput ? "Yes" : "No") +
                    ", Note: " + String(note);
    return result;
}

// Add methods for editing and deleting values without locks
void Pin::editValue(int index, float value)
{
    if (index >= 0 && index < valuesVoltage.size())
    {
        valuesVoltage[index] = value;
    }
}

void Pin::deleteValue(int index)
{
    if (index >= 0 && index < valuesVoltage.size())
    {
        valuesVoltage.erase(valuesVoltage.begin() + index);
    }
}
