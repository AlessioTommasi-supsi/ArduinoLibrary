#ifndef ADS1115_CONTROLLER_H
#define ADS1115_CONTROLLER_H

#include <Arduino.h>
#include <vector>
#include <Adafruit_ADS1X15.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include "ADS1115_model.h"
#include "SystemState.h"


class ADS1115_controller {
public:
    // Restituisce l'istanza singleton come puntatore
    static ADS1115_controller* getInstance();

    // Avvia la registrazione dei dati: signalType viene mappato in un canale (0..7).
    // interval è l'intervallo in ms.
    void startRecording(const String &signalType, int interval);
    void startMonitorTask(int outputPinNumber);
    void stopMonitorTask();

    void setChannel(const String &signalType);
    
    // Ferma la registrazione
    void stopRecording();
    
    // Restituisce lo storico delle letture (in volt)
    const std::vector<float>& getRecordedValues() const;
    
    // Restituisce il canale corrente selezionato
    int getCurrentChannel() const;
    
    // Restituisce il flag di fallimento dell'inizializzazione
    bool isInitializationFailed() const;
    
    // Riprova a inizializzare l'ADS1115 (aggiorna il flag)
    void reinitialize();
    
    // Mappatura dal signalType (stringa) al canale multiplexer (0..7)
    static int signalTypeToChannel(const String &signalType);

    float signalCorrectionValue(int channel);
    
    // Disabilita copia e assegnazione
    ADS1115_controller(const ADS1115_controller&) = delete;
    ADS1115_controller& operator=(const ADS1115_controller&) = delete;

    void printAllReadingsFromADS1115();
    void printReadFromADS1115(int channel);

    float read();


    Adafruit_ADS1115 ads;
    ADS1115_model *adsModel;

    int recordingInterval;

private:
    ADS1115_controller();
    static ADS1115_controller* instance;  // Puntatore statico all'unica istanza

    
    
    SemaphoreHandle_t mutex;
    
    bool recordingActive;
    unsigned long lastRecordTime;
    std::vector<float> recordedValues;
    TaskHandle_t recordingTask;
    TaskHandle_t monitorTask;

    int outputPinNumber;
    int currentChannel;
    bool initializationFailed;
    
    static void recordingTaskFunction(void *parameter);
    static void monitorTaskFunction(void *parameter);
};

#endif // ADS1115_CONTROLLER_H
