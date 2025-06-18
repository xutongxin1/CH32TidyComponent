#include <stdbool.h>
#include <string.h>

#include "mesh_uart_test.h"
#include "USB_CDC.h"
#include "WS2812.h"
//
// Created by xtx on 25-6-5.
//
bool isMeshUart = false;
bool isFirstRecv = true;
void CDC_RecvAnalyze(uint8_t *data, const uint8_t len) {
    if (len == 4) {
        //处理心跳包
        if (strncmp("AT\r\n", (char *) data, 4) == 0) {
            if (isFirstRecv) {
                isFirstRecv = false;
                ws2812_set_color(0, 101,110,1); // 设置第一个LED为红色
                ws2812_set_all_mode(LED_MODE_FLASH_FAST_1);
            }
            SendUSBData("OK!\r\n", 5);
            return;
        }
    }
    if (len == 12) {
        //先处理特殊数据的情况
        if (strncmp("MESH_WRITE\r\n", (char *) data, 12) == 0) {
            SendUSBData("MESH_WRITE_OK!\r\n", 16);
            isMeshUart = true;
            ws2812_set_color(0, 101,110,1); // 设置第一个LED为红色
            ws2812_set_all_mode(LED_MODE_BREATHE_FAST);
            return;
        }
        if (strncmp("MESH_CLOSE\r\n", (char *) data, 12) == 0) {
            SendUSBData("MESH_CLOSE_OK!\r\n", 16);
            isMeshUart = false;
            ws2812_set_color(0, 101,110,1); // 设置第一个LED为红色
            ws2812_set_all_mode(LED_MODE_FLASH_FAST_1);
            return;
        }
    }
    if (isMeshUart == true) {
        ProcessUartData(data, len);
        return;
    }
    // 处理其他数据
    return;
}
