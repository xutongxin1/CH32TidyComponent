#include "TCA9555.h"

uint8_t output_register_state_low 		= 0xFF;
uint8_t output_register_state_high 		= 0xFF;
uint8_t polarity_register_state_low 	= 0x00;
uint8_t polarity_register_state_high	= 0x00;
uint8_t pinmode_register_state_low 		= 0xFF;
uint8_t pinmode_register_state_high 	= 0xFF;

uint8_t TCA_address = 0x21;									//default address

I2C_HandleTypeDef I2CHandle;

void writeToRegister(uint16_t device_address, uint16_t memory_address, uint8_t *data)
{
	HAL_I2C_Mem_Write(&I2CHandle, (device_address<<1), memory_address, 1, data, 1, 1000);
}

void TCA_Init(uint8_t address, I2C_HandleTypeDef handle)
{
	TCA_address = address;
	I2CHandle = handle;
}

void TCA_PinMode(uint32_t pin, uint32_t mode)
{
	if(!is_initialised) return;
	if(pin > 15) return;
	if(mode != 0 && mode != 1) return;

	uint8_t mask =  1 << (pin % 8);

	uint16_t memory_address = pin < 8 ? PINMODE_REGISTER_LOW : PINMODE_REGISTER_HIGH;
	uint8_t *data = pin < 8 ? &pinmode_register_state_low : &pinmode_register_state_high;

	if (mode) 	*data |= mask;
	else 		*data &= ~mask;

	writeToRegister(TCA_address, memory_address, data);
}

void TCA_PinInvert(uint32_t pin, uint32_t invert)
{
	if(!is_initialised) return;
	if(pin > 15) return;
	if(invert != 0 && invert != 1) return;

	uint8_t mask =  1 << (pin % 8);

	uint16_t memory_address = pin < 8 ? POLARITY_REGISTER_LOW : POLARITY_REGISTER_HIGH;
	uint8_t *data = pin < 8 ? &polarity_register_state_low : &polarity_register_state_high;

	if (invert)	*data |= mask;
	else 		*data &= ~mask;

	writeToRegister(TCA_address, memory_address, data);
}

void TCA_WritePin(uint32_t pin, uint32_t value)
{
	if(!is_initialised) return;
	if(pin > 15) return;
	if(value != 0 && value != 1) return;

	uint8_t mask =  1 << (pin % 8);

	uint16_t memory_address = pin < 8 ? OUTPUT_REGISTER_LOW : OUTPUT_REGISTER_HIGH;
	uint8_t *data = pin < 8 ? &output_register_state_low : &output_register_state_high;

	if (value)	*data |= mask;
	else 		*data &= ~mask;

	writeToRegister(TCA_address, memory_address, data);
}

void TCA_TogglePin(uint32_t pin)
{
	if(!is_initialised) return;
	if(pin > 15) return;

	uint8_t mask = 1 << (pin % 8);

	uint16_t memory_address = pin < 8 ? OUTPUT_REGISTER_LOW : OUTPUT_REGISTER_HIGH;
	uint8_t *data = pin < 8 ? &output_register_state_low : &output_register_state_high;

	*data ^= mask;

	writeToRegister(TCA_address, memory_address, data);
}

uint32_t TCA_ReadPin(uint32_t pin)
{
	if(!is_initialised) return -1;
	if(pin > 15) return -1;

	uint8_t mask = 1 << (pin % 8);

	uint16_t memory_address = pin < 8 ? INPUT_REGISTER_LOW : INPUT_REGISTER_HIGH;

	uint8_t result;
	HAL_I2C_Mem_Read(&I2CHandle, TCA_address, memory_address, 1, &result, 1, 1000);

	return (result & mask) != 0;
}