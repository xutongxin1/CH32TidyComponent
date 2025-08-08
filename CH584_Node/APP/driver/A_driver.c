//
// Created by xtx on 25-7-14.
//
#include <A_driver.h>
#include "CH58x_common.h"
void A42_Init() {
    GPIOB_ModeCfg(GPIO_Pin_16, GPIO_ModeIN_PU);
    GPIOB_ModeCfg(GPIO_Pin_17, GPIO_ModeIN_PU);
    GPIOA_ModeCfg(GPIO_Pin_15, GPIO_ModeIN_PU);
    GPIOA_ModeCfg(GPIO_Pin_5, GPIO_ModeIN_PU);
    GPIOA_ResetBits(GPIO_Pin_4);
    GPIOA_ModeCfg(GPIO_Pin_4, GPIO_ModeOut_PP_20mA);
}

void A21_Init() {
    GPIOB_ModeCfg(GPIO_Pin_17, GPIO_ModeIN_PU);
    GPIOA_ModeCfg(GPIO_Pin_5, GPIO_ModeIN_PU);
    GPIOA_ResetBits(GPIO_Pin_4);
    GPIOA_ModeCfg(GPIO_Pin_4, GPIO_ModeOut_PP_20mA);
}

void CheckAxxBeep() {
    GPIOA_SetBits(GPIO_Pin_4);
    mDelaymS(100);
    GPIOA_ResetBits(GPIO_Pin_4);
}
