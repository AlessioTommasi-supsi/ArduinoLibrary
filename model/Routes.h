#ifndef ROUTES_H
#define ROUTES_H

#include <ESPAsyncWebSrv.h>
#include <Arduino.h>
#include <vector>
#include <string>

#include "viewCurrentRegister.h"
#include "viewHistory.h"
#include "Pinout.h"
#include "viewGraph.h"
#include "viewConfig.h"
#include "viewWifiSuccessChange.h"
#include "viewGeneric.h"

//#include "WiFiManager.h"
#include "SystemState.h"


//per monitor metriche
#include <esp_heap_caps.h>

class Routes
{
public:
    static void defineRoutes(AsyncWebServer &server);
};

#endif // ROUTES_H