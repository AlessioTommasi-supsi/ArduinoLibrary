#include "ModBusSlaveController.h"

// Inizializzazione dell'istanza singleton (ora public)
ModBusSlaveController* ModBusSlaveController::instance = nullptr;

// Funzione di polling eseguita nel task FreeRTOS
static void modbusPollTask(void *pvParameters) {
    ModBusSlaveController* controller = reinterpret_cast<ModBusSlaveController*>(pvParameters);
    while (true) {
        if (controller->modbusSlave) {
            controller->modbusSlave->poll();
        }
        // Ritardo breve per evitare di bloccare la CPU
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

// Funzione callback friend: ora NON è static perché la dichiarazione friend nell'header non lo indica
void modbusCallback(uint16_t startAddress, uint16_t quantity) {
    if (ModBusSlaveController::instance != nullptr) {
        ModBusSlaveController::instance->callback(startAddress, quantity);
    }
    else {
        Serial.println("Callback: instance non definita!");
    }
}

ModBusSlaveController::ModBusSlaveController()
{
    // Inizializzazione della seriale hardware (ad esempio, HardwareSerial(2))
    SerialPort = new HardwareSerial(2);

    // Creazione dell'oggetto Modbus slave usando il nome corretto della classe
    modbusSlave = new ModbusRTUSlave(*SerialPort, DE_PIN_SLAVE);

    // Inizializzazione del Modbus slave: ID, baudrate, formato, pin RX/TX
    modbusSlave->begin(SLAVE_ID, BAUDRATE, SERIAL_8N1, RX_PIN_SLAVE, TX_PIN_SLAVE);

    // Imposto la callback del Modbus
    modbusSlave->setFillHoldingRegistersCallback(modbusCallback);

    Serial.println("Inizializzazione ModbusSlaveController completata!");

    // Nessun task di polling avviato al momento
    pool_thread = NULL;

    // Creazione del mutex per operazioni thread-safe
    mutex = xSemaphoreCreateMutex();
}

ModBusSlaveController* ModBusSlaveController::getInstance(){
    if (instance == nullptr) {
        instance = new ModBusSlaveController();
    }
    return instance;
}

// Metodo per scrivere un registro intero
bool ModBusSlaveController::writeIntRegister(uint16_t address, int16_t value)
{
    if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
        // Conversione a float e impostazione di swap a false
        modbusSlave->insertIntoHoldingRegistersSlave(address, static_cast<float>(value), false);
        xSemaphoreGive(mutex);
        return true;
    }
    return false;
}

// Metodo per scrivere un registro float
bool ModBusSlaveController::writeFloatRegister(uint16_t address, float value, bool swap)
{
    if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
        modbusSlave->insertIntoHoldingRegistersSlave(address, value, swap);
        xSemaphoreGive(mutex);
        return true;
    }
    return false;
}

// Avvio del task di polling FreeRTOS
void ModBusSlaveController::poll()
{
    if (pool_thread == NULL) {
        BaseType_t result = xTaskCreate(
            modbusPollTask,         // Funzione task
            "ModbusSlavePoll",      // Nome del task (per debug)
            1024,                   // Dimensione dello stack (in words)
            this,                   // Parametro passato al task
            1,                      // Priorità del task
            &pool_thread            // Indirizzo dell'handle del task
        );
        if (result != pdPASS) {
            Serial.println("Errore: creazione task di polling fallita!");
        }
        else {
            Serial.println("Task di polling avviato!");
        }
    }
    else {
        Serial.println("Task di polling già in esecuzione");
    }
}
  
// Metodo callback chiamato dalla funzione friend modbusCallback
void ModBusSlaveController::callback(uint16_t startAddress, uint16_t quantity) 
{    
    Serial.print("Callback slave eseguita: startAddress = ");
    Serial.print(startAddress);
    Serial.print(", quantity = ");
    Serial.println(quantity);
}
