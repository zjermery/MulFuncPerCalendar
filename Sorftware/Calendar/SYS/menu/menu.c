#include "sys.h"
#include "solar.h"
#include "calendar.h"
uint8_t OperateMode=0;//运行模式标志位
static uint8_t InterFace=0;//页面显示标志位,若一页显示不完所有信息，则分两页显示，0显示第一页，1显示第二页
ALARM Alarm1;
ALARM Alarm2;
ALARM Alarm3;
/******************   	  
功能：模式选择及控制界面
参数：无
返回值：无
******************/
void Mode_Decide(void)//模式判定
{
    //按键判定
    if(isKey1)//若按键1按下
    {
        isKey1=isKey2=isKey3=isKey4=isKey5=0;//清除按键按下标志
        oled_Clear();
        OperateMode++;
    }
    else if(isKey2&&(!OperateMode))//若按键2按下,且当前模式为自动模式
    {
        isKey1=isKey2=isKey3=isKey4=isKey5=0;//清除按键按下标志
        oled_Clear();
        // InterFace=!InterFace;
        InterFace++;
        if(InterFace > 3)
        {
            InterFace = 0;
        }
    }
    Bluetooth_Analyse();
    //标志位限制
    if(OperateMode>=3) OperateMode=0;
    if(System.ClearFlag)
    {
        oled_Clear();
        System.ClearFlag=0;
    }
    /**根据模式选择显示界面**/
    switch(OperateMode)
    {
        case 0:
            Inform_Show();//显示信息
            AutoContrl();//自动控制
        break;
        
        case 1: 
            TimingSet();//时间校正
        break;
        
        case 2: 
            ThresholdSet();//闹钟设置
        break;

    }
    
}

void showChinaCalendar(uint16_t year,uint8_t month,uint8_t day)
{
	uint8_t NLyear[4];
	uint8_t SEyear;
	
	if(GetChinaCalendar(year,month,day,(uint8_t *)NLyear)==0)	return;
	GetSkyEarth(NLyear[0]*100+NLyear[1],&SEyear);
	oled_ShowCHinese(4,4,(SEyear%10)+58);//  甲
    oled_ShowCHinese(20,4,(SEyear%12)+68);//  子
    oled_ShowCHinese(36,4,19);//年
	if(NLyear[2]==1)	oled_ShowCHinese(52,4,17);//正
	else				oled_ShowCHinese(52,4,(NLyear[2]-1)+80);		
	oled_ShowCHinese(68,4,9);//月
	if(NLyear[3]>10) 	oled_ShowCHinese(84,4,(NLyear[3]/10)+92);	
	else				oled_ShowCHinese(84,4,92); //初
    oled_ShowCHinese(100,4,((NLyear[3]-1)%10)+79);
}

/******************   	  
功能：显示界面
参数：无
返回值：无
******************/
void Inform_Show(void) 
{
    uint8_t WeekDay,Month,Day;
    uint16_t Year;
    MyRTC_ReadTime();
    Year=MyRTC_Time[0];
    Month=MyRTC_Time[1];
    Day=MyRTC_Time[2];
    uint8_t temp = get_day_of_solar(Month,Day,Year);
    
    oled_ShowCHinese(40,0,18);
    oled_ShowCHinese(56,0,19);
    oled_ShowCHinese(72,0,20);//万年历
    switch(InterFace)    
    {
        case 1 :
            oled_ShowNum(8,2,MyRTC_Time[0],4,16);
            oled_ShowCHinese(40,2,8);//年
            oled_ShowNum(56,2,MyRTC_Time[1],2,16);
            oled_ShowCHinese(72,2,9);//月
            oled_ShowNum(88,2,MyRTC_Time[2],2,16);
            oled_ShowCHinese(104,2,10);//日

            oled_ShowNum(0,4,MyRTC_Time[3],4,16);
            oled_ShowCHinese(32,4,11);//时
            oled_ShowNum(48,4,MyRTC_Time[4],2,16);
            oled_ShowCHinese(64,4,12);//分
            oled_ShowNum(80,4,MyRTC_Time[5],2,16);
            oled_ShowCHinese(96,4,13);//秒

            oled_ShowCHinese(40,6,25);
            oled_ShowCHinese(56,6,24);

            if(1==Month)
            {
                Year=Year-1; 
                Month=13;
            }
            else if(2==Month)
            {
                Year=Year-1; 
                Month=14;        
            }
            WeekDay=(Day+2*Month+3*(Month+1)/5+Year+Year/4-Year/100+Year/400)%7+1;

            switch(WeekDay)//光标显示
            {
                case 1:oled_ShowCHinese(72,6,79+1);break;
                case 2:oled_ShowCHinese(72,6,79+2);break;
                case 3:oled_ShowCHinese(72,6,79+3);break;
                case 4:oled_ShowCHinese(72,6,79+4);break;    
                case 5:oled_ShowCHinese(72,6,79+5);break;
                case 6:oled_ShowCHinese(72,6,79+6);break;
                case 7:oled_ShowCHinese(72,6,79+7);break;         
            }
        break; 
    
        case 2:
            oled_ShowCHinese(40,0,18);
            oled_ShowCHinese(56,0,19);
            oled_ShowCHinese(72,0,20);//万年历

            oled_ShowCHinese(0,2,0);
            oled_ShowCHinese(16,2,2);  
            OLED_ShowChar(32,2,':',16);//温度

            oled_ShowCHinese(0,4,1);
            oled_ShowCHinese(16,4,2);
            OLED_ShowChar(32,4,':',16);//湿度

            oled_ShowCHinese(0,6,4);
            oled_ShowCHinese(16,6,5);
            OLED_ShowChar(32,6,':',16);//光照

            oled_ShowNum(48,2,dht11Data.temp_int,2,16);
            oled_ShowNum(48,4,dht11Data.humi_int,2,16);
            oled_ShowNum(48,6,SensorData.LightVal,4,16);        
            oled_ShowCHinese(64,2,3);//温度单位显示
            oled_ShowString(64,4,"%RH",16);//湿度单位显示
            oled_ShowString(88,6,"Lux",16);//湿度单位显示
        break;
        case 3:
            oled_ShowNum(8,2,MyRTC_Time[0],4,16);
            oled_ShowCHinese(40,2,8);//年
            oled_ShowNum(56,2,MyRTC_Time[1],2,16);
            oled_ShowCHinese(72,2,9);//月
            oled_ShowNum(88,2,MyRTC_Time[2],2,16);
            oled_ShowCHinese(104,2,10);//日
            showChinaCalendar(Year,Month,Day);
            oled_ShowCHinese(4,6,56);
            oled_ShowCHinese(20,6,57);
            OLED_ShowChar(35,6,':',16);//节气
            if(temp==1) //立春
            { oled_ShowCHinese(45,6,52-26); oled_ShowCHinese(60,6,53-26);}
            else if(temp == 2)
            { oled_ShowCHinese(45,6,54-26);oled_ShowCHinese(60,6,55-26);}
            else if(temp == 3)
            { oled_ShowCHinese(45,6,56-26);oled_ShowCHinese(60,6,57-26);}
            else if(temp == 4)
            { oled_ShowCHinese(45,6,53-26);oled_ShowCHinese(60,6,58-26);}
            else if(temp == 5)
            { oled_ShowCHinese(45,6,59-26);oled_ShowCHinese(60,6,60-26);}
            else if(temp == 6)
            { oled_ShowCHinese(45,6,61-26);oled_ShowCHinese(60,6,62-26);} //谷雨
            else if(temp == 7)
            { oled_ShowCHinese(45,6,52-26);oled_ShowCHinese(60,6,63-26);}
            else if(temp == 8)
            { oled_ShowCHinese(45,6,64-26);oled_ShowCHinese(60,6,65-26);}
            else if(temp == 9)
            { oled_ShowCHinese(45,6,66-26);oled_ShowCHinese(60,6,67-26);}
            else if(temp == 10)
            { oled_ShowCHinese(45,6,63-26);oled_ShowCHinese(60,6,68-26);}
            else if(temp == 11)
            { oled_ShowCHinese(45,6,64-26);oled_ShowCHinese(60,6,69-26);}
            else if(temp == 12)
            { oled_ShowCHinese(45,6,70-26);oled_ShowCHinese(60,6,69-26);}
            else if(temp == 13)
            { oled_ShowCHinese(45,6,52-26);oled_ShowCHinese(60,6,71-26);}
            else if(temp == 14)
            { oled_ShowCHinese(45,6,72-26);oled_ShowCHinese(60,6,69-26);}
            else if(temp == 15)
            { oled_ShowCHinese(45,6,73-26);oled_ShowCHinese(60,6,74-26);}
            else if(temp == 16)
            { oled_ShowCHinese(45,6,71-26);oled_ShowCHinese(60,6,76-26);}
            else if(temp == 17)
            { oled_ShowCHinese(45,6,77-26);oled_ShowCHinese(60,6,74-26);}
            else if(temp == 18)
            { oled_ShowCHinese(45,6,78-26);oled_ShowCHinese(60,6,79-26);}
            else if(temp == 19)
            { oled_ShowCHinese(45,6,52-26);oled_ShowCHinese(60,6,80-26);}
            else if(temp == 20)
            { oled_ShowCHinese(45,6,54-26);oled_ShowCHinese(60,6,55-26);}
            else if(temp == 21)
            { oled_ShowCHinese(45,6,64-26);oled_ShowCHinese(60,6,81-26);}
            else if(temp == 22)
            { oled_ShowCHinese(45,6,70-26);oled_ShowCHinese(60,6,81-26);}
            else if(temp == 23)
            { oled_ShowCHinese(45,6,64-26);oled_ShowCHinese(60,6,77-26);}
            else 
            { oled_ShowCHinese(45,6,70-26);oled_ShowCHinese(60,6,77-26);}

        break;
        default:
            oled_ShowNum(8,2,MyRTC_Time[0],4,16);
            oled_ShowCHinese(40,2,8);//年
            oled_ShowNum(56,2,MyRTC_Time[1],2,16);
            oled_ShowCHinese(72,2,9);//月
            oled_ShowNum(88,2,MyRTC_Time[2],2,16);
            oled_ShowCHinese(104,2,10);//日

            oled_ShowNum(0,4,MyRTC_Time[3],4,16);
            oled_ShowCHinese(32,4,11);//时
            oled_ShowNum(48,4,MyRTC_Time[4],2,16);
            oled_ShowCHinese(64,4,12);//分
            oled_ShowNum(80,4,MyRTC_Time[5],2,16);
            oled_ShowCHinese(96,4,13);//秒

            oled_ShowCHinese(40,6,25);
            oled_ShowCHinese(56,6,24);

            if(1==Month)
            {
                Year=Year-1; 
                Month=13;
            }
            else if(2==Month)
            {
                Year=Year-1; 
                Month=14;        
            }
            WeekDay=(Day+2*Month+3*(Month+1)/5+Year+Year/4-Year/100+Year/400)%7+1;

            switch(WeekDay)//光标显示
            {
                case 1:oled_ShowCHinese(72,6,79+1);break;
                case 2:oled_ShowCHinese(72,6,79+2);break;
                case 3:oled_ShowCHinese(72,6,79+3);break;
                case 4:oled_ShowCHinese(72,6,79+4);break;    
                case 5:oled_ShowCHinese(72,6,79+5);break;
                case 6:oled_ShowCHinese(72,6,79+6);break;
                case 7:oled_ShowCHinese(72,6,79+7);break;         
            }
            break; 
    } 
    // if(!InterFace)    
    // {
    //  show_datatime();
    // }
    // else
    // {
    //   show_tempAndHum();
    // } 
}


/******************   	  
功能：自动控制界面
参数：无
返回值：无
******************/

void AutoContrl(void)
{
    if(MyRTC_Time[3]==Alarm1.Hour&&MyRTC_Time[4]==Alarm1.Minute&&MyRTC_Time[5]==Alarm1.Second)
        System.Switch1=1;
    else if(MyRTC_Time[3]==Alarm2.Hour&&MyRTC_Time[4]==Alarm2.Minute&&MyRTC_Time[5]==Alarm2.Second)
        System.Switch1=1;
    else if(MyRTC_Time[3]==Alarm3.Hour&&MyRTC_Time[4]==Alarm3.Minute&&MyRTC_Time[5]==Alarm3.Second)
        System.Switch1=1;
    
    if(System.Switch1)
    {
        Beep_StateRefresh(2);
        LED_StateRefresh(2);
    }
    else
    {
        BEEP_OFF;
        LED1_OFF;
    }
    if(isKey5)
    {
        isKey5=0;
        if(System.Switch1) 
            System.Switch1=0;
    }
}



/******************   	  
功能：阈值设置
参数：无
返回值：无
******************/
void ThresholdSet(void)
{
    static uint8_t IndexFlag=1;//选项索引
    static uint8_t CursorFlag=1;//光标索引
    static uint8_t ColumnIndex=0;//列坐标
    static uint8_t RowIndex=0;//行坐标
    
    char Alarm1Str[20];
    char Alarm2Str[20];   
    char Alarm3Str[20];
    
    //闹钟设置
    oled_ShowCHinese(32,0,6);
    oled_ShowCHinese(48,0,7);
    oled_ShowCHinese(64,0,14);
    oled_ShowCHinese(80,0,15);

    //闹钟1
    oled_ShowCHinese(8,2,6);
    oled_ShowCHinese(24,2,7);
    OLED_ShowChar(40,2,'1',16);
    OLED_ShowChar(48,2,':',16);
    //闹钟2
    oled_ShowCHinese(8,4,6);
    oled_ShowCHinese(24,4,7);
    OLED_ShowChar(40,4,'2',16);
    OLED_ShowChar(48,4,':',16); 
    //闹钟3
    oled_ShowCHinese(8,6,6);
    oled_ShowCHinese(24,6,7);
    OLED_ShowChar(40,6,'3',16);
    OLED_ShowChar(48,6,':',16);    
    /*按键检测*/
    if(isKey2)
    {
        IndexFlag++;
        if(IndexFlag==4) IndexFlag=1;
        CursorFlag=IndexFlag;
        ColumnIndex=0;
        isKey2=0;
    }
    if(isKey5)
    {
        ColumnIndex++;
        if(ColumnIndex==4) ColumnIndex=0;
        isKey5=0;
    }
    if(isKey3)
    {
        isKey3=0;
        if(IndexFlag==1)
        {
           switch(ColumnIndex)
            {
                case 1:Alarm1.Hour++;break;
                case 2:Alarm1.Minute++;break;
                case 3:Alarm1.Second++;break; 
                default:break;
            } 
        }
        else if(IndexFlag==2)
        {
           switch(ColumnIndex)
            {
                case 1:Alarm2.Hour++;break;
                case 2:Alarm2.Minute++;break;
                case 3:Alarm2.Second++;break; 
                default:break;
            } 
        }
        else if(IndexFlag==3)
        {
           switch(ColumnIndex)
            {
                case 1:Alarm3.Hour++;break;
                case 2:Alarm3.Minute++;break;
                case 3:Alarm3.Second++;break; 
                default:break;
            } 
        }
    }
    
    if(isKey4)
    {
        isKey4=0;
        if(IndexFlag==1)
        {
           switch(ColumnIndex)
            {
                case 1:Alarm1.Hour--;break;
                case 2:Alarm1.Minute--;break;
                case 3:Alarm1.Second--;break; 
                default:break;
            } 
        }
        else if(IndexFlag==2)
        {
           switch(ColumnIndex)
            {
                case 1:Alarm2.Hour--;break;
                case 2:Alarm2.Minute--;break;
                case 3:Alarm2.Second--;break; 
                default:break;
            } 
        }
        else if(IndexFlag==3)
        {
           switch(ColumnIndex)
            {
                case 1:Alarm3.Hour--;break;
                case 2:Alarm3.Minute--;break;
                case 3:Alarm3.Second--;break; 
                default:break;
            } 
        }
    }
  
    switch(CursorFlag)//光标显示
    {
        case 1:
            OLED_ShowChar(0,2,'*',16);
            OLED_ShowChar(0,4,' ',16);
            OLED_ShowChar(0,6,' ',16);
        break;
        case 2:
            OLED_ShowChar(0,2,' ',16);
            OLED_ShowChar(0,4,'*',16);
            OLED_ShowChar(0,6,' ',16);
        break;
        case 3:
            OLED_ShowChar(0,2,' ',16);
            OLED_ShowChar(0,4,' ',16);
            OLED_ShowChar(0,6,'*',16);
        break;
    }
        if(1==CursorFlag) RowIndex=2;
        else if(2==CursorFlag) RowIndex=4;     
        else if(3==CursorFlag) RowIndex=6;
        switch(ColumnIndex)
        {
            case 1:oled_ShowString(60,RowIndex,"_",16);break;
            case 2:oled_ShowString(84,RowIndex,"_",16);break;
            case 3:oled_ShowString(108,RowIndex,"_",16);break; 
            default:break;
        }
        
    Time_Limit(&Alarm1.Hour,&Alarm1.Minute,&Alarm1.Second);
    Time_Limit(&Alarm2.Hour,&Alarm2.Minute,&Alarm2.Second);
    Time_Limit(&Alarm3.Hour,&Alarm3.Minute,&Alarm3.Second);  
        
    sprintf(Alarm1Str,"%2d:%2d:%2d",Alarm1.Hour,Alarm1.Minute,Alarm1.Second);
    oled_ShowString(56,2,(u8*)Alarm1Str,16); 
    sprintf(Alarm2Str,"%2d:%2d:%2d",Alarm2.Hour,Alarm2.Minute,Alarm2.Second);
    oled_ShowString(56,4,(u8*)Alarm2Str,16);
    sprintf(Alarm3Str,"%2d:%2d:%2d",Alarm3.Hour,Alarm3.Minute,Alarm3.Second);
    oled_ShowString(56,6,(u8*)Alarm3Str,16);
}


 void TimingSet(void)
{
    static uint8_t IndexFlag1=1;//选项索引
    static uint8_t CursorFlag1=1;//光标索引
    static uint8_t ColumnIndex=0;//列坐标
    static uint8_t RowIndex=0;//行坐标

    char Time3Str[20];
    char Time4Str[20]; 
       
    oled_ShowCHinese(32,0,21);
    oled_ShowCHinese(48,0,22);
    oled_ShowCHinese(64,0,16);
    oled_ShowCHinese(80,0,17);
    //时间
    oled_ShowCHinese(16,2,21);
    oled_ShowCHinese(32,2,22);
    OLED_ShowChar(48,2,':',16);
    //日期
    oled_ShowCHinese(16,4,23);
    oled_ShowCHinese(32,4,24);
    OLED_ShowChar(48,4,':',16);    
    
    /*按键检测*/
    if(isKey2)
    {
        IndexFlag1++;
        if(IndexFlag1==3) IndexFlag1=1;//选项个数限制
        CursorFlag1=IndexFlag1;
        ColumnIndex=0;
        isKey2=0;
    }
    if(isKey5)
    {
        ColumnIndex++;
        if(ColumnIndex==4) ColumnIndex=0;
        isKey5=0;
    }
    
    if(isKey3)
    {
        isKey3=0;
        if(IndexFlag1==1)
        {
           switch(ColumnIndex)
            {
                case 1:MyRTC_Time[3]++;break;
                case 2:MyRTC_Time[4]++;break;
                case 3:MyRTC_Time[5]++;break; 
                default:break;
            } 
        }
        else if(IndexFlag1==2)
        {
           switch(ColumnIndex)
            {
                case 1:MyRTC_Time[0]++;break;
                case 2:MyRTC_Time[1]++;break;
                case 3:MyRTC_Time[2]++;break; 
                default:break;
            } 
        }
    }
    
    if(isKey4)
    {
        isKey4=0;
        if(IndexFlag1==1)
        {
           switch(ColumnIndex)
            {
                case 1:MyRTC_Time[3]--;break;
                case 2:MyRTC_Time[4]--;break;
                case 3:MyRTC_Time[5]--;break; 
                default:break;
            } 
        }
        else if(IndexFlag1==2)
        {
           switch(ColumnIndex)
            {
                case 1:MyRTC_Time[0]--;break;
                case 2:MyRTC_Time[1]--;break;
                case 3:MyRTC_Time[2]--;break; 
                default:break;
            } 
        }
    }
  
    switch(CursorFlag1)//光标显示
    {
        case 1:
            OLED_ShowChar(0,2,'*',16);
            OLED_ShowChar(0,4,' ',16);
            OLED_ShowChar(0,6,' ',16);
        break;
        case 2:
            OLED_ShowChar(0,2,' ',16);
            OLED_ShowChar(0,4,'*',16);
            OLED_ShowChar(0,6,' ',16);
        break;
        case 3:
            OLED_ShowChar(0,2,' ',16);
            OLED_ShowChar(0,4,' ',16);
            OLED_ShowChar(0,6,'*',16);
        break;
    }
        if(1==CursorFlag1) RowIndex=2;
        else if(2==CursorFlag1) RowIndex=4;     
    
        switch(ColumnIndex)
        {
            case 1:oled_ShowString(60,RowIndex,"_",16);break;
            case 2:oled_ShowString(84,RowIndex,"_",16);break;
            case 3:oled_ShowString(108,RowIndex,"_",16);break; 
            default:break;
        }
    
    if((IndexFlag1==1||IndexFlag1==2)&&ColumnIndex!=0) MyRTC_SetTime();
    else MyRTC_ReadTime();//RTC读取时间，最新的时间存储到MyRTC_Time数组中 
    
    if(MyRTC_Time[3]>23) MyRTC_Time[3]=0;
    if(MyRTC_Time[4]>59) MyRTC_Time[4]=0;
    if(MyRTC_Time[5]>59) MyRTC_Time[5]=0;
    
    sprintf(Time3Str,"%2d:%2d:%2d",MyRTC_Time[3],MyRTC_Time[4],MyRTC_Time[5]);
    oled_ShowString(56,2,(u8*)Time3Str,16); 
    sprintf(Time4Str,"%2d-%2d-%2d",MyRTC_Time[0]%2000,MyRTC_Time[1],MyRTC_Time[2]);
    oled_ShowString(56,4,(u8*)Time4Str,16); 
        
}

void Time_Limit(int8_t* Hour,int8_t* Minutes,int8_t* Second)
{
    if(*Hour>23) *Hour=0;
    if(* Minutes>59) * Minutes=0;
    if(* Second>59) * Second=0;
    
    if(*Hour<0) *Hour=23;
    if(* Minutes<0) * Minutes=59;
    if(* Second<0) * Second=59;
}


