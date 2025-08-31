#ifndef ADS1115_CONTROLLER_H
#define ADS1115_CONTROLLER_H

#include <Arduino.h>
#include <vector>
#include <Adafruit_ADS1X15.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include "ADS1115_model.h"
#include "SystemState.h"
#include "TermocoppiaK.h"  // Include per la classe TermocoppiaK
#include "PT100.h"        // Include per la classe PT100
#include "PT1000.h"       // Include per la classe PT1000


class ADS1115_controller {
public:
    // Restituisce l'istanza singleton come puntatore
    static ADS1115_controller* getInstance();

    // Avvia la registrazione dei dati: signalType viene mappato in un canale (0..7).
    // interval è l'intervallo in ms.
    void startRecording(const String &signalType, int interval);
    // replica il valore letto da ads sul pin outputPinNumber
    void startMonitorTask(int outputPinNumber);
    // replica HIGH se il valore letto supera alertValue, LOW altrimenti
    void startAlertMonitorTask(int outputPinNumber, float alertValue);
    void stopMonitorTask();

    // Imposta il canale del multiplexer (0..7) in base al signalType
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

    void deleteRecordedValue(int index);
    
    // Mappatura dal signalType (stringa) al canale multiplexer (0..7)
    static int signalTypeToChannel(const String &signalType);

    float signalCorrectionValue(int channel, float volts = 0.0);
    
    // Disabilita copia e assegnazione
    ADS1115_controller(const ADS1115_controller&) = delete;
    ADS1115_controller& operator=(const ADS1115_controller&) = delete;

    void printAllReadingsFromADS1115();
    void printReadFromADS1115(int channel);

    float read();


    Adafruit_ADS1115 ads;
    ADS1115_model *adsModel;

    int recordingInterval;
    float alertValue;

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

    // Istanze delle classi per linearizzazione
    TermocoppiaK* termocoppiaK;
    PT100* pt100;
    PT1000* pt1000;
    
    static void recordingTaskFunction(void *parameter);
    static void monitorTaskFunction(void *parameter);
    static void monitorAlertTaskFunction(void *parameter);
};

#endif // ADS1115_CONTROLLER_H
