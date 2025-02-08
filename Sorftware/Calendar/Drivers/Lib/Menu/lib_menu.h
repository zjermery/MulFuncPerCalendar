/**
  **********************************************************************************************************************
  * @file    cot_menu.h
  * @brief   ���ļ��ṩ�˵�������к���ԭ��
  * @author  const_zpc    any question please send mail to const_zpc@163.com
  * @date    2024-06-09
  **********************************************************************************************************************
  *
  **********************************************************************************************************************
  */

/* Define to prevent recursive inclusion -----------------------------------------------------------------------------*/
#ifndef LIB_MENU_H
#define LIB_MENU_H


/* Includes ----------------------------------------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
 extern "C" {
#endif 


/******************************************* ������ ********************************************************************/

/* ���� _COT_MENU_USE_MALLOC_ ����� malloc/free �ķ�ʽʵ�ֶ༶�˵�, ����ͨ���������ʽ */
// #define _COT_MENU_USE_MALLOC_

/* ���� _COT_MENU_USE_SHORTCUT_ �����ÿ�ݲ˵�ѡ����빦�� */
#define _LIB_MENU_USE_SHORTCUT_

/* �༶�˵���� */
#define LIB_MENU_MAX_DEPTH              10

/* �˵�֧�ֵ����ѡ����Ŀ */
#define LIB_MENU_MAX_NUM                20

/******************************************* ������ ********************************************************************/


/* Exported types ----------------------------------------------------------------------------------------------------*/

#if LIB_MENU_MAX_NUM < 255
typedef uint8_t menusize_t;
#else
typedef uint16_t menusize_t;
#endif

typedef union
{
    void     *pVoid;                   /*!< ͨ��ָ��(Ŀ���������뾯��) */
    
    size_t    textId;                  /*!< �ı�ID */

    char     *pTextString;             /*!< �ı��ַ��� */
} LibMenuDsecStr_u;

/**
  * @brief      ����ѡ����Ϣ
  * 
  */
typedef struct
{
    LibMenuDsecStr_u uMenuDesc;         /*!< ��ѡ������� */

    void     *pExtendData;              /*!< ��ѡ��ע��ʱ����չ���� */
} LibMenuItemInfo_t;

typedef void (*libMenuCallFun_f)(const LibMenuItemInfo_t *pItemInfo);

/**
  * @brief      ����ѡ����˵��б����Ϣ
  * 
  */
typedef struct
{
    menusize_t itemsNum;                /*!< ��ǰ�˵���ѡ�������Ŀ */

    menusize_t selectItem;              /*!< ��ǰ�˵��б�ѡ�е�ѡ�� */

    menusize_t showBaseItem;            /*!< ��ǰ�˵��׸���ʾ��ѡ�� */

    LibMenuDsecStr_u uMenuDesc;         /*!< ��ǰ�˵������� */

    void     *pExtendData;              /*!< ��ǰ�˵�ע��ʱ����չ���� */

    LibMenuDsecStr_u uItemsListDesc[LIB_MENU_MAX_NUM];/*!< ��ǰ�˵���ѡ���б������ */

    void *pItemsListExtendData[LIB_MENU_MAX_NUM];   /*!< ��ǰ�˵���ѡ���б�ע��ʱ����չ���� */
} LibMenuShow_t;

typedef void (*libShowMenuCallFun_f)(LibMenuShow_t *ptShowInfo);

/**
  * @brief �˵���Ϣע��ṹ��
  * 
  */
typedef struct
{
    LibMenuDsecStr_u     uMenuDesc;        /*!< ��ǰ�˵������� */

    libMenuCallFun_f     pfnEnterCallFun;  /*!< ��ǰ�˵�ѡ�����ʱ(�Ӹ��˵�����)��Ҫִ��һ�εĺ���, ΪNULL��ִ�� */

    libMenuCallFun_f     pfnExitCallFun;   /*!< ��ǰ�˵�ѡ�������˳�ʱ(�˳������˵�)��Ҫִ��һ�εĺ���, ΪNULL��ִ�� */
    
    libMenuCallFun_f     pfnLoadCallFun;   /*!< ��ǰ�˵�ѡ��ÿ�μ���ʱ(�Ӹ��˵�������Ӳ˵��˳�)��Ҫִ��һ�εĺ���, ΪNULL��ִ�� */

    libMenuCallFun_f     pfnRunCallFun;    /*!< ��ǰ�˵�ѡ������ڵ��Ⱥ��� */

    void                *pExtendData;      /*!< ��ǰѡ��Ĳ˵���ʾЧ��������չ�������, ���������ø����� */
} LibMenuList_t, LibMainMenuCfg_t;

/* Exported constants ------------------------------------------------------------------------------------------------*/
/* Exported macro ----------------------------------------------------------------------------------------------------*/

/**
  * @brief  �����˵�ѡ���
  * 
  * @param  title    �˵�ѡ������
  * @param  enterFun ��ǰ�˵�ѡ�����ʱ(�Ӹ��˵�����)��Ҫִ��һ�εĺ���, ΪNULL��ִ��
  * @param  exitFun  ��ǰ�˵�ѡ�������˳�ʱ(�˳������˵�)��Ҫִ��һ�εĺ���, ΪNULL��ִ��
  * @param  loadFun  ��ǰ�˵�ѡ��ÿ�μ���ʱ(�Ӹ��˵�������Ӳ˵��˳�)��Ҫִ��һ�εĺ���, ΪNULL��ִ��
  * @param  runFun   ��ǰ�˵�ѡ������ڵ��Ⱥ���
  * @param  extendData ��ǰѡ��Ĳ˵���ʾЧ��������չ�������, ���������ø�����
  */
#define LIB_MENU_ITEM_BIND(title, enterFun, exitFun, loadFun, runFun, extendData) \
    { \
        .uMenuDesc = {.pVoid = (void *)title}, \
        .pfnEnterCallFun = enterFun, \
        .pfnExitCallFun = exitFun, \
        .pfnLoadCallFun = loadFun, \
        .pfnRunCallFun = runFun, \
        .pExtendData = extendData \
    }

// ����õ��˵��б�Ԫ����Ŀ
#define LIB_GET_MENU_NUM(X)    (sizeof(X) / sizeof(LibMenuList_t))

/* Exported functions ------------------------------------------------------------------------------------------------*/

/* �˵���ʼ���ͷ���ʼ�� */

extern int libMenu_Init(LibMainMenuCfg_t *pMainMenu);
extern int libMenu_DeInit(void);

extern int libMenu_Bind(LibMenuList_t *pMenuList, menusize_t menuNum, libShowMenuCallFun_f pfnShowMenuFun);

/* �˵�ѡ����ʾʱ��Ҫʹ�õĹ�����չ���� */

extern int libMenu_LimitShowListNum(LibMenuShow_t *ptMenuShow, menusize_t *pShowNum);
extern int libMenu_QueryParentMenu(LibMenuShow_t *ptMenuShow, uint8_t level);

/* �˵����� */

extern int libMenu_MainEnter(void);
extern int libMenu_MainExit(void);
extern int libMenu_Reset(void);
extern int libMenu_Enter(void);
extern int libMenu_Exit(bool isReset);
extern int libMenu_SelectPrevious(bool isAllowRoll);
extern int libMenu_SelectNext(bool isAllowRoll);
extern int libMenu_Select(menusize_t selectItem);
extern int libMenu_ShortcutEnter(bool isAbsolute, uint8_t deep, ...);

/* �˵���ѯ�������� */

extern int libMenu_Task(void);

#ifdef __cplusplus
 }
#endif

#endif // MENU_H



