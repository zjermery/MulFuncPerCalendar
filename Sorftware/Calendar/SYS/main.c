#include "sys.h"
#include "usart3.h"


SENSOR SensorData;//传感器结构体定义
SYSTEM System;//系统标志位结构体定义

void mqttPublic(void);
void Threshold_Init(void);

int main(void)
{
    delay_init();//延时函数初始化  
    NVIC_Config(); //中断优先级配置
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    My_USART1();//串口1初始化 （调试打印用） 波特率：9600 
    Usart3_Init(9600);//串口3初始化 （与HC-05通信用）波特率：115200
    LED_GPIO_Config();//LED灯初始化
    Beep_Init();
    oled_Init();//oled初始化
    BH1750_Init();//BH1750初始化
    
    DHT11_Init();//温湿度传感器初始化    
    KEY_Init();//按键初始化
    Threshold_Init();//传感器阈值及执行器件开关变量初始化
    TIM1_Int_Init(99,3599);//定时器1初始化 定时扫描按键
    MyRTC_Init();//RTC时钟初始化
    while(1)			
    {
        SensorData.LightVal = Light_Intensity();
        DHT11_Read_TempAndHumidity(&dht11Data);//获取温湿度;
        Mode_Decide();//模式判定 （按键1选择模式）

        if(System.BT_Pub)
        {
            u3_printf("Sensor1:%d#,Sensor2:%d#,Sensor3:%.2f#",dht11Data.temp_int,dht11Data.humi_int,SensorData.LightVal);
            System.BT_Pub=0;
        }      
    }
}
	

//传感器阈值及执行器件开关变量初始化
void Threshold_Init(void)
{ 
    System.Switch1=System.Switch2=System.Switch3=System.Switch4=0;
    System.BT_Pub=0;
    System.ClearFlag=0;
    Alarm1.Hour=10;
    Alarm1.Minute=20;   
    Alarm1.Second=0;
    Alarm2.Hour=11;
    Alarm2.Minute=20;   
    Alarm2.Second=0;  
    Alarm3.Hour=12;
    Alarm3.Minute=20;   
    Alarm3.Second=0;     
}


