#ifndef TCA9555_H_
#define TCA9555_H_

#include "stdint.h"
#include "stm32f1xx_hal.h"

typedef enum tca_pins{
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
}TCA_pins;

#define INPUT_REGISTER_LOW 		0x00
#define INPUT_REGISTER_HIGH 	0x01
#define OUTPUT_REGISTER_LOW 	0x02
#define OUTPUT_REGISTER_HIGH	0x03
#define POLARITY_REGISTER_LOW	0x04
#define POLARITY_REGISTER_HIGH 	0x05
#define PINMODE_REGISTER_LOW	0x06
#define PINMODE_REGISTER_HIGH	0x07

void writeToRegister(uint16_t device_address, uint16_t memory_address, uint8_t *data);

void TCA_Init(uint8_t address, I2C_HandleTypeDef handle);

void TCA_PinMode(uint32_t pin, uint32_t mode);

void TCA_PinInvert(uint32_t pin, uint32_t invert);

void TCA_WritePin(uint32_t pin, uint32_t value);

void TCA_TogglePin(uint32_t pin);

uint32_t TCA_ReadPin(uint32_t pin);

#endif /* TCA9555_H_ */