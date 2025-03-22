#ifndef MONITORRESOURCE_H
#define MONITORRESOURCE_H

#include "RoutesInterface.h"
#include <ESPAsyncWebSrv.h>

#include <Arduino.h>


#include "viewGraph.h"
#include "viewGeneric.h"

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

//per monitor metriche
#include <esp_heap_caps.h>


#include "SystemState.h"
#include "PinoutData.h"

class MonitorResource : public RoutesInterface
{

public:
    void defineRoutes(AsyncWebServer &server);
};

#endif // MONITORRESOURCE_H