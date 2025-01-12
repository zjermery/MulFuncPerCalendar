/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2025-01-05 08:52:11
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2025-01-12 18:57:04
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
#include "./BSP/OLED/bmp.h"
#include "./BSP/DH11/dht11.h"

extern unsigned char BMP1[];
extern const unsigned char thermometer23x44[138];

extern unsigned int rec_data[4];

Time_s newtime;

uint32_t tempAndhumi = 0;
uint8_t buf[4];
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
	printf("init finish\n");

    while (1)
    {
		// ds1302_get_time(&newtime);
        // printf("year:%d,month:%d,date:%d,day:%d,hour:%d,min:%d,sec:%d\r\n",newtime.year,newtime.month,newtime.date,newtime.day,newtime.hour,newtime.min,newtime.sec);
        // printf("clockSystem:%d,clockPeriod:%s\r\n",newtime.clockSystem?12:24,newtime.clockPeriod?"PM":"AM");
    DHT11_Read_Data(&tempAndhumi);
    // printf("tmperature:%02d.%02d℃,humidity:%02d.%02d%%\r\n",buf[0],buf[1],buf[2],buf[3]);
		OLED_Refresh();
		delay_ms(1000);
		OLED_Clear();
      DisplayShowTempHumi(35,10,tempAndhumi);
       
        // OLED_ShowNum(1,1,2025,4,16,1);
        // OLED_ShowChinese(32,1,1,16,1);
        // DisplayShowYearDate(1,1, newtime);
        

		// OLED_ShowString(8,16,"ZHANGWEI",16,1);
		// OLED_ShowString(20,32,"2014/05/01",16,1);
		// OLED_ShowString(0,48,"ASCII:",16,1);  
		// OLED_ShowString(63,48,"CODE:",16,1);
		// OLED_ShowChar(48,48,t,16,1);//显示ASCII字符	

    }
}





























