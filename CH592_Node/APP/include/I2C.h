//
// Created by xtx on 25-2-27.
//

#ifndef I2C_H
#define I2C_H
#include <CH59x_common.h>
#include "core_riscv.h"
#include <MESH_LIB.h>

#ifdef  I2C_DEBUG
#define I2C_PRINT(X...) printf(X)
#else
#define I2C_PRINT(X...)
#endif

void CH59X_I2C_Init(void);

uint8_t I2C_ReadOneByte(const uint8_t device_addr, const u16 mem_addr);

void I2C_WriteOneByte(const uint8_t device_addr, const u16 mem_addr, const uint8_t data);

void I2C_Read(const uint8_t device_addr, u16 memory_addr, u8 *data, u16 len);

void I2C_Write(const uint8_t device_addr, u16 memory_addr, const u8 *data, u16 len);

#endif //I2C_H
