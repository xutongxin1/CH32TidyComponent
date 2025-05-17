#include "TCA9555.h"

uint8_t output_register_state_low = 0xFF;
uint8_t output_register_state_high = 0xFF;
uint8_t polarity_register_state_low = 0x00;
uint8_t polarity_register_state_high = 0x00;
uint8_t pin_mode_register_state_low = 0xFF;
uint8_t pin_mode_register_state_high = 0xFF;

void writeToRegister(const uint8_t device_addr, const uint16_t mem_addr, const uint8_t data) {
    if (I2C_WriteOneByte_TimeOut((device_addr << 1), mem_addr, data, 1024) != I2C_OK) {
        PRINT("写入I2C TCA9555失败\r\n");
    }
}

void TCA_PinMode(const uint8_t device_addr, const uint32_t pin, const TCA_mode mode) {
    if (pin > 15)
        return;
    if (mode != 0 && mode != 1)
        return;

    const uint8_t mask = 1 << (pin % 8);

    const uint16_t memory_address = pin < 8 ? PIN_MODE_REGISTER_LOW : PIN_MODE_REGISTER_HIGH;
    uint8_t *data = pin < 8 ? &pin_mode_register_state_low : &pin_mode_register_state_high;

    if (mode)
        *data |= mask;
    else
        *data &= ~mask;

    writeToRegister(device_addr, memory_address, *data);
}

void TCA_SetAllPinsInput(const uint8_t device_addr) {
    writeToRegister(device_addr, PIN_MODE_REGISTER_LOW, pin_mode_register_state_low);

    writeToRegister(device_addr, PIN_MODE_REGISTER_HIGH, pin_mode_register_state_high);
    PRINT("已将%x的设备设置为输入\r\n", device_addr);
}

void TCA_PinInvert(const uint8_t device_addr, const uint32_t pin, const uint32_t invert) {
    if (pin > 15)
        return;
    if (invert != 0 && invert != 1)
        return;

    const uint8_t mask = 1 << (pin % 8);

    const uint16_t memory_address = pin < 8 ? POLARITY_REGISTER_LOW : POLARITY_REGISTER_HIGH;
    uint8_t *data = pin < 8 ? &polarity_register_state_low : &polarity_register_state_high;

    if (invert)
        *data |= mask;
    else
        *data &= ~mask;

    writeToRegister(device_addr, memory_address, *data);
}

void TCA_WritePin(const uint8_t device_addr, const uint32_t pin, const uint32_t value) {
    if (pin > 15)
        return;
    if (value != 0 && value != 1)
        return;

    const uint8_t mask = 1 << (pin % 8);

    const uint16_t memory_address = pin < 8 ? OUTPUT_REGISTER_LOW : OUTPUT_REGISTER_HIGH;
    uint8_t *data = pin < 8 ? &output_register_state_low : &output_register_state_high;

    if (value)
        *data |= mask;
    else
        *data &= ~mask;

    writeToRegister(device_addr, memory_address, *data);
}

void TCA_TogglePin(const uint8_t device_addr, const uint32_t pin) {
    if (pin > 15)
        return;

    const uint8_t mask = 1 << (pin % 8);

    const uint16_t memory_address = pin < 8 ? OUTPUT_REGISTER_LOW : OUTPUT_REGISTER_HIGH;
    uint8_t *data = pin < 8 ? &output_register_state_low : &output_register_state_high;

    *data ^= mask;

    writeToRegister(device_addr, memory_address, *data);
}

int8_t TCA_ReadPin(const uint8_t device_addr, const uint32_t pin) {
    if (pin > 15)
        return -1;

    const uint8_t mask = 1 << (pin % 8);

    const uint16_t memory_address = pin < 8 ? INPUT_REGISTER_LOW : INPUT_REGISTER_HIGH;

    uint8_t result;
    if (I2C_ReadOneByte_TimeOut((device_addr << 1) + 1, memory_address, &result, 1024) != I2C_OK) {
        printf("读取%c地址的TCA9555失败\r\n", device_addr);
        return -1;
    }
    return (result & mask) != 0;
}

uint16_t TCA_ReadAllPins(const uint8_t device_addr) {
    uint8_t low_byte, high_byte;

    // 读取低8位寄存器 (pins 0-7)
    if (I2C_ReadOneByte_TimeOut((device_addr << 1) + 1, INPUT_REGISTER_LOW, &low_byte, 1024) != I2C_OK) {
        printf("读取%c地址的TCA9555失败\r\n", device_addr);
        return -1;
    }

    // 读取高8位寄存器 (pins 8-15)
    if (I2C_ReadOneByte_TimeOut((device_addr << 1) + 1, INPUT_REGISTER_HIGH, &high_byte, 1024) != I2C_OK) {
        printf("读取%c地址的TCA9555失败\r\n", device_addr);
        return -1;
    }

    // 组合低8位和高8位成为16位结果
    uint16_t result = (high_byte << 8) | low_byte;

    return result;
}