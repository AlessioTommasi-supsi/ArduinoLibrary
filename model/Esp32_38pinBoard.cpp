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
    // NON chiamare Esp32_38pin::initializePins() perché è già chiamato nel costruttore del padre
    
    // Modifica i pin specifici per questa board
    // getPin() restituisce il riferimento al pin esistente se presente
    Pin &pinA = getPin(12);
    sprintf(pinA.note, "MUX S0 - A");
    pinA.type = PinType::DIGITAL;
    pinA.isInput = false; // Imposta come output
    pinA.write(false);

    Pin &pinB = getPin(13);
    sprintf(pinB.note, "MUX S1 - B");
    pinB.type = PinType::DIGITAL;
    pinB.isInput = false; // Imposta come output
    pinB.write(false);

    Pin &pinC = getPin(14);
    sprintf(pinC.note, "MUX S2 - C");
    pinC.type = PinType::DIGITAL;
    pinC.isInput = false; // Imposta come output
    pinC.write(false);        
}