//
// Created by xtx on 2025/4/17.
//
#include "ScanIO.h"

#include <stdlib.h>
#include <TCA9555.h>

#include "app_mesh.h"
#include "data_transfer.h"

// 用于处理引脚状态变化的处理函数
void handle_up(uint8 addr, uint8 pin); // 当引脚从低电平变为高电平时调用
void handle_down(uint8 addr, uint8 pin); // 当引脚从高电平变为低电平时调用

// 支持的最大TCA9555设备数量
#define MAX_TCA_DEVICES 8

// 存储每个TCA9555设备的前一次状态
static uint16_t previous_states[MAX_TCA_DEVICES];
// 跟踪设备是否已经初始化（用于第一次扫描）
static uint8_t device_initialized[MAX_TCA_DEVICES] = {0};

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
    const  uint16_t current_state = TCA_ReadAllPins(addr);
    printf("current_state --> %d\r\n", current_state);
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
                // 引脚从低电平变为高电平
                handle_up(addr, pin);
            } else {
                // 引脚从高电平变为低电平
                handle_down(addr, pin);
            }
        }
    }

    // 更新前一次状态为当前状态，为下次扫描做准备
    previous_states[index] = current_state;
}

//放回
//此处仅处理物理意义上的放回
void handle_up(const uint8 addr, const uint8 pin) {
    const uint8 n = addr - 0x80;
    const uint8 i = pin / 5;
    const uint8 j = pin % 5;
    char tmp[30] = {0};
    printf("handle_up addr:%d pin:%d\r\n", addr, pin);
    sprintf(tmp, "%s%d%d%d", (char *) MACAddr, n, i, j);
    SendData(0xC303, USER_DATA_TYPE, tmp);
}

//取出
void handle_down(const uint8 addr, const uint8 pin) {
    const uint8 n = addr - 0x80;
    const uint8 i = pin / 5;
    const uint8 j = pin % 5;
    char tmp[30] = {0};
    printf("handle_down addr:%d pin:%d\r\n", addr, pin);
    sprintf(tmp, "%s%d%d%d", (char *) MACAddr, n, i, j);
    SendData(0xC302, USER_DATA_TYPE, tmp);
}
