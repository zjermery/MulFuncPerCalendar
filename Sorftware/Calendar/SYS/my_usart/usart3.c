#include "usart3.h"
#include <string.h>
#include "stdarg.h"
/*
******����3��ʼ��*****************************************
*	��������:	����3��ʼ��
*	��ڲ���:	baud:�趨������
*	���ز���:	��
*	˵��:		TX-PB10		RX-PB11
**********************************************************
*/
//���ڽ��ջ����� 	
u8 USART3_RX_BUF[USART3_MAX_RECV_LEN]; 				//���ջ���,���USART3_MAX_RECV_LEN���ֽ�.
u8 USART3_Cach_BUF[USART3_MAX_RECV_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
u8 USART3_TX_BUF[USART3_MAX_SEND_LEN]; 			  //���ͻ���,���USART3_MAX_SEND_LEN�ֽ�
u8  USART3_RX_Count=0;
//ͨ���жϽ�������2���ַ�֮���ʱ������10ms�������ǲ���һ������������.
//���2���ַ����ռ������10ms,����Ϊ����1����������.Ҳ���ǳ���10msû�н��յ�
//�κ�����,���ʾ�˴ν������.
//���յ�������״̬
//[15]:0,û�н��յ�����;1,���յ���һ������.
//[14:0]:���յ������ݳ���
vu16 USART3_RX_STA=0;  
char dest[5]={0};

void Usart3_Init(unsigned int baud)
{
		GPIO_InitTypeDef GPIO_InitStructure;                         
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3 , ENABLE); 
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;               
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB , &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;             
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOB , &GPIO_InitStructure);
		
		USART_InitStructure.USART_BaudRate = baud;               
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		                  
		
		NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;         
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;       
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         
		NVIC_Init(&NVIC_InitStructure);                         
		
		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);  
        USART_Init(USART3,&USART_InitStructure);         
		USART_Cmd(USART3,ENABLE);
      
        USART3_RX_STA=0;		//����
	

}


void USART3_IRQHandler(void)
{
   if(USART_GetITStatus(USART3,USART_IT_RXNE) != RESET)  //�ж��Ƿ����ж�
    {
        if(!USART3_RX_STA)
        {
            USART3_RX_BUF[USART3_RX_Count]=USART_ReceiveData(USART3);
            USART3_RX_Count++;
        }
        if(USART3_RX_Count>=2)
        {
            if(USART3_RX_BUF[USART3_RX_Count-1]=='\r')
            {
                USART3_RX_BUF[USART3_RX_Count-1]='\0';
                USART3_RX_STA=1;
                USART3_RX_Count=0;
            }
        }
       USART_ClearITPendingBit(USART3, USART_IT_RXNE);
    }
}


void Bluetooth_Analyse(void)
{
    if(USART3_RX_STA)
    {
        u1_printf("%s\r\n",USART3_RX_BUF);
        if(strcmp((const char*)USART3_RX_BUF,"Mode1Open")==0)
        {
            OperateMode=2;
            System.ClearFlag=1;
        }
        else if(!strcmp((const char*)USART3_RX_BUF,"Mode1Close"))
        {
            OperateMode=0;
            System.ClearFlag=1;
        }

        
        else if(strstr((const char*)USART3_RX_BUF,"Alarm1"))
        {
            Alarm1.Hour  =atoi(mid(dest,(char*)USART3_RX_BUF,2,6));
            Alarm1.Minute=atoi(mid(dest,(char*)USART3_RX_BUF,2,8));
            Alarm1.Second=atoi(mid(dest,(char*)USART3_RX_BUF,2,10));
        }
        else if(strstr((const char*)USART3_RX_BUF,"Alarm2"))
        {
            Alarm2.Hour  =atoi(mid(dest,(char*)USART3_RX_BUF,2,6));
            Alarm2.Minute=atoi(mid(dest,(char*)USART3_RX_BUF,2,8));
            Alarm2.Second=atoi(mid(dest,(char*)USART3_RX_BUF,2,10));
        }
        else if(strstr((const char*)USART3_RX_BUF,"Alarm3"))
        {
            Alarm3.Hour  =atoi(mid(dest,(char*)USART3_RX_BUF,2,6));
            Alarm3.Minute=atoi(mid(dest,(char*)USART3_RX_BUF,2,8));
            Alarm3.Second=atoi(mid(dest,(char*)USART3_RX_BUF,2,10));
        }
        else if(strstr((const char*)USART3_RX_BUF,"Ntime")) 
        {
            MyRTC_Time[3] = atoi(mid(dest, (char*)USART3_RX_BUF, 2, 5)); 
            MyRTC_Time[4] = atoi(mid(dest, (char*)USART3_RX_BUF, 2, 7)); 
            MyRTC_Time[5] = atoi(mid(dest, (char*)USART3_RX_BUF, 2, 9)); 
            MyRTC_Time[0] = atoi(mid(dest, (char*)USART3_RX_BUF, 4, 11)); 
            MyRTC_Time[1] = atoi(mid(dest, (char*)USART3_RX_BUF, 2, 15)); 
            MyRTC_Time[2] = atoi(mid(dest, (char*)USART3_RX_BUF, 2, 17));
            MyRTC_SetTime();
        }
        USART3_RX_STA=0;
    }
}


///*
//******���ڷ��͵��ֽ�*****************************************
//*	��������:	���ڷ��͵��ֽ�
//*	��ڲ���:	pUSARTx:���ں�
//						ch����Ҫ���͵��ֽ�
//*	���ز���:	��
//**********************************************************
//*/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
    /* ����һ���ֽ����ݵ�USART */
    USART_SendData(pUSARTx,ch);

    /* �ȴ��������ݼĴ���Ϊ�� */
    while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
}



///*
//******���ڷ��Ͷ��ֽ�*****************************************
//*	��������:	���ڷ��͵��ֽ�
//*	��ڲ���:	pUSARTx:���ں�
//				Buffer����Ҫ���͵������ָ��
//              Length����Ҫ���͵�����ĳ���
//*	���ز���:	��
//**********************************************************
//*/
void Usart_SendBytes( USART_TypeDef * pUSARTx, uint8_t *Buffer, uint8_t Length)
{
    uint8_t i=0;
    while(i<Length)
        {
//            if(i<(Length-1))
//                Buffer[Length-1]+=Buffer[i];//�ۼ�Length-1ǰ������
            Usart_SendByte(pUSARTx,Buffer[i++]);
        }

}



///*
//******���ڷ����ַ���*****************************************
//*	��������:	���ڷ����ַ���
//*	��ڲ���:	pUSARTx:���ں�
//						str:��Ҫ���͵��ַ������׵�ַ
//*	���ز���:	��
//**********************************************************
//*/
//void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
//{
//    unsigned int k=0;
//    do
//  	{
//        Usart_SendByte( pUSARTx, *(str + k) );
//      	k++;
//  	} while(*(str + k)!='\0');
//  	/* �ȴ�������� */
//  	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
//  	{}
//}


/* CRC16 ��ʽ�� */
static uint16_t crctalbeabs[] =
{
    0x0000, 0xCC01, 0xD801, 0x1400, 0xF001, 0x3C00, 0x2800, 0xE401,
    0xA001, 0x6C00, 0x7800, 0xB401, 0x5000, 0x9C01, 0x8801, 0x4400
};


/*!
 *  ��  ��: CRC16У��
 *  param1: ָ��ҪУ������ݵ�ָ��
 *  param2: ҪУ������ݵĳ���
 *  retval: У�����õ���ֵ��uint16_t ����
 *
 *  ˵  ��: ����CRCУ��Ϊ���������ʽΪ x16+x15+x2+1(0x8005)��CRC�ĳ�ʼֵΪ0xFFFF
 */
uint16_t Crc16(uint8_t *ptr, uint32_t len)
{
    uint16_t crc = 0xffff;
    uint32_t i;
    uint8_t ch;

    for (i = 0; i < len; i++)
        {
            ch = *ptr++;
            crc = crctalbeabs[(ch ^ crc) & 15] ^ (crc >> 4);
            crc = crctalbeabs[((ch >> 4) ^ crc) & 15] ^ (crc >> 4);
        }

    return crc;
}


__align(8) char USART3_TxBuff[128];  

void u3_printf(char* fmt,...) 
{  
	unsigned int i =0,length=0;
	
	va_list ap;
	va_start(ap,fmt);
	vsprintf(USART3_TxBuff,fmt,ap);
	va_end(ap);
	
	length=strlen((const char*)USART3_TxBuff);
	while(i<length)
	{
		Usart_SendByte(USART3,USART3_TxBuff[i]);		
		i++;		
	}
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);
	
}

char * mid(char *dst,char *src, int n,int m) /*nΪ���ȣ�mΪλ��*/
{
    char *p = src;
    char *q = dst;
    int len = strlen(src);
    if(n>len) n = len-m;    /*�ӵ�m�������*/
    if(m<0) m=0;    /*�ӵ�һ����ʼ*/
    if(m>len) return NULL;
    p += m;
    while(n--) *(q++) = *(p++);
    *(q++)='\0'; /*�б�Ҫ�𣿺��б�Ҫ*/
    return dst;
}

