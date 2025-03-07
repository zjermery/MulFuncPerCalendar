#ifndef __USART3_H
#define __USART3_H
#include "stdio.h"
#include "sys.h"

#define USART3_MAX_RECV_LEN		400					//�����ջ����ֽ���
#define USART3_MAX_SEND_LEN		400					//����ͻ����ֽ���
#define USART3_RX_EN 			1					//0,������;1,����.
// ���������С
extern u8  USART3_RX_BUF[USART3_MAX_RECV_LEN]; 		//���ջ���,���USART3_MAX_RECV_LEN�ֽ�
extern u8  USART3_TX_BUF[USART3_MAX_SEND_LEN]; 		//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�
extern vu16 USART3_RX_STA;   						//��������״̬
extern u8  USART3_RX_Count;
extern float WindVelocity;//����
void Usart3_Init(unsigned int baud);
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void Usart_SendBytes( USART_TypeDef * pUSARTx, uint8_t *Buffer, uint8_t Length);
void Usart_SendString( USART_TypeDef * pUSARTx, char *str);
uint16_t Crc16(uint8_t *ptr, uint32_t len);
void SnowRx_Handel(void);
void u3_printf(char* fmt,...);
void Bluetooth_Analyse(void);
char * mid(char *dst,char *src, int n,int m); /*nΪ���ȣ�mΪλ��*/
#endif


