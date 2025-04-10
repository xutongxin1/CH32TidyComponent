#ifndef TCA9555_H_
#define TCA9555_H_

#include "stdint.h"
#include <I2C.h>

typedef enum tca_pins {
    P00 = 0,
    P01,
    P02,
    P03,
    P04,
    P05,
    P06,
    P07,
    P10,
    P11,
    P12,
    P13,
    P14,
    P15,
    P16,
    P17
} TCA_pins;

typedef enum tca_mode {
    OUTPUT = 0,
    INPUT = 1
} TCA_mode;

#define INPUT_REGISTER_LOW 		0x00
#define INPUT_REGISTER_HIGH 	0x01
#define OUTPUT_REGISTER_LOW 	0x02
#define OUTPUT_REGISTER_HIGH	0x03
#define POLARITY_REGISTER_LOW	0x04
#define POLARITY_REGISTER_HIGH 	0x05
#define PIN_MODE_REGISTER_LOW	0x06
#define PIN_MODE_REGISTER_HIGH	0x07

void writeToRegister(const uint8_t device_addr, const uint16_t mem_addr, const uint8_t data);

void TCA_PinMode(const uint8_t device_addr, const uint32_t pin, const TCA_mode mode);

void TCA_PinInvert(const uint8_t device_addr, const uint32_t pin, const uint32_t invert);

void TCA_WritePin(const uint8_t device_addr, const uint32_t pin, const uint32_t value);

void TCA_TogglePin(const uint8_t device_addr, const uint32_t pin);

int8_t TCA_ReadPin(const uint8_t device_addr, const uint32_t pin);

void TCA_SetAllPinsInput(const uint8_t device_addr);
#endif /* TCA9555_H_ */
