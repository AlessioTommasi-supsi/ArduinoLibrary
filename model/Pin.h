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
    bool isInput;     
    char note[200];       // Usa array di char per gestire le note

    size_t stackSize = 10000; //e la dimensione del thread recordingTask in Parole ovvero 1 parola = 4 byte
    size_t timeToRecord = 1000; //tempo di registrazione in millisecondi
    std::vector<uint16_t> valuesVoltage;
    
    TaskHandle_t recordingTask = NULL;

    Pin(uint8_t num, PinType t, uint8_t input, const char* n, uint16_t volt = 0);

    void setMode(uint8_t mode);
    bool write(bool goHigh); //false == LOW, true == HIGH 
    void setNote(const char* newNote);  // Metodo per cambiare la nota
    String toString() const;
    uint16_t read();
    PinType StringToPinType(String type);
    void setType(String type);
    void startRecording(int milliseconds);
    void stopRecording();
    String getType();
    bool getIsInput();
    void recordingFunction();

    std::vector<float> getValuesVoltage();

    size_t getUsedStackInWords();
    size_t getStackSizeInWords();

    static String pinTypeToString(PinType type);

};

#endif // PIN_H
