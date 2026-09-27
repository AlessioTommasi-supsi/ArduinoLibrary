#include "SmartHomeBoard.h"

SmartHomeBoard::SmartHomeBoard()
{
    Serial.println("Costruttore SmartHomeBoard");
    initializePins();
    Serial.println("Inizializzazione SmartHomeBoard effettuata!");
}

void SmartHomeBoard::initializePins()
{
#if defined(CONFIG_IDF_TARGET_ESP32S3) || defined(ARDUINO_ESP32S3_DEV)
    // Pinout specifico per ESP32-S3 (GPIO 1, 2, 3, 7, 8, 9, 10, 11)
    Pin &livingRoomPin = getPin(1);
    livingRoomPin.type = PinType::DIGITAL;
    livingRoomPin.isInput = false;
    livingRoomPin.setNote("Studio porta ");
    livingRoomPin.setMode(OUTPUT);

    Pin &livingRoomPin1 = getPin(2);
    livingRoomPin1.type = PinType::DIGITAL;
    livingRoomPin1.isInput = false;
    livingRoomPin1.setNote("Studio luce interna ");
    livingRoomPin1.setMode(OUTPUT);

    Pin &livingRoomPin2 = getPin(3);
    livingRoomPin2.type = PinType::DIGITAL;
    livingRoomPin2.isInput = false;
    livingRoomPin2.setNote("Studio Luce esterna");
    livingRoomPin2.setMode(OUTPUT);

    Pin &cantina1 = getPin(7);
    cantina1.type = PinType::DIGITAL;
    cantina1.isInput = false;
    cantina1.setNote("Cantina Presepe Luce interna");
    cantina1.setMode(OUTPUT);

    Pin &cantina2 = getPin(8);
    cantina2.type = PinType::DIGITAL;
    cantina2.isInput = false;
    cantina2.setNote("Cantina Luce interna");
    cantina2.setMode(OUTPUT);

    Pin &camino = getPin(9);
    camino.type = PinType::DIGITAL;
    camino.isInput = false;
    camino.setNote("Camino Luce interna");
    camino.setMode(OUTPUT);

    Pin &unused7 = getPin(10);
    unused7.type = PinType::DIGITAL;
    unused7.isInput = false;
    unused7.setNote("Unused Pin 10 associato a relay numero 7");
    unused7.setMode(OUTPUT);

    Pin &unused8 = getPin(11);
    unused8.type = PinType::DIGITAL;
    unused8.isInput = false;
    unused8.setNote("Unused Pin 11 associato a relay numero 8");
    unused8.setMode(OUTPUT);
#else
    // Pinout classico per ESP32 30 pin
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
#endif
}