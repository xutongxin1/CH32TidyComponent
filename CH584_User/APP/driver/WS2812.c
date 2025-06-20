//
// Created by 86180 on 2024/7/21.
//

#include "WS2812.h"

#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#include "USB_CDC.h"
/* PWM Output Mode Definition */
__attribute__ ((aligned (4))) uint32_t color_buf[COLOR_BUFFER_LEN] = {0};
void wheelLed(void);
/// 初始化WS2812控制器
void WS2812Init() {
    GPIOB_ModeCfg(GPIO_Pin_22, GPIO_ModeOut_PP_5mA);

    PRINT("TMR3 DMA PWM\r\n");
    TMR3_PWMCycleCfg(97); // 目标是800KHz 主频78MHz 因此PLL频率为78MHz/800KHz=97.5
    TMR3_PWMInit(High_Level, PWM_Times_1);

    TMR3_PWMDisable();
    TMR3_Disable();
    TMR3_DMACfg(ENABLE, (uint32_t) &color_buf[0], (uint32_t) &color_buf[COLOR_BUFFER_LEN], Mode_LOOP);
    TMR3_PWMEnable();
    TMR3_Enable();

    ws2812_effects_init();
}

/// 设置灯的颜色
///  @param id 灯的编号
///  @param r 红
///  @param g 绿
///  @param b 蓝
void setPixelColor(const uint16_t id, const uint8_t r, const uint8_t g, const uint8_t b) {
    int i = 0, j = id * 24u;
    // if (id >= 4) {
    //     return;
    // }

    for (i = 0; i < 8; i++) {
        if (g & (0x80 >> i)) {
            color_buf[j] = CODE_1;
        } else {
            color_buf[j] = CODE_0;
        }
        j++;
    }
    for (i = 0; i < 8; i++) {
        if (r & (0x80 >> i)) {
            color_buf[j] = CODE_1;
        } else {
            color_buf[j] = CODE_0;
        }
        j++;
    }
    for (i = 0; i < 8; i++) {
        if (b & (0x80 >> i)) {
            color_buf[j] = CODE_1;
        } else {
            color_buf[j] = CODE_0;
        }
        j++;
    }
}

void setPixelColorHEX(uint16_t n, uint32_t RGBColor) {
    if (n < LED_NUM) {
        // 直接将RGB (0xRRGGBB) 转换为GRB (0xGGRRBB)格式
        uint32_t GRBColor = ((RGBColor & 0x00FF00) << 8) | // G移至高8位
            ((RGBColor & 0xFF0000) >> 8) | // R移至中间8位
            (RGBColor & 0x0000FF); // B保持不变

        // 使用原来的位操作逻辑
        for (uint8_t i = 0; i < 24; ++i) {
            color_buf[24 * n + i] = (((GRBColor << i) & 0X800000) ? CODE_1 : CODE_0);
        }
    }
}
void setPixelColorHEX_Half(uint16_t n, uint32_t RGBColor) {
    if (n < LED_NUM) {
        // 提取RGB各分量
        uint8_t r = (RGBColor >> 16) & 0xFF;
        uint8_t g = (RGBColor >> 8) & 0xFF;
        uint8_t b = RGBColor & 0xFF;

        // 将每个分量减半（亮度降为50%）
        r = r >> 1; // 等同于 r / 2
        g = g >> 1; // 等同于 g / 2
        b = b >> 1; // 等同于 b / 2

        // 重组为GRB格式
        uint32_t GRBColor = ((uint32_t) g << 16) | ((uint32_t) r << 8) | b;

        // 使用原来的位操作逻辑
        for (uint8_t i = 0; i < 24; ++i) {
            color_buf[24 * n + i] = (((GRBColor << i) & 0X800000) ? CODE_1 : CODE_0);
        }
    }
}

void wheelLed() {
    // 为所有LED生成随机颜色
    uint32_t GRBColors[LED_NUM];
    for (int i = 0; i < LED_NUM; i++) {
        // 生成随机RGB颜色（每个分量0-255）
        uint8_t r = rand() % 256;
        uint8_t g = rand() % 256;
        uint8_t b = rand() % 256;

        // 转换为GRB格式
        GRBColors[i] = ((uint32_t) g << 16) | ((uint32_t) r << 8) | b;
    }

    // 呼吸效果 - 总时间2秒
    // 亮度上升（1秒）- 100个亮度等级，每级5ms
    for (int brightness = 0; brightness <= 100; brightness++) {
        // 更新所有LED的亮度
        for (int i = 0; i < LED_NUM; i++) {
            uint32_t color = GRBColors[i];

            // 提取原始RGB值
            uint8_t r = (color >> 8) & 0xFF;
            uint8_t g = (color >> 16) & 0xFF;
            uint8_t b = color & 0xFF;

            // 计算当前亮度对应的RGB值
            uint8_t curr_r = r * brightness / 100;
            uint8_t curr_g = g * brightness / 100;
            uint8_t curr_b = b * brightness / 100;

            // 转换为GRB格式
            uint32_t dimmedColor = ((uint32_t) curr_g << 16) | ((uint32_t) curr_r << 8) | curr_b;

            // 设置像素颜色
            for (uint8_t j = 0; j < 24; ++j) {
                color_buf[24 * i + j] = (((dimmedColor << j) & 0X800000) ? CODE_1 : CODE_0);
            }
        }

        // 延时10毫秒
        DelayMs(3);
    }

    // 亮度下降（1秒）- 100个亮度等级，每级5ms
    for (int brightness = 100; brightness >= 0; brightness--) {
        // 更新所有LED的亮度
        for (int i = 0; i < LED_NUM; i++) {
            uint32_t color = GRBColors[i];

            // 提取原始RGB值
            uint8_t r = (color >> 8) & 0xFF;
            uint8_t g = (color >> 16) & 0xFF;
            uint8_t b = color & 0xFF;

            // 计算当前亮度对应的RGB值
            uint8_t curr_r = r * brightness / 100;
            uint8_t curr_g = g * brightness / 100;
            uint8_t curr_b = b * brightness / 100;

            // 转换为GRB格式
            uint32_t dimmedColor = ((uint32_t) curr_g << 16) | ((uint32_t) curr_r << 8) | curr_b;

            // 设置像素颜色
            for (uint8_t j = 0; j < 24; ++j) {
                color_buf[24 * i + j] = (((dimmedColor << j) & 0X800000) ? CODE_1 : CODE_0);
            }
        }

        // 延时10毫秒
        DelayMs(3);
    }
}

// 全局LED状态数组
static led_state_t led_states[LED_NUM];
static uint16_t num_leds = 0;

// 初始化LED系统
void ws2812_effects_init() {

    num_leds = LED_NUM;

    for (uint16_t i = 0; i < num_leds; i++) {
        led_states[i].r = 0;
        led_states[i].g = 0;
        led_states[i].b = 0;
        led_states[i].mode = LED_MODE_DISABLE;
        led_states[i].timer = 0;
        led_states[i].flash_count = 0;
        led_states[i].brightness = 0;
        led_states[i].increasing = true;
    }
}

// 设置单个LED的颜色
void ws2812_set_color(uint16_t led_id, uint8_t r, uint8_t g, uint8_t b) {
    if (led_id >= num_leds) return;

    led_states[led_id].r = r;
    led_states[led_id].g = g;
    led_states[led_id].b = b;
}

// 设置单个LED的颜色（HEX格式）
void ws2812_set_color_hex(uint16_t led_id, uint32_t hex_color) {
    if (led_id >= num_leds) return;

    led_states[led_id].r = (hex_color >> 16) & 0xFF;
    led_states[led_id].g = (hex_color >> 8) & 0xFF;
    led_states[led_id].b = hex_color & 0xFF;
}

// 设置单个LED的模式
void ws2812_set_mode(uint16_t led_id, led_mode_t mode) {
    if (led_id >= num_leds) return;

    led_states[led_id].mode = mode;
    led_states[led_id].timer = 0;
    led_states[led_id].flash_count = 0;
    led_states[led_id].brightness = 0;
    led_states[led_id].increasing = true;
}

// 设置所有LED的颜色
void ws2812_set_all_color(uint8_t r, uint8_t g, uint8_t b) {
    for (uint16_t i = 0; i < num_leds; i++) {
        ws2812_set_color(i, r, g, b);
    }
}

// 设置所有LED的模式
void ws2812_set_all_mode(led_mode_t mode) {
    for (uint16_t i = 0; i < num_leds; i++) {
        ws2812_set_mode(i, mode);
    }
}

// 处理快闪模式
static void handle_flash_mode(uint16_t led_id, uint8_t flash_times) {

    led_state_t *led = &led_states[led_id];

    if (led->flash_count < flash_times * 2) {
        // 正在闪烁阶段
        if (led->timer < FLASH_FAST_ON_TIME && (led->flash_count % 2 == 0)) {
            // 亮
            setPixelColor(led_id, led->r, led->g, led->b);
        } else if (led->timer < FLASH_FAST_ON_TIME + FLASH_FAST_OFF_TIME) {
            // 灭
            setPixelColor(led_id, 0, 0, 0);
            if (led->timer == FLASH_FAST_ON_TIME + FLASH_FAST_OFF_TIME - 1) {
                led->flash_count++;
                led->timer = 0;
                return;
            }
        }
    } else {
        // 闪烁完成，进入间隔期
        setPixelColor(led_id, 0, 0, 0);
        if (led->timer >= FLASH_FAST_INTERVAL) {
            led->timer = 0;
            led->flash_count = 0;
            return;
        }
    }

    led->timer++;
}

// 处理慢闪模式
static void handle_slow_flash(uint16_t led_id) {
    led_state_t *led = &led_states[led_id];

    if (led->timer < FLASH_SLOW_ON_TIME) {
        // 亮
        setPixelColor(led_id, led->r, led->g, led->b);
    } else if (led->timer < FLASH_SLOW_ON_TIME + FLASH_SLOW_OFF_TIME) {
        // 灭
        setPixelColor(led_id, 0, 0, 0);
    } else {
        // 重置计时器
        led->timer = 0;
        return;
    }

    led->timer++;
}

// 处理呼吸模式
static void handle_breathe_mode(uint16_t led_id, uint8_t step) {
    led_state_t *led = &led_states[led_id];

    // 计算当前亮度
    uint8_t r = (led->r * led->brightness) / 255;
    uint8_t g = (led->g * led->brightness) / 255;
    uint8_t b = (led->b * led->brightness) / 255;

    setPixelColor(led_id, r, g, b);

    // 更新亮度
    if (led->increasing) {
        if (led->brightness + step >= 255) {
            led->brightness = 255;
            led->increasing = false;
        } else {
            led->brightness += step;
        }
    } else {
        if (led->brightness <= step) {
            led->brightness = 0;
            led->increasing = true;
        } else {
            led->brightness -= step;
        }
    }
}

// 周期性运行函数（主函数）
void ws2812_update(void) {
    for (uint16_t i = 0; i < num_leds; i++) {
        led_state_t *led = &led_states[i];

        switch (led->mode) {
            case LED_MODE_DISABLE:
                setPixelColor(i, 0, 0, 0);
                break;

            case LED_MODE_FLASH_FAST_1:
                handle_flash_mode(i, 1);
                break;

            case LED_MODE_FLASH_FAST_2:
                handle_flash_mode(i, 2);
                break;

            case LED_MODE_FLASH_FAST_3:
                handle_flash_mode(i, 3);
                break;

            case LED_MODE_FLASH_SLOW:
                handle_slow_flash(i);
                break;

            case LED_MODE_BREATHE_FAST:
                handle_breathe_mode(i, BREATHE_FAST_STEP);
                break;

            case LED_MODE_BREATHE_SLOW:
                handle_breathe_mode(i, BREATHE_SLOW_STEP);
                break;

            case LED_MODE_STATIC:
                setPixelColor(i, led->r, led->g, led->b);
                break;

            default:
                setPixelColor(i, 0, 0, 0);
                break;
        }
    }
}

// 便捷函数：设置LED并立即应用模式
void ws2812_set_led(uint16_t led_id, uint8_t r, uint8_t g, uint8_t b, led_mode_t mode) {
    ws2812_set_color(led_id, r, g, b);
    ws2812_set_mode(led_id, mode);
}

// 便捷函数：设置LED并立即应用模式（HEX格式）
void ws2812_set_led_hex(uint16_t led_id, uint32_t hex_color, led_mode_t mode) {
    ws2812_set_color_hex(led_id, hex_color);
    ws2812_set_mode(led_id, mode);
}

// 获取LED当前状态信息
void ws2812_get_led_state(uint16_t led_id, uint8_t *r, uint8_t *g, uint8_t *b, led_mode_t *mode) {
    if (led_id >= num_leds) return;

    if (r) *r = led_states[led_id].r;
    if (g) *g = led_states[led_id].g;
    if (b) *b = led_states[led_id].b;
    if (mode) *mode = led_states[led_id].mode;
}
