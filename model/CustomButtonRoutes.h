#ifndef CUSTOMBUTTONROUTES_H
#define CUSTOMBUTTONROUTES_H

#include "RoutesInterface.h"
#include <ESPAsyncWebSrv.h>
#include <Arduino.h>
#include "CustomButton.h"

class CustomButtonRoutes : public RoutesInterface
{
private:
    static CustomButtonManager* customButtonManager;
    
public:
    void defineRoutes(AsyncWebServer &server);
    
    // Content generation methods
    static String generateAddButtonContent();
    static String generateCustomButtonsContent();
    
    // Utility methods
    static void initializeCustomButtonManager();
    static CustomButtonManager* getCustomButtonManager();
};

#endif // CUSTOMBUTTONROUTES_H