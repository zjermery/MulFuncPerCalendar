/**
  **********************************************************************************************************************
  * @file    cot_key.h
  * @brief   ���ļ��ṩ��������ʶ���ܺ���ԭ��
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
  * @brief ����꣺���������������ӳٸ����滻
  *        ֻ���ڰ������е�״̬�²���������������Ϣ����ֹ�ϲ�Ӧ�ó�����ĳЩ��������ʱ�������벻��������
  * @note  ����ϲ�Ӧ�ó���ֻ���ڳ�ʼ��ʱ�Ÿ������ã���ô���Թرոù��ܣ������ڴ�ռ��
  */
#define _CONFIG_DELAY_UPDATE_

/* Exported types ----------------------------------------------------------------------------------------------------*/

/** ������ʽ */
#define COT_KEY_TRIGGER_WAY_PRESS           0x01  // ���´���
#define COT_KEY_TRIGGER_WAY_LOSSEN          0x02  // �ɿ�����

/**
  * @brief  KEY����ö�ٶ���
  */
typedef enum{
    COT_KEY_OFF = 0,                         	/*!< (0)�Ͽ�(�޶���) */
    COT_KEY_ON = !COT_KEY_OFF                   /*!< (1)�պ�(�ж���) */
} cotKeyState_e;

typedef uint16_t cotKey_t;
typedef cotKeyState_e (*cotKeyState_f)(void);
typedef void (*cotKeyListen_f)(cotKey_t);

typedef struct
{
    uint8_t mode;                               /*!< ����ģʽ */
    uint8_t triggerWay;                         /*!< �����¼������ص������Ĵ������� */
    uint16_t triggerInterval;                   /*!< ��ʱ�䰴��ʱ�����ص������ļ��ʱ�� */
    uint16_t longTime;                          /*!< ���ֶ̰��ͳ�����ʱ��, �ò�����λΪ1ms */
    cotKeyListen_f pfnEventCallback;            /*!< �����¼������ص����� */
} cotKeyInfo_t;

/**
  * @brief ����������Ϣ�ṹ�嶨��
  */
typedef struct
{
    cotKeyState_e eKeyStateBak;                 /*!< �����ϴο���״̬ */
    cotKeyState_e eKeyState;                    /*!< ������ǰ����״̬ */
    uint8_t clickCnt;                           /*!< ָ����ѡ�������������� */
    uint16_t uiPressTic;                        /*!< ���������ۼ�ʱ��, �ò�����λΪ1ms */
    uint16_t uiLossenTic;                       /*!< �����ɿ��ۼ�ʱ��, �ò�����λΪ1ms */
    uint8_t state;                              /*!< ����״̬ */
    cotKeyInfo_t curConfig;                     /*!< ������ǰ���� */
#if defined(_CONFIG_DELAY_UPDATE_)
    cotKeyInfo_t newConfig;                     /*!< �����µ����� */
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