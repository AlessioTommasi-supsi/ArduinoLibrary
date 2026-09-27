#ifndef SMART_HOME_BOARD_H
#define SMART_HOME_BOARD_H

/**
 * RICORDA: per far funzionare relay devi mandare a gnd il pin!
 */

#if defined(CONFIG_IDF_TARGET_ESP32S3) || defined(ARDUINO_ESP32S3_DEV)
  #include "Esp32_S3.h"
  typedef Esp32_S3 SmartHomeBaseBoard;
#else
  #include "Esp32_30pin.h"
  typedef Esp32_30pin SmartHomeBaseBoard;
#endif

class SmartHomeBoard : public SmartHomeBaseBoard {
private:
    // Aggiungi qui eventuali membri privati specifici per la tua classe
public:
    std::vector<Pin> pins;
    
    SmartHomeBoard();

    void initializePins() override;
    
};

#endif // SMART_HOME_BOARD_H
