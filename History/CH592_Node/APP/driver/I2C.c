#include "I2C.h"

/*********************************************************************
 * @fn      AT24CXX_Init
 *
 * @brief   Initializes AT24xx EEPROM.
 *
 * @return  none
 */
void CH59X_I2C_Init(void) {
    GPIOB_ModeCfg(GPIO_Pin_14 | GPIO_Pin_15, GPIO_ModeIN_PU);

    I2C_Init(I2C_Mode_I2C, 400000, I2C_DutyCycle_16_9, I2C_Ack_Enable, I2C_AckAddr_7bit, 0X00);
    while (I2C_GetFlagStatus(I2C_FLAG_BUSY) != RESET);
}

/*********************************************************************
 * @fn      AT24CXX_ReadOneByte
 *
 * @brief   Read one data from EEPROM.
 *
 * @param device_addr - Device address.
 * @param   mem_addr - Read frist address.
 *
 * @return  temp - Read data.
 */
uint8_t I2C_ReadOneByte(const uint8_t device_addr, const u16 mem_addr) {
    u8 temp = 0;
    I2C_PRINT("1\r\n");
    while (I2C_GetFlagStatus(I2C_FLAG_BUSY) != RESET);
    I2C_GenerateSTART(ENABLE);
    I2C_PRINT("2\r\n");
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(device_addr, I2C_Direction_Transmitter);
    I2C_PRINT("3\r\n");
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    I2C_PRINT("4\r\n");
#if (Address_Lenth  == Address_8bit)
    I2C_SendData((u8) (mem_addr & 0x00FF));
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED));

#elif (Address_Lenth  == Address_16bit)
	I2C_SendData(   (u8)(ReadAddr>>8) );
	while( !I2C_CheckEvent(   I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );

	I2C_SendData(   (u8)(ReadAddr&0x00FF) );
	while( !I2C_CheckEvent(   I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );

#endif
    I2C_PRINT("5\r\n");
    I2C_GenerateSTART(ENABLE);
    I2C_PRINT("6\r\n");
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(device_addr, I2C_Direction_Receiver);
    I2C_PRINT("7\r\n");
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
    while (I2C_GetFlagStatus(I2C_FLAG_RXNE) == RESET)
        I2C_AcknowledgeConfig(DISABLE);
    I2C_PRINT("8\r\n");
    temp = I2C_ReceiveData();
    I2C_GenerateSTOP(ENABLE);
    I2C_PRINT("9\r\n");
    return temp;
}
int I2C_ReadOneByte_TimeOut(const uint8_t device_addr, const u16 mem_addr, uint8_t *result, const uint32_t timeout) {
    uint32_t tryTimes = 0;
    I2C_PRINT("1\r\n");
    while (I2C_GetFlagStatus(I2C_FLAG_BUSY) != RESET) {
        if (tryTimes++ > timeout) {
            return I2C_ERROR;
        }
    }
    I2C_GenerateSTART(ENABLE);
    I2C_PRINT("2\r\n");
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT)) {
        if (tryTimes++ > timeout) {
            return I2C_ERROR;
        }
    }
    I2C_Send7bitAddress(device_addr, I2C_Direction_Transmitter);
    I2C_PRINT("3\r\n");
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
        if (tryTimes++ > timeout) {
            return I2C_ERROR;
        }
    }
    I2C_PRINT("4\r\n");
#if (Address_Lenth  == Address_8bit)
    I2C_SendData((u8) (mem_addr & 0x00FF));
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
        if (tryTimes++ > timeout) {
            return I2C_ERROR;
        }
    }

#elif (Address_Lenth  == Address_16bit)
    I2C_SendData(   (u8)(ReadAddr>>8) );
    while( !I2C_CheckEvent(   I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );

    I2C_SendData(   (u8)(ReadAddr&0x00FF) );
    while( !I2C_CheckEvent(   I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );

#endif
    I2C_PRINT("5\r\n");
    I2C_GenerateSTART(ENABLE);
    I2C_PRINT("6\r\n");
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT)) {
        if (tryTimes++ > timeout) {
            return I2C_ERROR;
        }
    }
    I2C_Send7bitAddress(device_addr, I2C_Direction_Receiver);
    I2C_PRINT("7\r\n");
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) {
        if (tryTimes++ > timeout) {
            return I2C_ERROR;
        }
    }
    while (I2C_GetFlagStatus(I2C_FLAG_RXNE) == RESET) {
        I2C_AcknowledgeConfig(DISABLE);
        if (tryTimes++ > timeout) {
            return I2C_ERROR;
        }
    }
    I2C_PRINT("8\r\n");
    *result = I2C_ReceiveData();
    I2C_GenerateSTOP(ENABLE);
    I2C_PRINT("9\r\n");
    return I2C_OK;
}

/*********************************************************************
 * @fn      AT24CXX_WriteOneByte
 *
 * @brief   Write one data to EEPROM.
 *
 * @param device_addr - Device address.
 * @param   mem_addr - Write frist address.
 * @param data - Write data.
 *
 * @return  DataToWrite - Write data.
 */
void I2C_WriteOneByte(const uint8_t device_addr, const u16 mem_addr, const uint8_t data) {
    while (I2C_GetFlagStatus(I2C_FLAG_BUSY) != RESET) {
    }
    I2C_PRINT("1\r\n");
    I2C_GenerateSTART(ENABLE);
    I2C_PRINT("2\r\n");
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(device_addr, I2C_Direction_Transmitter);
    I2C_PRINT("3\r\n");
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    I2C_PRINT("4\r\n");
#if (Address_Lenth  == Address_8bit)
    I2C_SendData((u8) (mem_addr & 0x00FF));
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED));

#elif (Address_Lenth  == Address_16bit)
	I2C_SendData(   (u8)(WriteAddr>>8) );
	while( !I2C_CheckEvent(   I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );

	I2C_SendData(   (u8)(WriteAddr&0x00FF) );
	while( !I2C_CheckEvent(   I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );

#endif
    I2C_PRINT("5\r\n");
    if (I2C_GetFlagStatus(I2C_FLAG_TXE) != RESET) {
        I2C_SendData(data);
    }
    I2C_PRINT("6\r\n");
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    I2C_GenerateSTOP(ENABLE);
    I2C_PRINT("7\r\n");
}

int I2C_WriteOneByte_TimeOut(const uint8_t device_addr, const u16 mem_addr, const uint8_t data,
                             const uint32_t timeout) {
    uint32_t tryTimes = 0;
    while (I2C_GetFlagStatus(I2C_FLAG_BUSY) != RESET) {
        if (tryTimes++ > timeout) {
            return I2C_ERROR;
        }
    }
    I2C_PRINT("1\r\n");
    I2C_GenerateSTART(ENABLE);
    I2C_PRINT("2\r\n");
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT)) {
        if (tryTimes++ > timeout) {
            return I2C_ERROR;
        }
    }
    I2C_Send7bitAddress(device_addr, I2C_Direction_Transmitter);
    I2C_PRINT("3\r\n");
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
        if (tryTimes++ > timeout) {
            return I2C_ERROR;
        }
    }
    I2C_PRINT("4\r\n");
#if (Address_Lenth  == Address_8bit)
    I2C_SendData((u8) (mem_addr & 0x00FF));
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
        if (tryTimes++ > timeout) {
            return I2C_ERROR;
        }
    }

#elif (Address_Lenth  == Address_16bit)
    I2C_SendData(   (u8)(WriteAddr>>8) );
    while( !I2C_CheckEvent(   I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );

    I2C_SendData(   (u8)(WriteAddr&0x00FF) );
    while( !I2C_CheckEvent(   I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );

#endif
    I2C_PRINT("5\r\n");
    if (I2C_GetFlagStatus(I2C_FLAG_TXE) != RESET) {
        I2C_SendData(data);
    }
    I2C_PRINT("6\r\n");
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
        if (tryTimes++ > timeout) {
            return I2C_ERROR;
        }
    }
    I2C_GenerateSTOP(ENABLE);
    I2C_PRINT("7\r\n");
    return I2C_OK;
}

/*********************************************************************
 * @fn      AT24CXX_Read
 *
 * @brief   Read multiple data from EEPROM.
 *
 * @param device_addr - Device address.
 * @param   memory_addr - Read address.
 * @param data read data.
 * @param len data length.
 *
 * @return  none
 */
void I2C_Read(const uint8_t device_addr, u16 memory_addr, u8 *data, u16 len) {
    while (len) {
        *data++ = I2C_ReadOneByte(device_addr, memory_addr++);
        len--;
    }
}

/*********************************************************************
 * @fn      AT24CXX_Write
 *
 * @brief   Write multiple data to EEPROM.
 *
 * @param device_addr - Device address.
 * @param   memory_addr - Write address.
 * @param data - Data buffer.
 * @param len - Data length.
 *
 * @return  none
 */
void I2C_Write(const uint8_t device_addr, u16 memory_addr, const u8 *data, u16 len) {
    while (len--) {
        I2C_WriteOneByte(device_addr, memory_addr, *data);
        memory_addr++;
        data++;
        DelayMs(2);
    }
}
