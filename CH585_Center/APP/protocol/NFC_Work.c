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
uint8_t b_key[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t ReadNFC_CID_text(char *data) {
    uint8_t res = 0;
    uint16_t text_pos = 0;

    if (data == NULL) {
        return 0xFF;
    }

    // 认证扇区1 (基于你的数据，文本在block 4-5)
    res = PcdAuthState(PICC_AUTHENT1B, 4, b_key, picc_uid);
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

uint8_t ReadNFC_CID_url(char *cid_data) {
    uint8_t res = 0;
    char url_buffer[128] = {0}; // 存储完整URL
    uint16_t url_pos = 0;
    const char *prefix = "https://api.h49591b27.nyat.app:10268/url/";

    if (cid_data == NULL) {
        return 0xFF;
    }

    // 认证扇区1
    res = PcdAuthState(PICC_AUTHENT1B, 4, b_key, picc_uid);
    if (res) {
        return res;
    }

    // 读取block 4, 5, 6 来获取完整URL数据
    for (uint8_t block = 4; block <= 6; block++) {
        res = PcdRead(block);
        if (res) {
            return res;
        }

        // URL数据从block 4的第9个字节开始（跳过NDEF头和URL前缀标识符04）
        uint8_t start_pos = (block == 4) ? 9 : 0;

        for (uint8_t j = start_pos; j < 16; j++) {
            uint8_t byte_val = g_nfca_pcd_recv_buf[j];

            // 检查是否为可打印字符
            if (byte_val >= 0x20 && byte_val <= 0x7E) {
                if (url_pos < sizeof(url_buffer) - 1) {
                    url_buffer[url_pos++] = (char)byte_val;
                }
            }
            // 遇到结束标记
            else if (byte_val == 0xFE || byte_val == 0x00) {
                url_buffer[url_pos] = '\0';
                goto parse_url; // 跳出双重循环
            }
        }
    }

    // 如果URL很长，可能需要读取扇区2
    res = PcdAuthState(PICC_AUTHENT1B, 8, b_key, picc_uid);
    if (res == 0) { // 如果扇区2认证成功，继续读取
        for (uint8_t block = 8; block <= 10; block++) {
            res = PcdRead(block);
            if (res) {
                break; // 读取失败就停止
            }

            for (uint8_t j = 0; j < 16; j++) {
                uint8_t byte_val = g_nfca_pcd_recv_buf[j];

                if (byte_val >= 0x20 && byte_val <= 0x7E) {
                    if (url_pos < sizeof(url_buffer) - 1) {
                        url_buffer[url_pos++] = (char)byte_val;
                    }
                }
                else if (byte_val == 0xFE || byte_val == 0x00) {
                    url_buffer[url_pos] = '\0';
                    goto parse_url;
                }
            }
        }
    }

parse_url:
    url_buffer[url_pos] = '\0';

    // 组装完整URL（添加https://前缀，因为NDEF中04标识符代表https://）
    char full_url[256];
    snprintf(full_url, sizeof(full_url), "https://%s", url_buffer);

    // 查找固定前缀并提取CID
    char *prefix_pos = strstr(full_url, prefix);
    if (prefix_pos != NULL) {
        // 找到前缀，提取CID部分
        char *cid_start = prefix_pos + strlen(prefix);

        // 复制CID到输出缓冲区
        int cid_pos = 0;
        while (*cid_start != '\0' && cid_pos < 63) { // 限制CID长度
            cid_data[cid_pos++] = *cid_start++;
        }
        cid_data[cid_pos] = '\0';

        return 0; // 成功
    }

    // 如果没有找到预期的前缀，返回错误
    cid_data[0] = '\0';
    return 0xFE; // 前缀不匹配错误
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
    ndef_buffer[offset++] = 0x00; // 预留位置，稍后填充NDEF消息长度

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

    ndef_buffer[offset++] = 0xFE; // 结束标记
    ndef_len = offset;
    ndef_buffer[3] = ndef_len - 5;
    PRINT("NDEF message length: %d bytes\r\n", ndef_len);

    res = PcdAuthState(PICC_AUTHENT1B, 0, b_key, picc_uid);
    if (res != PCD_NO_ERROR) {
        PRINT("ERR: Sector 0 authentication failed: 0x%x\r\n", res);
        return res;
    }
    uint8_t block1[16] = {
        0x14,0x01,0x03,0xE1,0x03,0xE1,0x03,0xE1,0x03,0xE1,0x03,0xE1,0x03,0xE1,0x03,0xE1
    };
    res = PcdWrite(1, block1);
    if (res != PCD_NO_ERROR) {
        PRINT("ERR: Block %d write failed: 0x%x\r\n", 1, res);
        return res;
    }
    uint8_t block2[16] = {
        0x03,0xE1,0x03,0xE1,0x03,0xE1,0x03,0xE1,0x03,0xE1,0x03,0xE1,0x03,0xE1,0x03,0xE1
    };
    res = PcdWrite(2, block2);
    if (res != PCD_NO_ERROR) {
        PRINT("ERR: Block %d write failed: 0x%x\r\n", 2, res);
        return res;
    }
    uint8_t block3[16] = {
        0xA0,0xA1,0xA2,0xA3,0xA4,0xA5,0xFF,0x07,0x80,0x69,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
    };
    res = PcdWrite(3, block3);
    if (res != PCD_NO_ERROR) {
        PRINT("ERR: Block %d write failed: 0x%x\r\n", 3, res);
        return res;
    }

    // 第1扇区鉴权 (使用KeyB方式)
    res = PcdAuthState(PICC_AUTHENT1B, 4, b_key, picc_uid);
    if (res != PCD_NO_ERROR) {
        PRINT("ERR: Sector 1 authentication failed: 0x%x\r\n", res);
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
    uint8_t block7[16] = {
        0xD3, 0xF7, 0xD3, 0xF7, 0xD3, 0xF7, 0x7F, 0x07, 0x88, 0x40, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
    };
    res = PcdWrite(7, block7);
    if (res != PCD_NO_ERROR) {
        PRINT("ERR: Block %d write failed: 0x%x\r\n", 7, res);
        return res;
    }

    PRINT("Block %d written successfully\r\n", 7);

    PRINT("NDEF text written to NFC successfully\r\n");
    return 0; // 成功
}

int nfc_write_ndef_url(const char *url_data) {
    int res = 0;
    uint8_t ndef_buffer[96] = {0}; // 6个块，每块16字节
    uint8_t ndef_len = 0;

    // 参数检查
    if (!url_data) {
        PRINT("ERR: Invalid URL data\r\n");
        return -1;
    }

    int url_len = strlen(url_data);
    if (url_len > 80) { // 考虑NDEF头部开销，限制URL长度
        PRINT("ERR: URL too long (max 80 chars)\r\n");
        return -2;
    }

    // 确定URL前缀标识符
    uint8_t prefix_id = 0x00;
    const char *url_without_prefix = url_data;

    if (strncmp(url_data, "https://", 8) == 0) {
        prefix_id = 0x04;
        url_without_prefix = url_data + 8;
    } else if (strncmp(url_data, "http://", 7) == 0) {
        prefix_id = 0x03;
        url_without_prefix = url_data + 7;
    } else if (strncmp(url_data, "mailto:", 7) == 0) {
        prefix_id = 0x06;
        url_without_prefix = url_data + 7;
    } else if (strncmp(url_data, "tel:", 4) == 0) {
        prefix_id = 0x05;
        url_without_prefix = url_data + 4;
    }
    // 如果没有匹配的前缀，使用完整URL

    int url_without_prefix_len = strlen(url_without_prefix);
    uint8_t payload_len = 1 + url_without_prefix_len; // 前缀ID + URL数据
    uint8_t offset = 0;

    // 构建NDEF格式数据
    // 添加前缀 000003xx (xx是NDEF消息长度)
    ndef_buffer[offset++] = 0x00;
    ndef_buffer[offset++] = 0x00;
    ndef_buffer[offset++] = 0x03;
    ndef_buffer[offset++] = 0x00; // 预留位置，稍后填充NDEF消息长度

    // NDEF消息头 (MB=1, ME=1, CF=0, SR=1, IL=0, TNF=001)
    ndef_buffer[offset++] = 0xD1; // 11010001b

    // Type Length (U=URL)
    ndef_buffer[offset++] = 0x01;

    // Payload Length
    ndef_buffer[offset++] = payload_len;

    // Type ("U" for URL)
    ndef_buffer[offset++] = 'U';

    // URL前缀标识符
    ndef_buffer[offset++] = prefix_id;

    // URL数据 (去掉前缀的部分)
    memcpy(&ndef_buffer[offset], url_without_prefix, url_without_prefix_len);
    offset += url_without_prefix_len;

    ndef_buffer[offset++] = 0xFE; // 结束标记

    ndef_len = offset;
    ndef_buffer[3] = ndef_len - 5; // 填充NDEF消息长度

    PRINT("NDEF URL message length: %d bytes\r\n", ndef_len);

    // 第0扇区鉴权和写入 (和原函数相同的配置块)
    res = PcdAuthState(PICC_AUTHENT1B, 0, b_key, picc_uid);
    if (res != PCD_NO_ERROR) {
        PRINT("ERR: Sector 0 authentication failed: 0x%x\r\n", res);
        return res;
    }

    uint8_t block1[16] = { 0x14,0x01,0x03,0xE1,0x03,0xE1,0x03,0xE1,0x03,0xE1,0x03,0xE1,0x03,0xE1,0x03,0xE1 };
    res = PcdWrite(1, block1);
    if (res != PCD_NO_ERROR) {
        PRINT("ERR: Block %d write failed: 0x%x\r\n", 1, res);
        return res;
    }

    uint8_t block2[16] = { 0x03,0xE1,0x03,0xE1,0x03,0xE1,0x03,0xE1,0x03,0xE1,0x03,0xE1,0x03,0xE1,0x03,0xE1 };
    res = PcdWrite(2, block2);
    if (res != PCD_NO_ERROR) {
        PRINT("ERR: Block %d write failed: 0x%x\r\n", 2, res);
        return res;
    }

    uint8_t block3[16] = { 0xA0,0xA1,0xA2,0xA3,0xA4,0xA5,0xFF,0x07,0x80,0x69,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF };
    res = PcdWrite(3, block3);
    if (res != PCD_NO_ERROR) {
        PRINT("ERR: Block %d write failed: 0x%x\r\n", 3, res);
        return res;
    }

    // 第1扇区鉴权 (使用KeyB方式)
    res = PcdAuthState(PICC_AUTHENT1B, 4, b_key, picc_uid);
    if (res != PCD_NO_ERROR) {
        PRINT("ERR: Sector 1 authentication failed: 0x%x\r\n", res);
        return res;
    }

    uint8_t block_addresses_s1[3] = {4, 5, 6}; // 第1扇区的前三个块

    // 依次写入扇区1的三个块
    for (int i = 0; i < 3; i++) {
        uint8_t block_buffer[16] = {0};

        // 复制对应的16字节数据到块缓冲区
        if ((i * 16) < ndef_len) {
            int copy_len = (ndef_len - i * 16) > 16 ? 16 : (ndef_len - i * 16);
            memcpy(block_buffer, &ndef_buffer[i * 16], copy_len);
        }

        res = PcdWrite(block_addresses_s1[i], block_buffer);
        if (res != PCD_NO_ERROR) {
            PRINT("ERR: Block %d write failed: 0x%x\r\n", block_addresses_s1[i], res);
            return res;
        }
        PRINT("Block %d written successfully\r\n", block_addresses_s1[i]);
    }

    // 写入扇区1的尾块
    uint8_t block7[16] = { 0xD3, 0xF7, 0xD3, 0xF7, 0xD3, 0xF7, 0x7F, 0x07, 0x88, 0x40, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    res = PcdWrite(7, block7);
    if (res != PCD_NO_ERROR) {
        PRINT("ERR: Block %d write failed: 0x%x\r\n", 7, res);
        return res;
    }
    PRINT("Block %d written successfully\r\n", 7);

    // 如果NDEF数据超过48字节，需要写入第2扇区
    if (ndef_len > 48) {
        // 第2扇区鉴权 (使用KeyB方式)
        res = PcdAuthState(PICC_AUTHENT1B, 8, b_key, picc_uid);
        if (res != PCD_NO_ERROR) {
            PRINT("ERR: Sector 2 authentication failed: 0x%x\r\n", res);
            return res;
        }

        uint8_t block_addresses_s2[3] = {8, 9, 10}; // 第2扇区的前三个块

        // 依次写入扇区2的三个块
        for (int i = 0; i < 3; i++) {
            uint8_t block_buffer[16] = {0}; // 初始化为0
            int src_offset = 48 + i * 16; // 从第48字节开始

            // 如果还有数据需要写入
            if (src_offset < ndef_len) {
                int copy_len = (ndef_len - src_offset) > 16 ? 16 : (ndef_len - src_offset);
                memcpy(block_buffer, &ndef_buffer[src_offset], copy_len);
            }

            res = PcdWrite(block_addresses_s2[i], block_buffer);
            if (res != PCD_NO_ERROR) {
                PRINT("ERR: Block %d write failed: 0x%x\r\n", block_addresses_s2[i], res);
                return res;
            }
            PRINT("Block %d written successfully\r\n", block_addresses_s2[i]);
        }

        // 写入扇区2的尾块
        uint8_t block11[16] = { 0xD3, 0xF7, 0xD3, 0xF7, 0xD3, 0xF7, 0x7F, 0x07, 0x88, 0x40, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
        res = PcdWrite(11, block11);
        if (res != PCD_NO_ERROR) {
            PRINT("ERR: Block %d write failed: 0x%x\r\n", 11, res);
            return res;
        }
        PRINT("Block %d written successfully\r\n", 11);
    }

    PRINT("NDEF URL written to NFC successfully\r\n");
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
                    char tmp[100]={0};
                    sprintf(tmp,"https://api.h49591b27.nyat.app:10268/url/%s", WriteData);
                    result = nfc_write_ndef_url(tmp);
                    if (result == 0) {
                        ReadNFC_CID_url(data);
                        Work_Result = true;
                        PRINT("写入成功: %s\r\n", tmp);
                        memset(WriteData, 0, sizeof(WriteData));
                    } else {
                        PRINT("写入失败, 错误码: 0x%x\r\n", result);
                    }
                } else {
                    result = ReadNFC_CID_url(data);
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
