/**
* @file distribution_addr.h
 * @brief 持久化地址分配模块的头文件
 */

#ifndef DISTRIBUTION_ADDR_H
#define DISTRIBUTION_ADDR_H
#include <stdint.h>

/* 设备类型常量 */
#define DEVICE_TYPE_USER_NODE  0x02  // 用户节点
#define DEVICE_TYPE_A43        0x10  // A43设备
#define DEVICE_TYPE_A22        0x11  // A22设备
#define DEVICE_TYPE_B55        0x20  // B53设备

/* 各设备类型的地址范围 */
#define ADDR_RANGE_USER_NODE_START  0x0100  // 用户节点地址起始
#define ADDR_RANGE_USER_NODE_END    0x0FFF  // 用户节点地址结束
#define ADDR_RANGE_A42_START        0x1000  // A42设备地址起始
#define ADDR_RANGE_A42_END          0x17FF  // A41设备地址结束
#define ADDR_RANGE_A21_START        0x1800  // A21设备地址起始
#define ADDR_RANGE_A21_END          0x1FFF  // A21设备地址结束
#define ADDR_RANGE_B53_START        0x2000  // B53设备地址起始
#define ADDR_RANGE_B53_END          0x2FFF  // B53设备地址结束

/* EEPROM相关常量 */
#define EEPROM_START_ADDR      0x0000  // EEPROM起始地址
#define EEPROM_END_ADDR        0x2000  // EEPROM结束地址
#define RECORD_SIZE            8       // 每条记录大小：2字节地址 + 6字节MAC

/**
 * @brief 初始化地址分配模块
 *
 * 扫描EEPROM确定每种设备类型的下一个可分配地址和可用EEPROM位置
 */
void init_persistent_addr(void);

/**
 * @brief 根据MAC地址和设备类型分配网络地址
 *
 * @param mac 设备MAC地址（6字节）
 * @param device_type 设备类型标识符
 * @return uint16_t 分配的地址，如果分配失败则返回0
 */
uint16_t allocate_address(const uint8_t mac[6], uint8_t device_type);

/**
 * @brief 通过MAC地址查询分配的地址（不考虑设备类型）
 *
 * @param mac MAC地址（6字节）
 * @return uint16_t 查找到的地址，如果未找到返回0
 */
uint16_t find_address_by_mac(const uint8_t mac[6]);

/**
 * @brief 通过地址查询对应的MAC地址
 *
 * @param addr 要查询的地址
 * @param mac 输出参数，存储找到的MAC地址
 * @return int 找到返回1，未找到返回0
 */
int find_mac_by_address(uint16_t addr, uint8_t mac[6]);
#endif //DISTRIBUTION_ADDR_H
