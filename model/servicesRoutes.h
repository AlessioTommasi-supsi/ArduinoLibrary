#ifndef SERVICESROUTES_H
#define SERVICESROUTES_H

#include "RoutesInterface.h"
#include <ESPAsyncWebSrv.h>
#include <Arduino.h>

class servicesRoutes : public RoutesInterface
{
public:
    void defineRoutes(AsyncWebServer &server);
    static String generateServicesContent();
};

#endif // SERVICESROUTES_H