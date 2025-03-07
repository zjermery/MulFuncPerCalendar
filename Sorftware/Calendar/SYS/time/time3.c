#include "sys.h"
/********************
��ʹ�ܶ�ʱ��ʱ�ӡ�
RCC_APB1PeriphClockCmd();
�� ��ʼ����ʱ��������ARR,PSC��
TIM_TimeBaseInit();
�۶�ʱ���ж����ã�ͬʱ����NVIC��
void TIM_ITConfig();
NVIC_Init();
��ʹ�ܶ�ʱ����
TIM_Cmd();
�ݱ�д�жϷ�������
TIMx_IRQHandler();
*****************/
u8 Tim3Count=0;
uint8_t PingCount=0;
void TIM3_Init(u32 arr,u32 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_Prescaler = psc;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
	
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);		
	TIM_Cmd(TIM3, ENABLE);
}

void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  
	{    
        Tim3Count++;
        if(Tim3Count>=5)
        {
            Tim3Count=0;
            mqtt_Ping();
            PingCount++;
        }
        
        if(Ping_flag) Ping_flag=PingCount=0;    	
		else
        {
            if(PingCount>=2) Connect_flag = Ping_flag =PingCount= 0; 
        }
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update); 					
	}

}
