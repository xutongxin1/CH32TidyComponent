//
// Created by xtx on 25-7-1.
//

#ifndef NFC_WORK_H
#define NFC_WORK_H
#include <stdbool.h>
void NFC_Init();
bool NFC_Start();
bool NFC_Work(char *data);
#endif //NFC_WORK_H
