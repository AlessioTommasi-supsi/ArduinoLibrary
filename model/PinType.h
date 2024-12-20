#ifndef PINTYPE_H
#define PINTYPE_H

enum class PinType {
    ANALOGIC,
    DIGITAL,
    VIN,
    VOUT,
    GND,
    ADC,
    DAC,
    PWM,
    TOUCH,
    UART,
    SPI,
    I2C,
    // Aggiungi altri tipi se necessario
};

#endif // PINTYPE_H
