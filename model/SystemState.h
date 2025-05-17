#ifndef SYSTEMSTATE_H
#define SYSTEMSTATE_H

#include <vector>
#include <Arduino.h>
#include "WiFiManager.h"
#include "WebServer.h"
#include "Logger.h"
#include "PinoutData.h"


#include <string>
#include <vector>
#include <mutex>
#include <thread>
#include <atomic>
#include <chrono>
#include <map>

//multithread

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>



class WiFiManager;

enum class State
{
    INIT,
    MASTER,
    SLAVE,
    ERROR,
    WIFI_CHANGE,
    READ_VOLTAGE
};

class SystemState
{
private:
    static SystemState *instance;
    State state = State::INIT;

    static char * error_message;
    
    /*Modbus attribute*/
    std::vector<int> address;
    std::vector<float> value;

    std::vector<int> pinNumber;
    std::vector<float> pinValue;

    std::mutex registerMutex;

    std::map<int, std::thread> recordingThreads;
    std::map<int, std::atomic<bool>> recordingActive;
    
    
    static int sd_pin;

    SystemState() {
        // Inizializza il mutex per la sincronizzazione
        modbus_mutex = xSemaphoreCreateMutex();
        pinout_mutex = xSemaphoreCreateMutex();
    }

public:
    SemaphoreHandle_t modbus_mutex; // Mutex per sincronizzazione thread-safe
    SemaphoreHandle_t pinout_mutex; // Mutex per sincronizzazione thread-safe


    static PinoutData *pinoutData;


    static WiFiManager *wifiManager;

    static bool isStopped;

    static SystemState *getInstance();

    ~SystemState();

    void update();

    void  logCurrentState();

    void setState(State newState);

    State getState();

    char * getStateString();

    void setError(char * message);

    void setError(const char * message);

    char * getError();

    std::vector<float> getAllRegisterValue(int address);

    std::vector<float> getAllRegisterValue();

    std::vector<int> getAllRegisterAddress();

    void defaultClear();

    void setWifiManager(WiFiManager *wifiController);

    void setPinoutData(PinoutData *pinoutData);

    void switchNetwork(const char *ssid, const char *password);

    void deleteValue(int index);

    void editValue(int index, float value);

    bool getPinoutLock()
    {
        return xSemaphoreTake(pinout_mutex, portMAX_DELAY);
    }
    void releasePinoutLock()
    {
        xSemaphoreGive(pinout_mutex);
    }
    
};

#endif // SYSTEMSTATE_H