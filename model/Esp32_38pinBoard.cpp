#include "Esp32_38pinBoard.h"

Esp32_38pinBoard::Esp32_38pinBoard() {
    Serial.println("Costruttore Esp32_38pinBoard");
    initializePins();
    Serial.println("Inizializzazione Esp32_38pinBoard effettuata!");
}

void Esp32_38pinBoard::initializePins()
{
    //richiama metodo padre
    //Esp32_38pin::initializePins();
    //utilizza setdi metodo padre per modificare i pin
    Pin &readedPin = getPin(23);

    if (readedPin.type == PinType::UNKNOWN)
    {
        /* il pin non era stato inizializzato, quindi devo aggiungerlo! */
        Esp32_38pin::addPin(readedPin);
    }
    sprintf(readedPin.note, "Esp32_38pinBoard - VSPI MOSI or input digital");
        

}