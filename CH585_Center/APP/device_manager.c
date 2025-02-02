//
// Created by xtx on 25-1-31.
//
/* device_manager.c */
#include "device_manager.h"
#include "CONFIG.h"
#include "MESH_LIB.h"
#include "HAL.h"
#include "app_mesh_config.h"
#include "app.h"
#include <string.h>

/* 全局设备存储数组 (4个字节对齐) */
static __attribute__((aligned(4))) DeviceInfo g_devices[MAX_DEVICES];

int ReadAllDevices(void) {
    return EEPROM_READ(FLASH_START_ADDR, g_devices, sizeof(g_devices));
}

int WriteDevice(int index) {
    if (index < 0 || index >= MAX_DEVICES) return -1;

    const uint32_t addr = FLASH_START_ADDR + index * sizeof(DeviceInfo);
    EEPROM_ERASE(addr, sizeof(DeviceInfo));
    return EEPROM_WRITE(addr, &g_devices[index], sizeof(DeviceInfo));
}

bool GetMacByMesh(uint16_t meshAddr, uint8_t mac[MAC_ADDR_SIZE]) {
    if (meshAddr > MESH_ADDR_MAX) {
        memset(mac, 0, MAC_ADDR_SIZE);
        return false;
    }

    for (int i = 0; i < MAX_DEVICES; i++) {
        if (g_devices[i].MeshAddr == meshAddr) {
            memcpy(mac, g_devices[i].MacAddr, MAC_ADDR_SIZE);
            return true;
        }
    }

    memset(mac, 0, MAC_ADDR_SIZE);
    return false;
}

uint16_t GetMeshByMac(const uint8_t mac[MAC_ADDR_SIZE]) {
    for (int i = 0; i < MAX_DEVICES; i++) {
        if (memcmp(g_devices[i].MacAddr, mac, MAC_ADDR_SIZE) == 0) {
            return g_devices[i].MeshAddr;
        }
    }
    return INVALID_MESH_ADDR;
}
int AddDeviceNode(uint16_t meshAddr, const uint8_t mac[MAC_ADDR_SIZE], uint8_t devType) {
    // 查找空索引（MeshAddr为0表示未使用）
    for (int i = 0; i < MAX_DEVICES; i++) {
        if (g_devices[i].MeshAddr == INVALID_MESH_ADDR) {
            g_devices[i].MeshAddr = meshAddr;
            memcpy(g_devices[i].MacAddr, mac, MAC_ADDR_SIZE);
            SET_ACTIVE(g_devices[i].flags, true);
            SET_DEV_TYPE(g_devices[i].flags, devType);
            memset(g_devices[i].reserved, 0, sizeof(g_devices[i].reserved));

            if (WriteDevice(i) == 0) return i;
            return -1; // 写入失败
        }
    }
    return -1; // 设备池已满
}

void DeactivateAllDevices(void) {
    for (int i = 0; i < MAX_DEVICES; i++) {
        SET_ACTIVE(g_devices[i].flags, false);
        // WriteDevice(i); // 需要优化可以批量写入
    }
}

int DeleteDeviceByMac(const uint8_t mac[MAC_ADDR_SIZE]) {
    for (int i = 0; i < MAX_DEVICES; i++) {
        if (memcmp(g_devices[i].MacAddr, mac, MAC_ADDR_SIZE) == 0) {
            g_devices[i].MeshAddr = INVALID_MESH_ADDR;
            memset(g_devices[i].MacAddr, 0, MAC_ADDR_SIZE);
            g_devices[i].flags = 0;
            return WriteDevice(i);
        }
    }
    return -1;
}

uint16_t FindAvailableMeshAddr(void) {
    uint16_t candidate = 0x0002;
    bool used[MESH_ADDR_MAX + 1] = {false};

    // 标记已用地址
    for (int i = 0; i < MAX_DEVICES; i++) {
        if (g_devices[i].MeshAddr != INVALID_MESH_ADDR) {
            used[g_devices[i].MeshAddr] = true;
        }
    }

    // 线性查找最小可用地址
    while (candidate <= MESH_ADDR_MAX) {
        if (!used[candidate]) return candidate;
        candidate++;
    }
    return INVALID_MESH_ADDR;
}

int UpdateDeviceActivation(uint16_t meshAddr, bool isActive) {
    for (int i = 0; i < MAX_DEVICES; i++) {
        if (g_devices[i].MeshAddr == meshAddr) {
            SET_ACTIVE(g_devices[i].flags, isActive);
            return 0;
        }
    }
    return -1;
}


