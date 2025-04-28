#include "Esp32_38pinBoard.h"

Esp32_38pinBoard::Esp32_38pinBoard() {
    Serial.println("Costruttore Esp32_38pinBoard");
    initializePins();
    printPinsOnSerial();
    Serial.println("Inizializzazione Esp32_38pinBoard effettuata!");
}

/**
 * di default seleziono il canale 0 del MUX!
 */
void Esp32_38pinBoard::initializePins()
{
    //richiama metodo padre
    //Esp32_38pin::initializePins();
    //utilizza set di metodo padre per modificare i pin
    Pin &pinA = getPin(12);

    sprintf(pinA.note, "MUX S0 - A");
    pinA.type = PinType::DIGITAL;
    pinA.write(false);
    addPin(pinA);

    Pin &pinB = getPin(13);
    sprintf(pinB.note, "MUX S1 - B");
    pinB.type = PinType::DIGITAL;
    pinB.write(false);
    addPin(pinB);

    Pin &pinC = getPin(14);
    sprintf(pinC.note, "MUX S2 - C");
    pinC.type = PinType::DIGITAL;
    pinC.write(false);        
    addPin(pinC);

}