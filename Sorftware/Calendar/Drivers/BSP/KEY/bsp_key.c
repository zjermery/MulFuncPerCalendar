/**
  **********************************************************************************************************************
  * @file    cot_key.c
  * @brief   ���ļ��ṩ��������ʶ����
  * @author  const_zpc  any question please send mail to const_zpc@163.com
  * @version V1.0.0
  * @date    2023-11-05
  *
  * @details  ������ϸ˵����
  *           + ������ʼ��
  *           + ������������
  *           + ����������Ϣ��ȡ
  *           + ������ѯ����
  *
  **********************************************************************************************************************
  * Դ��·����https://gitee.com/cot_package/cot_key.git �������⼰������ڸ���ַ��д Issue
  *
  * ʹ�÷�ʽ:
  *    1��ʹ��ǰ��ʼ������ KEY_Init
  *    2�����ڵ��ú��� KEY_Scan, ��������������ʶ����¼�����
  *    3��ʹ�����������Ի�ȡ��������������Ϣ
  *
  **********************************************************************************************************************
  */

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "cot_key.h"

/* Private typedef ---------------------------------------------------------------------------------------------------*/
/* Private define ----------------------------------------------------------------------------------------------------*/

#define KEY_MODE_NORMAL         0
#define KEY_MODE_LONG_PRESS     1
#define KEY_MODE_CLICK          2
#define KEY_MODE_CLICK_COMP     3

#define KEY_STATE_IDLE          0
#define KEY_STATE_PRESS         1
#define KEY_STATE_LOSSEN        2
#define KEY_STATE_LONG_PRESS    3
#define KEY_STATE_LONG_LOSSEN   4

/* Private macro -----------------------------------------------------------------------------------------------------*/

#define TIME_MAX_VAL            (uint32_t)0xffff
#define TIME_ADD(tic, time)     (TIME_MAX_VAL - (tic) < (time) ? ((tic) = 0xffff) : (tic += time))

/* Private variables -------------------------------------------------------------------------------------------------*/

static cotKeyCfg_t *sg_pKeyTable = NULL;
static size_t sg_keyNum = 0;

/* Private function prototypes ---------------------------------------------------------------------------------------*/
/* Private function --------------------------------------------------------------------------------------------------*/

/**
  * @brief      ������ʼ��
  *
  * @note       ����Ĭ��Ϊ��ͨģʽ�����º��ɿ����ᴥ���ص�����
  * @return     0,�ɹ�; -1,ʧ��
  */
int cotKey_Init(cotKeyCfg_t pCfgTable[], size_t num)
{
    uint8_t key;

    sg_pKeyTable = pCfgTable;
    sg_keyNum = num;

    for (key = 0; key < num; key++)
    {
        sg_pKeyTable[key].proc.clickCnt = 0;
        sg_pKeyTable[key].proc.uiPressTic = 0;
        sg_pKeyTable[key].proc.uiLossenTic = 0;
        sg_pKeyTable[key].proc.eKeyStateBak = COT_KEY_OFF;
        sg_pKeyTable[key].proc.state = KEY_STATE_IDLE;

        cotKey_Listen((cotKey_t)key, NULL);
        cotKey_ResetDefaultConfig((cotKey_t)key);
    }

    return 0;
}

/**
  * @brief      ����ָ������״̬�������ص�����
  *
  * @param[in]  eKey ָ������
  * @param[in]  pfnKeyFun ���������ص�����
  * @return     0,�ɹ�; -1,ʧ��
  */
int cotKey_Listen(cotKey_t eKey, cotKeyListen_f pfnKeyFun)
{
    if (sg_pKeyTable == NULL || sg_keyNum == 0 || eKey >= sg_keyNum)
    {
        return -1;
    }

#if defined(_CONFIG_DELAY_UPDATE_)
    sg_pKeyTable[eKey].proc.newConfig.pfnEventCallback = pfnKeyFun;
    
    if (sg_pKeyTable[eKey].proc.state == KEY_STATE_IDLE)
    {
        sg_pKeyTable[eKey].proc.curConfig = sg_pKeyTable[eKey].proc.newConfig;
    }
#else
    sg_pKeyTable[eKey].proc.curConfig.pfnEventCallback = pfnKeyFun;
#endif
    return 0;
}

/**
  * @brief      ���ð����ָ�ΪĬ��ģʽ
  * 
  * @note       Ĭ����ͨģʽ�����º��ɿ����ᴥ���ص�����
  * @param[in]  eKey ָ������
  * @return     0,�ɹ�; -1,ʧ��
  */
int cotKey_ResetDefaultConfig(cotKey_t eKey)
{
    if (sg_pKeyTable == NULL || sg_keyNum == 0 || eKey >= sg_keyNum)
    {
        return -1;
    }

#if defined(_CONFIG_DELAY_UPDATE_)
    sg_pKeyTable[eKey].proc.newConfig.triggerWay = (COT_KEY_TRIGGER_WAY_PRESS | COT_KEY_TRIGGER_WAY_LOSSEN);
    sg_pKeyTable[eKey].proc.newConfig.mode = KEY_MODE_NORMAL;
    sg_pKeyTable[eKey].proc.newConfig.longTime = 0;
    sg_pKeyTable[eKey].proc.newConfig.triggerInterval = 0;
#else
    sg_pKeyTable[eKey].proc.curConfig.triggerWay = (COT_KEY_TRIGGER_WAY_PRESS | COT_KEY_TRIGGER_WAY_LOSSEN);
    sg_pKeyTable[eKey].proc.curConfig.mode = KEY_MODE_NORMAL;
    sg_pKeyTable[eKey].proc.curConfig.longTime = 0;
    sg_pKeyTable[eKey].proc.curConfig.triggerInterval = 0;
#endif
    return 0;
}

/**
  * @brief      ���ð���Ϊ��ͨģʽ
  * 
  * @note       ��ģʽ��ֻ�а���/�ɿ�״̬
  * @param[in]  eKey ָ������
  * @param[in]  triggerWay ������ʽ, ȡֵ������ COT_KEY_TRIGGER_WAY_PRESS �� COT_KEY_TRIGGER_WAY_LOSSEN ���������
  * @param[in]  triggerInterval ���²��ɿ�ʱ�ظ������¼��ļ��ʱ��, Ϊ0��ֻ����һ��, ��λ����
  * @return     0,�ɹ�; -1,ʧ��
  */
int cotKey_ConfigNormalMode(cotKey_t eKey, uint8_t triggerWay, uint16_t triggerInterval)
{
    if (sg_pKeyTable == NULL || sg_keyNum == 0 || eKey >= sg_keyNum)
    {
        return -1;
    }

#if defined(_CONFIG_DELAY_UPDATE_)
    sg_pKeyTable[eKey].proc.newConfig.mode = KEY_MODE_NORMAL;
    sg_pKeyTable[eKey].proc.newConfig.triggerWay = triggerWay;
    sg_pKeyTable[eKey].proc.newConfig.triggerInterval = triggerInterval;
    
    if (sg_pKeyTable[eKey].proc.state == KEY_STATE_IDLE)
    {
        sg_pKeyTable[eKey].proc.curConfig = sg_pKeyTable[eKey].proc.newConfig;
    }
#else
    sg_pKeyTable[eKey].proc.curConfig.mode = KEY_MODE_NORMAL;
    sg_pKeyTable[eKey].proc.curConfig.triggerWay = triggerWay;
    sg_pKeyTable[eKey].proc.curConfig.triggerInterval = triggerInterval;
#endif
    return 0;
}

/**
  * @brief      ���ð���Ϊ����ģʽ
  * 
  * @note       ��ģʽ�����ֶ̰��ͳ�������״̬��Ĭ��˫�ߴ���ģʽ�����´���/�ɿ�������
  * @param[in]  eKey ָ������
  * @param[in]  longPressTime   �������ֶ̰��ͳ�����ʱ��, ��λ����
  * @param[in]  triggerInterval ��ȷ��Ϊ����ʱ�����²��ɿ�ʱ�ظ������¼��ļ��ʱ��, Ϊ0��ֻ����һ��, ��λ����
  * @return     0,�ɹ�; -1,ʧ��
  */
int cotKey_ConfigLongPressMode(cotKey_t eKey, uint16_t longPressTime, uint16_t triggerInterval)
{
    if (sg_pKeyTable == NULL || sg_keyNum == 0 || eKey >= sg_keyNum)
    {
        return -1;
    }

#if defined(_CONFIG_DELAY_UPDATE_)
    sg_pKeyTable[eKey].proc.newConfig.mode = KEY_MODE_LONG_PRESS;
    sg_pKeyTable[eKey].proc.newConfig.longTime = longPressTime;
    sg_pKeyTable[eKey].proc.newConfig.triggerInterval = triggerInterval;
    
    if (sg_pKeyTable[eKey].proc.state == KEY_STATE_IDLE)
    {
        sg_pKeyTable[eKey].proc.curConfig = sg_pKeyTable[eKey].proc.newConfig;
    }
#else
    sg_pKeyTable[eKey].proc.curConfig.mode = KEY_MODE_LONG_PRESS;
    sg_pKeyTable[eKey].proc.curConfig.longTime = longPressTime;
    sg_pKeyTable[eKey].proc.curConfig.triggerInterval = triggerInterval;
#endif
    return 0;
}

/**
  * @brief      ���ð���Ϊ����ģʽ
  * 
  * @note       ��ģʽ���������ֵ�����˫����������һ��ʱ���ڶ�������ĳ���ʹ��
  * @param[in]  eKey ָ������
  * @param[in]  triggerInterval ���ε���ļ��ʱ���, ��λ����
  * @return     0,�ɹ�; -1,ʧ��
  */
int cotKey_ConfigClickMode(cotKey_t eKey, uint16_t triggerInterval)
{
    if (sg_pKeyTable == NULL || sg_keyNum == 0 || eKey >= sg_keyNum)
    {
        return -1;
    }

#if defined(_CONFIG_DELAY_UPDATE_)
    sg_pKeyTable[eKey].proc.newConfig.mode = KEY_MODE_CLICK;
    sg_pKeyTable[eKey].proc.newConfig.triggerInterval = triggerInterval;
    
    if (sg_pKeyTable[eKey].proc.state == KEY_STATE_IDLE)
    {
        sg_pKeyTable[eKey].proc.curConfig = sg_pKeyTable[eKey].proc.newConfig;
    }
#else
    sg_pKeyTable[eKey].proc.curConfig.mode = KEY_MODE_CLICK;
    sg_pKeyTable[eKey].proc.curConfig.triggerInterval = triggerInterval;
#endif
    return 0;
}

/**
  * @brief      ���ð���Ϊ������ͨ����ģʽ
  * 
  * @note       ��ģʽ���������ֳ�ʱ�䰴�¡�������˫����������һ��ʱ���ڶ�������ĳ���ʹ��
  *             ��������״̬�µ����³���һ��ʱ��ʱ�����ٴ�������ģʽ����Ϊ��ͨģʽ
  * @param[in]  eKey ָ������
  * @param[in]  triggerInterval ���ε���ļ��ʱ���, ��λ����
  * @param[in]  pressMaxTime    ���³�ʱʱ��, ��λ����
  * @param[in]  triggerWay      ������ʽ, ȡֵ������ COT_KEY_TRIGGER_WAY_PRESS �� COT_KEY_TRIGGER_WAY_LOSSEN ���������
  * @return     0,�ɹ�; -1,ʧ��
  */
int cotKey_ConfigClickCompMode(cotKey_t eKey, uint16_t triggerInterval, uint16_t pressMaxTime, uint8_t triggerWay)
{
    if (sg_pKeyTable == NULL || sg_keyNum == 0 || eKey >= sg_keyNum)
    {
        return -1;
    }

#if defined(_CONFIG_DELAY_UPDATE_)
    sg_pKeyTable[eKey].proc.newConfig.mode = KEY_MODE_CLICK_COMP;
    sg_pKeyTable[eKey].proc.newConfig.triggerInterval = triggerInterval;
    sg_pKeyTable[eKey].proc.newConfig.triggerWay = triggerWay;
    sg_pKeyTable[eKey].proc.newConfig.longTime = pressMaxTime;
    
    if (sg_pKeyTable[eKey].proc.state == KEY_STATE_IDLE)
    {
        sg_pKeyTable[eKey].proc.curConfig = sg_pKeyTable[eKey].proc.newConfig;
    }
#else
    sg_pKeyTable[eKey].proc.curConfig.mode = KEY_MODE_CLICK_COMP;
    sg_pKeyTable[eKey].proc.curConfig.triggerInterval = triggerInterval;
    sg_pKeyTable[eKey].proc.curConfig.triggerWay = triggerWay;
    sg_pKeyTable[eKey].proc.curConfig.longTime = pressMaxTime;
#endif
    return 0;
}

/**
  * @brief      ��ȡָ�������ĵ������
  * 
  * @attention  ��Ҫ����������Ϊ����ģʽ��������ͨ����ģʽ
  * @param      eKey ָ������, ȡֵΪ @reg cotKey_t
  * @return     ������� 
  */
uint8_t cotKey_GetClickCount(cotKey_t eKey)
{
    if (sg_pKeyTable == NULL || sg_keyNum == 0 || eKey >= sg_keyNum)
    {
        return 0;
    }

    if (sg_pKeyTable[eKey].proc.curConfig.mode != KEY_MODE_CLICK &&
        sg_pKeyTable[eKey].proc.curConfig.mode != KEY_MODE_CLICK_COMP )
    {
        return 0;
    }

    return sg_pKeyTable[eKey].proc.clickCnt;
}

/**
  * @brief      �жϰ�����������Ƿ����Ҫ��
  * 
  * @attention  ��Ҫ����������Ϊ����ģʽ
  * @param[in]  eKey ָ������
  * @param[in]  count �������
  * @retval     true,����; false,������.
  */
bool cotKey_IsClickCount(cotKey_t eKey, uint8_t count)
{
    if (sg_pKeyTable == NULL || sg_keyNum == 0 || eKey >= sg_keyNum)
    {
        return false;
    }

    if (sg_pKeyTable[eKey].proc.curConfig.mode != KEY_MODE_CLICK)
    {
        return false;
    }
    
    if (sg_pKeyTable[eKey].proc.clickCnt != count)
    {
        return false;
    }
    
    return true;
}

/**
  * @brief      �жϰ���״̬�Ƿ�Ϊ�����˳���״̬
  * 
  * @attention  ��Ҫ����������Ϊ����ģʽ
  * @param      eKey ָ������, ȡֵΪ @reg cotKey_t
  * @return     true ����
  * @return     false �̰�
  */
bool cotKey_IsLongPress(cotKey_t eKey)
{
    if (sg_pKeyTable == NULL || sg_keyNum == 0 || eKey >= sg_keyNum)
    {
        return false;
    }

    if (sg_pKeyTable[eKey].proc.curConfig.mode != KEY_MODE_LONG_PRESS)
    {
        return false;
    }

    if (!(sg_pKeyTable[eKey].proc.state == KEY_STATE_LONG_PRESS || sg_pKeyTable[eKey].proc.state == KEY_STATE_LONG_LOSSEN))
    {
        return false;
    }

    return true;
}

/**
  * @brief      �жϰ���״̬�Ƿ����Ҫ��
  *
  * @attention  ��������ͨģʽ�ͳ���ģʽ������ģʽʱ��Ҫͨ�����ú���cotKey_IsLongPress�ж��Ƿ���˳���
  * @param[in]  eKey ָ������
  * @param[in]  triggerWay ��������״̬, ȡֵ������ COT_KEY_TRIGGER_WAY_PRESS �� COT_KEY_TRIGGER_WAY_LOSSEN ���������
  * @retval     true,����; false,������.
  */
bool cotKey_IsTriggerWayMet(cotKey_t eKey, uint8_t triggerWay)
{
    if (sg_pKeyTable == NULL || sg_keyNum == 0 || eKey >= sg_keyNum)
    {
        return false;
    }
    
    if ((triggerWay & COT_KEY_TRIGGER_WAY_PRESS) && 
        (sg_pKeyTable[eKey].proc.state == KEY_STATE_PRESS || sg_pKeyTable[eKey].proc.state == KEY_STATE_LONG_PRESS))
    {
        return true;
    }

    if ((triggerWay & COT_KEY_TRIGGER_WAY_LOSSEN) && 
        (sg_pKeyTable[eKey].proc.state == KEY_STATE_LOSSEN || sg_pKeyTable[eKey].proc.state == KEY_STATE_LONG_LOSSEN))
    {
        return true;
    }
    
    return false;
}

/**
  * @brief      �жϰ���״̬�Ͱ���ʱ���Ƿ����Ҫ��
  *
  * @param[in]  eKey ָ������
  * @param[in]  triggerWay ��������״̬, ȡֵ������ COT_KEY_TRIGGER_WAY_PRESS �� COT_KEY_TRIGGER_WAY_LOSSEN ���������
  * @param[in]  time       ����ʱ��
  * @retval     true,����; false,������.
  */
bool cotKey_IsPressTimeMet(cotKey_t eKey, uint8_t triggerWay, uint16_t time)
{
    if (sg_pKeyTable == NULL || sg_keyNum == 0 || eKey >= sg_keyNum)
    {
        return false;
    }
    
    if (!cotKey_IsTriggerWayMet(eKey, triggerWay))
    {
        return false;
    }
    
    if (sg_pKeyTable[eKey].proc.uiPressTic < time)
    {
        return false;
    }
    
    return true;
}


/**
  * @brief      �жϰ���״̬���ɿ�ʱ���Ƿ����Ҫ��
  *
  * @param[in]  eKey ָ������
  * @param[in]  triggerWay ��������״̬, ȡֵ������ COT_KEY_TRIGGER_WAY_PRESS �� COT_KEY_TRIGGER_WAY_LOSSEN ���������
  * @param[in]  time       �ɿ�ʱ��
  * @retval     true,����; false,������.
  */
bool cotKey_IsLossenTimeMet(cotKey_t eKey, uint8_t triggerWay, uint16_t time)
{
    if (sg_pKeyTable == NULL || sg_keyNum == 0 || eKey >= sg_keyNum)
    {
        return false;
    }
    
    if (!cotKey_IsTriggerWayMet(eKey, triggerWay))
    {
        return false;
    }
    
    if (sg_pKeyTable[eKey].proc.uiLossenTic < time)
    {
        return false;
    }
    
    return true;
}

static bool ProcessNormalKeyState(cotKeyProc_t *pKey, cotKey_t newState, uint16_t intervalTime)
{
    bool isCallFun = false;

    switch (pKey->state)
    {
        case KEY_STATE_IDLE:
            if (newState == COT_KEY_ON)
            {
                pKey->state = KEY_STATE_PRESS;
                pKey->uiPressTic = 0;
                
                if (pKey->curConfig.triggerWay & COT_KEY_TRIGGER_WAY_PRESS)
                {
                    isCallFun = true;
                }
            }
        break;
        case KEY_STATE_PRESS:
            if (newState == COT_KEY_OFF)
            {
                pKey->state = KEY_STATE_LOSSEN;
                
                if (pKey->curConfig.triggerWay & COT_KEY_TRIGGER_WAY_LOSSEN)
                {
                    isCallFun = true;
                }
            }
            else
            {
                if (0 != pKey->curConfig.triggerInterval)
                {
                    TIME_ADD(pKey->uiPressTic, intervalTime);

                    if (pKey->uiPressTic > pKey->curConfig.triggerInterval)
                    {
                        pKey->uiPressTic = 0;
                        isCallFun = true;
                    }
                }
            }
        break;
        case KEY_STATE_LOSSEN:
            pKey->state = KEY_STATE_IDLE;
        break;
    }
    
    return isCallFun;
}

static bool ProcessLongPressKeyState(cotKeyProc_t *pKey, cotKey_t newState, uint16_t intervalTime)
{
    bool isCallFun = false;

    switch (pKey->state)
    {
        case KEY_STATE_IDLE:
            if (newState == COT_KEY_ON)
            {
                pKey->state = KEY_STATE_PRESS;
                pKey->uiPressTic = 0;

                if (pKey->curConfig.triggerWay & COT_KEY_TRIGGER_WAY_PRESS)
                {
                    isCallFun = true;
                }
            }
        break;
        case KEY_STATE_PRESS:
            if (newState == COT_KEY_OFF)
            {
                pKey->state = KEY_STATE_LOSSEN;
                isCallFun = true;
            }
            else
            {
                TIME_ADD(pKey->uiPressTic, intervalTime);

                if (pKey->uiPressTic >= pKey->curConfig.longTime)
                {
                    pKey->state = KEY_STATE_LONG_PRESS;
                    pKey->uiPressTic = 0;
                    
                    if (pKey->curConfig.triggerWay & COT_KEY_TRIGGER_WAY_PRESS)
                    {
                        isCallFun = true;
                    }
                }
            }
        break;
        case KEY_STATE_LOSSEN:
            pKey->state = KEY_STATE_IDLE;
        break;
        case KEY_STATE_LONG_PRESS:
            if (newState == COT_KEY_OFF)
            {
                pKey->state = KEY_STATE_LONG_LOSSEN;

                if (pKey->curConfig.triggerWay & COT_KEY_TRIGGER_WAY_LOSSEN)
                {
                    isCallFun = true;
                }
            }
            else
            {
                TIME_ADD(pKey->uiPressTic, intervalTime);

                if (0 != pKey->curConfig.triggerInterval)
                {
                    if (pKey->uiPressTic > pKey->curConfig.triggerInterval)
                    {
                        pKey->uiPressTic = 0;
                        isCallFun = true;
                    }
                }
            }
        break;
        case KEY_STATE_LONG_LOSSEN:
            pKey->state = KEY_STATE_IDLE;
        break;
    }
    
    return isCallFun;
}


static bool ProcessClickKeyState(cotKeyProc_t *pKey, cotKey_t newState, uint16_t intervalTime)
{
    bool isCallFun = false;

    switch (pKey->state)
    {
        case KEY_STATE_IDLE:
            if (newState == COT_KEY_ON)
            {
                pKey->state = KEY_STATE_PRESS;
                pKey->uiPressTic = 0;
                pKey->uiLossenTic = 0;
                pKey->clickCnt = 0;
            }
        break;
        case KEY_STATE_PRESS:
            if (newState == COT_KEY_OFF)
            {
                pKey->state = KEY_STATE_LOSSEN;
                pKey->uiLossenTic = 0;
                pKey->clickCnt++;
            }
        break;
        case KEY_STATE_LOSSEN:
            TIME_ADD(pKey->uiLossenTic, intervalTime);
        
            if (pKey->uiLossenTic > pKey->curConfig.triggerInterval)
            {
                isCallFun = true;
                pKey->state = KEY_STATE_IDLE;
            }
            else
            {
                if (newState == COT_KEY_ON)
                {
                    pKey->state = KEY_STATE_PRESS;
                }
            }
        break;
    }
    
    return isCallFun;
}

static bool ProcessClickCompKeyState(cotKeyProc_t *pKey, cotKey_t newState, uint16_t intervalTime)
{
    bool isCallFun = false;

    switch (pKey->state)
    {
        case KEY_STATE_IDLE:
            if (newState == COT_KEY_ON)
            {
                pKey->state = KEY_STATE_PRESS;
                pKey->uiPressTic = 0;
                pKey->uiLossenTic = 0;
                pKey->clickCnt = 0;
            }
        break;
        case KEY_STATE_PRESS:
            if (newState == COT_KEY_OFF)
            {
                pKey->state = KEY_STATE_LOSSEN;
                pKey->uiLossenTic = 0;

                if (pKey->uiPressTic < pKey->curConfig.longTime)
                {
                    pKey->clickCnt++;
                }
                else
                {
                    if (pKey->curConfig.triggerWay & COT_KEY_TRIGGER_WAY_LOSSEN)
                    {
                        isCallFun = true;
                    }
                }
            }
            else
            {
                if (pKey->clickCnt == 0)
                {
                    TIME_ADD(pKey->uiPressTic, intervalTime);

                    if (pKey->uiPressTic >= pKey->curConfig.longTime && 
                        (pKey->curConfig.triggerWay & COT_KEY_TRIGGER_WAY_PRESS))
                    {
                        isCallFun = true;
                    }
                }
            }
        break;
        case KEY_STATE_LOSSEN:
            TIME_ADD(pKey->uiLossenTic, intervalTime);
        
            if (pKey->uiLossenTic > pKey->curConfig.triggerInterval)
            {
                isCallFun = true;
                pKey->state = KEY_STATE_IDLE;
            }
            else
            {
                if (newState == COT_KEY_ON)
                {
                    pKey->state = KEY_STATE_PRESS;
                }
            }
        break;
    }
    
    return isCallFun;
}

/**
  * @brief      ����ɨ����������
  *
  * @param[in]  sysTime ϵͳ����ʱ��, ��λ����
  * @return     0,�ɹ�; -1,ʧ��
  */
int cotKey_Scan(uint32_t sysTime)
{
    uint8_t key;
    cotKey_t eKeyNewState;
    bool isCallFun = false;
    static uint32_t s_sysTimeBak = 0;

    if (sg_pKeyTable == NULL || sg_keyNum == 0)
    {
        return -1;
    }

    if (s_sysTimeBak == 0)
    {
        s_sysTimeBak = sysTime;
    }

    for (key = 0; key < sg_keyNum; key++)
    {
        sg_pKeyTable[key].proc.eKeyState = sg_pKeyTable[key].pfnKeyState();
    }

    for (key = 0; key < sg_keyNum; key++)
    {
        if (sg_pKeyTable[key].proc.eKeyStateBak != sg_pKeyTable[key].proc.eKeyState)
        {
            eKeyNewState = sg_pKeyTable[key].proc.eKeyStateBak;
        }
        else
        {
            eKeyNewState = sg_pKeyTable[key].proc.eKeyState;
        }
        
        switch (sg_pKeyTable[key].proc.curConfig.mode)
        {
            case KEY_MODE_NORMAL:
                isCallFun = ProcessNormalKeyState(&sg_pKeyTable[key].proc, eKeyNewState, sysTime - s_sysTimeBak);
            break;
            case KEY_MODE_LONG_PRESS:
                isCallFun = ProcessLongPressKeyState(&sg_pKeyTable[key].proc, eKeyNewState, sysTime - s_sysTimeBak);
            break;
            case KEY_MODE_CLICK:
                isCallFun = ProcessClickKeyState(&sg_pKeyTable[key].proc, eKeyNewState, sysTime - s_sysTimeBak);
            break;
            case KEY_MODE_CLICK_COMP:
                isCallFun = ProcessClickCompKeyState(&sg_pKeyTable[key].proc, eKeyNewState, sysTime - s_sysTimeBak);
            break;
            default:
                
            break;
        }

        if (isCallFun && NULL != sg_pKeyTable[key].proc.curConfig.pfnEventCallback)
        {
            sg_pKeyTable[key].proc.curConfig.pfnEventCallback((cotKey_t)key);
        }

        if (sg_pKeyTable[key].proc.state == KEY_STATE_IDLE)
        {
#if defined(_CONFIG_DELAY_UPDATE_)
            sg_pKeyTable[key].proc.curConfig = sg_pKeyTable[key].proc.newConfig;  // ��������ʱ�Ÿ�������
#endif
            sg_pKeyTable[key].proc.clickCnt = 0;
        }

        sg_pKeyTable[key].proc.eKeyStateBak = sg_pKeyTable[key].proc.eKeyState;
    }
    
    s_sysTimeBak = sysTime;
    return 0;
}