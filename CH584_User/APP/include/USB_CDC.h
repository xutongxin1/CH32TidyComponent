//
// Created by xtx on 25-5-14.
//

#ifndef USB_CDC_H
#define USB_CDC_H
#include "CH58x_common.h"

#define dg_log printf
// #define dg_log

void USB_IRQProcessHandler( void );
void InitUSBDevPara(void);
void InitUSBDevice(void);
UINT8 SendUSBData(UINT8 *p_send_dat,UINT16 send_len);
#endif //USB_CDC_H
