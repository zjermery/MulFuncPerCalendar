/**
  **********************************************************************************************************************
  * @file    cot_key.h
  * @brief   该文件提供按键动作识别功能函数原型
  * @author  const_zpc    any question please send mail to const_zpc@163.com
  * @version V1.0.0
  * @date    2023-11-05
  **********************************************************************************************************************
  *
  **********************************************************************************************************************
  */

/* Define to prevent recursive inclusion -----------------------------------------------------------------------------*/
#ifndef _COT_KEY_H_
#define _COT_KEY_H_

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/**
  * @brief 定义宏：启动按键新配置延迟更新替换
  *        只有在按键空闲的状态下才真正更新配置信息，防止上层应用程序在某些场景更新时出现意想不到的问题
  * @note  如果上层应用程序只有在初始化时才更新配置，那么可以关闭该功能，降低内存占用
  */
#define _CONFIG_DELAY_UPDATE_

/* Exported types ----------------------------------------------------------------------------------------------------*/

/** 触发方式 */
#define COT_KEY_TRIGGER_WAY_PRESS           0x01  // 按下触发
#define COT_KEY_TRIGGER_WAY_LOSSEN          0x02  // 松开触发

/**
  * @brief  KEY开关枚举定义
  */
typedef enum{
    COT_KEY_OFF = 0,                         	/*!< (0)断开(无动作) */
    COT_KEY_ON = !COT_KEY_OFF                   /*!< (1)闭合(有动作) */
} cotKeyState_e;

typedef uint16_t cotKey_t;
typedef cotKeyState_e (*cotKeyState_f)(void);
typedef void (*cotKeyListen_f)(cotKey_t);

typedef struct
{
    uint8_t mode;                               /*!< 按键模式 */
    uint8_t triggerWay;                         /*!< 按键事件触发回调函数的触发条件 */
    uint16_t triggerInterval;                   /*!< 长时间按下时触发回调函数的间隔时间 */
    uint16_t longTime;                          /*!< 区分短按和长按的时间, 该参数单位为1ms */
    cotKeyListen_f pfnEventCallback;            /*!< 按键事件触发回调函数 */
} cotKeyInfo_t;

/**
  * @brief 按键动作信息结构体定义
  */
typedef struct
{
    cotKeyState_e eKeyStateBak;                 /*!< 按键上次开关状态 */
    cotKeyState_e eKeyState;                    /*!< 按键当前开关状态 */
    uint8_t clickCnt;                           /*!< 指定所选按键的连击次数 */
    uint16_t uiPressTic;                        /*!< 按键按下累计时间, 该参数单位为1ms */
    uint16_t uiLossenTic;                       /*!< 按键松开累计时间, 该参数单位为1ms */
    uint8_t state;                              /*!< 按键状态 */
    cotKeyInfo_t curConfig;                     /*!< 按键当前配置 */
#if defined(_CONFIG_DELAY_UPDATE_)
    cotKeyInfo_t newConfig;                     /*!< 按键新的配置 */
#endif
} cotKeyProc_t;

typedef struct
{
    cotKeyProc_t proc;
    cotKeyState_f pfnKeyState;
} cotKeyCfg_t;

/* Exported constants ------------------------------------------------------------------------------------------------*/
/* Exported macro ----------------------------------------------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------------------------------------------------*/

extern int cotKey_Init(cotKeyCfg_t pCfgTable[], size_t num);

extern int cotKey_Listen(cotKey_t eKey, cotKeyListen_f pfnKeyFun);

extern int cotKey_ResetDefaultConfig(cotKey_t eKey);
extern int cotKey_ConfigNormalMode(cotKey_t eKey, uint8_t triggerWay, uint16_t triggerInterval);
extern int cotKey_ConfigLongPressMode(cotKey_t eKey, uint16_t longPressTime, uint16_t triggerInterval);
extern int cotKey_ConfigClickMode(cotKey_t eKey, uint16_t triggerInterval);
extern int cotKey_ConfigClickCompMode(cotKey_t eKey, uint16_t triggerInterval, uint16_t pressMaxTime, uint8_t triggerWay);

extern uint8_t cotKey_GetClickCount(cotKey_t eKey);
extern bool cotKey_IsClickCount(cotKey_t eKey, uint8_t count);
extern bool cotKey_IsLongPress(cotKey_t eKey);
extern bool cotKey_IsTriggerWayMet(cotKey_t eKey, uint8_t triggerWay);
extern bool cotKey_IsPressTimeMet(cotKey_t eKey, uint8_t triggerWay, uint16_t time);
extern bool cotKey_IsLossenTimeMet(cotKey_t eKey, uint8_t triggerWay, uint16_t time);

extern int cotKey_Scan(uint32_t sysTime);


#endif