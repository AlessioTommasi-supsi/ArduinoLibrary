#ifndef pinout_H
#define pinout_H

#include <Arduino.h>
#include "viewGeneric.h"
#include <vector>
#include "SystemState.h"

class Pinout
{
private:
public:
    static String html;
    static String generateHTML();
};

#endif