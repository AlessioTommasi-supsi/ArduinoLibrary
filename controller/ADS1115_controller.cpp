#include "ADS1115_controller.h"

// Inizializzazione della variabile static
ADS1115_controller* ADS1115_controller::instance = nullptr;

ADS1115_controller* ADS1115_controller::getInstance() {
    if(instance == nullptr) {
        instance = new ADS1115_controller();
    }
    return instance;
}

ADS1115_controller::ADS1115_controller()
  :  recordingActive(false), recordingInterval(1000),
    lastRecordTime(0), recordingTask(NULL), monitorTask(NULL), currentChannel(0), initializationFailed(false)
{

    // Inizializza il modello ADS1115
    adsModel = new ADS1115_model();


    mutex = xSemaphoreCreateMutex();
    // Inizializza l'ADS1115; se fallisce, imposta il flag
    if (!ads.begin()) {
        Serial.println("Failed to initialize ADS1115.");
        initializationFailed = true;
    } else {
        initializationFailed = false;
    }
}

int ADS1115_controller::signalTypeToChannel(const String &signalType) {
    if (signalType == "resistenza") {
        return 0;
    } else if (signalType == "tensione_non_amplificato") {
        return 1;
    } else if (signalType == "tensione_amp_331") {
        return 2;
    } else if (signalType == "tensione_amp_0.216") {
        return 3;
    } else if (signalType == "CN10_resistenza") {
        return 4;
    } else if (signalType == "CN10_tensione_non_amplificato") {
        return 5;
    } else if (signalType == "CN10_tensione_amp_331") {
        return 6;
    } else if (signalType == "CN10_tensione_amp_0.216") {
        return 7;
    } else {
        return -1;
    }
}

float ADS1115_controller::signalCorrectionValue(int channel) {
    float correctionValue = 1.0;
    switch (channel)
    {
    case 0:
        correctionValue = 1/9000;  //ritorna dato in KOhm!
    break;
    case 4:
        correctionValue = 1/9000;  //ritorna dato in KOhm!
    break;
    
    case 1:
        correctionValue = 1/1000; //ritorna dato in mV!

    default:
        break;
    }

    return correctionValue;
}

void ADS1115_controller::setChannel(const String &signalType) {
    int channel = signalTypeToChannel(signalType);
    if (channel < 0) {
        Serial.println("Errore: signalType non valido: " + signalType);
        return;
    }
    
    if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
        adsModel->setChannel(channel);
        currentChannel = channel;
        xSemaphoreGive(mutex);
    }
}


void ADS1115_controller::startRecording(const String &signalType, int interval) {
    int channel = signalTypeToChannel(signalType);
    if (channel < 0) {
        Serial.println("Errore: signalType non valido: " + signalType);
        return;
    }
    if (initializationFailed) {
        Serial.println("Errore: ads non inizializzato!: " + signalType);
        return;
    }
    
    if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
        recordingInterval = interval;
        recordingActive = true;
        lastRecordTime = millis();
        recordedValues.clear();
        currentChannel = channel;
        
        // Configura il multiplexer
        adsModel->setChannel(currentChannel);
        
        // Lettura immediata
        int16_t adc = ads.readADC_SingleEnded(0);
        float volts = ads.computeVolts(adc);
        volts = volts * signalCorrectionValue(currentChannel); // Applica la correzione del segnale
        recordedValues.push_back(volts);
        Serial.print("Lettura iniziale ADS (canale ");
        Serial.print(currentChannel);
        Serial.print("): ");
        Serial.print(volts);
        Serial.println(" V");
        
        if (recordingTask == NULL) {
            xTaskCreatePinnedToCore(
                recordingTaskFunction,
                "ADS1115RecordingTask",
                4096,
                this,
                1,
                &recordingTask,
                0
            );
        }
        xSemaphoreGive(mutex);
        Serial.print("Registrazione avviata sul canale ADS ");
        Serial.print(currentChannel);
        Serial.print(" con intervallo ");
        Serial.print(interval);
        Serial.println(" ms");
    }
}

void ADS1115_controller::stopRecording() {
    if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
        recordingActive = false;
        if (recordingTask != NULL) {
            vTaskDelete(recordingTask);
            recordingTask = NULL;
            Serial.println("Recording task terminato.");
        }
        xSemaphoreGive(mutex);
    }
}

void ADS1115_controller::printReadFromADS1115(int channel)
{
  int16_t adc;
  float volts;
  //65563/2 = 32668
  adc = ads.readADC_SingleEnded(channel);
  volts = ads.computeVolts(adc);
  volts = volts; // Aggiungi un offset della scheda
  Serial.println("");
  Serial.print("AIN");Serial.print(channel);Serial.print(": ");  Serial.print(adc);  Serial.print("  "); Serial.print(volts); Serial.println("V");
  
}

void ADS1115_controller::printAllReadingsFromADS1115()
{
  Serial.println("-------------------------------------------------");
  for (int i = 0; i < 4; i++)
  {
    printReadFromADS1115(i);
  }
  Serial.println("-------------------------------------------------");
}

float ADS1115_controller::read(){
    float volts = 0;
    if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
        // Lettura immediata
        int16_t adc = ads.readADC_SingleEnded(0);
        volts = ads.computeVolts(adc);
        volts = volts * signalCorrectionValue(currentChannel); // Applica la correzione del segnale
        Serial.print("Read  ADS (canale ");
        Serial.print(currentChannel);
        Serial.print("): ");
        Serial.print(volts);
        Serial.println(" V");
        xSemaphoreGive(mutex);
    }
    return volts;
}

void ADS1115_controller::startMonitorTask(int outputPinNumber) {

    if (initializationFailed) {
        Serial.println("Errore: ads non inizializzato!: ");
        return;
    }
    if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
        this->outputPinNumber = outputPinNumber;
        if (monitorTask == NULL) {
            xTaskCreatePinnedToCore(
                monitorTaskFunction,
                "ADS1115MonitorTask",
                4096,
                this,
                1,
                &monitorTask,
                0
            );
        }
        xSemaphoreGive(mutex);
        Serial.println("Monitor task avviato.");

    }
}

void ADS1115_controller::stopMonitorTask() {
    if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
        if (monitorTask != NULL) {
            vTaskDelete(monitorTask);
            monitorTask = NULL;
            Serial.println("Monitor task terminato.");
        }
        xSemaphoreGive(mutex);
    }
}

void ADS1115_controller::monitorTaskFunction(void *parameter) {
    ADS1115_controller *controller = static_cast<ADS1115_controller*>(parameter);
    
    while (true) {
        //Serial.println("Monitor task in esecuzione...");
        unsigned long currentTime = millis();
        if (currentTime - controller->lastRecordTime >= (unsigned long)controller->recordingInterval) {
            if (xSemaphoreTake(controller->mutex, portMAX_DELAY) == pdTRUE) {
                try
                {
                    controller->adsModel->setChannel(controller->currentChannel);
                    int16_t adc = controller->ads.readADC_SingleEnded(0);
                    float volts = controller->ads.computeVolts(adc);
                    Serial.print("Monitor ADS (canale ");
                    Serial.print(controller->currentChannel);
                    Serial.print("): ");
                    Serial.print(volts);
                    Serial.println(" V");

                    Pin &outputPin = SystemState::getInstance()->pinoutData->getPin(controller->outputPinNumber);
                    bool goHigh = volts > 1.5; // Soglia di attivazione
                    outputPin.write(goHigh);
                    
                    controller->lastRecordTime = currentTime;
                }
                catch(...)
                {
                    Serial.println("Errore nella lettura dell'ADS1115.");
                }
                
                xSemaphoreGive(controller->mutex);
            }
            
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);// aspetto ms prima di fare un'altra lettura
    }
    vTaskDelete(NULL);
}


void ADS1115_controller::recordingTaskFunction(void *parameter) {
    ADS1115_controller *controller = static_cast<ADS1115_controller*>(parameter);
    
    while (true) {
        if (controller->recordingActive) {
            unsigned long currentTime = millis();
            if (currentTime - controller->lastRecordTime >= (unsigned long)controller->recordingInterval) {
                if (xSemaphoreTake(controller->mutex, portMAX_DELAY) == pdTRUE) {
                    controller->adsModel->setChannel(controller->currentChannel);
                    int16_t adc = controller->ads.readADC_SingleEnded(0);
                    float volts = controller->ads.computeVolts(adc);
                    volts = volts * controller->signalCorrectionValue(controller->currentChannel); // Applica la correzione del segnale
                    controller->recordedValues.push_back(volts);
                    controller->lastRecordTime = currentTime;
                    Serial.print("Lettura ADS (canale ");
                    Serial.print(controller->currentChannel);
                    Serial.print("): ");
                    Serial.print(volts);
                    Serial.println(" V");
                    // Stampa tutte le letture
                    //controller->printAllReadingsFromADS1115();
                    xSemaphoreGive(controller->mutex);
                }
            }
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

const std::vector<float>& ADS1115_controller::getRecordedValues() const {
    return recordedValues;
}

int ADS1115_controller::getCurrentChannel() const {
    return currentChannel;
}

bool ADS1115_controller::isInitializationFailed() const {
    return initializationFailed;
}

void ADS1115_controller::reinitialize() {
    if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
        if (ads.begin()) {
            initializationFailed = false;
            Serial.println("Reinizializzazione riuscita.");
        } else {
            initializationFailed = true;
            Serial.println("Reinizializzazione fallita.");
        }
        xSemaphoreGive(mutex);
    }
}
