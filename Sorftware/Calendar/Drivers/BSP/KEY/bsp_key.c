/**
  **********************************************************************************************************************
  * @file    cot_key.c
  * @brief   该文件提供按键动作识别功能
  * @author  const_zpc  any question please send mail to const_zpc@163.com
  * @version V1.0.0
  * @date    2023-11-05
  *
  * @details  功能详细说明：
  *           + 按键初始化
  *           + 按键动作监听
  *           + 按键动作信息获取
  *           + 按键轮询任务
  *
  **********************************************************************************************************************
  * 源码路径：https://gitee.com/cot_package/cot_key.git 具体问题及建议可在该网址填写 Issue
  *
  * 使用方式:
  *    1、使用前初始化函数 KEY_Init
  *    2、周期调用函数 KEY_Scan, 用来处理按键动作识别和事件触发
  *    3、使用其他函数对获取按键动作附加信息
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
  * @brief      按键初始化
  *
  * @note       按键默认为普通模式，按下和松开都会触发回调函数
  * @return     0,成功; -1,失败
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
  * @brief      监听指定按键状态，触发回调函数
  *
  * @param[in]  eKey 指定按键
  * @param[in]  pfnKeyFun 监听触发回调函数
  * @return     0,成功; -1,失败
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
  * @brief      配置按键恢复为默认模式
  * 
  * @note       默认普通模式，按下和松开都会触发回调函数
  * @param[in]  eKey 指定按键
  * @return     0,成功; -1,失败
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
  * @brief      配置按键为普通模式
  * 
  * @note       该模式是只有按下/松开状态
  * @param[in]  eKey 指定按键
  * @param[in]  triggerWay 触发方式, 取值可以是 COT_KEY_TRIGGER_WAY_PRESS 和 COT_KEY_TRIGGER_WAY_LOSSEN 的任意组合
  * @param[in]  triggerInterval 按下不松开时重复触发事件的间隔时长, 为0则只触发一次, 单位毫秒
  * @return     0,成功; -1,失败
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
  * @brief      配置按键为长按模式
  * 
  * @note       该模式是区分短按和长按两种状态，默认双边触发模式（按下触发/松开触发）
  * @param[in]  eKey 指定按键
  * @param[in]  longPressTime   用来区分短按和长按的时间, 单位毫秒
  * @param[in]  triggerInterval 当确认为长按时，按下不松开时重复触发事件的间隔时长, 为0则只触发一次, 单位毫秒
  * @return     0,成功; -1,失败
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
  * @brief      配置按键为连击模式
  * 
  * @note       该模式是用于区分单击、双击和三击等一定时间内多次连击的场景使用
  * @param[in]  eKey 指定按键
  * @param[in]  triggerInterval 两次点击的间隔时间差, 单位毫秒
  * @return     0,成功; -1,失败
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
  * @brief      配置按键为连击普通复合模式
  * 
  * @note       该模式是用于区分长时间按下、单击、双击和三击等一定时间内多次连击的场景使用
  *             按键空闲状态下当按下超过一定时长时，则不再触发连击模式，变为普通模式
  * @param[in]  eKey 指定按键
  * @param[in]  triggerInterval 两次点击的间隔时间差, 单位毫秒
  * @param[in]  pressMaxTime    按下超时时长, 单位毫秒
  * @param[in]  triggerWay      触发方式, 取值可以是 COT_KEY_TRIGGER_WAY_PRESS 和 COT_KEY_TRIGGER_WAY_LOSSEN 的任意组合
  * @return     0,成功; -1,失败
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
  * @brief      获取指定按键的点击次数
  * 
  * @attention  需要将按键配置为连击模式或连击普通复合模式
  * @param      eKey 指定按键, 取值为 @reg cotKey_t
  * @return     点击次数 
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
  * @brief      判断按键点击次数是否符合要求
  * 
  * @attention  需要将按键配置为连击模式
  * @param[in]  eKey 指定按键
  * @param[in]  count 点击次数
  * @retval     true,符合; false,不符合.
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
  * @brief      判断按键状态是否为触发了长按状态
  * 
  * @attention  需要将按键配置为长按模式
  * @param      eKey 指定按键, 取值为 @reg cotKey_t
  * @return     true 长按
  * @return     false 短按
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
  * @brief      判断按键状态是否符合要求
  *
  * @attention  适用于普通模式和长按模式，长按模式时需要通过调用函数cotKey_IsLongPress判断是否除了长按
  * @param[in]  eKey 指定按键
  * @param[in]  triggerWay 按键触发状态, 取值可以是 COT_KEY_TRIGGER_WAY_PRESS 和 COT_KEY_TRIGGER_WAY_LOSSEN 的任意组合
  * @retval     true,符合; false,不符合.
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
  * @brief      判断按键状态和按下时长是否符合要求
  *
  * @param[in]  eKey 指定按键
  * @param[in]  triggerWay 按键触发状态, 取值可以是 COT_KEY_TRIGGER_WAY_PRESS 和 COT_KEY_TRIGGER_WAY_LOSSEN 的任意组合
  * @param[in]  time       按下时长
  * @retval     true,符合; false,不符合.
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
  * @brief      判断按键状态和松开时长是否符合要求
  *
  * @param[in]  eKey 指定按键
  * @param[in]  triggerWay 按键触发状态, 取值可以是 COT_KEY_TRIGGER_WAY_PRESS 和 COT_KEY_TRIGGER_WAY_LOSSEN 的任意组合
  * @param[in]  time       松开时长
  * @retval     true,符合; false,不符合.
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
  * @brief      按键扫描周期任务
  *
  * @param[in]  sysTime 系统运行时间, 单位毫秒
  * @return     0,成功; -1,失败
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
            sg_pKeyTable[key].proc.curConfig = sg_pKeyTable[key].proc.newConfig;  // 按键空闲时才更新配置
#endif
            sg_pKeyTable[key].proc.clickCnt = 0;
        }

        sg_pKeyTable[key].proc.eKeyStateBak = sg_pKeyTable[key].proc.eKeyState;
    }
    
    s_sysTimeBak = sysTime;
    return 0;
}