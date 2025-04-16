#ifndef VIEW_MULTIPLEX_H
#define VIEW_MULTIPLEX_H

#include <Arduino.h>
#include "viewGeneric.h"


class viewMultiplex
{
private:
    /* data */
public:

    static String Config();

    static String pageContent();

    static String pinStartAndStopForm(int PinNumber, String signalType);
};

#endif