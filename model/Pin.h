#ifndef PIN_H
#define PIN_H

#include "PinType.h"
#include <Arduino.h>
#include <vector>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

class Pin
{
public:
    uint8_t number;   // Numero del pin
    PinType type;     // Tipo del pin
    uint16_t voltage; // Voltaggio in mV
    bool isInput;     // Flag per modalità input/output
    char note[200];   // Descrizione del pin

    size_t stackSize = 10000;            // Dimensione dello stack per il task in parole (1 parola = 4 byte)
    size_t timeToRecord = 1000;          // Tempo di registrazione in ms
    std::vector<uint16_t> valuesVoltage; // Valori registrati del voltaggio

    TaskHandle_t recordingTask = NULL; // Task per la registrazione

    // Costruttore
    Pin(uint8_t num, PinType t, uint8_t input, const char *n, uint16_t volt = 0);

    // Metodi per configurazione e utilizzo del pin
    void setMode(uint8_t mode);
    bool write(bool goHigh);
    uint16_t read();
    void setType(String type);
    String getType();
    bool getIsInput();
    void setNote(const char *newNote);

    // Metodi per la registrazione
    void startRecording(int milliseconds);
    void stopRecording();
    void recordingFunction();
    std::vector<float> getValuesVoltage();
    void editValue(int index, float value);
    void deleteValue(int index);

    // Metodi di utilità
    size_t getUsedStackInWords();
    size_t getStackSizeInWords();
    String toString() const;

    // Metodi statici per conversione
    static String pinTypeToString(PinType type);
    static PinType StringToPinType(String type);
};

#endif // PIN_H
