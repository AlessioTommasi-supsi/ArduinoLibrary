#include "Pin.h"
#include <cstring>

Pin::Pin(uint8_t num, PinType t, uint8_t input, const char* n, uint16_t volt)
    : number(num), type(t), voltage(volt), isInput(input) {
    strncpy(note, n, sizeof(note));  // Copia la nota nell'array
    note[sizeof(note) - 1] = '\0';   // Assicura la terminazione null
    if (number != static_cast<uint8_t>(-1)) {
        pinMode(number, input ? INPUT : OUTPUT);
    }
}

void Pin::setMode(uint8_t mode) {
    isInput = mode == INPUT;
    if (number != static_cast<uint8_t>(-1)) {
        pinMode(number, mode);
    }
}

bool Pin::write(bool goHigh) {
    isInput = false;
    voltage = goHigh ? 3300 : 0;
    setMode(OUTPUT);
    digitalWrite(number, goHigh ? HIGH : LOW);
    return true;
}

bool Pin::getIsInput()
{
    return isInput == true;
}

void Pin::setNote(const char* newNote) {
    strncpy(note, newNote, sizeof(note));
    note[sizeof(note) - 1] = '\0';   // Assicura la terminazione null
}

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
    case PinType::UNKNOWN:
        return "UNKNOWN";
    default:
        return "UNKNOWN";
    }
}

PinType Pin::StringToPinType(String type)
{
    if(type == "ANALOGIC")
    {
        return PinType::ANALOGIC;
    }
    else if(type == "DIGITAL")
    {
        return PinType::DIGITAL;
    }
    else if(type == "VIN")
    {
        return PinType::VIN;
    }
    else if(type == "VOUT")
    {
        return PinType::VOUT;
    }
    else if(type == "GND")
    {
        return PinType::GND;
    }
    else if(type == "EN")
    {
        return PinType::EN;
    }
    else if(type == "ADC")
    {
        return PinType::ADC;
    }
    else if(type == "DAC")
    {
        return PinType::DAC;
    }
    else if(type == "PWM")
    {
        return PinType::PWM;
    }
    else if(type == "TOUCH")
    {
        return PinType::TOUCH;
    }
    else if(type == "UART")
    {
        return PinType::UART;
    }
    else if(type == "SPI")
    {
        return PinType::SPI;
    }
    else if(type == "I2C")
    {
        return PinType::I2C;
    }
    else
    {
        return PinType::UNKNOWN;
    }
}

void Pin::setType(String type)
{
    this->type = StringToPinType(type);
}

String Pin::getType()
{
    return pinTypeToString(type);
}

String Pin::toString() const
{
    String result = "Pin number: " + String(number) + 
        ", Type: " + pinTypeToString(type) 
        +", Voltage: " + String(voltage / 1000.0, 3) + " V" // Converte da mV a V per la stampa
        + ", Input: " + (isInput ? "Yes" : "No") + 
        ", Note: " + String(note);
    return result;
}

uint16_t Pin::read()
{
    if (!isInput) /*controlla se isInput != 0 allora entro nel true! */
    {
        // Gestisci errore: il pin è configurato come output
        //Serial.println("Errore: Il pin " + String(number) + " è configurato come output.");
        //voltage = -1;
        //ritorno semplicemente ultimo voltaggio letto!
        return voltage;
    }

    switch (type)
    {
    case PinType::ADC:
    case PinType::ANALOGIC:
        voltage = static_cast<uint16_t>(analogRead(number) * (3300.0 / 4095.0)); // Conversione in mV
        break;
    case PinType::TOUCH:
         voltage = static_cast<uint16_t>(touchRead(number)); // Valore grezzo da touchRead
        break;
    default:
         voltage = static_cast<uint16_t>(digitalRead(number) * 1000); // HIGH = 1000 mV, LOW = 0 mV
        break;
    }
    //valuesVoltage.push_back(voltage);
    return  voltage;
}


void Pin:: recordingFunction()
{
    
    while (true)
    {
        uint16_t value = read();
        Serial.println("Recording value: " + String(value) + " at pin " + String(number));
        valuesVoltage.push_back(value);
        delay(timeToRecord);
    }
}


void Pin::startRecording(int milliseconds)
{
    timeToRecord = milliseconds;
    if (recordingTask == NULL)
    {
        xTaskCreatePinnedToCore(
            [](void* parameter) {
                Pin* pin = static_cast<Pin*>(parameter);
                pin->recordingFunction();
            },
            "recordingTask",
            stackSize,
            this,
            1,
            &recordingTask,
            0);
    }else
    {
        Serial.println("Recording already active");
        Serial.println("Stoppin recording...");
        stopRecording();
        Serial.println("Starting recording...");
        startRecording(milliseconds);
    }
}

void Pin::stopRecording()
{
    if (recordingTask != NULL)
    {
        vTaskDelete(recordingTask);
        recordingTask = NULL;
        Serial.println("Recording stopped!");
    }
}

std::vector<float> Pin::getValuesVoltage()
{
    //ritorna una copia del vettore
    std::vector<float> copyValues(valuesVoltage.begin(), valuesVoltage.end());
    return copyValues;
}


size_t Pin::getUsedStackInWords()
{
    //ritorna la dimensione dello stack sulla base della dimensione della variabile valuesVoltage
    return valuesVoltage.size()*sizeof(valuesVoltage[0])/4;
}

size_t Pin::getStackSizeInWords()
{
    return stackSize;
}
