#ifndef ROUTES_H
#define ROUTES_H

#include <ESPAsyncWebSrv.h>
#include <Arduino.h>
#include <vector>
#include <string>

#include "viewGraph.h"
#include "viewConfig.h"
#include "viewWifiSuccessChange.h"
#include "viewGeneric.h"
#include "viewHome.h"

#include "SystemState.h"

#include "RoutesInterface.h"
#include "pinoutRoutes.h"
#include "wifiRoutes.h"
#include "monitorResource.h"
#include "cssRoutes.h"

// Nuove classi specializzate
#include "CustomButtonRoutes.h"
#include "ServiceControlRoutes.h"
#include "HomeRoutes.h"

class Routes : public RoutesInterface
{
public:
    void defineRoutes(AsyncWebServer &server);
};

#endif // ROUTES_H