//
// Created by xtx on 25-4-24.
//

#include <stdio.h>

#include "app_mesh_config.h"
#include "data_transfer.h"

void Message_C301(const uint16_t group_addr, const DATATYPE dataType, char *recvData) {

}

/// 接收成功回调
/// @param addr 来源地址
/// @param group_addr 来源组地址
/// @param dataType
/// @param recvData
void RecvMessage(const uint16_t addr, const uint16_t group_addr, const DATATYPE dataType, char *recvData) {
    APP_DBG("Received from 0x%04X, group 0x%04X: Type=%d, Data=%s", addr, group_addr, dataType, recvData);

    switch (group_addr) {
        case 0xC301:
            Message_C301(group_addr, dataType, recvData);
            break;
        default:
            break;
    }
}
