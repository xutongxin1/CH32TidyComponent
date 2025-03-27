//
// Created by 86180 on 2024/7/21.
//

#include "WS2812.h"
/* PWM Output Mode Definition */
__attribute__ ((aligned (4))) uint32_t color_buf[COLOR_BUFFER_LEN] = {0};
__attribute__ ((aligned (4))) uint32_t PwmBuf[100];

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
        setPixelColor(i, 0, 0, 128);
    }
    //第一次绕过while判断更新
    R16_TMR2_DMA_BEG = R16_TMR2_DMA_END;
    w2812_sync();
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
void w2812_sync() {
    PRINT("更新WS2812\r\n");
    while (R32_TMR2_DMA_NOW != R32_TMR2_DMA_END) {
    }
    TMR2_PWMDisable();
    TMR2_Disable();
    TMR2_DMACfg(ENABLE, (uint32_t) &color_buf[0], (uint32_t) &color_buf[COLOR_BUFFER_LEN], Mode_Single);
    TMR2_PWMEnable();
    TMR2_Enable();
}
