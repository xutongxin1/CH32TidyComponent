//
// Created by 86180 on 2024/7/21.
//

#include "WS2812.h"

#include <CH59xBLE_LIB.h>
#include <stdlib.h>
#include <time.h>
/* PWM Output Mode Definition */
__attribute__ ((aligned (4))) uint32_t color_buf[COLOR_BUFFER_LEN] = {0};
// __attribute__ ((aligned (4))) uint32_t PwmBuf[100];
void wheelLed();
/// 初始化WS2812控制器
void WS2812Init() {
    GPIOA_ModeCfg(GPIO_Pin_11, GPIO_ModeOut_PP_5mA);

    // 映射到PB11
    //  GPIOB_ModeCfg(GPIO_Pin_11, GPIO_ModeOut_PP_5mA);
    //  GPIOPinRemap(ENABLE, RB_PIN_TMR2);

    PRINT("TMR2 DMA PWM\r\n");
    TMR2_PWMCycleCfg(75); // 周期 2000us  主频是60Mhz 每秒震荡60M次 震荡60次为1微秒
    TMR2_PWMInit(High_Level, PWM_Times_1);
    // TMR2_PWMActDataWidth (35);
    // TMR2_DMACfg (ENABLE, (uint32_t)&color_buf[0], (uint32_t)&color_buf[COLOR_BUFFER_LEN], Mode_Single);
    // TMR2_PWMEnable();
    // TMR2_Enable();

    /* 开启计数溢出中断，计满100个周期进入中断 */
    // TMR2_ClearITFlag(TMR1_2_IT_DMA_END);
    // TMR2_ITCfg(ENABLE, TMR1_2_IT_DMA_END);
    // PFIC_EnableIRQ(TMR2_IRQn);

    for (int i = 0; i < LED_NUM; i++) {
        setPixelColorHEX_Half(i, 0xFFFFFF);
    }

    TMR2_PWMDisable();
    TMR2_Disable();
    TMR2_DMACfg(ENABLE, (uint32_t) &color_buf[0], (uint32_t) &color_buf[COLOR_BUFFER_LEN], Mode_LOOP);
    TMR2_PWMEnable();
    TMR2_Enable();

    // //第一次绕过while判断更新
    // R16_TMR2_DMA_BEG = R16_TMR2_DMA_END;
    // w2812_sync();
    wheelLed();
}

/// 设置灯的颜色
///  @param id 灯的编号
///  @param r 红
///  @param g 绿
///  @param b 蓝
void setPixelColor(uint16_t id, uint8_t r, uint8_t g, uint8_t b) {
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

/// 发送缓冲区数据
// void w2812_sync() {
//     PRINT("更新WS2812\r\n");
//     while (R32_TMR2_DMA_NOW != R32_TMR2_DMA_END) {
//     }
//     TMR2_PWMDisable();
//     TMR2_Disable();
//     TMR2_DMACfg(ENABLE, (uint32_t) &color_buf[0], (uint32_t) &color_buf[COLOR_BUFFER_LEN], Mode_Single);
//     TMR2_PWMEnable();
//     TMR2_Enable();
// }

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
    // 亮度上升（1秒）- 100个亮度等级，每级10ms
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
        DelayMs(5);
    }

    // 亮度下降（1秒）- 100个亮度等级，每级10ms
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
        DelayMs(5);
    }
}
