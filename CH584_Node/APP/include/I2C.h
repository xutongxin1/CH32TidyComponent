//
// Created by xtx on 25-2-27.
//

#ifndef I2C_H
#define I2C_H
#include <CH58x_common.h>
#include "core_riscv.h"
#include <MESH_LIB.h>
#include <stdbool.h>

// #define I2C_DEBUG
#ifdef  I2C_DEBUG
#define I2C_PRINT(X...) printf(X)
#else
#define I2C_PRINT(X...)
#endif

void CH58X_I2C_Init(void);

uint8_t I2C_ReadOneByte(const uint8_t device_addr, const u16 mem_addr);
int I2C_ReadOneByte_TimeOut(const uint8_t device_addr, const u16 mem_addr, uint8_t *result, const uint32_t timeout);

void I2C_WriteOneByte(const uint8_t device_addr, const u16 mem_addr, const uint8_t data);
int I2C_WriteOneByte_TimeOut(const uint8_t device_addr, const u16 mem_addr,const uint8_t data,
                             const uint32_t timeout);

void I2C_Read(const uint8_t device_addr, u16 memory_addr, u8 *data, u16 len);

void I2C_Write(const uint8_t device_addr, u16 memory_addr, const u8 *data, u16 len);

bool I2C_CheckDeviceExists(const uint8_t addr);

enum {
    I2C_OK = 0,
    I2C_ERROR = 1
};
#endif //I2C_H
