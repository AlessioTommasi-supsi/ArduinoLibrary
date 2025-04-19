#include "SmartHomeBoard.h"

SmartHomeBoard::SmartHomeBoard()
{
    Serial.println("Costruttore SmartHomeBoard");
    initializePins();
    Serial.println("Inizializzazione SmartHomeBoard effettuata!");
}

void SmartHomeBoard::initializePins()
{
    //richiama metodo padre
    Esp32_30pin::initializePins();
    //utilizza setdi metodo padre per modificare i pin
    Pin &livingRoomPin = getPin(32);
    livingRoomPin.type = PinType::DIGITAL;
    livingRoomPin.isInput = false;
    livingRoomPin.setNote("Studio porta ");
    livingRoomPin.setMode(OUTPUT);

    Pin &livingRoomPin1 = getPin(33);
    livingRoomPin1.type = PinType::DIGITAL;
    livingRoomPin1.isInput = false;
    livingRoomPin1.setNote("Studio luce interna ");
    livingRoomPin1.setMode(OUTPUT);

    Pin &livingRoomPin2 = getPin(25);
    livingRoomPin2.type = PinType::DIGITAL;
    livingRoomPin2.isInput = false;
    livingRoomPin2.setNote("Studio Luce esterna");
    livingRoomPin2.setMode(OUTPUT);

    Pin &cantina1 = getPin(26);
    cantina1.type = PinType::DIGITAL;
    cantina1.isInput = false;
    cantina1.setNote("Cantina Presepe Luce interna");
    cantina1.setMode(OUTPUT);

    Pin &cantina2 = getPin(27);
    cantina2.type = PinType::DIGITAL;
    cantina2.isInput = false;
    cantina2.setNote("Cantina Luce interna");
    cantina2.setMode(OUTPUT);

    Pin &camino = getPin(14);
    camino.type = PinType::DIGITAL;
    camino.isInput = false;
    camino.setNote("Camino Luce interna");
    camino.setMode(OUTPUT);

    Pin &unused7 = getPin(12);
    unused7.type = PinType::DIGITAL;
    unused7.isInput = false;
    unused7.setNote("Unused Pin 12 associato a relay numero 7");
    unused7.setMode(OUTPUT);

    Pin &unused8 = getPin(13);
    unused8.type = PinType::DIGITAL;
    unused8.isInput = false;
    unused8.setNote("Unused Pin 13 associato a relay numero 8");
    unused8.setMode(OUTPUT);

}