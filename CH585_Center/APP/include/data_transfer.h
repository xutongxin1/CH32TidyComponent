/* data_transfer.h */
#ifndef DATA_TRANSFER_H
#define DATA_TRANSFER_H

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#define MAX_DATA_LEN     128
#define MAX_PENDING      20
#define TIMEOUT_MS       2000
#define MAX_RETRIES      1

typedef enum {
    ACK_SUCCESS = 0,
    ACK_CRC_FAIL = 1,
    USER_DATA_TYPE = 10
} DATATYPE;

typedef struct {
    uint16_t crc;
    uint16_t addr;
    DATATYPE dataType;
    uint8_t data[MAX_DATA_LEN];
    uint16_t dataLen;
    uint32_t sendTimeCounter;
    uint8_t retries;
} PendingPacket;

typedef void (*RecTrueDataCallback)(uint16_t addr, uint16_t group_addr,DATATYPE dataType, char* recvData);
typedef void (*SendErrorCallback)(uint16_t addr, DATATYPE dataType, char* sendData);

void SendData(uint16_t addr, DATATYPE dataType, const char* sendData);
void InitDataTransfer(RecTrueDataCallback recvCb, SendErrorCallback errCb);
void CheckPendingPackets(void);
void HandleReceivedData(uint16_t addr, uint16_t group_addr, const uint8_t *pdata, uint16_t len);
void RecvHandler(uint16_t addr, uint16_t group_addr,DATATYPE dataType, char* recvData);
void ErrorHandler(uint16_t addr, DATATYPE dataType, char* sendData);
uint16_t BingSubAddr(uint16_t net_idx, uint16_t addr);
extern int vendor_model_cli_send(uint16_t addr, uint8_t *pData, uint16_t len);

#endif