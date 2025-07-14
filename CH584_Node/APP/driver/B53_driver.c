//
// Created by xtx on 25-4-2.
//

#include <TCA9555.h>
bool DeviceExists[8] = {false};
uint8 B53_Num = 0;
void CheckB53_() {
    DeviceExists[0] = true; //自己
    DeviceExists[1] = I2C_CheckDeviceExists(0x21);
    DeviceExists[2] = I2C_CheckDeviceExists(0x22);
    DeviceExists[3] = I2C_CheckDeviceExists(0x23);
    DeviceExists[4] = I2C_CheckDeviceExists(0x24);
    DeviceExists[5] = I2C_CheckDeviceExists(0x25);
    DeviceExists[6] = I2C_CheckDeviceExists(0x26);
    DeviceExists[7] = I2C_CheckDeviceExists(0x27);

    printf("0x20: %d, 0x21: %d, 0x22: %d, 0x23: %d, 0x24: %d, 0x25: %d, 0x26: %d, 0x27: %d\r\n",
           DeviceExists[0], DeviceExists[1], DeviceExists[2], DeviceExists[3], DeviceExists[4],
           DeviceExists[5], DeviceExists[6], DeviceExists[7]);
    for (int i = 0; i < 8; i++) {
        if (DeviceExists[i]) {
            // TCA_SetAllPinsInput(0x20 + i);
            TCA_PinMode(0x20 + i,P17, OUTPUT); // 设置PB16为输出
            B53_Num++;
        }
    }

    //关闭蜂鸣器
    for (int i = 0; i < 8; i++) {
        if (DeviceExists[i]) {
            TCA_WritePin(0x20 + i, P17, 0);
        }
    }
}

void CheckB53Beep() {
    for (int i = 0; i < 8; i++) {
        if (DeviceExists[i]) {
            TCA_WritePin(0x20 + i, P17, 1);
            mDelaymS(100);
            TCA_WritePin(0x20 + i, P17, 0);
            mDelaymS(50);
        }
    }
}