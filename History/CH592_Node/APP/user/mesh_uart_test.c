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
#include <stdlib.h>

#ifdef ENABLE_MESH_UART_TEST
// 功能配置宏（用户可定义）
// #define ENABLE_MESH_UART_TEST   // 启用mesh测试功能
// #define ENABLE_MESH_UART_DEBUG  // 启用调试模式，回传验证数据

// 全局变量
uint8_t RxBuff[256]; // 接收UART数据的缓冲区
uint8_t trigB; // UART触发字节计数

char mesh_uart_sendBuf[128] = {0};
uint16_t mesh_uart_addr = 0;
uint8_t mesh_uart_dataType = 0;
uint8_t mesh_uart_isWorking = 0;
/**
 * @brief 处理接收到的UART数据并发送到mesh网络
 *
 * @param data 接收到的数据指针
 * @param len 数据长度
 */
void ProcessUartData(const uint8_t *data, uint8_t len) {
    // 确保数据长度足够
    if (len < 5) {
        // 至少需要 "A 1 x" 格式
        return;
    }

    // 将数据转为字符串并确保以null结尾
    char dataBuf[128] = {0};
    if (len >= sizeof(dataBuf)) {
        len = sizeof(dataBuf) - 1; // 防止缓冲区溢出
    }
    memcpy(dataBuf, data, len);
    dataBuf[len] = '\0';

    char *addrEnd = NULL;
    char *typeEnd = NULL;
    char *dataStart = NULL;

    // 查找第一个空格位置 (addr和type之间)
    addrEnd = strchr(dataBuf, ' ');
    if (addrEnd == NULL) return;

    // 查找第二个空格位置 (type和data之间)
    typeEnd = strchr(addrEnd + 1, ' ');
    if (typeEnd == NULL) return;

    // 数据起始位置
    dataStart = typeEnd + 1;

    // 提取地址字符串
    size_t addrLen = addrEnd - dataBuf;
    char addrStr[5] = {0};
    if (addrLen > 4) addrLen = 4; // 最多4个字符
    memcpy(addrStr, dataBuf, addrLen);
    addrStr[addrLen] = '\0';

    // 提取数据类型字符串
    size_t typeLen = typeEnd - (addrEnd + 1);
    char typeStr[4] = {0}; // 足够存放3位数的类型值
    if (typeLen > 3) typeLen = 3;
    memcpy(typeStr, addrEnd + 1, typeLen);
    typeStr[typeLen] = '\0';

    // 提取数据内容
    char sendBuf[128] = {0};
    size_t dataLen = strlen(dataStart);
    memcpy(sendBuf, dataStart, dataLen);
    sendBuf[dataLen] = '\0';

    memset(mesh_uart_sendBuf, 0, sizeof(mesh_uart_sendBuf));
    // 将十六进制地址字符串转换为uint16_t
    mesh_uart_addr = (uint16_t) strtol(addrStr, NULL, 16);

    // 将数据类型字符串转换为uint8_t
    mesh_uart_dataType = (uint8_t) atoi(typeStr);

    // 检查是否包含"MAC"字符串并替换
    const char *macPos = strstr(sendBuf, "MAC");
    if (macPos != NULL) {
        // 计算要替换的位置和偏移量
        const uint8_t offset = macPos - sendBuf;
        const uint8_t remainLen = strlen(macPos + 3); // "MAC"后面的内容
        // 构建新的字符串：MAC前 + mac[6] + MAC后

        // 复制"MAC"前面的内容
        memcpy(mesh_uart_sendBuf, sendBuf, offset);
        // 复制mac[6]到相应位置
        memcpy(mesh_uart_sendBuf + offset, MACAddr, 6);
        // 复制"MAC"后面的内容
        if (remainLen > 0) {
            memcpy(mesh_uart_sendBuf + offset + 6, macPos + 3, remainLen);
        }
        // 设置正确的结束符
        mesh_uart_sendBuf[offset + 6 + remainLen] = '\0';
        mesh_uart_isWorking = 1;
    } else {
        strncpy(mesh_uart_sendBuf, sendBuf, strlen(sendBuf));
        mesh_uart_isWorking = 1;
    }
}

void InitMESHUartTest() {
    UART1_ByteTrigCfg(UART_7BYTE_TRIG);
    trigB = 7;
    UART1_INTCfg(ENABLE, RB_IER_RECV_RDY | RB_IER_LINE_STAT);
    PFIC_EnableIRQ(UART1_IRQn);
}
volatile uint8_t recCNT = 0;
/**
 * @fn      UART0_IRQHandler
 *
 * @brief   UART0中断处理函数
 *
 * @return  none
 */
__INTERRUPT
__HIGH_CODE
void UART1_IRQHandler(void) {
    volatile uint8_t i;
    uint8_t len = 0;
    uint8_t tmp[10] = {0};
    switch (UART1_GetITFlag()) {
        case UART_II_LINE_STAT: // 线路状态错误
        {
            UART1_GetLinSTA();
            break;
        }
        case UART_II_RECV_RDY: // 数据达到设置触发点
            for (i = 0; i != trigB; i++) {
                RxBuff[recCNT + i] = UART1_RecvByte();
                // UART1_SendByte(RxBuff[i]);
            }
            recCNT += trigB;
            break;
        case UART_II_RECV_TOUT: // 接收超时，暂时一帧数据接收完成

            len = UART1_RecvString(tmp);
            strncpy((char *) RxBuff + recCNT, tmp, len);
            recCNT += len;
            UART1_SendString(RxBuff, recCNT);
            ProcessUartData(RxBuff, recCNT);
            memset(RxBuff, 0, sizeof(RxBuff));
            recCNT = 0;
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
