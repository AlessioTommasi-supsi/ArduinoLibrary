#ifndef SMART_HOME_BOARD_H
#define SMART_HOME_BOARD_H

/**
 * RICORDA: per far funzionare relay devi mandare a gnd il pin!
 */

#include "Esp32_30pin.h"

class SmartHomeBoard : public Esp32_30pin {
private:
    // Aggiungi qui eventuali membri privati specifici per la tua classe
public:
    std::vector<Pin> pins;
    
    SmartHomeBoard();

    void initializePins() override;
    
};

#endif // SMART_HOME_BOARD_H
