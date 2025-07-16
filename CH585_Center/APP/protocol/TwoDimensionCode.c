#include "CH58x_common.h"
#include "data_transfer.h"
//
// Created by xtx on 25-7-15.
//
// 全局变量
uint8_t RxBuff[256]; // 接收UART数据的缓冲区
uint8_t trigB; // UART触发字节计数
char GetCID[16]={0};
bool isGetCID=false;
void ProcessUartData(const uint8_t *data) {
    char *pc_start = strstr((char*)RxBuff, "pc:");
    if (pc_start == NULL) {
        return; // 未找到pc字段
    }

    // 跳过"pc:"
    pc_start += 3;

    // 查找下一个逗号或结束符
    char *pc_end = strchr(pc_start, ',');
    if (pc_end == NULL) {
        pc_end = strchr(pc_start, '}');
        if (pc_end == NULL) {
            return; // 格式错误
        }
    }

    // 计算长度
    int len = pc_end - pc_start;

    // 复制字符串
    strncpy(GetCID, pc_start, len);
    printf("解析到CID: %s\r\n", GetCID);
    isGetCID=true;
}
void InitTwoDimensionCode() {
    GPIOA_SetBits(GPIO_Pin_5);
    GPIOA_ModeCfg(GPIO_Pin_4, GPIO_ModeIN_PU);
    GPIOA_ModeCfg(GPIO_Pin_5, GPIO_ModeOut_PP_5mA);
    UART3_DefInit();
    UART3_ByteTrigCfg(UART_7BYTE_TRIG);
    trigB = 7;
    UART3_INTCfg(ENABLE, RB_IER_RECV_RDY | RB_IER_LINE_STAT);
    PFIC_EnableIRQ(UART3_IRQn);
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
void UART3_IRQHandler(void) {
    volatile uint8_t i;
    uint8_t len = 0;
    uint8_t tmp[10] = {0};
    switch (UART3_GetITFlag()) {
        case UART_II_LINE_STAT: // 线路状态错误
        {
            UART3_GetLinSTA();
            break;
        }
        case UART_II_RECV_RDY: // 数据达到设置触发点
            for (i = 0; i != trigB; i++) {
                RxBuff[recCNT + i] = UART3_RecvByte();
            }
            recCNT += trigB;
            break;
        case UART_II_RECV_TOUT: // 接收超时，暂时一帧数据接收完成

            len = UART3_RecvString(tmp);
            strncpy((char *) RxBuff + recCNT, tmp, len);
            recCNT += len;
            printf("%s\r\n", (char *) RxBuff); // 打印接收到的数据
            ProcessUartData(RxBuff);
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
