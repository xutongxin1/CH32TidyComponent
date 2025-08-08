//
// Created by xtx on 25-4-24.
//

#include <stdio.h>
#include <stdlib.h>

#include "app_mesh.h"
#include "app_mesh_config.h"
#include "data_transfer.h"
#include "led_manager.h"

bool Message_C301(const uint16_t group_addr, const DATATYPE dataType, char *recvData) {
    bool isAnswer = false;

    // 局部变量
    char macStr[18]; // MAC地址字符串
    char rgbStr[8]; // RGB字符串
    uint8_t parsedMAC[6]; // 解析后的MAC地址
    int nij = 0;
    int lightMode = 0;
    uint32_t color = 0;
    // 使用sscanf解析数据
    int result = sscanf(recvData, "%17s %d %7s %d",
                        macStr, &nij, rgbStr, &lightMode);

    if (result != 4) {
        printf("解析失败，只解析了 %d 个字段\n", result);
        return -1;
    }

    // 解析MAC地址：AA:BB:CC:DD:EE:FF -> 6个uint8_t
    int mac_parts[6];
    result = sscanf(macStr, "%02x:%02x:%02x:%02x:%02x:%02x",
                    &mac_parts[0], &mac_parts[1], &mac_parts[2],
                    &mac_parts[3], &mac_parts[4], &mac_parts[5]);

    if (result != 6) {
        printf("MAC地址解析失败\n");
        return false;
    }

    // 转换为uint8_t数组
    for (int i = 0; i < 6; i++) {
        parsedMAC[i] = (uint8_t) mac_parts[i];
    }

    // 与全局MAC数组比较
    int macMatch = 1;
    for (int i = 0; i < 6; i++) {
        if (parsedMAC[i] != MACAddr[i]) {
            macMatch = 0;
            break;
        }
    }

    if (!macMatch) {
        printf("MAC地址不匹配\n");
        printf("解析的MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
               parsedMAC[0], parsedMAC[1], parsedMAC[2],
               parsedMAC[3], parsedMAC[4], parsedMAC[5]);
        printf("期望的MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
               MACAddr[0], MACAddr[1], MACAddr[2], MACAddr[3], MACAddr[4], MACAddr[5]);
        return false;
    }

    // 解析RGB颜色：#RRGGBB -> uint32_t
    if (rgbStr[0] != '#' || strlen(rgbStr) != 7) {
        printf("RGB格式错误\n");
        return false;
    }

    // 跳过#号，解析十六进制颜色值
    char *endptr;
    color = (uint32_t) strtol(rgbStr + 1, &endptr, 16);

    if (*endptr != '\0') {
        printf("RGB颜色值解析失败\n");
        return false;
    }

    printf("解析成功！\r\n");
    printf("MAC匹配: 是\r\n");
    printf("nij: %d\r\n", nij);
    printf("颜色值: 0x%06X (R:%d, G:%d, B:%d)\r\n",
           color,
           (color >> 16) & 0xFF, // R
           (color >> 8) & 0xFF, // G
           color & 0xFF); // B
    printf("亮灯形式: %d\r\n", lightMode);

    const int led_index = (nij / 100 - 1) * 17 + ((nij % 100) / 10) * 5 + (nij % 10) - 4;
    printf("led_index: %d\r\n", led_index);
    printf("dataType: %d\r\n", dataType);
    switch (dataType) {
        case 10:
            return led_manager_turn_on(nij, led_index, 60, color, (led_mode_t) lightMode,false,false);
        case 11:
            return led_manager_turn_on(nij, led_index, 60, color, (led_mode_t) lightMode,false,true);
        case 12:
            return led_manager_turn_on(nij, led_index, 60, color, (led_mode_t) lightMode,true,true);
        case 20:
            return led_manager_turn_on(nij, led_index, 10, color, (led_mode_t) lightMode,false,false);
        case 21:
            return led_manager_turn_on(nij, led_index, 10, color, (led_mode_t) lightMode,false,true);
        case 22:
            return led_manager_turn_on(nij, led_index, 10, color, (led_mode_t) lightMode,true,true);
        case 30:
            return led_manager_turn_on(nij, led_index, 60, color, (led_mode_t) lightMode,false,false);
        default:
            return false;
    }
}

bool Message_C101(const uint16_t group_addr, const DATATYPE dataType, char *recvData) {
    bool isAnswer = false;

    // 局部变量
    char macStr[18]; // MAC地址字符串
    char rgbStr[8]; // RGB字符串
    uint8_t parsedMAC[6]; // 解析后的MAC地址
    int n = 0;
    int lightMode = 0;
    uint32_t color = 0;
    // 使用sscanf解析数据
    int result = sscanf(recvData, "%17s %d %7s %d",
                        macStr, &n, rgbStr, &lightMode);

    if (result != 4) {
        printf("解析失败，只解析了 %d 个字段\n", result);
        return -1;
    }

    // 解析MAC地址：AA:BB:CC:DD:EE:FF -> 6个uint8_t
    int mac_parts[6];
    result = sscanf(macStr, "%02x:%02x:%02x:%02x:%02x:%02x",
                    &mac_parts[0], &mac_parts[1], &mac_parts[2],
                    &mac_parts[3], &mac_parts[4], &mac_parts[5]);

    if (result != 6) {
        printf("MAC地址解析失败\n");
        return false;
    }

    // 转换为uint8_t数组
    for (int i = 0; i < 6; i++) {
        parsedMAC[i] = (uint8_t) mac_parts[i];
    }

    // 与全局MAC数组比较
    int macMatch = 1;
    for (int i = 0; i < 6; i++) {
        if (parsedMAC[i] != MACAddr[i]) {
            macMatch = 0;
            break;
        }
    }

    if (!macMatch) {
        printf("MAC地址不匹配\n");
        printf("解析的MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
               parsedMAC[0], parsedMAC[1], parsedMAC[2],
               parsedMAC[3], parsedMAC[4], parsedMAC[5]);
        printf("期望的MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
               MACAddr[0], MACAddr[1], MACAddr[2], MACAddr[3], MACAddr[4], MACAddr[5]);
        return false;
    }

    // 解析RGB颜色：#RRGGBB -> uint32_t
    if (rgbStr[0] != '#' || strlen(rgbStr) != 7) {
        printf("RGB格式错误\n");
        return false;
    }

    // 跳过#号，解析十六进制颜色值
    char *endptr;
    color = (uint32_t) strtol(rgbStr + 1, &endptr, 16);

    if (*endptr != '\0') {
        printf("RGB颜色值解析失败\n");
        return false;
    }

    printf("解析成功！\r\n");
    printf("MAC匹配: 是\r\n");
    printf("n: %d\r\n", n);
    printf("颜色值: 0x%06X (R:%d, G:%d, B:%d)\r\n",
           color,
           (color >> 16) & 0xFF, // R
           (color >> 8) & 0xFF, // G
           color & 0xFF); // B
    printf("亮灯形式: %d\r\n", lightMode);

    int row = n / 10; // 十位数字 (1,2,3,4)
    int col = n % 10; // 个位数字 (1,2)

    const int led_index = (row - 1) * 2 + col + 1;
    printf("led_index: %d\r\n", led_index);
    printf("dataType: %d\r\n", dataType);
    switch (dataType) {
        case 10:
            return led_manager_turn_on(n, led_index, 60, color, (led_mode_t) lightMode,false,false);
        case 11:
            return led_manager_turn_on(n, led_index, 60, color, (led_mode_t) lightMode,false,true);
        case 12:
            return led_manager_turn_on(n, led_index, 60, color, (led_mode_t) lightMode,true,true);
        case 20:
            return led_manager_turn_on(n, led_index, 10, color, (led_mode_t) lightMode,false,false);
        case 21:
            return led_manager_turn_on(n, led_index, 10, color, (led_mode_t) lightMode,false,true);
        case 22:
            return led_manager_turn_on(n, led_index, 10, color, (led_mode_t) lightMode,true,true);
        case 30:
            return led_manager_turn_on(n, led_index, 60, color, (led_mode_t) lightMode,false,false);
        default:
            return false;
    }
}

bool Message_C201(const uint16_t group_addr, const DATATYPE dataType, char *recvData) {
    bool isAnswer = false;

    // 局部变量
    char macStr[18]; // MAC地址字符串
    char rgbStr[8]; // RGB字符串
    uint8_t parsedMAC[6]; // 解析后的MAC地址
    int n = 0;
    int lightMode = 0;
    uint32_t color = 0;
    // 使用sscanf解析数据
    int result = sscanf(recvData, "%17s %d %7s %d",
                        macStr, &n, rgbStr, &lightMode);

    if (result != 4) {
        printf("解析失败，只解析了 %d 个字段\n", result);
        return -1;
    }

    // 解析MAC地址：AA:BB:CC:DD:EE:FF -> 6个uint8_t
    int mac_parts[6];
    result = sscanf(macStr, "%02x:%02x:%02x:%02x:%02x:%02x",
                    &mac_parts[0], &mac_parts[1], &mac_parts[2],
                    &mac_parts[3], &mac_parts[4], &mac_parts[5]);

    if (result != 6) {
        printf("MAC地址解析失败\n");
        return false;
    }

    // 转换为uint8_t数组
    for (int i = 0; i < 6; i++) {
        parsedMAC[i] = (uint8_t) mac_parts[i];
    }

    // 与全局MAC数组比较
    int macMatch = 1;
    for (int i = 0; i < 6; i++) {
        if (parsedMAC[i] != MACAddr[i]) {
            macMatch = 0;
            break;
        }
    }

    if (!macMatch) {
        printf("MAC地址不匹配\n");
        printf("解析的MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
               parsedMAC[0], parsedMAC[1], parsedMAC[2],
               parsedMAC[3], parsedMAC[4], parsedMAC[5]);
        printf("期望的MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
               MACAddr[0], MACAddr[1], MACAddr[2], MACAddr[3], MACAddr[4], MACAddr[5]);
        return false;
    }

    // 解析RGB颜色：#RRGGBB -> uint32_t
    if (rgbStr[0] != '#' || strlen(rgbStr) != 7) {
        printf("RGB格式错误\n");
        return false;
    }

    // 跳过#号，解析十六进制颜色值
    char *endptr;
    color = (uint32_t) strtol(rgbStr + 1, &endptr, 16);

    if (*endptr != '\0') {
        printf("RGB颜色值解析失败\n");
        return false;
    }

    printf("解析成功！\r\n");
    printf("MAC匹配: 是\r\n");
    printf("n: %d\r\n", n);
    printf("颜色值: 0x%06X (R:%d, G:%d, B:%d)\r\n",
           color,
           (color >> 16) & 0xFF, // R
           (color >> 8) & 0xFF, // G
           color & 0xFF); // B
    printf("亮灯形式: %d\r\n", lightMode);

    const int led_index = n + 1;
    printf("led_index: %d\r\n", led_index);
    printf("dataType: %d\r\n", dataType);
    switch (dataType) {
        case 10:
            return led_manager_turn_on(n, led_index, 60, color, (led_mode_t) lightMode,false,false);
        case 11:
            return led_manager_turn_on(n, led_index, 60, color, (led_mode_t) lightMode,false,true);
        case 12:
            return led_manager_turn_on(n, led_index, 60, color, (led_mode_t) lightMode,true,true);
        case 20:
            return led_manager_turn_on(n, led_index, 10, color, (led_mode_t) lightMode,false,false);
        case 21:
            return led_manager_turn_on(n, led_index, 10, color, (led_mode_t) lightMode,false,true);
        case 22:
            return led_manager_turn_on(n, led_index, 10, color, (led_mode_t) lightMode,true,true);
        case 30:
            return led_manager_turn_on(n, led_index, 60, color, (led_mode_t) lightMode,false,false);
        default:
            return false;
    }
}

/// 接收成功回调
/// @param addr 来源地址
/// @param group_addr 来源组地址
/// @param dataType
/// @param recvData
bool RecvMessage(const uint16_t addr, const uint16_t group_addr, const DATATYPE dataType, char *recvData) {
    APP_DBG("Received from 0x%04X, group 0x%04X: Type=%d, Data=%s", addr, group_addr, dataType, recvData);

    switch (group_addr) {
        case 0xC000:
            //测试地址，直接返回
            return true;
#ifdef DEVICE_TYPE_B53
        case 0xC301:
            //查找，包含了取出和放回的查找，完全依赖磁传感决定目前到底是取出还是放回
            return Message_C301(group_addr, dataType, recvData);
#elifdef DEVICE_TYPE_A42
        case 0xC101:
            return Message_C101(group_addr, dataType, recvData);
#elifdef DEVICE_TYPE_A21
        case 0xC201:
            return Message_C201(group_addr, dataType, recvData);
#endif

        default:
            return false; // 未处理的组地址
    }
}
