/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2025-01-12 17:00:17
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2025-01-22 14:48:24
 * @FilePath: \Calendar\Drivers\BSP\DH11\dht11.h
 * @Description: ����Ĭ������,������`customMade`, ��koroFileHeader�鿴���� ��������: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __DHT11_H
#define __DHT11_H

#include "sys.h"
#include "delay.h"
#include "stdio.h"
#include "error_code.h"
		
//���״̬����
#define OUT 1
#define IN  0

//DHT11���Ŷ���
#define DHT11_GPIO_PORT                  GPIOA
#define DHT11_GPIO_PIN                   GPIO_PIN_8
#define DHT11_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PA��ʱ��ʹ�� */


#define DHT11_Low                      HAL_GPIO_WritePin(DHT11_GPIO_PORT,DHT11_GPIO_PIN,GPIO_PIN_RESET)//SCL
#define DHT11_High                     HAL_GPIO_WritePin(DHT11_GPIO_PORT,DHT11_GPIO_PIN,GPIO_PIN_SET)
#define DHT11_READ_DATA()              HAL_GPIO_ReadPin(DHT11_GPIO_PORT,DHT11_GPIO_PIN)



 
error_code_t DHT11_Init(void);//��ʼ��DHT11
error_code_t DHT11_Read_Data(uint32_t *tempAndhumi);  

#endif



