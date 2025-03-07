#include "stm32f10x.h"
#include "key.h"
#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK��ӢSTM32������
//������������
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////

u8 isKey1 = 0;
u8 isKey2 = 0;
u8 isKey3 = 0;
u8 isKey4 = 0;
u8 isKey5 = 0;
u8 isKey6 = 0;
u8 isKey7 = 0;
u8 isKey8 = 0;

//
/*
******������ʼ������*****************************************
*	����:	������ʼ������
*	��ڲ���:	��
*	���ز���:	��
*	˵��:		KEY1-PB14		KEY2-PB12   KEY3-PB15    KEY4-PB13
************************************************************
*/
void KEY_Init(void) //IO��ʼ��
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(KEY1_GPIO_CLK,ENABLE);

    GPIO_InitStructure.GPIO_Pin  = KEY1_GPIO_PIN|KEY2_GPIO_PIN|KEY3_GPIO_PIN|KEY4_GPIO_PIN|KEY5_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //���ó���������

    GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);//
    GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStructure);//
    GPIO_Init(KEY3_GPIO_PORT, &GPIO_InitStructure);//
    GPIO_Init(KEY4_GPIO_PORT, &GPIO_InitStructure);//
    GPIO_Init(KEY5_GPIO_PORT, &GPIO_InitStructure);//
    
}


void Key1Press(void)
{
    if(isKey1 == 0)
        isKey1=1;
}

void Key2Press(void)
{
    if(isKey2 == 0)
        isKey2=1;

}

void Key3Press(void)
{
    if(isKey3 == 0)
        isKey3=1;

}

void Key4Press(void)
{
    if(isKey4 == 0)
        isKey4=1;

}

void Key5Press(void)
{
    if(isKey5 == 0)
        isKey5=1;
}

void Key6Press(void)
{
    if(isKey6 == 0)
        isKey6=1;

}

void Key7Press(void)
{
    if(isKey7 == 0)
        isKey7=1;

}

void Key8Press(void)
{
    if(isKey8 == 0)
        isKey8=1;

}

/*
******����ɨ�躯��*****************************************
*	����:	����ɨ�躯��
*	��ڲ���:	��
*	���ز���:	��
*	˵��:		KEY1-PB14		KEY2-PB12   KEY3-PB15    KEY4-PB13
************************************************************
*/
void KeyScan(void)
{
    static int keyCount = 0;
    static int keyState = 0;
    if (KEY1 == 0 && keyState == 0) //��������
    {
        keyCount++;
        if (keyCount > 2 && KEY1 == 0 && keyState == 0) //�����������ӳ�10ms�����ý���
        {
            /*ToDo:��������ִ�еĲ���*/
            Key1Press();
            keyState = 1;
        }
    }
    else if (KEY2 == 0 && keyState == 0)
    {
        keyCount++;
        if (keyCount > 2 && KEY2 == 0 && keyState == 0)
        {
            /*ToDo:��������ִ�еĲ���*/
            Key2Press();
            keyState = 1;
        }
    }
    else if (KEY3 == 0 && keyState == 0)
    {
        keyCount++;
        if (keyCount > 2 && KEY3 == 0 && keyState == 0)
        {
            /*ToDo:��������ִ�еĲ���*/
            Key3Press();
            keyState = 1;
        }
    }
    else if (KEY4 == 0 && keyState == 0)
    {
        keyCount++;
        if (keyCount > 2 && KEY4 == 0 && keyState == 0)
        {
            /*ToDo:��������ִ�еĲ���*/
            Key4Press();
            keyState = 1;
        }
    }
    else if (KEY5 == 0 && keyState == 0)
    {
        keyCount++;
        if (keyCount > 2 && KEY5 == 0 && keyState == 0)
        {
            /*ToDo:��������ִ�еĲ���*/
            Key5Press();
            keyState = 1;
        }
    }
    
    else if (KEY1 == 1 && KEY2 == 1 && KEY3 == 1 && KEY4 == 1&& KEY5 == 1 &&keyState == 1) //�����а���������̧��״̬��״̬ˢ��  && KEY5 == 1 && KEY6 == 1 && KEY7 == 1 && KEY8 == 1&&keyState == 
    {
        keyCount = 0;
        keyState = 0;
    }
}
