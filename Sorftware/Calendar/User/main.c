/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2025-01-05 08:52:11
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2025-01-05 14:45:35
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
#include "./BSP/OLED/bsp_oled.h"

extern unsigned char BMP1[];
extern uint8_t write[];
extern uint8_t read[];
uint8_t gewei[7],shiwei[7];
Time_s newtime;
int main(void)
{
    HAL_Init();                         /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9); /* 设置时钟, 72Mhz */
    delay_init(72);                     /* 延时初始化 */
    usart_init(115200);                 /* 串口初始化为115200 */
    led_init();                         /* 初始化LED */
    OLED_Init();
	ds1302_init(); 
	printf("init finish\n");

    while (1)
    {
		ds1302_get_time(&newtime);
        printf("year:%d,month:%d,date:%d,day:%d,hour:%d,min:%d,sec:%d\r\n",newtime.year,newtime.month,newtime.date,newtime.day,newtime.hour,newtime.min,newtime.sec);
        printf("clockSystem:%d,clockPeriod:%s\r\n",newtime.clockSystem?12:24,newtime.clockPeriod?"PM":"AM");
//        delay_ms(2000);
        
//        OLED_ShowChinese(0,0,0,16,1);//中
//		OLED_ShowChinese(18,0,1,16,1);//景
//		OLED_ShowChinese(36,0,2,16,1);//园
//		OLED_ShowChinese(54,0,3,16,1);//电
//		OLED_ShowChinese(72,0,4,16,1);//子
//		OLED_ShowChinese(90,0,5,16,1);//技
//		OLED_ShowChinese(108,0,6,16,1);//术
        // printf("key0:%d,key1:%d,wkup:%d\r\n",KEY0,KEY1,WK_UP);
        // OLED_ShowPicture(0,0,128,64,BMP1,1);
		OLED_Refresh();
		delay_ms(500);
		OLED_Clear();
        // OLED_ShowNum(1,1,2025,4,16,1);
        // OLED_ShowChinese(32,1,1,16,1);
        OLED_ShowYearDate(1,1, newtime);
//        OLED_ShowChar(1,1,newtime,16,1);
//        OLED_ShowChar(8,1,gewei[6],16,1);
//        OLED_ShowChar(16,1,shiwei[5],16,1);
//        OLED_ShowChar(24,1,gewei[5],16,1);
//        OLED_ShowChar(32,1,shiwei[4],16,1);
//        OLED_ShowChar(40,1,gewei[4],16,1);
//        OLED_ShowChar(1,16,shiwei[3],16,1);
//        OLED_ShowChar(8,16,gewei[3],16,1);
//        OLED_ShowChar(16,16,shiwei[2],16,1);
//        OLED_ShowChar(24,16,gewei[2],16,1);
//        OLED_ShowChar(32,16,shiwei[1],16,1);
//        OLED_ShowChar(40,16,gewei[1],16,1);

		// OLED_ShowString(8,16,"ZHANGWEI",16,1);
		// OLED_ShowString(20,32,"2014/05/01",16,1);
		// OLED_ShowString(0,48,"ASCII:",16,1);  
		// OLED_ShowString(63,48,"CODE:",16,1);
		// OLED_ShowChar(48,48,t,16,1);//显示ASCII字符	

    }
}





























