/**
  **********************************************************************************************************************
  * @file    cot_menu.c
  * @brief   ���ļ��ṩ�˵���ܹ���
  * @author  const_zpc  any question please send mail to const_zpc@163.com
  * @version V1.3.0
  * @date    2024-06-09
  *
  * @details  ������ϸ˵����
  *           + �˵���ʼ������
  *           + �������˵�����
  *           + �˵����ƺ���
  *           + �˵���ѯ������
  *
  **********************************************************************************************************************
  * Դ��·����https://gitee.com/cot_package/cot_menu.git �������⼰������ڸ���ַ��д Issue
  *
  * ʹ�÷�ʽ:
  *    1��ʹ��ǰ��ʼ������ cotMenu_Init, �������˵�����
  *    2�����ڵ��ú��� cotMenu_Task, ��������˵���ʾ��ִ����ػص�����
  *    3��ʹ�����������Բ˵��������
  *
  **********************************************************************************************************************
  */

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "lib_menu.h"

#ifdef _LIB_MENU_USE_MALLOC_
#include <malloc.h>
#endif

#ifdef _LIB_MENU_USE_SHORTCUT_
#include <stdarg.h>
#endif

/* Private typedef ---------------------------------------------------------------------------------------------------*/
typedef struct MenuCtrl
{
    struct MenuCtrl         *pParentMenuCtrl;    /*!< ���˵����ƴ��� */
    LibMenuDsecStr_u         uMenuDesc;          /*!< ��ǰ�˵����ַ������� */
    void                    *pExtendData;        /*!< ��ǰѡ��ע��ʱ����չ���� */
    libShowMenuCallFun_f     pfnShowMenuFun; /*!< ��ǰ�˵���ʾЧ������ */
    LibMenuList_t           *pMenuList;          /*!< ��ǰ�˵��б� */
    libMenuCallFun_f         pfnLoadCallFun;     /*!< ��ǰ�˵����غ��� */
    libMenuCallFun_f         pfnRunCallFun;      /*!< ��ǰѡ��ķǲ˵����ܺ��� */
    menusize_t               itemsNum;           /*!< ��ǰ�˵�ѡ������Ŀ */
    menusize_t               showBaseItem;       /*!< ��ǰ�˵��׸���ʾ��ѡ�� */
    menusize_t               selectItem;         /*!< ��ǰ�˵�ѡ�е�ѡ�� */
    bool                     isSelected;         /*!< �˵�ѡ���Ƿ��Ѿ���ѡ�� */
}MenuCtrl_t;

typedef struct
{
    MenuCtrl_t              *pMenuCtrl;           /*!< ��ǰ�˵����ƴ��� */
    libMenuCallFun_f         pfnMainEnterCallFun; /*!< ���˵�����ʱ(����˵�)��Ҫִ��һ�εĺ��� */
    libMenuCallFun_f         pfnMainExitCallFun;  /*!< ���˵�������˳�ʱ(�˳��˵�)��Ҫִ��һ�εĺ��� */
    libMenuCallFun_f         pfnLoadCallFun;      /*!< �ؼ��غ��� */
    uint8_t                  isEnterMainMenu : 1; /*!< �Ƿ���������˵� */
}MenuManage_t;

/* Private define ----------------------------------------------------------------------------------------------------*/
/* Private macro -----------------------------------------------------------------------------------------------------*/
/* Private variables -------------------------------------------------------------------------------------------------*/
static MenuManage_t sg_tMenuManage;

#ifndef _LIB_MENU_USE_MALLOC_
static MenuCtrl_t sg_arrMenuCtrl[LIB_MENU_MAX_DEPTH];
#endif

static uint8_t    sg_currMenuDepth = 0;

/* Private function prototypes ---------------------------------------------------------------------------------------*/
static MenuCtrl_t *NewMenu(void);
static void DeleteMenu(MenuCtrl_t *pMenu);

/* Private function --------------------------------------------------------------------------------------------------*/
/**
  * @brief      �½��˵��㼶
  * 
  * @return     MenuCtrl_t* 
  */
static MenuCtrl_t *NewMenu(void)
{
    MenuCtrl_t *pMenuCtrl = NULL;

    if (sg_currMenuDepth < LIB_MENU_MAX_DEPTH)
    {
#ifdef _LIB_MENU_USE_MALLOC_
        pMenuCtrl = (MenuCtrl_t *)malloc(sizeof(MenuCtrl_t));
#else  
        pMenuCtrl = &sg_arrMenuCtrl[sg_currMenuDepth];
#endif
        sg_currMenuDepth++;
    }

    return pMenuCtrl;
}

/**
  * @brief      ���ٲ˵��㼶
  * 
  * @param      pMenu 
  */
static void DeleteMenu(MenuCtrl_t *pMenu)
{
#ifdef _LIB_MENU_USE_MALLOC_
    free(pMenu);
#endif
    if (sg_currMenuDepth > 0)
    {
        sg_currMenuDepth--;
    }
}

/**
  * @brief      �˵���ʼ��
  * 
  * @param[in]  pMainMenu        ���˵�ע����Ϣ
  * @return     0,�ɹ�; -1,ʧ�� 
  */
int libMenu_Init(LibMainMenuCfg_t *pMainMenu)
{
    MenuCtrl_t *pNewMenuCtrl = NULL;

    if (sg_tMenuManage.pMenuCtrl != NULL)
    {
        return -1;
    }

#if LIB_MENU_MAX_DEPTH != 0
    sg_currMenuDepth = 0;
#endif

    if ((pNewMenuCtrl = NewMenu()) == NULL)
    {
        return -1;
    }

    pNewMenuCtrl->uMenuDesc = pMainMenu->uMenuDesc;
    pNewMenuCtrl->pExtendData = pMainMenu->pExtendData;
    pNewMenuCtrl->pParentMenuCtrl = NULL;
    pNewMenuCtrl->pfnLoadCallFun = pMainMenu->pfnLoadCallFun;
    pNewMenuCtrl->pfnShowMenuFun = NULL;
    pNewMenuCtrl->pfnRunCallFun = pMainMenu->pfnRunCallFun;
    
    pNewMenuCtrl->pMenuList = NULL;
    pNewMenuCtrl->itemsNum = 0;
    pNewMenuCtrl->selectItem = 0;
    pNewMenuCtrl->showBaseItem = 0;

    sg_tMenuManage.pMenuCtrl = pNewMenuCtrl;
    sg_tMenuManage.isEnterMainMenu = 0;
    sg_tMenuManage.pfnMainEnterCallFun = pMainMenu->pfnEnterCallFun;
    sg_tMenuManage.pfnMainExitCallFun = pMainMenu->pfnExitCallFun;
    sg_tMenuManage.pfnLoadCallFun = pNewMenuCtrl->pfnLoadCallFun;

    return 0;
}

/**
  * @brief  �˵�����ʼ��
  * 
  * @attention  ���ܴ����κν��涼�����˳������˵��󣨻�����˳������������˳����˵�����󷴳�ʼ��
  * @return 0,�ɹ�; -1,ʧ��  
  */
int libMenu_DeInit(void)
{
    if (sg_tMenuManage.pMenuCtrl == NULL)
    {
        return -1;
    }

    libMenu_MainExit();

    DeleteMenu(sg_tMenuManage.pMenuCtrl);
    sg_tMenuManage.pMenuCtrl = NULL;
    sg_tMenuManage.isEnterMainMenu = 0;
    sg_tMenuManage.pfnMainEnterCallFun = NULL;
    sg_tMenuManage.pfnMainExitCallFun = NULL;
    sg_tMenuManage.pfnLoadCallFun = NULL;

    return 0;
}

/**
  * @brief      �Ӳ˵��󶨵�ǰ�˵�ѡ��
  * 
  * @param      pMenuList       �µĲ˵��б�
  * @param      menuNum         �µĲ˵��б���Ŀ
  * @param      pfnShowMenuFun  �µĲ˵��б���ʾЧ���ص�����, ΪNULL�������ϼ��˵���ʾЧ��
  * @return     0,�ɹ�; -1,ʧ�� 
  */
int libMenu_Bind(LibMenuList_t *pMenuList, menusize_t menuNum, libShowMenuCallFun_f pfnShowMenuFun)
{
    if (sg_tMenuManage.pMenuCtrl == NULL)
    {
        return -1;
    }

    if (sg_tMenuManage.pMenuCtrl->pMenuList != NULL)
    {
        return 0;
    }

    sg_tMenuManage.pMenuCtrl->pMenuList = pMenuList;
    sg_tMenuManage.pMenuCtrl->itemsNum = menuNum;
    printf("menuNum:%d\r\n",menuNum);
    if (pfnShowMenuFun != NULL)
    {
        printf("show pfnShowMenuFun\r\n");
        sg_tMenuManage.pMenuCtrl->pfnShowMenuFun = pfnShowMenuFun;
    }

    return 0;
}

/**
  * @brief      ��λ�˵�, �ص����˵�����
  * 
  * @note       �ø�λ�ص����˵�����ִ���˳�����Ҫִ�еĻص�����
  * @return     0,�ɹ�; -1,ʧ�� 
  */
int libMenu_Reset(void)
{
    if (sg_tMenuManage.pMenuCtrl == NULL || sg_tMenuManage.isEnterMainMenu == 0)
    {
        return -1;
    }

    while (sg_tMenuManage.pMenuCtrl->pParentMenuCtrl != NULL)
    {
        MenuCtrl_t *pMenuCtrl = sg_tMenuManage.pMenuCtrl;

        sg_tMenuManage.pMenuCtrl = sg_tMenuManage.pMenuCtrl->pParentMenuCtrl;
        DeleteMenu(pMenuCtrl);
    }

    sg_tMenuManage.pMenuCtrl->selectItem = 0;
    
    return 0;
}

/**
  * @brief      ���˵�����
  * 
  * @return     0,�ɹ�; -1,ʧ��  
  */
int libMenu_MainEnter(void)
{
    if (sg_tMenuManage.pMenuCtrl == NULL || sg_tMenuManage.isEnterMainMenu == 1)
    {
        return -1;
    }

    if (sg_tMenuManage.pfnMainEnterCallFun != NULL)
    {
        LibMenuItemInfo_t tItemInfo;

        tItemInfo.uMenuDesc = sg_tMenuManage.pMenuCtrl->uMenuDesc;
        tItemInfo.pExtendData = sg_tMenuManage.pMenuCtrl->pExtendData;
        sg_tMenuManage.pfnMainEnterCallFun(&tItemInfo);
    }

    sg_tMenuManage.isEnterMainMenu = 1;
    sg_tMenuManage.pfnLoadCallFun = sg_tMenuManage.pMenuCtrl->pfnLoadCallFun;

    return 0;
}

/**
  * @brief      ���˵��˳�
  * 
  * @attention  ���ܴ����κν��涼�����˳������˵��󣨻�����˳������������˳����˵�
  * @return     0,�ɹ�; -1,ʧ��  
  */
int libMenu_MainExit(void)
{
    if (sg_tMenuManage.pMenuCtrl == NULL || sg_tMenuManage.isEnterMainMenu == 0)
    {
        return -1;
    }

    while (libMenu_Exit(1) == 0){}

    if (sg_tMenuManage.pfnMainExitCallFun != NULL)
    {
        sg_tMenuManage.pfnMainExitCallFun(NULL);
    }

    sg_tMenuManage.isEnterMainMenu = 0;

    return 0;
}

/**
  * @brief      ���뵱ǰ�˵�ѡ��
  * 
  * @return     0,�ɹ�; -1,ʧ��  
  */
int libMenu_Enter(void)
{
    MenuCtrl_t *pNewMenuCtrl = NULL;
    MenuCtrl_t *pCurrMenuCtrl = sg_tMenuManage.pMenuCtrl;

    if (sg_tMenuManage.pMenuCtrl == NULL || sg_tMenuManage.pMenuCtrl->pMenuList == NULL || sg_tMenuManage.isEnterMainMenu == 0)
    {
        return -1;
    }

    if ((pNewMenuCtrl = NewMenu()) == NULL)
    {
        return -1;
    }

    pNewMenuCtrl->uMenuDesc = pCurrMenuCtrl->pMenuList[pCurrMenuCtrl->selectItem].uMenuDesc;
    pNewMenuCtrl->pExtendData = pCurrMenuCtrl->pMenuList[pCurrMenuCtrl->selectItem].pExtendData;
    pNewMenuCtrl->pMenuList = NULL;
    pNewMenuCtrl->itemsNum = 0;
    pNewMenuCtrl->pfnShowMenuFun = pCurrMenuCtrl->pfnShowMenuFun;
    pNewMenuCtrl->pfnLoadCallFun = pCurrMenuCtrl->pMenuList[pCurrMenuCtrl->selectItem].pfnLoadCallFun;
    pNewMenuCtrl->pfnRunCallFun = pCurrMenuCtrl->pMenuList[pCurrMenuCtrl->selectItem].pfnRunCallFun;
    pNewMenuCtrl->selectItem = 0;
    pNewMenuCtrl->isSelected = true;
    pNewMenuCtrl->pParentMenuCtrl = pCurrMenuCtrl;
    
    sg_tMenuManage.pMenuCtrl = pNewMenuCtrl;
    sg_tMenuManage.pfnLoadCallFun = pNewMenuCtrl->pfnLoadCallFun;

    if (pCurrMenuCtrl->pMenuList[pCurrMenuCtrl->selectItem].pfnEnterCallFun != NULL)
    {
        LibMenuItemInfo_t tItemInfo;

        tItemInfo.uMenuDesc = sg_tMenuManage.pMenuCtrl->uMenuDesc;
        tItemInfo.pExtendData = sg_tMenuManage.pMenuCtrl->pExtendData;
        pCurrMenuCtrl->pMenuList[pCurrMenuCtrl->selectItem].pfnEnterCallFun(&tItemInfo);
    }

    return 0;
}

/**
  * @brief      �˳���ǰѡ�������һ��˵�
  * 
  * @param[in]  isReset �˵�ѡ���Ƿ��ͷѡ��
  * @return     0,�ɹ�; -1,ʧ��, ��Ŀǰ�������˵�, �޷�����
  */
int libMenu_Exit(bool isReset)
{
    MenuCtrl_t *pMenuCtrl = sg_tMenuManage.pMenuCtrl;

    if (sg_tMenuManage.pMenuCtrl == NULL || sg_tMenuManage.isEnterMainMenu == 0)
    {
        return -1;
    }

    if (sg_tMenuManage.pMenuCtrl->pParentMenuCtrl == NULL)
    {
        return -1;
    }

    sg_tMenuManage.pMenuCtrl = sg_tMenuManage.pMenuCtrl->pParentMenuCtrl;
    sg_tMenuManage.pfnLoadCallFun = sg_tMenuManage.pMenuCtrl->pfnLoadCallFun;
    DeleteMenu(pMenuCtrl);
    pMenuCtrl = NULL;
    
    if (sg_tMenuManage.pMenuCtrl->pMenuList[sg_tMenuManage.pMenuCtrl->selectItem].pfnExitCallFun != NULL)
    {
        LibMenuItemInfo_t tItemInfo;

        tItemInfo.uMenuDesc = sg_tMenuManage.pMenuCtrl->pMenuList[sg_tMenuManage.pMenuCtrl->selectItem].uMenuDesc;
        tItemInfo.pExtendData = sg_tMenuManage.pMenuCtrl->pMenuList[sg_tMenuManage.pMenuCtrl->selectItem].pExtendData;
        sg_tMenuManage.pMenuCtrl->isSelected = false;
        sg_tMenuManage.pMenuCtrl->pMenuList[sg_tMenuManage.pMenuCtrl->selectItem].pfnExitCallFun(&tItemInfo);
    }

    if (isReset)
    {
        sg_tMenuManage.pMenuCtrl->selectItem = 0;
    }

    return 0;
}

/**
  * @brief      ѡ����һ���˵�ѡ��
  * 
  * @param[in]  isAllowRoll ��һ��ѡ��ʱ�Ƿ����ת�����һ��ѡ��
  * @return     0,�ɹ�; -1,ʧ��
  */
int libMenu_SelectPrevious(bool isAllowRoll)
{
    if (sg_tMenuManage.pMenuCtrl == NULL || sg_tMenuManage.pMenuCtrl->pMenuList == NULL || sg_tMenuManage.isEnterMainMenu == 0)
    {
        return -1;
    }

    if (sg_tMenuManage.pMenuCtrl->selectItem > 0)
    {
        sg_tMenuManage.pMenuCtrl->selectItem--;
    }
    else
    {
        if (isAllowRoll)
        {
            sg_tMenuManage.pMenuCtrl->selectItem = sg_tMenuManage.pMenuCtrl->itemsNum - 1;
        }
        else
        {
            sg_tMenuManage.pMenuCtrl->selectItem = 0;
            return -1;
        }        
    }

    return 0;
}

/**
  * @brief      ѡ����һ���˵�ѡ��
  * 
  * @param[in]  isAllowRoll ���һ��ѡ��ʱ�Ƿ���ת����һ��ѡ��
  * @return     0,�ɹ�; -1,ʧ�� 
  */
int libMenu_SelectNext(bool isAllowRoll)
{
    if (sg_tMenuManage.pMenuCtrl == NULL || sg_tMenuManage.pMenuCtrl->pMenuList == NULL || sg_tMenuManage.isEnterMainMenu == 0)
    {
        return -1;
    }

    if (sg_tMenuManage.pMenuCtrl->selectItem < (sg_tMenuManage.pMenuCtrl->itemsNum - 1))
    {
        sg_tMenuManage.pMenuCtrl->selectItem++;
    }
    else
    {
        if (isAllowRoll)
        {
            sg_tMenuManage.pMenuCtrl->selectItem = 0;
        }
        else
        {
            sg_tMenuManage.pMenuCtrl->selectItem = sg_tMenuManage.pMenuCtrl->itemsNum - 1;
            return -1;
        }
    }

    return 0;
}

/**
  * @brief      ѡ��ָ���Ĳ˵�ѡ��
  * 
  * @param      selectItem ָ���Ĳ˵�ѡ��
  * @return     0,�ɹ�; -1,ʧ�� 
  */
int libMenu_Select(menusize_t selectItem)
{
    if (sg_tMenuManage.pMenuCtrl == NULL || sg_tMenuManage.pMenuCtrl->pMenuList == NULL || sg_tMenuManage.isEnterMainMenu == 0)
    {
        return -1;
    }

    if (selectItem >= sg_tMenuManage.pMenuCtrl->itemsNum)
    {
        return -1;
    }

    sg_tMenuManage.pMenuCtrl->selectItem = selectItem;

    return 0;
}

#ifdef _LIB_MENU_USE_SHORTCUT_

/**
 * @brief      ������˵���ǰ�˵�ͨ���¼����˵��������ٽ���ָ��ѡ��
 * 
 * @param[in]  isAbsolute �Ƿ���þ��Բ˵������������˵���ʼ��
 * @param[in]  deep �˵���ȣ����� 0
 * @param[in]  ...  �����˵�����ֵ(��0��ʼ), ��θ����� deep ��ֵ����
 * @return     0,�ɹ�; -1,ʧ��
 */
int libMenu_ShortcutEnter(bool isAbsolute, uint8_t deep, ...)
{
    uint8_t selectDeep = 0;
    va_list pItemList;
    menusize_t selectItem;

    if (sg_tMenuManage.pMenuCtrl == NULL || sg_tMenuManage.pMenuCtrl->pMenuList == NULL || sg_tMenuManage.isEnterMainMenu == 0)
    {
        return -1;
    }

    if (isAbsolute)
    {
        libMenu_Reset();
    }

    va_start(pItemList, deep);

    while (selectDeep < deep)
    {
        selectItem = va_arg(pItemList, int);

        if (selectItem >= sg_tMenuManage.pMenuCtrl->itemsNum)
        {
            va_end(pItemList);
            return -1;
        }

        sg_tMenuManage.pMenuCtrl->selectItem = selectItem;
        libMenu_Enter();
        selectDeep++;
    }

    va_end(pItemList);

    return 0;
}

#endif

/**
  * @brief      ���Ƶ�ǰ�˵����������ʾ�Ĳ˵���Ŀ
  * 
  * @note       �ڲ˵���ʾЧ���ص�������ʹ��, ʹ�ó�Ա���� showBaseItem �õ���ʾ����ĵ�һ��ѡ������
  * @param[in,out]  tMenuShow   ��ǰ�˵���ʾ��Ϣ
  * @param[in,out]  showNum     ��ǰ�˵�����Ҫ��ʾ��ѡ����Ŀ, ���ݵ�ǰ�˵�ѡ��������õ����յ���ʾ��ѡ����Ŀ
  * @return     0,�ɹ�; -1,ʧ�� 
  */
int libMenu_LimitShowListNum(LibMenuShow_t *ptMenuShow, menusize_t *pShowNum)
{
    if (ptMenuShow == NULL || pShowNum == NULL)
    {
        return -1;
    }

    if (*pShowNum > ptMenuShow->itemsNum)
    {
        *pShowNum = ptMenuShow->itemsNum;
    }

    if (ptMenuShow->selectItem < ptMenuShow->showBaseItem)
    {
        ptMenuShow->showBaseItem = ptMenuShow->selectItem;
    }
    else if ((ptMenuShow->selectItem - ptMenuShow->showBaseItem) >= *pShowNum)
    {
        ptMenuShow->showBaseItem = ptMenuShow->selectItem - *pShowNum + 1;
    }
    else
    {
        // ����
    }

    return 0;
}

/**
 * @brief       ��ȡ��ǰ���˵���ʾ��Ϣ
 *              ���ȡ��ǰ�˵��Ķ������˵���Ϣ��level Ϊ2
 * 
 * @param[out]  ptMenuShow �� n ���˵���ʾ��Ϣ
 * @param[in]   level      n ��, ���� 0
 * @return      0,�ɹ�; -1,ʧ��  
 */
int libMenu_QueryParentMenu(LibMenuShow_t *ptMenuShow, uint8_t level)
{
    int i;
    LibMenuList_t *pMenu;
    MenuCtrl_t *pMenuCtrl = NULL;

    if (sg_tMenuManage.pMenuCtrl == NULL || sg_tMenuManage.isEnterMainMenu == 0)
    {
        return -1;
    }

    pMenuCtrl = sg_tMenuManage.pMenuCtrl->pParentMenuCtrl;

    while (level && pMenuCtrl != NULL)
    {
        pMenu = pMenuCtrl->pMenuList;
        ptMenuShow->itemsNum = pMenuCtrl->itemsNum;
        ptMenuShow->selectItem = pMenuCtrl->selectItem;
        ptMenuShow->showBaseItem = pMenuCtrl->showBaseItem;
        ptMenuShow->uMenuDesc = sg_tMenuManage.pMenuCtrl->uMenuDesc;

        for (i = 0; i < ptMenuShow->itemsNum && i < LIB_MENU_MAX_NUM; i++)
        {
            ptMenuShow->uItemsListDesc[i] = pMenu[i].uMenuDesc;
            ptMenuShow->pItemsListExtendData[i] = pMenu[i].pExtendData;
        }

        pMenuCtrl = pMenuCtrl->pParentMenuCtrl;
        level--;
    }

    if (level != 0 && pMenuCtrl == NULL)
    {
        return -1;
    }

    return 0;
}

/**
  * @brief  �˵�����
  * 
  * @return 0,�ɹ�, ���ڲ˵�ģʽ��; -1,ʧ��, δ���ڲ˵�ģʽ�� 
  */
int libMenu_Task(void)
{
    int i;
    LibMenuList_t *pMenuList;
    LibMenuShow_t tMenuShow;

    if (sg_tMenuManage.pMenuCtrl == NULL || sg_tMenuManage.isEnterMainMenu == 0)
    {
        return -1;
    }

    if (sg_tMenuManage.pfnLoadCallFun != NULL)
    {
        LibMenuItemInfo_t tItemInfo;

        tItemInfo.uMenuDesc = sg_tMenuManage.pMenuCtrl->uMenuDesc;
        tItemInfo.pExtendData = sg_tMenuManage.pMenuCtrl->pExtendData;
        sg_tMenuManage.pfnLoadCallFun(&tItemInfo);
        sg_tMenuManage.pfnLoadCallFun = NULL;
    }
    
    if (sg_tMenuManage.pMenuCtrl->pMenuList != NULL)
    {
        pMenuList = sg_tMenuManage.pMenuCtrl->pMenuList;
        tMenuShow.itemsNum = sg_tMenuManage.pMenuCtrl->itemsNum;
        tMenuShow.selectItem = sg_tMenuManage.pMenuCtrl->selectItem;
        tMenuShow.showBaseItem = sg_tMenuManage.pMenuCtrl->showBaseItem;

        tMenuShow.uMenuDesc = sg_tMenuManage.pMenuCtrl->uMenuDesc;
        tMenuShow.pExtendData = sg_tMenuManage.pMenuCtrl->pExtendData;

        for (i = 0; i < tMenuShow.itemsNum && i < LIB_MENU_MAX_NUM; i++)
        {
            tMenuShow.uItemsListDesc[i] = pMenuList[i].uMenuDesc;
            tMenuShow.pItemsListExtendData[i] = pMenuList[i].pExtendData;
        }

        if (sg_tMenuManage.pMenuCtrl->pfnShowMenuFun != NULL)
        {
            sg_tMenuManage.pMenuCtrl->pfnShowMenuFun(&tMenuShow);
        }

        sg_tMenuManage.pMenuCtrl->showBaseItem = tMenuShow.showBaseItem;
    }

    if (sg_tMenuManage.pMenuCtrl->pfnRunCallFun != NULL)
    {
        LibMenuItemInfo_t tItemInfo;

        tItemInfo.uMenuDesc = sg_tMenuManage.pMenuCtrl->uMenuDesc;
        tItemInfo.pExtendData = sg_tMenuManage.pMenuCtrl->pExtendData;
        sg_tMenuManage.pMenuCtrl->pfnRunCallFun(&tItemInfo);
    }

    return 0;
}


