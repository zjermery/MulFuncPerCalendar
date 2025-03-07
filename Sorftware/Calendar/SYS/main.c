#include "sys.h"
#include "usart3.h"


SENSOR SensorData;//�������ṹ�嶨��
SYSTEM System;//ϵͳ��־λ�ṹ�嶨��

void mqttPublic(void);
void Threshold_Init(void);

int main(void)
{
    delay_init();//��ʱ������ʼ��  
    NVIC_Config(); //�ж����ȼ�����
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    My_USART1();//����1��ʼ�� �����Դ�ӡ�ã� �����ʣ�9600 
    Usart3_Init(9600);//����3��ʼ�� ����HC-05ͨ���ã������ʣ�115200
    LED_GPIO_Config();//LED�Ƴ�ʼ��
    Beep_Init();
    oled_Init();//oled��ʼ��
    BH1750_Init();//BH1750��ʼ��
    
    DHT11_Init();//��ʪ�ȴ�������ʼ��    
    KEY_Init();//������ʼ��
    Threshold_Init();//��������ֵ��ִ���������ر�����ʼ��
    TIM1_Int_Init(99,3599);//��ʱ��1��ʼ�� ��ʱɨ�谴��
    MyRTC_Init();//RTCʱ�ӳ�ʼ��
    while(1)			
    {
        SensorData.LightVal = Light_Intensity();
        DHT11_Read_TempAndHumidity(&dht11Data);//��ȡ��ʪ��;
        Mode_Decide();//ģʽ�ж� ������1ѡ��ģʽ��

        if(System.BT_Pub)
        {
            u3_printf("Sensor1:%d#,Sensor2:%d#,Sensor3:%.2f#",dht11Data.temp_int,dht11Data.humi_int,SensorData.LightVal);
            System.BT_Pub=0;
        }      
    }
}
	

//��������ֵ��ִ���������ر�����ʼ��
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


