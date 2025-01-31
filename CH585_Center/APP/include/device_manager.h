/* device_manager.h */
#ifndef DEVICE_MANAGER_H
#define DEVICE_MANAGER_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_DEVICES 256         ///< 最大设备数量
#define FLASH_START_ADDR 0x0000 ///< Flash起始地址
#define MAC_ADDR_SIZE 6         ///< MAC地址长度
#define MESH_ADDR_MAX 0x7FFF    ///< Mesh地址最大值
#define INVALID_MESH_ADDR 0x0000///< 无效Mesh地址标识

#pragma pack(push, 1)
/**
 * @brief 设备信息结构体
 *
 * 完整占用16字节存储空间，包含设备网络信息及状态标志
 */
typedef struct {
    uint16_t MeshAddr;    ///< 2字节Mesh网络地址
    uint8_t MacAddr[6];   ///< 6字节物理MAC地址
    uint8_t flags;        ///< 状态标志位 (bit0:激活状态, bit1-2:设备类型)
    uint8_t reserved[7];  ///< 7字节保留空间
} DeviceInfo;
#pragma pack(pop)

/* 设备状态操作宏 */
/** @brief 判断设备是否激活 */
#define IS_ACTIVE(f) ((f) & 0x01)
/** @brief 设置激活状态 (a=true/false) */
#define SET_ACTIVE(f, a) ((a) ? ((f) | 0x01) : ((f) & ~0x01))
/** @brief 获取设备类型 (返回1-3) */
#define GET_DEV_TYPE(f) (((f) >> 1) & 0x03)
/** @brief 设置设备类型 (t=1-3) */
#define SET_DEV_TYPE(f, t) ((f) = ((f) & ~0x06) | (((t) & 0x03) << 1))

/**
 * @brief 从Flash读取全部设备数据
 * @return 0-成功 其他-失败错误码
 */
int ReadAllDevices(void);

/**
 * @brief 写入单个设备数据到Flash
 * @param index 设备索引 (0~255)
 * @return 0-成功 -1-索引错误
 */
int WriteDevice(int index);

/**
 * @brief 通过Mesh地址查找MAC地址
 * @param meshAddr 要查询的Mesh地址
 * @param[out] mac 输出MAC地址缓冲区
 * @return true-找到 false-未找到
 */
bool GetMacByMesh(uint16_t meshAddr, uint8_t mac[MAC_ADDR_SIZE]);

/**
 * @brief 通过MAC地址查找Mesh地址
 * @param mac 要查询的MAC地址
 * @return 找到的Mesh地址，未找到返回INVALID_MESH_ADDR
 */
uint16_t GetMeshByMac(const uint8_t mac[MAC_ADDR_SIZE]);

/**
 * @brief 安全设置Mesh地址
 * @param index 设备索引 (0~255)
 * @param meshAddr 要设置的Mesh地址 (<=0x7FFF)
 * @return 0-成功 -1-索引错误 -2-地址超限
 */
int SetMeshAddress(int index, uint16_t meshAddr);
/**
 * @brief 添加新设备节点
 * @param meshAddr 分配的Mesh地址 (需确保未重复)
 * @param mac 设备MAC地址
 * @param devType 设备类型 (1-3)
 * @return 成功返回设备索引，失败返回-1
 */
int AddDeviceNode(uint16_t meshAddr, const uint8_t mac[MAC_ADDR_SIZE], uint8_t devType);

/**
 * @brief 重置所有设备激活状态
 */
void DeactivateAllDevices(void);

/**
 * @brief 通过MAC地址删除设备
 * @param mac 要删除设备的MAC地址
 * @return 成功返回0，未找到返回-1
 */
int DeleteDeviceByMac(const uint8_t mac[MAC_ADDR_SIZE]);

/**
 * @brief 查找第一个可用的Mesh地址
 * @return 可用地址 (0x0001~0x7FFF)，无可用返回INVALID_MESH_ADDR
 */
uint16_t FindAvailableMeshAddr(void);

/**
 * @brief 更新设备激活状态
 * @param meshAddr 目标设备地址
 * @param isActive 激活状态
 * @return 成功0，失败-1
 */
int UpdateDeviceActivation(uint16_t meshAddr, bool isActive);

#endif // DEVICE_MANAGER_H