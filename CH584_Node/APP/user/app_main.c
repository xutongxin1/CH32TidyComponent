/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.1
 * Date               : 2022/01/18
 * Description        :
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

/******************************************************************************/
/* 头文件包含 */
#include <data_transfer.h>
#include <I2C.h>
#include <mesh_uart_test.h>
#include <TCA9555.h>
#include <WS2812.h>

#include "CONFIG.h"
#include "MESH_LIB.h"
#include "HAL.h"
#include "app_mesh_config.h"
#include "app_mesh.h"
#include "device_type_define.h"
#include "ScanIO.h"
#include "B53_driver.h"
#include "led_manager.h"
/*********************************************************************
 * GLOBAL TYPEDEFS
 */
__attribute__ ((aligned (4))) uint32_t MEM_BUF[BLE_MEMHEAP_SIZE / 4];
extern uint8_t Main_App_TaskID; // Task ID for internal task/event processing

#ifdef ENABLE_MESH_UART_TEST
extern uint8_t mesh_uart_isWorking;
extern char mesh_uart_sendBuf[128];
extern uint16_t mesh_uart_addr;
extern uint8_t mesh_uart_dataType;
#endif

bool isMeshConnected = false; // 是否连接到Mesh网络
bool isDebugLED = false;
/*********************************************************************
 * @fn      Main_Circulation
 *
 * @brief   主循环
 *
 * @return  none
 */
__HIGH_CODE
__attribute__ ((noinline)) void Main_Circulation() {
    while (1) {
        TMOS_SystemProcess();
    }
}

void SelfCheck(void) {
    GPIOB_ModeCfg(GPIO_Pin_8, GPIO_ModeIN_PU);
    // if (GPIOB_ReadPortPin(GPIO_Pin_8) == 0) // 配网重置按键
    // {
    //     bt_mesh_reset();
    //     PRINT("重置配网\r\n");
    // }

    for (int i = 0; i < 8; i++) {
        if (DeviceExists[i]) {
            TCA_WritePin(0x20 + i, P17, 1);
        }
    }

    wheelLed();

    if (GPIOB_ReadPortPin(GPIO_Pin_8) == 0) // 测试模式按键
    {
        ws2812_set_led_hex(0, 0x000066, LED_MODE_FLASH_FAST_3);
        PRINT("进入检测功能Debug模式\r\n");
        isDebugLED = true;
        tmos_start_task(Main_App_TaskID, APP_SCANIO, K_MSEC(100));
    } else {
        ws2812_set_led_hex(0, 0x666600, LED_MODE_FLASH_SLOW);
        tmos_start_task(Main_App_TaskID, APP_SCANIO, K_MSEC(1000));
    }

    for (int i = 0; i < 8; i++) {
        if (DeviceExists[i]) {
            TCA_WritePin(0x20 + i, P17, 0);
        }
    }
}
/*********************************************************************
 * @fn      main
 *
 * @brief   主函数
 *
 * @return  none
 */
int main(void) {
    HSECFG_Capacitance(HSECap_18p);
    SetSysClock(CLK_SOURCE_HSE_PLL_78MHz);

#ifdef DEBUG
    // GPIOA_SetBits(GPIO_Pin_14);
    // GPIOPinRemap(ENABLE, RB_PIN_UART0);
    // GPIOA_ModeCfg(GPIO_Pin_15, GPIO_ModeIN_PU);
    // GPIOA_ModeCfg(GPIO_Pin_14, GPIO_ModeOut_PP_5mA);

    GPIOB_SetBits(GPIO_Pin_7);
    GPIOB_ModeCfg(GPIO_Pin_4, GPIO_ModeIN_PU);
    GPIOB_ModeCfg(GPIO_Pin_7, GPIO_ModeOut_PP_5mA);

    UART0_DefInit();
    PRINT("Working\r\n");
#endif

    // 初始化I2C
    CH58X_I2C_Init();

#ifdef DEVICE_TYPE_B55
    CheckB53_();
#endif

    // 初始化WS2812
    WS2812Init(); // PB22
    led_manager_init();

    // 初始化蓝牙
    PRINT("%s\r\n", (char *) VER_LIB);
    PRINT("%s\r\n", (char *)VER_MESH_LIB);
    CH58x_BLEInit();
    HAL_Init();
    bt_mesh_lib_init();
    App_Init();

    printf("MAC地址");
    for (int i = 0; i < 6; i++) {
        printf("%02X", MACAddr[i]);
    }
    printf("\r\n");

#ifdef ENABLE_MESH_UART_TEST
    InitMESHUartTest();
#endif

    SelfCheck(); //自检

    PRINT("进入主循环\r\n");

    tmos_start_task(Main_App_TaskID, APP_WS2812, MS1_TO_SYSTEM_TIME(200));
    Main_Circulation();
}

/*********************************************************************
 * @fn      App_ProcessEvent
 *
 * @brief   应用层事件处理函数
 *
 * @param   task_id  - The TMOS assigned task ID.
 * @param   events - events to process.  This is a bit map and can
 *                   contain more than one event.
 *
 * @return  events not processed
 */
uint16_t App_ProcessEvent(uint8_t task_id, uint16_t events) {
    if (events & APP_NODE_TEST_EVT) {
        if (isDebugLED) {
            tmos_start_task(Main_App_TaskID, APP_NODE_TEST_EVT, MS1_TO_SYSTEM_TIME(100));
            if (GPIOB_ReadPortPin(GPIO_Pin_8) == 0) {
                for (int i = 0; i < 8; i++) {
                    if (DeviceExists[i]) {
                        TCA_WritePin(0x20 + i, P17, 1);
                    }
                }
            }
            else {
                for (int i = 0; i < 8; i++) {
                    if (DeviceExists[i]) {
                        TCA_WritePin(0x20 + i, P17, 0);
                    }
                }
            }
        } else {
            tmos_start_task(Main_App_TaskID, APP_NODE_TEST_EVT, MS1_TO_SYSTEM_TIME(1000));
        }

        ScanIO();
        led_manager_update();

        return (events ^ APP_NODE_TEST_EVT);
    }

    if (events & APP_WS2812) {
        ws2812_update();
        tmos_start_task(Main_App_TaskID, APP_WS2812, MS1_TO_SYSTEM_TIME(50));
        return (events ^ APP_WS2812);
    }

    if (events & APP_CHECK_PENDING_PACKETS) {
        CheckPendingPackets();
        tmos_start_task(Main_App_TaskID, APP_CHECK_PENDING_PACKETS, K_MSEC(100));
        return (events ^ APP_CHECK_PENDING_PACKETS);
    }

    if (events & APP_DELETE_LOCAL_NODE_EVT) {
        // 收到删除命令，删除自身网络信息
        APP_DBG("Delete local node");
        // 复位自身网络状态
        bt_mesh_reset();
        return (events ^ APP_DELETE_LOCAL_NODE_EVT);
    }

    if (events & APP_SCANIO) {
        tmos_start_task(Main_App_TaskID, APP_SCANIO, K_MSEC(500));
#ifdef ENABLE_MESH_UART_TEST
        if (mesh_uart_isWorking) {
            mesh_uart_isWorking = 0;
            SendData(mesh_uart_addr, mesh_uart_dataType, mesh_uart_sendBuf);
#ifdef ENABLE_MESH_UART_DEBUG
            // 回传验证数据
            PRINT("测试接口发送数据 0x%04X地址: Type=%d, Data=%s\r\n", mesh_uart_addr, mesh_uart_dataType, mesh_uart_sendBuf);
#endif
        }
#endif

        return (events ^ APP_SCANIO);
    }

    // Discard unknown events
    return 0;
}

/******************************** endfile @ main ******************************/
