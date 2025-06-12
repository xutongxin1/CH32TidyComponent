//
// Created by xtx on 25-3-3.
//

#ifndef WS2812_H
#define WS2812_H

#include "CH58x_common.h"
#include <MESH_LIB.h>
#include <stdbool.h>

//高低占比
#define CODE_1 68 //97PSR *0.7
#define CODE_0 29 //97PSR *0.3

#define RESET_LEN    (300)
#ifdef DEVICE_TYPE_B55
#define  LED_NUM (3*5+1)*8
#elifdef  DEVICE_TYPE_A22
#define  LED_NUM 8
#elifdef DEVICE_TYPE_A43
#define  LED_NUM 8
#elifdef DEVICE_TYPE_USER
#define  LED_NUM 1
#endif
#define COLOR_BUFFER_LEN ((LED_NUM*(3*8))+RESET_LEN)
#define MAX_STEP (200)
#define LIST_SIZE(list) (sizeof(list)/sizeof(list[0]))
#define hex2rgb(c) (((c)>>16)&0xff),(((c)>>8)&0xff),((c)&0xff)

extern __attribute__ ((aligned (4))) uint32_t color_buf[COLOR_BUFFER_LEN];
// 时间参数定义（单位：周期数）
#define FLASH_FAST_ON_TIME      3   // 快闪亮时间
#define FLASH_FAST_OFF_TIME     3   // 快闪灭时间
#define FLASH_FAST_INTERVAL     20   // 快闪间隔时间

#define FLASH_SLOW_ON_TIME      50   // 慢闪亮时间
#define FLASH_SLOW_OFF_TIME     50   // 慢闪灭时间

#define BREATHE_FAST_STEP       5    // 快速呼吸步进
#define BREATHE_SLOW_STEP       2    // 慢速呼吸步进

// LED模式定义
typedef enum {
    LED_MODE_DISABLE = 0,      // 禁用模式
    LED_MODE_FLASH_FAST_1,     // 快闪1次
    LED_MODE_FLASH_FAST_2,     // 快闪2次
    LED_MODE_FLASH_FAST_3,     // 快闪3次
    LED_MODE_FLASH_SLOW,       // 慢闪
    LED_MODE_BREATHE_FAST,     // 快速呼吸
    LED_MODE_BREATHE_SLOW,     // 慢速呼吸
    LED_MODE_STATIC,           // 静态显示
    LED_MODE_MAX
} led_mode_t;

// LED状态结构体
typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    led_mode_t mode;
    uint16_t timer;           // 内部计时器
    uint8_t flash_count;      // 闪烁计数
    uint8_t brightness;       // 当前亮度（用于呼吸效果）
    bool increasing;          // 亮度增加标志（用于呼吸效果）
} led_state_t;

void setPixelColor(uint16_t id, uint8_t r, uint8_t g, uint8_t b);
// void w2812_sync();
void WS2812Init();
void wheelLed();
void ws2812_update(void);
void ws2812_effects_init();
void ws2812_set_led_hex(uint16_t led_id, uint32_t hex_color, led_mode_t mode);
void ws2812_set_led(uint16_t led_id, uint8_t r, uint8_t g, uint8_t b, led_mode_t mode);
void ws2812_set_all_color(uint8_t r, uint8_t g, uint8_t b);
void ws2812_set_all_mode(led_mode_t mode);
#endif //WS2812_H
