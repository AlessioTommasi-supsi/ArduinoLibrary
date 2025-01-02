#ifndef PIN_H
#define PIN_H

#include "PinType.h"
#include <Arduino.h>
#include <vector>

class Pin {
public:
    uint8_t number;      // Usa uint8_t per ridurre la dimensione
    PinType type;
    uint16_t voltage;
    uint8_t isInput;     // Usa uint8_t per risparmiare spazio
    char note[200];       // Usa array di char per gestire le note

    size_t stackSize = 10000; //e la dimensione del thread recordingTask in Parole ovvero 1 parola = 4 byte
    size_t timeToRecord = 1000; //tempo di registrazione in millisecondi
    std::vector<uint16_t> valuesVoltage;
    
    TaskHandle_t recordingTask = NULL;

    Pin(uint8_t num, PinType t, uint8_t input, const char* n, uint16_t volt = 0);

    void setMode(uint8_t input);
    void setNote(const char* newNote);  // Metodo per cambiare la nota
    String toString() const;
    uint16_t read();

    void startRecording(int milliseconds);
    void stopRecording();

    void recordingFunction();

    std::vector<float> getValuesVoltage();

    size_t getUsedStackInWords();
    size_t getStackSizeInWords();

    


};

#endif // PIN_H
