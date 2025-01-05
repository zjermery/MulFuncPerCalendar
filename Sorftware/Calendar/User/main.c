#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/OLED/oled.h"
#include "./BSP/KEY/key.h"
#include "./MALLOC/malloc.h"
#include "./BSP/DS1302/ds1302.h"

extern unsigned char BMP1[];
extern uint8_t write[];
extern uint8_t read[];
uint8_t gewei[7],shiwei[7];

int main(void)
{
    HAL_Init();                         /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9); /* 设置时钟, 72Mhz */
    delay_init(72);                     /* 延时初始化 */
    usart_init(115200);                 /* 串口初始化为115200 */
    led_init();                         /* 初始化LED */
    OLED_Init();
	  ds1302_init(); 
		

    while (1)
    {
		uint8_t nianyueri[8];
        uint8_t shifenmiao[8];
        ds1302_print_data(read);							//????DS1302??
		nianyueri[0]=2;
        nianyueri[1]=0;
        nianyueri[2]=shiwei[6];
        nianyueri[3]=gewei[6];
        nianyueri[4]=shiwei[4];
        nianyueri[5]=gewei[4];
        nianyueri[6]=shiwei[3];
        nianyueri[7]=gewei[3];
        shifenmiao[0]=shiwei[2];
        shifenmiao[1]=gewei[2];
        shifenmiao[2]=shiwei[1];
        shifenmiao[3]=gewei[1];
        shifenmiao[4]=shiwei[0];
        shifenmiao[5]=gewei[0];
        shifenmiao[6]=shiwei[5];
        shifenmiao[7]=gewei[5];	
        // OLED_ShowPicture(0,0,128,64,BMP1,1);
		OLED_Refresh();
		delay_ms(500);
		OLED_Clear();
        OLED_ShowChar(1,1,shiwei[6],16,1);
        OLED_ShowChar(8,1,gewei[6],16,1);
        OLED_ShowChar(16,1,shiwei[5],16,1);
        OLED_ShowChar(24,1,gewei[5],16,1);
        OLED_ShowChar(32,1,shiwei[4],16,1);
        OLED_ShowChar(40,1,gewei[4],16,1);
        OLED_ShowChar(1,16,shiwei[3],16,1);
        OLED_ShowChar(8,16,gewei[3],16,1);
        OLED_ShowChar(16,16,shiwei[2],16,1);
        OLED_ShowChar(24,16,gewei[2],16,1);
        OLED_ShowChar(32,16,shiwei[1],16,1);
        OLED_ShowChar(40,16,gewei[1],16,1);

		// OLED_ShowString(8,16,"ZHANGWEI",16,1);
		// OLED_ShowString(20,32,"2014/05/01",16,1);
		// OLED_ShowString(0,48,"ASCII:",16,1);  
		// OLED_ShowString(63,48,"CODE:",16,1);
		// OLED_ShowChar(48,48,t,16,1);//显示ASCII字符	

    }
}





























