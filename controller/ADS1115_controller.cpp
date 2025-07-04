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

    // Inizializza le classi per linearizzazione
    termocoppiaK = new TermocoppiaK();
    pt100 = new PT100();
    pt1000 = new PT1000();

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
    } else if (signalType == "termocoppia_cn2") {
        return 8;
    } else if (signalType == "PT100_cn2") {
        return 9;
    } else if (signalType == "termocoppia_cn10") {
        return 10;
    } else if (signalType == "PT100_cn10") {
        return 11;
    } else if (signalType == "PT1000_cn2") {
        return 12; // Aggiunto per PT1000 CN2
    } else if (signalType == "PT1000_cn10") {
        return 13;
    } else if (signalType == "TEST") {
        return 999; // Canale speciale per la modalità TEST
    } else {
        return -1;
    }
}

float ADS1115_controller::signalCorrectionValue(int channel, float volts) {
    Serial.print("SignalCorrectionValue!! raw READINGS FOR ADS: ");
    Serial.println(volts);
    float correctionValue = 1.0;
    switch (channel)
    {//ohm
    case 0:
        
        
        
        //volts = volts * correctionValue; //ritorna dato in V!
        Serial.print("valore in volt letto da AD:     ");
        Serial.println(volts);
        //formula data da prof: R da calcolare = (tensione letta in ch0 * 2700)/ (2.5 - tensione letta in ch0)
        volts = (volts * 2700) / (2.5 - volts); //ritorna dato in Ohm!
    
    break;
    case 1:
        volts  = (volts/110)*1000; //ma corrente
        correctionValue = 1;  //
    break;
    case 2:
        correctionValue = 1;  //
    break;
    case 3:
        //Serial.println("Canale 3 selezionato!");
        correctionValue = correctionValue / 0.216; //ritorna dato in micro V!
    break;
    case 4:
        volts = (volts * 2700) / (2.5 - volts); //ritorna dato in Ohm!
        break;
    case 5:
        volts  = (volts/110)*1000; //ma corrente
        correctionValue = 1;  //
        
    break;
    case 6:
        correctionValue = 1;  //
        break;
    case 7:
        correctionValue = 1/0.216; //ritorna dato in V!
        break;


    case 8:// Termocoppia CN2
        // Applica il fattore di correzione per convertire in microV (simile al caso 3)
        volts = volts * TermocoppiaK::CORRECTION_FACTOR; // Converte in microV
        // Usa la tabella di linearizzazione per ottenere la temperatura
        volts = termocoppiaK->getTemperature(volts); // Input: microV, Output: °C
        break;
    case 9:// PT100 CN2
        //formula data da prof: R da calcolare = (tensione letta in ch0 * 2700)/ (2.5 - tensione letta in ch0)
        volts = (volts * 2700) / (2.5 - volts); //ritorna dato in Ohm!
        // Usa la classe PT100 per convertire la resistenza in temperatura
        volts = pt100->getTemperature(volts); // Input: Ohm, Output: °C
        break;
    case 10:
        correctionValue = 1;  //
        break;
    case 11:
        correctionValue = 1/0.216; //ritorna dato in V!
        break;
    

    case 12:// PT1000 CN2
        //formula data da prof: R da calcolare = (tensione letta in ch0 * 2700)/ (2.5 - tensione letta in ch0)
        volts = (volts * 2700) / (2.5 - volts); //ritorna dato in Ohm!
        // Usa la classe PT1000 per convertire la resistenza in temperatura
        volts = pt1000->getTemperature(volts); // Input: Ohm, Output: °C
        break;
    case 999:// TEST MODE - return simulated value
        // In modalità TEST, restituisce un valore simulato senza elaborazioni
        Serial.println("TEST mode: returning simulated value");
        volts = 20.0 + random(-50, 50) / 10.0; // Simula temperatura 15-25°C
        break;
    default:
        Serial.println("Default Case! channel not set!  Channel: "+ channel);
    break;
    }

    return volts;
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
        volts =  signalCorrectionValue(currentChannel, volts); // Applica la correzione del segnale
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
  volts = volts; // Aggiungi un offset o correctionvalue etc della scheda se necessario
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
        volts = signalCorrectionValue(currentChannel, volts); // Applica la correzione del segnale
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
    create_startMonitorTask:
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
        }else {
            Serial.println("Monitor task già in esecuzione, lo stoppo e ne creo uno nuovo! .");
            xSemaphoreGive(mutex);
            stopMonitorTask();
            goto create_startMonitorTask; // Riprova a creare il task
        }
        xSemaphoreGive(mutex);
        Serial.println("Monitor task avviato.");

    }
}


void ADS1115_controller::startAlertMonitorTask(int outputPinNumber, float alertValue) {
    if (initializationFailed) {
        Serial.println("Errore: ads non inizializzato!: ");
        return;
    }
    create_alertMonitorTask:
    if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
        this->outputPinNumber = outputPinNumber;
        this->alertValue = alertValue;
        if (monitorTask == NULL) {
            xTaskCreatePinnedToCore(
                monitorAlertTaskFunction,
                "ADS1115MonitorAlertTask",
                4096,
                this,
                1,
                &monitorTask,
                0
            );
        }else {
            Serial.println("Alert  task già in esecuzione, lo stoppo e ne creo uno nuovo! .");
            xSemaphoreGive(mutex);
            stopMonitorTask();
            goto create_alertMonitorTask; // Riprova a creare il task
        }
        xSemaphoreGive(mutex);
        Serial.println("Monitor task avviato.");
    }
}

void ADS1115_controller::stopMonitorTask() { /*cancello sia che stia facendo alert che solito monitor!*/
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
                    volts = controller->signalCorrectionValue(controller->currentChannel, volts); // Applica la correzione del segnale
                    Serial.print("Monitor ADS (canale ");
                    Serial.print(controller->currentChannel);
                    Serial.print("): ");
                    Serial.print(volts);
                    Serial.println(" V");

                    Pin &outputPin = SystemState::getInstance()->pinoutData->getPin(controller->outputPinNumber);
                    
                    if (controller->outputPinNumber == 25) // Se l'uscita è digitale
                    {
                        // Esegui il monitoraggio digitale
                        Serial.println("Monitoraggio digitale attivo.");
                        bool goHigh = volts > 1.5; // Soglia di attivazione
                        outputPin.write(goHigh);
                    }
                    else if (controller->outputPinNumber == 26) // Se l'uscita è analogica
                    {
                        // Esegui il monitoraggio analogico
                        Serial.println("Monitoraggio analogico attivo.");
                        pinMode(26, OUTPUT);
                        //devo convertrire: 0 - 3.3V in 0 - 255
                        outputPin.voltage = volts; 
                        int dacValue = static_cast<int>((volts / 3.3) * 255); // Converti 0-3.3V in 0-255
                        dacWrite(26, dacValue);  // Scrivi il valore convertito nel DAC

                    }
                    

                    
                    
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

void ADS1115_controller::monitorAlertTaskFunction(void *parameter) {
    ADS1115_controller *controller = static_cast<ADS1115_controller*>(parameter);
    
    while (true) {
        unsigned long currentTime = millis();
        if (currentTime - controller->lastRecordTime >= (unsigned long)controller->recordingInterval) {
            if (xSemaphoreTake(controller->mutex, portMAX_DELAY) == pdTRUE) {
                try
                {
                    controller->adsModel->setChannel(controller->currentChannel);
                    int16_t adc = controller->ads.readADC_SingleEnded(0);
                    float volts = controller->ads.computeVolts(adc);
                    volts =  controller->signalCorrectionValue(controller->currentChannel, volts); // Applica la correzione del segnale
                    Serial.print("Monitor Alert ADS (canale ");
                    Serial.print(controller->currentChannel);
                    Serial.print("): ");
                    Serial.print(volts);
                    Serial.println(" V ");
                    Serial.print("Soglia: ");
                    Serial.print(controller->alertValue);
                    Serial.println(" ");

                    Pin &outputPin = SystemState::getInstance()->pinoutData->getPin(controller->outputPinNumber);
                    bool goHigh = volts > controller->alertValue; // Soglia di attivazione
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

/*MISURA RESISTENZA O MUSURA TENSIONE STACCA SEMPRE LA TENSIONE*/
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
                    float correction = controller->signalCorrectionValue(controller->currentChannel,volts); 
                    volts =  correction; // Applica la correzione del segnale
                    Serial.println("Selected correction value: "+String(correction));
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
