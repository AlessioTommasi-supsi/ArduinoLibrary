#ifndef VIEWEDITPIN_H
#define VIEWEDITPIN_H

#include <Arduino.h>
#include "viewGeneric.h"
#include <vector>
#include "SystemState.h"

class viewEditPin
{
private:
public:
    static String html;
    static String generateHTML();
    static String generateForm();
    static String generateHTML(int pinNumber);
    static String generateHTML(int pinNumber, String script);
    static String addDefaultScript();
    static String addCustomScript(String script);
};

#endif