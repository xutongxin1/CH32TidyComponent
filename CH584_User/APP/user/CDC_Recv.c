#include <stdbool.h>
#include <string.h>

#include "mesh_uart_test.h"
#include "USB_CDC.h"
//
// Created by xtx on 25-6-5.
//
bool isMeshUart = false;
void CDC_RecvAnalyze(uint8_t *data, const uint8_t len) {
    if (len == 4) {
        //处理心跳包
        if (strncpy("AT\r\n", (char *) data, 4) == 0) {
            SendUSBData("OK!\r\n", 5);
            return;
        }
    }
    if (len == 12) {
        //先处理特殊数据的情况
        if (strncpy("MESH_WRITE\r\n", (char *) data, 12) == 0) {
            SendUSBData("MESH_WRITE_OK!\r\n", 16);
            isMeshUart = true;
            return;
        }
        if (strncpy("MESH_CLOSE\r\n", (char *) data, 12) == 0) {
            SendUSBData("MESH_CLOSE_OK!\r\n", 16);
            isMeshUart = false;
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
