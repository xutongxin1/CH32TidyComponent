//
// Created by xtx on 25-3-23.
//

/**
 * @file distribution_addr.c
 * @brief 持久化地址分配模块的实现文件
 */

#include "distribution_addr.h"

#include <app_mesh_config.h>
#include <ISP585.h>
#include <MESH_LIB.h>
#include <stdio.h>
#include <string.h>

/* 全局变量，存储每种设备类型的下一个可分配地址 */
static uint16_t next_addr_user_node = ADDR_RANGE_USER_NODE_START;
static uint16_t next_addr_a43 = ADDR_RANGE_A43_START;
static uint16_t next_addr_a22 = ADDR_RANGE_A22_START;
static uint16_t next_addr_b55 = ADDR_RANGE_B55_START;

/* 下一个可用的EEPROM存储位置 */
static uint16_t next_eeprom_addr = EEPROM_START_ADDR;

/**
 * @brief 检查地址是否在指定设备类型的范围内
 *
 * @param addr 要检查的地址
 * @param device_type 设备类型
 * @return int 如果地址在指定设备类型范围内则返回1，否则返回0
 */
static int is_addr_in_device_range(uint16_t addr, uint8_t device_type) {
    switch (device_type) {
        case DEVICE_TYPE_USER_NODE:
            return (addr >= ADDR_RANGE_USER_NODE_START && addr <= ADDR_RANGE_USER_NODE_END);
        case DEVICE_TYPE_A43:
            return (addr >= ADDR_RANGE_A43_START && addr <= ADDR_RANGE_A43_END);
        case DEVICE_TYPE_A22:
            return (addr >= ADDR_RANGE_A22_START && addr <= ADDR_RANGE_A22_END);
        case DEVICE_TYPE_B55:
            return (addr >= ADDR_RANGE_B55_START && addr <= ADDR_RANGE_B55_END);
        default:
            return 0;
    }
}

/**
 * @brief 初始化函数，扫描EEPROM确定下一个可分配地址和可用EEPROM位置
 */
void init_persistent_addr(void) {
    uint8_t record[RECORD_SIZE];
    uint16_t addr;
    uint16_t eeprom_addr = EEPROM_START_ADDR;

    /* 重置为默认起始地址 */
    next_addr_user_node = ADDR_RANGE_USER_NODE_START;
    next_addr_a43 = ADDR_RANGE_A43_START;
    next_addr_a22 = ADDR_RANGE_A22_START;
    next_addr_b55 = ADDR_RANGE_B55_START;
    next_eeprom_addr = EEPROM_START_ADDR; // 确保EEPROM地址也被重置

    /* 扫描EEPROM中的记录，查找每种设备类型的最大已分配地址 */
    while (eeprom_addr < EEPROM_END_ADDR) {
        EEPROM_READ(eeprom_addr, record, RECORD_SIZE);

        /* 检查记录是否有效（简单检查：地址不为FF） */
        addr = (record[0] << 8) | record[1];
        if (addr == 65535) {
            /* 找到第一个空记录位置，表示扫描结束 */
            break;
        }

        /* 根据地址范围更新下一个可分配地址 */
        if (addr >= ADDR_RANGE_USER_NODE_START && addr <= ADDR_RANGE_USER_NODE_END) {
            if (addr + 1 > next_addr_user_node) {
                next_addr_user_node = addr + 1; // 更新用户节点下一个地址
            }
        } else if (addr >= ADDR_RANGE_A43_START && addr <= ADDR_RANGE_A43_END) {
            if (addr + 1 > next_addr_a43) {
                next_addr_a43 = addr + 1; // 更新A43设备下一个地址
            }
        } else if (addr >= ADDR_RANGE_A22_START && addr <= ADDR_RANGE_A22_END) {
            if (addr + 1 > next_addr_a22) {
                next_addr_a22 = addr + 1; // 更新A22设备下一个地址
            }
        } else if (addr >= ADDR_RANGE_B55_START && addr <= ADDR_RANGE_B55_END) {
            if (addr + 1 > next_addr_b55) {
                next_addr_b55 = addr + 1; // 更新B55设备下一个地址
            }
        }

        eeprom_addr += RECORD_SIZE; // 移动到下一条记录
    }

    /* 更新下一个可用的EEPROM位置 */
    next_eeprom_addr = eeprom_addr;
}

/**
 * @brief 根据MAC地址和设备类型查找已分配的地址
 *
 * @param mac MAC地址
 * @param device_type 设备类型
 * @param addr 如果找到，存储已分配的地址
 * @return int 找到返回1，未找到返回0
 */
static int find_address_by_mac_and_type(const uint8_t mac[6], uint8_t device_type, uint16_t *addr) {
    uint8_t record[RECORD_SIZE];
    uint16_t eeprom_addr = EEPROM_START_ADDR;
    uint16_t found_addr;

    /* 遍历EEPROM中的所有记录 */
    while (eeprom_addr < next_eeprom_addr) {
        EEPROM_READ(eeprom_addr, record, RECORD_SIZE);

        /* 比较MAC地址 */
        if (memcmp(&record[2], mac, 6) == 0) {
            /* MAC地址匹配，检查地址是否在指定设备类型的范围内 */
            found_addr = (record[0] << 8) | record[1];
            if (is_addr_in_device_range(found_addr, device_type)) {
                *addr = found_addr;
                return 1; // 找到匹配的地址
            }
        }

        eeprom_addr += RECORD_SIZE;
    }

    return 0; // 未找到匹配的地址
}

/**
 * @brief 将新的地址和MAC写入EEPROM
 *
 * @param addr 分配的地址
 * @param mac MAC地址
 * @return int 写入成功返回1，失败返回0
 */
static int write_address_to_eeprom(const uint16_t addr, const uint8_t mac[6]) {
    uint8_t record[RECORD_SIZE];

    /* 检查EEPROM是否有足够空间 */
    if (next_eeprom_addr + RECORD_SIZE > EEPROM_END_ADDR) {
        return 0; // EEPROM已满
    }

    /* 准备记录数据：2字节地址 + 6字节MAC */
    record[0] = (addr >> 8) & 0xFF; // 地址高字节
    record[1] = addr & 0xFF; // 地址低字节
    memcpy(&record[2], mac, 6); // MAC地址

    /* 写入EEPROM */
    EEPROM_WRITE(next_eeprom_addr, record, RECORD_SIZE);

    /* 更新下一个可用的EEPROM位置 */
    next_eeprom_addr += RECORD_SIZE;

    return 1; // 写入成功
}

/**
 * @brief 获取指定设备类型的下一个可用地址
 *
 * @param device_type 设备类型
 * @return uint16_t 下一个可用地址，如果地址范围已用尽则返回0
 */
static uint16_t get_next_addr_for_device_type(uint8_t device_type) {
    switch (device_type) {
        case DEVICE_TYPE_USER_NODE:
            if (next_addr_user_node > ADDR_RANGE_USER_NODE_END) {
                return 0; // 用户节点地址范围已用尽
            }
            return next_addr_user_node++;
        case DEVICE_TYPE_A43:
            if (next_addr_a43 > ADDR_RANGE_A43_END) {
                return 0; // A43设备地址范围已用尽
            }
            return next_addr_a43++;
        case DEVICE_TYPE_A22:
            if (next_addr_a22 > ADDR_RANGE_A22_END) {
                return 0; // A22设备地址范围已用尽
            }
            return next_addr_a22++;
        case DEVICE_TYPE_B55:
            if (next_addr_b55 > ADDR_RANGE_B55_END) {
                return 0; // B55设备地址范围已用尽
            }
            return next_addr_b55++;
        default:
            return 0; // 未知设备类型
    }
}

/**
 * @brief 分配地址函数
 *
 * @param mac 设备MAC地址
 * @param device_type 设备类型
 * @return uint16_t 分配的地址，如果分配失败则返回0
 */
void bt_node_del(uint16_t addr);
uint16_t allocate_address(const uint8_t mac[6], const uint8_t device_type) {
    uint16_t addr;

    /* 检查是否已为此MAC和设备类型分配了地址 */
    if (find_address_by_mac_and_type(mac, device_type, &addr)) {
        printf("Found existing address: 0x%04X\r\n", addr);
        /* 找到已分配的地址，返回它 */
        bt_node_del(addr);
        return addr;
    }

    /* 为该设备类型分配一个新地址 */
    addr = get_next_addr_for_device_type(device_type);
    printf("Allocated new address: 0x%04X\n", addr);
    if (addr == 0) {
        return 0; // 无法分配地址（范围已用尽）
    }

    /* 将新地址和MAC写入EEPROM */
    if (!write_address_to_eeprom(addr, mac)) {
        /* 写入失败，还原下一个可用地址计数器 */
        switch (device_type) {
            case DEVICE_TYPE_USER_NODE:
                next_addr_user_node--;
                break;
            case DEVICE_TYPE_A43:
                next_addr_a43--;
                break;
            case DEVICE_TYPE_A22:
                next_addr_a22--;
                break;
            case DEVICE_TYPE_B55:
                next_addr_b55--;
                break;
        }
        return 0; // EEPROM已满，无法存储新记录
    }

    return addr; // 返回成功分配的地址
}

/**
 * @brief 通过MAC地址查询分配的地址（不考虑设备类型）
 *
 * @param mac MAC地址
 * @return uint16_t 查找到的地址，如果未找到返回0
 */
uint16_t find_address_by_mac(const uint8_t mac[6]) {
    uint8_t record[RECORD_SIZE];
    uint16_t eeprom_addr = EEPROM_START_ADDR;
    uint16_t found_addr;

    /* 遍历EEPROM中的所有记录 */
    while (eeprom_addr < next_eeprom_addr) {
        EEPROM_READ(eeprom_addr, record, RECORD_SIZE);

        /* 比较MAC地址 */
        if (memcmp(&record[2], mac, 6) == 0) {
            /* MAC地址匹配，返回对应的地址 */
            found_addr = (record[0] << 8) | record[1];
            return found_addr;
        }

        eeprom_addr += RECORD_SIZE;
    }

    return 0; // 未找到匹配的地址
}

/**
 * @brief 通过地址查询对应的MAC地址
 *
 * @param addr 要查询的地址
 * @param mac 输出参数，存储找到的MAC地址
 * @return int 找到返回1，未找到返回0
 */
int find_mac_by_address(uint16_t addr, uint8_t mac[6]) {
    uint8_t record[RECORD_SIZE];
    uint16_t eeprom_addr = EEPROM_START_ADDR;
    uint16_t record_addr;

    /* 遍历EEPROM中的所有记录 */
    while (eeprom_addr < next_eeprom_addr) {
        EEPROM_READ(eeprom_addr, record, RECORD_SIZE);

        /* 获取记录中的地址 */
        record_addr = (record[0] << 8) | record[1];

        /* 比较地址 */
        if (record_addr == addr) {
            /* 地址匹配，复制MAC地址到输出参数 */
            memcpy(mac, &record[2], 6);
            return 1; // 找到匹配的MAC地址
        }

        eeprom_addr += RECORD_SIZE;
    }

    return 0; // 未找到匹配的MAC地址
}
