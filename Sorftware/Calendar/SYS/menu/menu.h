#ifndef __MENU_H
#define __MENU_H	 
#include "sys.h"

extern int16_t Servo_Speed;
extern uint8_t OperateMode;
extern uint8_t SwitchFlag1,SwitchFlag2,SwitchFlag3,SwitchFlag4;

void Inform_Show(void) ;
void Inform_Show2(void) ;
void Mode_Decide(void);//Ä£Ê½ÅÐ¶¨
void AutoContrl(void);
void Manual_Contrl(void);
void ThresholdSet(void);
void TimingSet(void);
void Time_Limit(int8_t* Hour,int8_t* Minutes,int8_t* Second);
#endif

