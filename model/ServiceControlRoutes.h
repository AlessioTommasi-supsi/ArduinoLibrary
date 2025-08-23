#ifndef SERVICECONTROLROUTES_H
#define SERVICECONTROLROUTES_H

#include "RoutesInterface.h"
#include <ESPAsyncWebSrv.h>
#include <Arduino.h>

class ServiceControlRoutes : public RoutesInterface
{
public:
    void defineRoutes(AsyncWebServer &server);
    
    // Content generation methods
    static String generateServicesContent();
    static String generateServiceControlsContent();
    static String generateServicesJavaScript();
};

#endif // SERVICECONTROLROUTES_H