/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2025-01-05 08:52:11
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2025-02-08 17:49:08
 * @FilePath: \Calendar\User\main.c
 * @Description: ����Ĭ������,������`customMade`, ��koroFileHeader�鿴���� ��������: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
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

extern unsigned char BMP1[];
extern const unsigned char thermometer23x44[138];

extern unsigned int rec_data[4];

//Time_s newtime;

uint32_t tempAndhumi = 0;
//uint8_t buf[4];
int main(void)
{
    HAL_Init();                         /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9); /* ����ʱ��, 72Mhz */
    delay_init(72);                     /* ��ʱ��ʼ�� */
    usart_init(115200);                 /* ���ڳ�ʼ��Ϊ115200 */
    led_init();                         /* ��ʼ��LED */
    OLED_Init();
	ds1302_init(); 
    DHT11_Init();
	beep_init();
	printf("init finish\n");

    while (1)
    {
        OLED_Display();
//        GUI_ShowCHinese(0,0,16,"��ʪ�ȡ�",1);
//        GUI_ShowCHineseAndASCII(0,0,"��23ʪ�ȡ�",1);
        // DHT11_Read_Data(&tempAndhumi); 
        DisplayShowTimeData();
        // DisplayShowTempHumi(tempAndhumi);
//        delay_ms(1000);
        // OLED_Clear(0);  
    }
}





























