#include "SystemState.h"
#include "WiFiManager.h"

#include "esp_task_wdt.h"


SystemState *SystemState::instance = nullptr;
bool SystemState::isStopped = false;
WiFiManager *SystemState::wifiManager = nullptr;
PinoutData *SystemState::pinoutData = nullptr;

int SystemState::sd_pin = 0;


char * SystemState::error_message = "";


SystemState::~SystemState()
{
    defaultClear();
}

void SystemState::defaultClear()
{
    delete instance;
    instance = nullptr;
}

SystemState *SystemState::getInstance()
{
    if (instance == nullptr)
    {
        instance = new SystemState();
        error_message = "";
        sd_pin = 5;
        
        // Inizializza il Logger e la scheda SD
        //Logger::getInstance().begin(sd_pin);

        
    }
    return instance;
}

void SystemState::update()
{
    //qui per esempio posso modificare led in base allo stato del componente!
    //logCurrentState();
    //Serial.println("Updating system state...");
}


void SystemState::setState(State newState)
{
    
    state = newState;
    update();
}


void SystemState::setPinoutData(PinoutData *pinout)
{
    pinoutData = pinout;
}

State SystemState::getState()
{    
    return state;
}

char * SystemState::getStateString()
{
    switch (state)
    {
    case State::INIT:
        return "INIT";
    case State::MASTER:
        return "MASTER";
    case State::SLAVE:
        return "SLAVE";
    default:
        return "ERROR";
    }
}

void SystemState::setError(const char * message)
{
    char error[100];
    strncpy(error, message, sizeof(error) - 1);
    error[sizeof(error) - 1] = '\0';
    setError(error);
}


void SystemState::setError( char* message)
{
    
    error_message = message;

    Serial.println("Error occourred!");

    setState(State::ERROR);
}

char * SystemState::getError()
{
    return error_message;
}


void SystemState::setWifiManager(WiFiManager *wifiController)
{
    wifiManager = wifiController;
}

WiFiManager* SystemState::getWifiManager()
{
    return wifiManager;
}

void SystemState::switchNetwork(const char *ssid, const char *password)
{
    setState(State::WIFI_CHANGE);
    wifiManager->setNetwork(ssid, password);
}

void SystemState::deleteValue(int index)
{
    if (xSemaphoreTake(modbus_mutex, portMAX_DELAY))
    { 
        address.erase(address.begin() + index);
        value.erase(value.begin() + index);
        xSemaphoreGive(modbus_mutex);
    }
}

void SystemState::editValue(int index, float value)
{   
    if (xSemaphoreTake(modbus_mutex, portMAX_DELAY))
    {
        this->value[index] = value;
        xSemaphoreGive(modbus_mutex);
    }
    
}