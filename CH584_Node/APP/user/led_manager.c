//
// Created by xtx on 25-6-27.
//

#include "led_manager.h"
#include <string.h>

#include "app_mesh.h"
#include "data_transfer.h"
#include "TCA9555.h"

// LED管理数组，直接用LED ID作为数组索引
static led_info_t led_array[LED_NUM];
static bool manager_initialized = false;

// 内部函数声明
static void turn_off_led_internal(uint16_t led_id);

/**
 * @brief 初始化LED管理器
 */
void led_manager_init(void) {
    // 清空所有LED信息
    memset(led_array, 0, sizeof(led_array));

    // 将所有LED设置为关闭状态
    for (int i = 0; i < LED_NUM; i++) {
        led_array[i].is_on = false;
        led_array[i].remaining_time = 0;
        led_array[i].color = 0;
        led_array[i].mode = LED_MODE_STATIC;
        led_array[i].nij = 0;
    }

    manager_initialized = true;
}

/**
 * @brief 定时更新函数，每秒调用一次
 */
void led_manager_update(void) {
    if (!manager_initialized) {
        return;
    }

    for (int i = 0; i < LED_NUM; i++) {
        if (led_array[i].is_on && led_array[i].remaining_time > 0) {
            led_array[i].remaining_time--;

            // 如果时间到了，关闭LED
            if (led_array[i].remaining_time == 0) {
                turn_off_led_internal(i);
                char tmp[64] = {0};
                sprintf(tmp, "%02X:%02X:%02X:%02X:%02X:%02X %d",
                        MACAddr[0], MACAddr[1], MACAddr[2],
                        MACAddr[3], MACAddr[4], MACAddr[5], led_array[i].nij);
#ifdef DEVICE_TYPE_B53
                SendData(0xC304, USER_DATA_TYPE, tmp);
#elifdef DEVICE_TYPE_A42
                SendData(0xC104, USER_DATA_TYPE, tmp);
#endif
            }
        }
    }
}

/**
 * @brief 开启LED灯
 */
bool led_manager_turn_on(uint8_t nij, uint16_t led_id, uint32_t duration, uint32_t color, led_mode_t mode,
                         bool isUseDeep, bool isUseBigLED) {
    if (!manager_initialized) {
        return false;
    }

    if (led_id >= LED_NUM || duration == 0) {
        return false;
    }

    // 直接用LED ID作为数组索引
    led_array[led_id].is_on = true;
    led_array[led_id].remaining_time = duration;
    led_array[led_id].color = color;
    led_array[led_id].mode = mode;
    led_array[led_id].nij = nij;
    led_array[led_id].isUseBigLED = isUseBigLED;
    led_array[led_id].isUseDeep = isUseDeep;

#ifdef DEVICE_TYPE_B53

    if (isUseBigLED) {
        const uint16_t tmp = (led_id / 17) * 17 + 1;
        ws2812_set_led_hex(tmp, color, mode);
    }
    if (isUseDeep) {
        const uint8_t tmp = led_id / 17;
        TCA_WritePin(0x20 + tmp, P17, 1);
    }
#elifdef DEVICE_TYPE_A42
    if (isUseBigLED) {
        ws2812_set_led_hex(1, color, mode);
    }
    if (isUseDeep) {
        GPIOA_SetBits(GPIO_Pin_4);
    }
#endif

    // 调用底层函数开启LED
    ws2812_set_led_hex(led_id, color, mode);

    return true;
}

/**
 * @brief 关闭LED灯
 */
bool led_manager_turn_off(uint16_t led_id) {
    if (!manager_initialized) {
        return false;
    }

    if (led_id >= LED_NUM || !led_array[led_id].is_on) {
        return false;
    }

    turn_off_led_internal(led_id);
    return true;
}

/**
 * @brief 获取LED状态
 */
const led_info_t *led_manager_get_status(uint16_t led_id) {
    if (!manager_initialized || led_id >= LED_NUM) {
        return NULL;
    }

    return &led_array[led_id];
}

/**
 * @brief 获取当前开启的LED数量
 */
uint32_t led_manager_get_active_count(void) {
    if (!manager_initialized) {
        return 0;
    }

    uint32_t count = 0;
    for (int i = 0; i < LED_NUM; i++) {
        if (led_array[i].is_on) {
            count++;
        }
    }
    return count;
}

// ========== 内部函数实现 ==========

/**
 * @brief 内部关闭LED函数
 * @param led_id LED的ID
 */
static void turn_off_led_internal(uint16_t led_id) {
    if (led_id < LED_NUM && led_array[led_id].is_on) {
        // 调用底层函数关闭LED（颜色设为0）
        ws2812_set_led_hex(led_id, 0, led_array[led_id].mode);

        // 更新状态
        led_array[led_id].is_on = false;
        led_array[led_id].remaining_time = 0;
        // 保留color和mode信息，以便后续可能的查询

#ifdef DEVICE_TYPE_B53
        if (led_array[led_id].isUseBigLED) {
            const uint16_t tmp = (led_id / 17) * 17 + 1;
            ws2812_set_led_hex(tmp, 0, led_array[led_id].mode);
        }
        if (led_array[led_id].isUseDeep) {
            const uint8_t tmp = led_id / 17;
            TCA_WritePin(0x20 + tmp, P17, 0);
        }
#elifdef DEVICE_TYPE_A42
        if (led_array[led_id].isUseBigLED) {
            ws2812_set_led_hex(1, 0, led_array[led_id].mode);
        }
        if (led_array[led_id].isUseDeep) {
            GPIOA_ResetBits(GPIO_Pin_4);
        }
#endif
    }
}
