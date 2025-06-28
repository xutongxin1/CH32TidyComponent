//
// Created by xtx on 2025/4/17.
//
#include "ScanIO.h"

#include <stdlib.h>
#include <TCA9555.h>

#include "app_mesh.h"
#include "B53_driver.h"
#include "data_transfer.h"
#include "led_manager.h"
#include "WS2812.h"

// 用于处理引脚状态变化的处理函数
void handle_up(uint8 addr, uint8 pin); // 当引脚从低电平变为高电平时调用
void handle_down(uint8 addr, uint8 pin); // 当引脚从高电平变为低电平时调用

// 支持的最大TCA9555设备数量
#define MAX_TCA_DEVICES 8

// 存储每个TCA9555设备的前一次状态
static uint16_t previous_states[MAX_TCA_DEVICES];
// 跟踪设备是否已经初始化（用于第一次扫描）
static uint8_t device_initialized[MAX_TCA_DEVICES] = {0};

/// 扫描所有TCA9555设备的引脚状态
void ScanIO() {
    for (int i = 0; i < MAX_TCA_DEVICES; i++) {
        if (DeviceExists[i]) {
            Scan(0x20 + i);
        }
    }
}
/**
 * 扫描TCA9555设备的所有引脚并处理状态变化
 * @param addr TCA9555设备地址
 */
void Scan(const uint8_t addr) {
    // 确保地址在有效范围内
    const uint8 index = addr - 0x20;
    if (index > MAX_TCA_DEVICES) {
        printf("设备地址%d超出范围\r\n", addr);
        return;
    }

    // 获取此设备所有引脚的当前状态
    const uint16_t current_state = TCA_ReadAllPins(addr);
    // printf("current_state --> %d\r\n", current_state);
    // 如果读取失败，直接返回
    if (current_state < 0) {
        return; // TCA_ReadAllPins已经打印了错误信息
    }

    // 检查是否是第一次扫描此设备
    if (!device_initialized[index]) {
        // 使用当前状态初始化前一次状态（不报告变化）
        previous_states[index] = current_state;
        device_initialized[index] = 1;
        return;
    }

    // 获取前一次的状态
    const uint16_t prev_state = previous_states[index];

    // 计算哪些引脚的状态发生了变化（异或操作）
    const uint16_t changed_pins = prev_state ^ current_state;

    // 对于每个发生变化的引脚
    for (uint8_t pin = 0; pin < 16; pin++) {
        // 检查此引脚是否发生变化
        if (changed_pins & (1 << pin)) {
            // 检查引脚的新状态
            if (current_state & (1 << pin)) {
                // 引脚从高电平变为低电平
                handle_down(addr, pin);
            } else {
                // 引脚从低电平变为高电平
                handle_up(addr, pin);
            }
        }
    }

    // 更新前一次状态为当前状态，为下次扫描做准备
    previous_states[index] = current_state;
}

//放回
//此处仅处理物理意义上的放回
void handle_up(const uint8 addr, const uint8 pin) {
    const uint8 n = addr - 0x20 + 1;
    const uint8 i = pin / 5 + 1;
    const uint8 j = pin % 5 + 1;
    char tmp[30] = {0};
    const int led_index = n * 17 + i * 5 + j - 4;
    PRINT("放回了 addr:%d pin:%d，对应%d个B53的%i行%d个\r\n", addr, pin, n, i, j);

    sprintf(tmp, "%02X:%02X:%02X:%02X:%02X:%02X %d%d%d",
            MACAddr[0], MACAddr[1], MACAddr[2],
            MACAddr[3], MACAddr[4], MACAddr[5], n, i, j);
    SendData(0xC303, USER_DATA_TYPE, tmp);
    if (isDebugLED == true) {
        ws2812_set_led_hex(led_index, 0xAAAAAA, LED_MODE_STATIC);
    }
}

//取出
void handle_down(const uint8 addr, const uint8 pin) {
    const uint8 n = addr - 0x20 + 1;
    const uint8 i = pin / 5 + 1;
    const uint8 j = pin % 5 + 1;
    char tmp[30] = {0};
    PRINT("取出了 addr:%d pin:%d，对应%d个B55的%i行%d个\r\n", addr, pin, n, i, j);
    const int led_index = n * 17 + i * 5 + j - 4;
    if (isDebugLED == true) {
        ws2812_set_led_hex(led_index, 0x000000, LED_MODE_DISABLE);
    }

    //取出，提前关灯
    led_manager_turn_off(led_index);
    sprintf(tmp, "%02X:%02X:%02X:%02X:%02X:%02X %d%d%d",
            MACAddr[0], MACAddr[1], MACAddr[2],
            MACAddr[3], MACAddr[4], MACAddr[5], n, i, j);
    SendData(0xC302, USER_DATA_TYPE, tmp);
}
