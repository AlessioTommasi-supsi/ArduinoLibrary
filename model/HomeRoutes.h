#ifndef HOMEROUTES_H
#define HOMEROUTES_H

#include "RoutesInterface.h"
#include <ESPAsyncWebSrv.h>
#include <Arduino.h>

class HomeRoutes : public RoutesInterface
{
public:
    void defineRoutes(AsyncWebServer &server);
    
    // Home page component methods
    static String generateHomeHeaderContent();
    static String generateHomeMainContent();
    static String generateHomeScriptsContent();
};

#endif // HOMEROUTES_H