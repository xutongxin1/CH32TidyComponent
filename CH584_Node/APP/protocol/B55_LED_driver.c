//
// Created by xtx on 25-4-2.
//

#include <TCA9555.h>
void Init_B55_LED(void) {
    //第一个B55
    TCA_SetAllPinsInput(0x20);
    TCA_SetAllPinsInput(0x21);

    //检查第二个B55是否存在
    if (TCA_ReadPin(0x21, P15) == 1) {
        PRINT("第二个B55存在\r\n");
        //第二个B55-
        TCA_SetAllPinsInput(0x22);
        TCA_SetAllPinsInput(0x23);
        //检查第三个B55-是否存在
        if (TCA_ReadPin(0x21, P16) == 1) {
            PRINT("第三个B55-存在\r\n");
            //第三个B55-
            TCA_SetAllPinsInput(0x24);
            TCA_SetAllPinsInput(0x25);
            //检查第四个B55-是否存在
            if (TCA_ReadPin(0x21, P17) == 1) {
                PRINT("第四个B55-存在\r\n");
                //第四个B55-
                TCA_SetAllPinsInput(0x26);
                TCA_SetAllPinsInput(0x27);
            } else {
                PRINT("第四个B55-不存在\r\n");
            }
        } else {
            PRINT("第三个B55-不存在\r\n");
        }
    } else {
        PRINT("第二个B55不存在\r\n");
    }
}
