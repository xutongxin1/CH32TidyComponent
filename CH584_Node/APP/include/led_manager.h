//
// Created by xtx on 25-6-27.
//

#ifndef LED_MANAGER_H
#define LED_MANAGER_H
#include "WS2812.h"

// LED信息结构体
typedef struct {
    uint16_t led_id; // 灯的ID
    bool is_on; // 是否开启
    uint32_t remaining_time; // 剩余开启时间（秒）
    uint32_t color; // 灯的颜色
    led_mode_t mode; // 灯的模式
    uint8_t nij; // 具体位置（用于超时回调）
} led_info_t;

// 函数声明

/**
 * @brief 初始化LED管理器
 */
void led_manager_init(void);

/**
 * @brief 定时更新函数，每秒调用一次
 * 检查是否有灯到期，到期则关灯
 */
void led_manager_update(void);

/**
 * @brief 开启LED灯
 * @param nij 具体位置（用于超时回调）
 * @param led_id LED的ID
 * @param duration 开启时间（秒）
 * @param color 灯的颜色（32位hex值）
 * @param mode 灯的模式
 * @return true 成功，false 失败
 */
bool led_manager_turn_on(uint8_t nij, uint16_t led_id, uint32_t duration, uint32_t color, led_mode_t mode);

/**
 * @brief 关闭LED灯
 * @param led_id LED的ID
 * @return true 成功，false 失败（LED不存在或已关闭）
 */
bool led_manager_turn_off(uint16_t led_id);

/**
 * @brief 获取LED状态
 * @param led_id LED的ID
 * @return LED信息指针，如果不存在则返回NULL
 */
const led_info_t *led_manager_get_status(uint16_t led_id);

/**
 * @brief 获取当前开启的LED数量
 * @return 开启的LED数量
 */
uint32_t led_manager_get_active_count(void);

#endif //LED_MANAGER_H
