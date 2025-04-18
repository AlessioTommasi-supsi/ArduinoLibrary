#ifndef ADS1115_CONTROLLER_H
#define ADS1115_CONTROLLER_H

#include <Arduino.h>
#include <vector>
#include <Adafruit_ADS1X15.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include "ADS1115_model.h"

class ADS1115_controller {
public:
    bool initializationFailed = false; // Flag per indicare se l'inizializzazione ha avuto successo
    // Inizializza l'istanza interna dell'ADS1115 e il modello per il multiplexer
    ADS1115_controller();

    // Avvia la registrazione dei dati 
    // Il parametro signalType (stringa) viene mappato nel canale corretto (0..7)
    // e interval è l'intervallo di tempo (ms) tra una lettura ed una successiva.
    void startRecording(const String &signalType, int interval);
    
    // Ferma la registrazione
    void stopRecording();
    
    // Ritorna lo storico delle letture (in volt)
    const std::vector<float>& getRecordedValues() const;
    
    // Funzione di mapping: dal signalType (stringa) al canale multiplexer (0..7)
    static int signalTypeToChannel(const String &signalType);
    
    // Restituisce il canale corrente selezionato
    int getCurrentChannel() const;
    
    // Ritorna l'handle del task di registrazione (se necessario)
    TaskHandle_t getRecordingTaskHandle() const { return recordingTask; }
    
private:
    // Istanza interna dell'ADS1115
    Adafruit_ADS1X15 ads;
    // Modello per il multiplexer
    ADS1115_model adsModel;
    
    // Thread-safety: mutex per l'accesso alle variabili condivise
    SemaphoreHandle_t mutex;
    
    // Stato della registrazione e relativo intervallo (ms)
    bool recordingActive;
    int recordingInterval;
    unsigned long lastRecordTime;
    
    // Storico delle letture (in volt)
    std::vector<float> recordedValues;
    
    // Handle del task FreeRTOS per la registrazione
    TaskHandle_t recordingTask;
    
    // Membro per salvare il canale corrente, aggiornato da startRecording()
    int currentChannel;
    
    // La funzione task che esegue in loop la registrazione (simile a Pin::recordingFunction)
    static void recordingTaskFunction(void *parameter);
};

#endif // ADS1115_CONTROLLER_H
