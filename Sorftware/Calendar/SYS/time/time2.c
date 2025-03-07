#include "sys.h"
#include "usart3.h"
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
uint8_t Tim2Count=0;
extern u8 mqttflag;//MQTT��Ϣ������־
void TIM2_Init(u32 arr,u32 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_Prescaler = psc;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);		
	TIM_Cmd(TIM2, ENABLE);
}


void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  
	{		
        KeyScan();//����ɨ��
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update); 					
	}

}

/*
 *==============================================================================
���ܣ�TIM2 PWM���ֳ�ʼ������ʼ������ʹ�����PWM�ź�
������arr���Զ���װֵ
			psc��ʱ��Ԥ��Ƶ��
			ch: ��ʱ��ͨ���� ��Χ��1--4
			RCC_APB2Periph: APB2��Ӧ��gpio�˿ں� �磺RCC_APB2Periph_GPIOA��RCC_APB2Periph_GPIOC
			GPIOx: GPIO�˿� ��GPIOA
			GPIO_Pin��GPIO�� ��GPIO_Pin_2
����ֵ����
Ӧ��ʾ����TIM2_PWM_Init(4999,7199,3,RCC_APB2Periph_GPIOB,GPIOB,GPIO_Pin_2);

ע�⣡����
1.����ʹ����ӳ��ʱ��Ĭ��TIM2�ĸ���IO����PA0��PA1��PA2��PA3

2.Ҫʹ��PA15��PB3��PA2��PA3�Ķ˿���ϣ�Ҫ��������������в�����ӳ�䣺
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);   //��ӳ�����Ҫ��AFIOʱ��
  GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);

3.Ҫʹ��PA0��PA1��PB10��PB11�Ķ˿���ϣ�Ҫ��������������в�����ӳ�䣺
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);   //��ӳ�����Ҫ��AFIOʱ��
  GPIO_PinRemapConfig(GPIO_PartialRemap2_TIM2, ENABLE);

4.Ҫʹ��PA15��PB3��PB10��PB11�Ķ˿���ϣ�Ҫ�����������������ȫ��ӳ�䣺
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);   //��ӳ�����Ҫ��AFIOʱ��
  GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);	//��ֹJTAG���ܣ���PB3��PB4��Ϊ��ͨIO��ʹ��
ͬʱ��Ҫ����JTAG���ܣ�PA15��PB3��PB10��PB11�Ż����������
 *==============================================================================
 */
void TIM2_PWM_Init(u16 arr,u16 psc,u8 ch,uint32_t RCC_APB2Periph,GPIO_TypeDef* GPIOx,uint16_t GPIO_Pinx)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//ʹ�ܶ�ʱ��3ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
	
	GPIO_PinRemapConfig(GPIO_PartialRemap2_TIM2, ENABLE); //Timer3������ӳ��  TIM2_CH2->PB5 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);   //��ӳ�����Ҫ��AFIOʱ��
//    GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);
//    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);	//��ֹJTAG���ܣ���PB3��PB4��Ϊ��ͨIO��ʹ��
 
   //���ø�����Ϊ�����������,���TIM2 CH2��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pinx; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOx, &GPIO_InitStructure);//��ʼ��GPIO
 
   //��ʼ��TIM2
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//��ʼ��TIM2 Channel2 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	if(ch==1)
	{
		TIM_OC1Init(TIM2, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM2 OC1
		TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);  //ʹ��TIM2��CCR2�ϵ�Ԥװ�ؼĴ���
	}
	else if(ch==2)
	{
		TIM_OC2Init(TIM2, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM2 OC2
		TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);  //ʹ��TIM2��CCR2�ϵ�Ԥװ�ؼĴ���
	}
	else if(ch==3)
	{
		TIM_OC3Init(TIM2, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM2 OC3
		TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);  //ʹ��TIM2��CCR2�ϵ�Ԥװ�ؼĴ���
	}
	else if(ch==4)
	{
		TIM_OC4Init(TIM2, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM2 OC4
		TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);  //ʹ��TIM2��CCR2�ϵ�Ԥװ�ؼĴ���
	}
 
	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIM2
	//TIM_Cmd(TIM2, DISABLE);  //ʧ��TIM2
}



