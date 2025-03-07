#include "sys.h"

char WIFI_Config0(int time)
{
	memset(WiFi_RX_BUF,0,1024);
	WiFi_RxCounter = 0;
	
	u2_printf("AT+RST");
	
	while(time--)
	{
		
		delay_ms(100);
		if(strstr(WiFi_RX_BUF,"ready"))
			break;
		WIFI_RST;	
		u1_printf ("%d ",time); 
		
	}
	if(time>0)
       return 0; 
    else
       return 1;
}



char WIFI_Config(int time,char*cmd,char*response)
{
	memset(WiFi_RX_BUF,0,1024);
	WiFi_RxCounter = 0;
	u2_printf("%s\r\n",cmd); 
	while(time--)
	{
		delay_ms(100);
		if(strstr(WiFi_RX_BUF,response))
			break;
		u1_printf ("%d ",time); 
	}
	if(time>0)
       return 0; 
    else
       return 1;
}

char WIFI_Router(int time)
{
	memset(WiFi_RX_BUF,0,1024);
	WiFi_RxCounter = 0;
	
	u2_printf("AT+CWJAP=\"%s\",\"%s\"\r\n",ID,PASSWORD);
	while(time--)
	{
		delay_ms(1000);
		if(strstr(WiFi_RX_BUF,"OK"))
		
			break;
		u1_printf ("%d ",time); 
	}
	if(time>0)
       return 0; 
    else
       return 1;
}
char WIFI_ConnectTCP(int time)
{
	memset(WiFi_RX_BUF,0,1024);
	WiFi_RxCounter = 0;
	
	u2_printf("AT+CIPSTART=\"TCP\",\"%s\",%d\r\n",ServerIP,ServerPort);
	while(time--)
	{
		delay_ms(1000);
		if(strstr(WiFi_RX_BUF,"OK"))
		
			break;
		u1_printf ("%d ",time); 
	}
	if(time>0)
       return 0; 
    else
       return 1;
}

char WIFI_Connect()
{
	wifi_GPIO_Config();
/*1������WIFIģʽ*/
	u1_printf ("1��׼������WIFIģʽ!\r\n");
	oled_ShowNum(16*6,2*3,1,1,16);

	
	if(WIFI_Config(50,"AT+CWMODE=1\r\n","OK"))
	{
		u1_printf ("����WIFIģʽʧ��!\r\n");
		return 2;
	}
	else
		u1_printf ("����WIFIģʽ�ɹ�!\r\n");	                        
	u1_printf ("\r\n");	
/*2������(���ʽ)*/	
	u1_printf ("2��׼����λ!\r\n");
oled_ShowNum(16*6,2*3,2,1,16);
	if(WIFI_Config(50,"AT+RST\r\n","ready"))
	{
		u1_printf ("��λʧ��!\r\n");
		return 3;
	}
	else
	{
		u1_printf ("��λ�ɹ�!\r\n");		
	}
	u1_printf ("\r\n");
/*3��ȡ���Զ�����*/		
	u1_printf ("3��׼��ȡ���Զ�����\r\n");
		oled_ShowNum(16*6,2*3,3,1,16);
	if(WIFI_Config(50,"AT+CWAUTOCONN=0\r\n","OK"))
	{
		u1_printf ("ȡ���Զ�����ʧ��!\r\n");
		return 4;
	}
	else
	{
		u1_printf ("ȡ���Զ����ӳɹ�!\r\n");
	}	
	u1_printf ("\r\n");
/*4������·����*/	
	u1_printf ("4��׼������·����\r\n");
	oled_ShowNum(16*6,2*3,4,1,16);
	if(WIFI_Router(50))
	{
		u1_printf ("����·����ʧ��!\r\n");
		return 5;
	}
	else
	{
		u1_printf ("����·�����ɹ�!\r\n");
	}
	u1_printf ("\r\n");
/*5�����õ�·����ģʽ*/		
	u1_printf ("5��׼�����õ�·����ģʽ!\r\n");
	oled_ShowNum(16*6,2*3,5,1,16);
	if(WIFI_Config(50,"AT+CIPMUX=0\r\n","OK"))
	{
		u1_printf ("���õ�·����ģʽʧ��!\r\n");
		return 6;
	}
	else
	{
		u1_printf ("���õ�·����ģʽ�ɹ�!\r\n");
	}
	u1_printf ("\r\n");
/*6������͸��ģʽ*/		
	u1_printf ("6��׼������͸��ģʽ\r\n");
	oled_ShowNum(16*6,2*3,6,1,16);
	if(WIFI_Config(50,"AT+CIPMODE=1\r\n","OK"))
	{
		u1_printf ("����͸��ģʽʧ��!\r\n");
		return 7;
	}
	else
	{
		u1_printf ("����͸��ģʽ�ɹ�!\r\n");
	}
	u1_printf ("\r\n");
/*7������TCP����*/		
	u1_printf ("7��׼������TCP����\r\n");
	oled_ShowNum(16*6,2*3,7,1,16);
	if(WIFI_ConnectTCP(50))
	{
		u1_printf ("����TCP����ʧ��!\r\n");
		return 8;
	}
	else
	{
		u1_printf ("����TCP���ӳɹ�!\r\n");
	}
	u1_printf ("\r\n");
/*8������͸��ģʽ*/	
	u1_printf ("8��׼������͸��ģʽ\r\n");
	oled_ShowNum(16*6,2*3,8,1,16);
	if(WIFI_Config(50,"AT+CIPSEND\r\n","\r\nOK\r\n\r\n>"))
	{
		u1_printf ("����͸��ģʽʧ��!\r\n");
		return 9;
	}
	else
	{
        oled_ShowString(16*6,2*3,"ok",16);
		delay_ms(100);
		oled_ShowString(16*6,2*3,"  ",16);
        oled_Clear();
		u1_printf ("����͸��ģʽ�ɹ�!\r\n");
	}
	return 0;
}
