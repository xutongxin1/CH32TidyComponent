//
// Created by xtx on 25-3-3.
//

#ifndef WS2812_H
#define WS2812_H

#include "CH59x_common.h"
#include <MESH_LIB.h>

//¸ßµÍÕ¼±È
// #define CODE_1 37 //75PSR *0.7
// #define CODE_0 18 //75PSR *0.3
#define CODE_1 53 //75PSR *0.7
#define CODE_0 22 //75PSR *0.3

#define RESET_LEN    (300)
#define  LED_NUM 8
#define COLOR_BUFFER_LEN ((LED_NUM*(3*8))+RESET_LEN)
#define MAX_STEP (200)
#define LIST_SIZE(list) (sizeof(list)/sizeof(list[0]))
#define hex2rgb(c) (((c)>>16)&0xff),(((c)>>8)&0xff),((c)&0xff)

void setPixelColor(uint16_t id, uint8_t r, uint8_t g, uint8_t b);
void w2812_sync();
void WS2812Init();

#endif //WS2812_H
