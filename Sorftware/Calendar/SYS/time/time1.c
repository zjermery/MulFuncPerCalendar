#include "sys.h"
 
/******************   	  
���ܣ��߼���ʱ��1�жϳ�ʼ��,����ʱ��ѡ��ΪAPB2
������arr���Զ���װֵ��
			psc��ʱ��Ԥ��Ƶ��
����ֵ����
******************/
uint16_t Time1Count=0;
uint16_t Time1Count1=0;
uint16_t AlarmCount;
void TIM1_Int_Init(u16 arr,u16 psc)
{  
 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);	//ʹ�ܶ�ʱ��1ʱ��
 
   //��ʼ��TIM1
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //������ʼ��
    TIM_ClearFlag(TIM1, TIM_FLAG_Update);//���жϱ�־λ
	TIM_ITConfig(TIM1, TIM_IT_Update|TIM_IT_Trigger,ENABLE ); //ʹ��ָ����TIM1�ж�,��������ж� �������ж�Դ 
	
	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  //TIM1�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
 
	TIM_Cmd(TIM1, ENABLE);  //ʹ��TIM1	
 
}
 
//��ʱ��1�жϷ������
void TIM1_UP_IRQHandler(void)   //TIM1�ж�
{  	     
	if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)//���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
		//Ӧ�ô����д��	
        KeyScan();//����ɨ��
        Time1Count++;
        if(Time1Count>=300)
        {
            Time1Count=0;
            System.BT_Pub=1;
        }
        if(System.Switch1)
        {
            Time1Count1++;
            if(Time1Count1>=100)
            {
               AlarmCount++;
               Time1Count1=0;
            }
            if(AlarmCount>=15)
            {
                System.Switch1=0;
                AlarmCount=Time1Count1=0;
            }
            
        }
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);//���TIMx���жϴ�����λ:TIM �ж�Դ 
	}	     
}

/*
 *==============================================================================
���ܣ�TIM1 PWM���ֳ�ʼ������ʼ������ʹ�����PWM�ź�
������arr���Զ���װֵ
			psc��ʱ��Ԥ��Ƶ��
			ch: ��ʱ��ͨ���� ��Χ��1--4
			RCC_APB2Periph: APB2��Ӧ��gpio�˿ں� �磺RCC_APB2Periph_GPIOB��RCC_APB2Periph_GPIOC
			GPIOx: GPIO�˿� ��GPIOA
			GPIO_Pin��GPIO�� ��GPIO_Pin_8
����ֵ����
Ӧ��ʾ����TIM1_PWM_Init(4999,7199,1,RCC_APB2Periph_GPIOA,GPIOA,GPIO_Pin_8);
 *==============================================================================
 */

void TIM1_PWM_Init(u16 arr,u16 psc,u8 ch,uint32_t RCC_APB2Periph,GPIO_TypeDef* GPIOx,uint16_t GPIO_Pinx)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph, ENABLE);	//ʹ�ܶ�ʱ��1ʱ��
	
 
   //���ø�����Ϊ�����������,���TIM2 CH1��PWM���岨��	GPIOA.0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pinx; //TIM_CH1  TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOx, &GPIO_InitStructure);//��ʼ��GPIO
 
   //��ʼ��TIM2
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//��ʼ��TIM Channel 1-4 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	if(ch==1)
	{
		TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 OC1
		TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //ʹ��TIM1_CH1�ϵ�Ԥװ�ؼĴ���
	}
	else if(ch==2)
	{
		TIM_OC2Init(TIM1, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 OC1
		TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);  //ʹ��TIM1_CH1�ϵ�Ԥװ�ؼĴ���
	}
	else if(ch==3)
	{
		TIM_OC3Init(TIM1, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 OC1
		TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);  //ʹ��TIM1_CH1�ϵ�Ԥװ�ؼĴ���
	}
	else if(ch==4)
	{
		TIM_OC4Init(TIM1, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 OC1
		TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);  //ʹ��TIM1_CH1�ϵ�Ԥװ�ؼĴ���
	}
 
	TIM_Cmd(TIM1, ENABLE);  //ʹ��TIM1
	TIM_ARRPreloadConfig(TIM1, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	TIM_CtrlPWMOutputs(TIM1,ENABLE);        //MOE �����ʹ��,�߼���ʱ�����뿪�����
 
}

