#include "Esp32_38pinBoard.h"

Esp32_38pinBoard::Esp32_38pinBoard() {
    Serial.println("Costruttore Esp32_38pinBoard");
    initializePins();
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
    Pin &readedPin = getPin(12);

    sprintf(readedPin.note, "MUX S0 - A");
    readedPin.type = PinType::DIGITAL;
    readedPin.write(false);

    readedPin = getPin(13);
    sprintf(readedPin.note, "MUX S1 - B");
    readedPin.type = PinType::DIGITAL;
    readedPin.write(false);

    readedPin = getPin(14);
    sprintf(readedPin.note, "MUX S2 - C");
    readedPin.type = PinType::DIGITAL;
    readedPin.write(false);
        

}