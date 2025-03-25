/*
 * mesh_test.c - 接收串口数据并发送到mesh网络
 *
 * 功能：
 * 1. 接收UART1串口数据
 * 2. 将数据编码处理
 * 3. 通过mesh网络发送数据
 * 4. 支持"MAC"指令，发送设备MAC地址
 * 5. 可选debug功能，回传验证数据
 */

#include "mesh_uart_test.h"

#include <app_mesh.h>

// 功能配置宏（用户可定义）
// #define ENABLE_MESH_UART_TEST   // 启用mesh测试功能
// #define ENABLE_MESH_UART_DEBUG  // 启用调试模式，回传验证数据

// 全局变量
uint8_t RxBuff[256];     // 接收UART数据的缓冲区
uint8_t trigB;           // UART触发字节计数

#ifdef ENABLE_MESH_UART_TEST
/**
 * @brief 处理接收到的UART数据并发送到mesh网络
 *
 * @param data 接收到的数据指针
 * @param len 数据长度
 */
void ProcessUartData(uint8_t *data, uint8_t len)
{
    // 确保至少有3字节：地址(2字节) + 数据类型(1字节)
    if (len < 3) {
        return;
    }

    // 解析地址(uint16_t)：前两个字节，小端模式
    const uint16_t addr = (uint16_t)data[0] | ((uint16_t)data[1] << 8);

    // 解析数据类型(uint8_t)：第三个字节
    const uint8_t dataType = data[2];

    // 解析数据内容：第4个字节开始
    char sendBuf[256] = {0};

    // 复制数据内容到发送缓冲区
    memcpy(sendBuf, data + 3, len - 3);
    sendBuf[len - 3] = '\0';

    // 检查是否包含"MAC"字符串并替换
    const char *macPos = strstr(sendBuf, "MAC");
    if (macPos != NULL) {
        // 计算要替换的位置和偏移量
        const uint8_t offset = macPos - sendBuf;
        const uint8_t remainLen = strlen(macPos + 3); // "MAC"后面的内容

        // 构建新的字符串：MAC前 + mac[6] + MAC后
        char tempBuf[256] = {0};

        // 复制"MAC"前面的内容
        memcpy(tempBuf, sendBuf, offset);

        // 复制mac[6]到相应位置
        memcpy(tempBuf + offset, MACAddr, 6);

        // 复制"MAC"后面的内容
        if (remainLen > 0) {
            memcpy(tempBuf + offset + 6, macPos + 3, remainLen);
        }

        // 设置正确的结束符
        tempBuf[offset + 6 + remainLen] = '\0';

        // 将替换后的内容发送到mesh网络
        SendData(addr, (DATATYPE)dataType, tempBuf);

#ifdef ENABLE_MESH_UART_DEBUG
        // 回传验证数据
        APP_DBG("测试接口发送数据 0x%04X地址: Type=%d, Data=%s", addr, dataType, sendBuf);
#endif
    } else {
        // 直接发送原始数据
        SendData(addr, (DATATYPE)dataType, sendBuf);

#ifdef ENABLE_MESH_UART_DEBUG
        // 回传验证数据
        APP_DBG("测试接口发送数据 0x%04X地址: Type=%d, Data=%s", addr, dataType, sendBuf);
#endif
    }
}

void InitMESHUartTest() {
    UART0_ByteTrigCfg(UART_7BYTE_TRIG);
    trigB = 7;
    UART0_INTCfg(ENABLE, RB_IER_RECV_RDY | RB_IER_LINE_STAT);
    PFIC_EnableIRQ(UART0_IRQn);
}

/**
 * @fn      UART0_IRQHandler
 *
 * @brief   UART0中断处理函数
 *
 * @return  none
 */
__INTERRUPT
__HIGH_CODE
void UART0_IRQHandler(void)
{
    volatile uint8_t i;
    uint8_t len = 0;

    switch(UART1_GetITFlag())
    {
        case UART_II_LINE_STAT: // 线路状态错误
        {
            UART1_GetLinSTA();
            break;
        }
        case UART_II_RECV_RDY: // 数据达到设置触发点
            for(i = 0; i != trigB; i++)
            {
                RxBuff[i] = UART1_RecvByte();
                UART1_SendByte(RxBuff[i]);
            }
            len = trigB;
            ProcessUartData(RxBuff, len);
            break;
        case UART_II_RECV_TOUT: // 接收超时，暂时一帧数据接收完成
            len = UART1_RecvString(RxBuff);
            UART1_SendString(RxBuff, len);
            ProcessUartData(RxBuff, len);
            break;
        case UART_II_THR_EMPTY: // 发送缓存区空，可继续发送
            break;
        case UART_II_MODEM_CHG: // 只支持串口0
            break;
        default:
            break;
    }
}
#endif // ENABLE_MESH_UART_TEST