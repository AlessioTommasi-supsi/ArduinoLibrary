#ifndef MODBUS_SLAVE_CONTROLLER_H
#define MODBUS_SLAVE_CONTROLLER_H

#define RX_PIN_SLAVE 33  // Pin RX
#define TX_PIN_SLAVE 32  // Pin TX
#define DE_PIN_SLAVE 27  // Pin DE/RE
#define SLAVE_ID 1
#define BAUDRATE 9600

#include "ModbusRTUMaster.h"
#include "ModbusRTUSlave.h"
#include "Utils.h"
#include <HardwareSerial.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

class ModBusSlaveController {
public:
    // Spostati in public come richiesto
    ModbusRTUSlave* modbusSlave;
    HardwareSerial* SerialPort;

    // Puntatore all'istanza singleton (residente in public)
    static ModBusSlaveController* instance;

    // Ritorna l'istanza singleton
    static ModBusSlaveController* getInstance();

    // Scrittura di un registro intero (16-bit)
    bool writeIntRegister(uint16_t address, int16_t value);

    // Scrittura di un registro float (32-bit, su 2 word)
    bool writeFloatRegister(uint16_t address, float value, bool swap = true);

    // Avvia il task di polling del Modbus in background
    void poll();

    // Metodo callback richiamato dalla funzione friend
    void callback(uint16_t startAddress, uint16_t quantity);

    // Rende amica la funzione modbusCallback per permetterle di accedere ai membri
    friend void modbusCallback(uint16_t, uint16_t);

    // Se necessario, puoi rendere anche il task di polling public per il debug.
    // Nota: solo modbusPollTask rimane file-local, perché è definito come static a livello di file.
    
    // Handle del task di polling
    TaskHandle_t pool_thread;
    
    // Mutex per la sincronizzazione
    SemaphoreHandle_t mutex;

    // Costruttore pubblico (se vuoi rendere tutto pubblico invece di private)
    ModBusSlaveController();
};

#endif // MODBUS_SLAVE_CONTROLLER_H
