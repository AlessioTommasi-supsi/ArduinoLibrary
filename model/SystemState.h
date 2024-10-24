#ifndef SYSTEMSTATE_H
#define SYSTEMSTATE_H

#include <vector>
#include <Arduino.h>
#include "MasterModbus.h"
#include "WiFiManager.h"
#include "WebServer.h"
#include "Logger.h"

#include <string>
#include <vector>
#include <mutex>
#include <thread>
#include <atomic>
#include <chrono>
#include <map>
#include <esp_task_wdt.h>



class WiFiManager;

enum class State
{
    INIT,
    MASTER,
    SLAVE,
    ERROR,
    WIFI_CHANGE
};

class SystemState
{
private:
    static SystemState *instance;
    State state = State::INIT;

    static char * error_message;
    
    std::vector<int> address;
    std::vector<float> value;

        
    std::mutex registerMutex;

    std::map<int, std::thread> recordingThreads;
    std::map<int, std::atomic<bool>> recordingActive;
    std::mutex recordingMutex;

    
    static int sd_pin;

    SystemState() {}

public:
    static MasterModbus *masterModbus;

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

    void pushRegister(int address, float value);

    void startRecordingRegister(int addr, int milliseconds);

    void stopRecordingRegister(int addr);

    std::vector<float> getAllRegisterValue(int address);

    std::vector<float> getAllRegisterValue();

    std::vector<int> getAllRegisterAddress();

    void clearRegisters();

    void defaultClear();

    void setWifiManager(WiFiManager *wifiController);

    void switchNetwork(const char *ssid, const char *password);

    void deleteValue(int index);

    void editValue(int index, float value);
    
};

#endif // SYSTEMSTATE_H