//
// Created by xtx on 25-3-3.
//

#ifndef WS2812_H
#define WS2812_H

#include "CH58x_common.h"
#include <MESH_LIB.h>

//¸ßµÍÕ¼±È
#define CODE_1 68 //97PSR *0.7
#define CODE_0 29 //97PSR *0.3

#define RESET_LEN    (300)
#ifdef DEVICE_TYPE_B55
#define  LED_NUM (4*5+1)*4
#elifdef  DEVICE_TYPE_A22
#define  LED_NUM 8
#elifdef DEVICE_TYPE_A43
#define  LED_NUM 8
#endif
#define COLOR_BUFFER_LEN ((LED_NUM*(3*8))+RESET_LEN)
#define MAX_STEP (200)
#define LIST_SIZE(list) (sizeof(list)/sizeof(list[0]))
#define hex2rgb(c) (((c)>>16)&0xff),(((c)>>8)&0xff),((c)&0xff)

void setPixelColor(uint16_t id, uint8_t r, uint8_t g, uint8_t b);
// void w2812_sync();
void WS2812Init();
void setPixelColorHEX(uint16_t n, uint32_t RGBColor);
void setPixelColorHEX_Half(uint16_t n, uint32_t RGBColor);
#endif //WS2812_H
