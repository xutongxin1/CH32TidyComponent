/* data_transfer.c */
#include "data_transfer.h"

#include <app_mesh_config.h>
#include <stdio.h>

#include "USB_CDC.h"

static PendingPacket pendingList[MAX_PENDING];
static uint8_t pendingCount = 0;
static RecTrueDataCallback userRecvCb = NULL;
static SendErrorCallback userErrorCb = NULL;
static uint8_t timeCounter = 0;

/* CRC16-CCITT实现 */
static uint16_t crc16(const uint8_t *data, uint16_t len) {
    uint16_t crc = 0xFFFF;
    while (len--) {
        crc ^= (uint16_t) (*data++) << 8;
        for (uint8_t i = 0; i < 8; i++) {
            crc = crc & 0x8000 ? (crc << 1) ^ 0x1021 : crc << 1;
        }
    }
    return crc;
}

void InitDataTransfer(RecTrueDataCallback recvCb, SendErrorCallback errCb) {
    userRecvCb = recvCb;
    userErrorCb = errCb;
}

void SendData(const uint16_t addr, const DATATYPE dataType, const char *sendData) {
    if (dataType < USER_DATA_TYPE || pendingCount >= MAX_PENDING) return;

    /* 构造数据包 */
    uint16_t dataLen = strlen(sendData);
    uint8_t packet[1 + MAX_DATA_LEN + 2] = {0};

    packet[0] = (uint8_t) dataType;
    memcpy(packet + 1, sendData, dataLen);

    uint16_t crc = crc16(packet, 1 + dataLen);
    packet[1 + dataLen] = crc >> 8;
    packet[1 + dataLen + 1] = crc & 0xFF;

    vendor_model_srv_send(addr, packet, 1 + dataLen + 2);

    /* 记录发送状态 */
    PendingPacket *p = &pendingList[pendingCount++];
    p->crc = crc;
    p->addr = addr;
    p->dataType = dataType;
    p->dataLen = dataLen;
    p->sendTimeCounter = timeCounter;
    p->retries = 0;
    memcpy(p->data, sendData, dataLen);
}

void HandleReceivedData(const uint16_t addr, const uint16_t group_addr, const uint8_t *pdata, uint16_t len) {
    if (len < 3) return;

    DATATYPE rxType = pdata[0];

    /* 处理ACK */
    if (rxType <= ACK_CRC_FAIL) {
        uint16_t ackCrc = (pdata[1] << 8) | pdata[2];

        for (int i = 0; i < pendingCount; i++) {
            if (pendingList[i].crc == ackCrc) {
                if (rxType != ACK_SUCCESS && userErrorCb) {
                    char buf[MAX_DATA_LEN + 1];
                    memcpy(buf, pendingList[i].data, pendingList[i].dataLen);
                    buf[pendingList[i].dataLen] = '\0';
                    userErrorCb(pendingList[i].addr, pendingList[i].dataType, buf);
                }

                /* 移除条目 */
                memmove(&pendingList[i], &pendingList[i + 1],
                        (pendingCount - i - 1) * sizeof(PendingPacket));
                pendingCount--;
                break;
            }
        }
    }
    /* 处理数据 */
    else {
        uint16_t calcCrc = crc16(pdata, len - 2);
        uint16_t recvCrc = (pdata[len - 2] << 8) | pdata[len - 1];

        /* 发送ACK */
        const uint8_t ackType = (calcCrc == recvCrc) ? ACK_SUCCESS : ACK_CRC_FAIL;
        bool isAnswer = false;
        /* 传递有效数据,并根据bool返回值确定是否需要应答 */
        if (ackType == ACK_SUCCESS) {
            char buf[MAX_DATA_LEN + 1];
            uint16_t payloadLen = len - 3;
            memcpy(buf, pdata + 1, payloadLen);
            buf[payloadLen] = '\0';
            isAnswer = userRecvCb(addr, group_addr, rxType, buf);
        } else {
            isAnswer = false; //认为CRC错误都不需要应答
        }
        //只应答属于自己的包
        if (isAnswer) {
            uint8_t ackPacket[3] = {ackType, pdata[len - 2], pdata[len - 1]};
            vendor_model_srv_send(addr, ackPacket, sizeof(ackPacket));
        }
    }
}

void CheckPendingPackets(void) {
    /* 更新时间计数器 */
    timeCounter++; // 自动循环在0-255之间

    /* 检查所有待处理包 */
    for (int i = 0; i < pendingCount;) {
        PendingPacket *p = &pendingList[i];

        /* 计算时间差（考虑无符号溢出） */
        uint8_t timeDiff = timeCounter - p->sendTimeCounter;

        if (timeDiff >= 20) {
            // 20 * 100ms = 2s
            if (p->retries < MAX_RETRIES) {
                /* 重传数据 */
                uint8_t packet[1 + MAX_DATA_LEN + 2];
                packet[0] = p->dataType;
                memcpy(packet + 1, p->data, p->dataLen);
                packet[1 + p->dataLen] = p->crc >> 8;
                packet[1 + p->dataLen + 1] = p->crc & 0xFF;

                vendor_model_srv_send(p->addr, packet, 1 + p->dataLen + 2);
                p->sendTimeCounter = timeCounter;
                p->retries++;
                i++;
            } else {
                /* 上报错误 */
                if (userErrorCb) {
                    char buf[MAX_DATA_LEN + 1];
                    memcpy(buf, p->data, p->dataLen);
                    buf[p->dataLen] = '\0';
                    userErrorCb(p->addr, p->dataType, buf);
                }

                memmove(&pendingList[i], &pendingList[i + 1],
                        (pendingCount - i - 1) * sizeof(PendingPacket));
                pendingCount--;
            }
        } else {
            i++;
        }
    }
}

// 接收成功回调
bool RecvHandler(const uint16_t addr, const uint16_t group_addr, const DATATYPE dataType, char *recvData) {
    // addr：来源地址（16位）
    // dataType：原始用户数据类型（>=10）
    // recvData：保证以'\0'结尾的字符串
    APP_DBG("Received from 0x%04X, group 0x%04X: Type=%d, Data=%s", addr, group_addr, dataType, recvData);
    //用户侧就地处理
    char tmp[100] = {0};
    switch (group_addr) {
        case 0xC000:
            return true;
        case 0xC302:
            sprintf(tmp, "0xC302 %d %s\r\n", dataType, recvData);
            SendUSBData(tmp, strlen(tmp));
            return true;
        default:
            return false;
    }
}

// 发送失败回调
void ErrorHandler(const uint16_t addr, const DATATYPE dataType, char *sendData) {
    // addr：目标地址
    // dataType：原始用户数据类型
    // sendData：原始发送数据内容
    APP_DBG("Failed to send to 0x%04X: Type=%d, Data=%s", addr, dataType, sendData);
}
