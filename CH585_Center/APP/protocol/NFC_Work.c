#include <CH58xBLE_LIB.h>
#include <stdint.h>
#include <stdio.h>

#include "NFC_Work.h"

#include <stdbool.h>

#include "CH585SFR.h"
#include "CH58x_adc.h"
#include "wch_nfca_mifare_classic.h"
#include "wch_nfca_pcd.h"
#include "wch_nfca_pcd_bsp.h"
//
// Created by xtx on 25-7-1.
//
/*********************************************************************
 * @fn      sys_get_vdd
 *
 * @brief   系统电压检测
 *
 * @param   none
 *
 * @return  检测的ADC值
 */
uint16_t sys_get_vdd(void) {
    uint8_t sensor, channel, config, tkey_cfg;
    uint16_t adc_data;

    tkey_cfg = R8_TKEY_CFG;
    sensor = R8_TEM_SENSOR;
    channel = R8_ADC_CHANNEL;
    config = R8_ADC_CFG;

    R8_TKEY_CFG &= ~RB_TKEY_PWR_ON;
    R8_ADC_CHANNEL = CH_INTE_VBAT;
    R8_ADC_CFG = RB_ADC_POWER_ON | RB_ADC_BUF_EN | (0 << 4); /* 使用-12dB模式 */
    R8_ADC_CONVERT &= ~RB_ADC_PGA_GAIN2;
    R8_ADC_CONVERT |= (3 << 4); /* 7个Tadc */
    R8_ADC_CONVERT |= RB_ADC_START;
    while (R8_ADC_CONVERT & RB_ADC_START);
    adc_data = R16_ADC_DATA;

    R8_TEM_SENSOR = sensor;
    R8_ADC_CHANNEL = channel;
    R8_ADC_CFG = config;
    R8_TKEY_CFG = tkey_cfg;
    return (adc_data);
}

void NFC_Init() {
    nfca_pcd_init();
    nfca_pcd_lpcd_calibration(); /* 低功耗检卡ADC值校准，和天线等相关，可在生产时进行检测。 */

    uint16_t adc_vdd;
    int vdd_value;

    adc_vdd = sys_get_vdd();
    vdd_value = ADC_VoltConverSignalPGA_MINUS_12dB(adc_vdd);
    PRINT("vdd_value: %d\r\n", vdd_value);
    if (vdd_value > 3400) {
        nfca_pcd_set_out_drv(NFCA_PCD_DRV_CTRL_LEVEL0);
        PRINT("LV0\r\n");
    } else if (vdd_value > 3000) {
        nfca_pcd_set_out_drv(NFCA_PCD_DRV_CTRL_LEVEL1);
        PRINT("LV1\r\n");
    } else if (vdd_value > 2600) {
        nfca_pcd_set_out_drv(NFCA_PCD_DRV_CTRL_LEVEL2);
        PRINT("LV2\r\n");
    } else {
        nfca_pcd_set_out_drv(NFCA_PCD_DRV_CTRL_LEVEL3);
        PRINT("LV3\r\n");
    }
    PRINT("初始化NFC成功\r\n");
}
bool NFC_Start() {
    nfca_pcd_start();
    if (nfca_pcd_lpcd_check() == 0) {
        PRINT("NO CARD\r\n");
        nfca_pcd_stop();
        return false;
    }
    PRINT("CARD DETECT\r\n");
    return true;
}

uint8_t picc_uid[4];
uint8_t default_key[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t ReadNFC_CID(char *data) {
    uint8_t res = 0;
    uint16_t text_pos = 0;

    if (data == NULL) {
        return 0xFF;
    }

    // 认证扇区1 (基于你的数据，文本在block 4-5)
    res = PcdAuthState(PICC_AUTHENT1B, 4, default_key, picc_uid);
    if (res) {
        return res;
    }

    // 读取block 4和5
    for (uint8_t block = 4; block <= 5; block++) {
        res = PcdRead(block);
        if (res) {
            return res;
        }

        // 根据你的数据，文本从block 4的第11个字节开始
        uint8_t start_pos = (block == 4) ? 11 : 0;

        for (uint8_t j = start_pos; j < 16; j++) {
            uint8_t byte_val = g_nfca_pcd_recv_buf[j];

            // 检查是否为可打印字符
            if (byte_val >= 0x20 && byte_val <= 0x7E) {
                if (text_pos < 63) {
                    data[text_pos++] = (char) byte_val;
                }
            }
            // 遇到结束标记
            else if (byte_val == 0xFE || byte_val == 0x00) {
                data[text_pos] = '\0';
                return 0;
            }
        }
    }

    data[text_pos] = '\0';
    return 0;
}
void NFC_Work() {
    uint16_t res;
    res = PcdRequest(PICC_REQALL);
    if (res == 0x0004) {
        res = PcdAnticoll(PICC_ANTICOLL1);
        if (res == PCD_NO_ERROR) {
            picc_uid[0] = g_nfca_pcd_recv_buf[0];
            picc_uid[1] = g_nfca_pcd_recv_buf[1];
            picc_uid[2] = g_nfca_pcd_recv_buf[2];
            picc_uid[3] = g_nfca_pcd_recv_buf[3];
            PRINT("uid: %02x %02x %02x %02x\r\n", picc_uid[0], picc_uid[1], picc_uid[2], picc_uid[3]);

            res = PcdSelect(PICC_ANTICOLL1, picc_uid);
            if (res == PCD_NO_ERROR) {
                PRINT("\nselect OK, SAK:%02x\r\n", g_nfca_pcd_recv_buf[0]);

                char cid_string[64]; // 11个字符 + 1个结束符

                uint8_t result = ReadNFC_CID(cid_string);
                if (result == 0) {
                    PRINT("Successfully read CID: %s\r\n", cid_string);
                    // 这里会输出: "22334445555"
                } else {
                    PRINT("Failed to read CID, error: 0x%x\r\n", result);
                }
            }
        }
    }
    nfca_pcd_stop();
}
