#ifndef __GY30IIC_H
#define __GY30IIC_H
#include "sys.h" 


/*********************************
**	????:					**
**			SCL ---> PXx		**
**			SDA ---> PXx		**
**********************************/
	
/*******************************????***********************************/  
#define  gyIIC_Delay()		{	delay_us(5);	}	   		   

/*************************** IO????(GPIOx * 4) ****************************/
 
					/**		PXx ??????		**/
#define  gySDA_IN()  {GPIOA -> CRL &= 0X0FFFFFFF; GPIOA -> CRL |= (u32)8 << 28;}
					/**		PXx ??????		**/
#define  gySDA_OUT() {GPIOA -> CRL &= 0X0FFFFFFF; GPIOA -> CRL |= (u32)3 << 28;}





/*************************** IO???? *************************/	 
#define 		gyIIC_SCL    		PAout(6) 	//SCL ??
#define 		gyIIC_SDA    		PAout(7) 	//SDA ??	
#define 		gyRead_SDA   		PAin (7)  	//SDA ??





/***************************** IIC?????? **********************************/

void 				gyIIC_Init				(void);             //???IIC?IO?
void 				gyIIC_Start				(void);				//??IIC????
void 				gyIIC_Stop				(void);	  			//??IIC????
void 				gyIIC_Send_Byte			(uint8_t Txd);		//IIC??????
uint8_t 			gyIIC_Read_Byte			(uint8_t ACK);		//IIC??????
uint8_t 			gyIIC_Wait_Ack			(void); 			//IIC??ACK??
void 				gyIIC_Ack					(void);				//IIC??ACK??
void 				gyIIC_NAck				(void);				//IIC???ACK??


 
#endif
