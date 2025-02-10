/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2025-01-05 08:52:11
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2025-02-08 17:49:08
 * @FilePath: \Calendar\User\main.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/OLED/oled.h"
#include "./BSP/KEY/key.h"
#include "./MALLOC/malloc.h"
#include "./BSP/DS1302/ds1302.h"
#include "./BSP/OLED/bmp.h"
#include "./BSP/DH11/dht11.h"
#include "./BSP/BEEP/beep.h"
#include "gui.h"
#include "app_ui.h"
#include "lib_menu.h"
#include "lib_language.h"
#include "string.h"
extern unsigned char BMP1[];
extern const unsigned char thermometer23x44[138];

extern unsigned int rec_data[4];

#define USE_MENU_TASK 0

uint32_t tempAndhumi = 0;
//uint8_t buf[4];

#if USE_MENU_TASK
// 扩展数据为图标文件名字，每个菜单选项描述为文本ID
LibMenuList_t sg_MainMenuTable[] = 
{
#if 0
    LIB_MENU_ITEM_BIND(TEXT_MUSIC,  Hmi_MusicEnter, Hmi_MusicExit, Hmi_MusicLoad, Hmi_MusicTask, (MenuImage_t *)&sgc_MusicImage),
  
    LIB_MENU_ITEM_BIND(TEXT_VIDEO,  NULL, Hmi_VideoExit, Hmi_VideoLoad, Hmi_VideoTask, (MenuImage_t *)&sgc_VideoImage),
  
    LIB_MENU_ITEM_BIND(TEXT_CAMERA,  Hmi_CameraEnter, Hmi_CameraExit, Hmi_CameraLoad, Hmi_CameraTask, (MenuImage_t *)&sgc_CameraImage),
  
    LIB_MENU_ITEM_BIND(TEXT_SETTING,  Hmi_SetEnter, Hmi_SetExit, Hmi_SetLoad,   Hmi_SetTask, (MenuImage_t *)&sgc_SettingImage),
#endif
#if 1
    LIB_MENU_ITEM_BIND(TEXT_MUSIC,  NULL, NULL, NULL, NULL, NULL),
  
    LIB_MENU_ITEM_BIND(TEXT_VIDEO,  NULL, NULL, NULL, NULL, NULL),
  
    LIB_MENU_ITEM_BIND(TEXT_CAMERA,  NULL, NULL, NULL, NULL, NULL),
  
    LIB_MENU_ITEM_BIND(TEXT_SETTING,  NULL, NULL, NULL,   NULL, NULL),
#endif
};

/* 主菜单显示效果 */
static void ShowMainMenu(LibMenuShow_t *ptShowInfo)
{
    printf("show main menu\n");
#if 1
    const char *pszSelectDesc = get_text((TextId_e)ptShowInfo->uItemsListDesc[ptShowInfo->selectItem].textId);

    char idx = (128 - 6 * strlen(pszSelectDesc)) / 2;

    // cotOled_DrawGraphic(40, 0, (const char *)ptShowInfo->pItemsListExtendData[ptShowInfo->selectItem], 1);
    // cotOled_SetText(0, 50, "                ", 0, FONT_12X12);
    // GUI_ShowString(0,50,(uint8_t*)"hello world",16,1);
    // cotOled_SetText(idx, 50, pszSelectDesc, 0, FONT_12X12);

    // DisplayShowTempHumi1();
    DisplayshowCursor();
#endif
}

void Hmi_EnterMainHmi(const LibMenuItemInfo_t *pItemInfo)
{
    printf("enter main hmi\n");
    libMenu_Bind(sg_MainMenuTable, LIB_GET_MENU_NUM(sg_MainMenuTable), ShowMainMenu);
}

// static LibMainMenuCfg_t sg_tMainMenu = {"主菜单", Hmi_EnterMainHmi, NULL, NULL, NULL};


#endif

int main(void)
{
    HAL_Init();                         /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9); /* 设置时钟, 72Mhz */
    delay_init(72);                     /* 延时初始化 */
    usart_init(115200);                 /* 串口初始化为115200 */
    led_init();                         /* 初始化LED */
    OLED_Init();
	ds1302_init(); 
    DHT11_Init();
	beep_init();

#if USE_MENU_TASK

    set_language(SYSTEM_LANGUAGE_CHINESE);  // 默认设置中文
    printf("language set to chinese\n");

    LibMainMenuCfg_t tMainMenu = LIB_MENU_ITEM_BIND(TEXT_MAIN_MENU, Hmi_EnterMainHmi, NULL, NULL, NULL, NULL);
  
    int ret = libMenu_Init(&tMainMenu);
    libMenu_MainEnter();
#endif

    while (1)
    {
        

        // if (timeFlag)
        // {
        //     timeFlag = 0;
        #if USE_MENU_TASK
            libMenu_Task(); // 周期调度
        #endif
        // } 
    }
}


























