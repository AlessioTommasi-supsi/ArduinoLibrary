#ifndef MODBUSROUTES_H
#define MODBUSROUTES_H


#include "RoutesInterface.h"
#include <ESPAsyncWebSrv.h>
#include <Arduino.h>
#include <vector>
#include <string>



#include "viewGraph.h"
#include "viewGeneric.h"
#include "viewHistory.h"
#include "viewCurrentRegister.h"

#include "SystemState.h"
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>


class ModbusRoutes
{
public:
    void defineRoutes(AsyncWebServer &server);
};

#endif // MODBUSROUTES_H