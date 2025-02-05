#ifndef ERROR_CODE_H
#define ERROR_CODE_H 

#include <stdint.h>

typedef uint32_t error_code_t;

#define    ERROR_CODE_SUCCESS             0x00000000                 // 操作成功
#define    ERROR_CODE_ERROR               0xFFFFFFFF                 // 通用错误
#define    ERROR_CODE_TIMEOUT             0x00000001                 // 超时错误
#define    ERROR_CODE_INVALID_PARAMETER   0x00000002                 // 无效参数
#define    ERROR_CODE_NOT_INITIALIZED     0x00000003                 // 未初始化
#define    ERROR_CODE_DEVICE_NOT_FOUND    0x00000004                 // 设备未找到
#define    ERROR_CODE_COMMUNICATION_ERROR 0x00000005                 // 通信错误
#define    ERROR_CODE_BUFFER_OVERFLOW     0x00000006                 // 缓冲区溢出
#define    ERROR_CODE_OUT_OF_MEMORY       0x00000007                 // 内存不足
#define    ERROR_CODE_INVALID_STATE       0x00000008                 // 无效状态
#define    ERROR_CODE_PERMISSION_DENIED   0x00000009                 // 权限被拒绝
#define    ERROR_CODE_UNKNOWN_COMMAND     0x0000000A                 // 未知命令
#define    ERROR_CODE_HARDWARE_ERROR      0x0000000B                 // 硬件错误
#define    ERROR_CODE_CHECKSUM_ERROR      0x0000000C                 // 校验和错误
#define    ERROR_CODE_INVALID_DATA        0x0000000D                 // 无效数据
#define    ERROR_CODE_BUSY                0x0000000E                 // 设备繁忙
#define    ERROR_CODE_NOT_SUPPORTED       0x0000000F                 // 功能不支持
#define    ERROR_CODE_INTERRUPTED         0x00000010                 // 操作被中断
#define    ERROR_CODE_INTERNAL_ERROR      0x00000011                 // 内部错误

#define    DEV_ERROR_CODE_BASE            0x80001000                 // 设备错误基址
#define    ERROR_CODE_DEV_NOT_FOUND       0x80001001                 // 设备未找到
#define    ERROR_CODE_DEV_NOT_SUPPORT     0x80001002                 // 设备不支持


#endif // ERROR_H

