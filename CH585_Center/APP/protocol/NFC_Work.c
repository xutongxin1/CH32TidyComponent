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

bool isWritting = false;
char WriteData[20] = {0};
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
        // PRINT("NO CARD\r\n");
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
/**
 * @brief 将文本数据按NDEF格式写入NFC第1扇区的前三个块
 * @param text_data: 要写入的文本数据指针
 * @return int: 0=成功, 其他值=错误码
 */
int nfc_write_ndef_text(const char *text_data) {
    int res = 0;
    uint8_t ndef_buffer[48] = {0}; // 3个块，每块16字节
    uint8_t ndef_len = 0;

    // 参数检查
    if (!text_data) {
        PRINT("ERR: Invalid text data\r\n");
        return -1;
    }

    int text_len = strlen(text_data);
    if (text_len > 36) {
        // 考虑NDEF头部开销，限制文本长度
        PRINT("ERR: Text too long (max 40 chars)\r\n");
        return -2;
    }

    // 构建NDEF格式数据
    // NDEF Text Record 格式:
    // [Header][Type Length][Payload Length][Type][Lang Code Length + Lang][Text]

    uint8_t lang_code[] = "en"; // 语言代码
    uint8_t lang_len = strlen((char *) lang_code);
    uint8_t payload_len = 1 + lang_len + text_len; // 语言长度字节 + 语言代码 + 文本

    uint8_t offset = 0;
    // 添加前缀 0000030C
    ndef_buffer[offset++] = 0x00;
    ndef_buffer[offset++] = 0x00;
    ndef_buffer[offset++] = 0x03;
    ndef_buffer[offset++] = 0x00;// 预留位置，稍后填充NDEF消息长度

    // NDEF消息头 (MB=1, ME=1, CF=0, SR=1, IL=0, TNF=001)
    ndef_buffer[offset++] = 0xD1; // 11010001b

    // Type Length (T=Text)
    ndef_buffer[offset++] = 0x01;

    // Payload Length
    ndef_buffer[offset++] = payload_len;

    // Type ("T" for Text)
    ndef_buffer[offset++] = 'T';

    // Language Code Length (bit7=0表示UTF-8编码, bit0-5=语言代码长度)
    ndef_buffer[offset++] = lang_len;

    // Language Code
    memcpy(&ndef_buffer[offset], lang_code, lang_len);
    offset += lang_len;

    // Text Data
    memcpy(&ndef_buffer[offset], text_data, text_len);
    offset += text_len;

    ndef_buffer[offset++]=0xFE; // 结束标记
    ndef_len = offset;
    ndef_buffer[3]=ndef_len-5;
    PRINT("NDEF message length: %d bytes\r\n", ndef_len);

    // 第1扇区鉴权 (使用KeyB方式)
    res = PcdAuthState(PICC_AUTHENT1B, 4, default_key, picc_uid);
    if (res != PCD_NO_ERROR) {
        PRINT("ERR: Sector 0 authentication failed: 0x%x\r\n", res);
        return res;
    }

    uint8_t block_addresses[3] = {4, 5, 6};

    // 依次写入三个块
    for (int i = 0; i < 3; i++) {
        // 准备16字节的块数据
        uint8_t block_buffer[16];
        memcpy(block_buffer, &ndef_buffer[i * 16], 16);

        // 写入当前块
        res = PcdWrite(block_addresses[i], block_buffer);
        if (res != PCD_NO_ERROR) {
            PRINT("ERR: Block %d write failed: 0x%x\r\n", block_addresses[i], res);
            return res;
        }

        PRINT("Block %d written successfully\r\n", block_addresses[i]);
    }

    PRINT("NDEF text written to NFC successfully\r\n");
    return 0; // 成功
}

bool NFC_Work(char *data) {
    bool Work_Result = false;
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
                uint8_t result = 0;
                if (isWritting) {
                    result = nfc_write_ndef_text(WriteData);
                    if (result == 0) {
                        isWritting = false;
                        PRINT("写入成功: %s\r\n", WriteData);
                        memset(WriteData, 0, sizeof(WriteData));
                    }else {
                        PRINT("写入失败, 错误码: 0x%x\r\n", result);
                    }
                } else {
                    result = ReadNFC_CID(data);
                    if (result == 0) {
                        PRINT("Successfully read CID: %s\r\n", data);
                        Work_Result = true;
                    } else {
                        PRINT("Failed to read CID, error: 0x%x\r\n", result);
                    }
                }
            }
        }
    }
    nfca_pcd_stop();
    return Work_Result;
}
