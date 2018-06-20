/*********************Copyright(C) 2015-2016 TYUT JBD TEAM B.*******************
Copyright (C), 2015-2016, TYUT JBD TEAM B.
File name: Isr.c 
Author: TEAM B, Alopex
Version: 1.0               
Date: 2015.10.28(E)
      2015.12.31(C)
      2016.03.15(A)
Description: 
            Row Interrupt Function(行中断函数)
            Field Interrupt Function (场中断函数)
            Ultrasonic Interrupt Function(超声波中断函数)
            UART transform Function(串口发送函数)
Others: Noting
Function List:
            1.ThousandToSingleUp 键盘显示传参函数
            2.ParameterSent 3-6变量串口发送函数
            3.calculate 赛道信息计算函数
History:
<author> <time> <version > <desc>
 何雨行 2015/10/28 1.0 修改程序
        2015/12/31 1.1 添加注释
        2016/2/7   1.2 添加注释
        2016/4/1   2.0 添加注释
        2016/6/30  3.0 黑色主板
        2016/8/10  4.0 国赛终版(CSU)

It was quiet before.
But now it's pullin'us in for more.
You're out of your mind.
But it's delivered back,just in time.
Beyond yourself?
And we're holdin'up.
I was tearin'up.
Could we lose it all?
I was losin'strong.
Just listen up.
This is all we've.
********************************************************************************/
#include  "declare.h"

int DAC_Count = 0;                      //DAC计数
int guAngleOffset = 0;                  //偏移距离
unsigned char DAC_flag = 0;            //DAC标志
unsigned char guFlagStand = 0;         //直立执行标志
unsigned char guFlagCtrl = 1;          //方向计算标志
unsigned char guOddEven = 0;           //奇偶场标志
unsigned char guRoadBlock = 0;         //路障标志
unsigned char guRoadBlockflag = 0;     //路障延时标志
unsigned char guRoadBlockCount = 0;    //路障个数计数
unsigned char guAngleflag = 0;         //偏移角度标志
unsigned char guStraight = 0;          //红外检测到,摄像头没检测到直走标志
unsigned char Switch_Staut_Old = 0;    //上次开关状态
unsigned int Ultrasonic = 0;           //超声波距离
unsigned int Ultrasonic_Dis[6]={0};    //超声波距离数组

unsigned char guModifyRownumber[] ={30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,
64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,
98,99,100,102,104,106,108,110,112,114,116,118,121,124,127,130,133,136,140,144,148,152,156,161,166,173,
180,188,198,208,218,228,238,245};//2cm选行
unsigned char *guP_Href = &guModifyRownumber[0];       //指向实际行号数组用于选择存储
unsigned char guHrefCount = 0;         //行计数
unsigned char guHrefElect = 0;         //数组存储行计数


/**
**Function: PORTC3_IRQHandler
**Description: 场中断服务函数
**Calls:   AngleCalculate();
**         AngleControl(); 
**         DirectionControl
**         SetMotorVoltage();
**Called By: 无
**Table Accessed: 无
**Table Updated: 无
**Input:输入参数说明，包括每个参数的数据类型、作用、取值（范围）说明及参数间关系。
**Output: gfCurrentSpeedRight
**Return: 无
**Others:无
**/
void PORTC3_IRQHandler(void)//外部中断C3场中断
{
    if (Field_InterruptMark) //PTC3触发中断
    {
        Field_InterruptClear; //写1清中断标志位
        
        guFlagStand = 1;
        
        if(guOddEven==0)
        {
            guOddEven=1;
        }
        else
        {
            #ifdef Photograph
            guFlagCtrl=guFlagCtrl;//0(拍图:guFlagCtrl 跑车:0)
            #elif defined StaticPhotograph
            guFlagCtrl=guFlagCtrl;//0(拍图:guFlagCtrl 跑车:0)
            #else
            guFlagCtrl=0;//0(拍图:guFlagCtrl 跑车:0)
            #endif
            guOddEven=0;
        }
        
        guP_Href = &guModifyRownumber[0];
        guHrefElect = 0;
        guHrefCount = 0;
        
        if(guOddEven == 0)
        {
          ExtractChangeNum();
          Seek_Beacon();
          BeaconClock();
          Ultrasonic_Trig();
          FindRoadBlock();
          SendSD();
          //TestPortUp;
          /*giGyro_Z = MPU_Get_Z_Gyro();
          giGyro_Z -= Gyro_Static;
          
          if(giLostBeacon == 1)
          {
            gfGyroIntegral += giGyro_Z * 0.00201422;
          }
          
          else
          {
            gfGyroIntegral = 0;
          }*/
          
          //gfSendGreen = giGyro_Z;
          //TestPortDown;
          
          if(Switch_Staut == 0 && Switch_Staut_Old == 1)//开关由开到关
          {
            f_close(&fdst);//同步数据并关闭文件
            FlagSD=SDTIME;
            SD_Record = 1;//SD卡记录完成后置1
            dac_once_convert(DAC0,4000);//DAC0转换
            dac_once_convert(DAC1,4000);//DAC1转换
          }
          else if(Switch_Staut == 1 && Switch_Staut_Old == 0)//开关由关到开
          {
            /*拆分信标次序到gcBeaconOrder数组*/
            gcBeaconOrder[0] = giBeaconOrder1 / 10000;
            gcBeaconOrder[1] = (giBeaconOrder1 % 10000) / 1000;
            gcBeaconOrder[2] = (giBeaconOrder1 % 1000) / 100;
            gcBeaconOrder[3] = (giBeaconOrder1 % 100) / 10;
            gcBeaconOrder[4] = giBeaconOrder1 % 10;
            
            gcBeaconOrder[5] = giBeaconOrder2 / 10000;
            gcBeaconOrder[6] = (giBeaconOrder2 % 10000) / 1000;
            gcBeaconOrder[7] = (giBeaconOrder2 % 1000) / 100;
            gcBeaconOrder[8] = (giBeaconOrder2 % 100) / 10;
            gcBeaconOrder[9] = giBeaconOrder2 % 10;
            
            gcBeaconOrder[10] = giBeaconOrder3 / 10000;
            gcBeaconOrder[11] = (giBeaconOrder3 % 10000) / 1000;
            gcBeaconOrder[12] = (giBeaconOrder3 % 1000) / 100;
            gcBeaconOrder[13] = (giBeaconOrder3 % 100) / 10;
            gcBeaconOrder[14] = giBeaconOrder3 % 10;
            
            /*拆分信标速度到gcBeaconSpeed数组*/
            gcBeaconSpeed[0] = giBeaconSpeed1 / 10000;
            gcBeaconSpeed[1] = (giBeaconSpeed1 % 10000) / 1000;
            gcBeaconSpeed[2] = (giBeaconSpeed1 % 1000) / 100;
            gcBeaconSpeed[3] = (giBeaconSpeed1 % 100) / 10;
            gcBeaconSpeed[4] = giBeaconSpeed1 % 10;
            
            gcBeaconSpeed[5] = giBeaconSpeed2 / 10000;
            gcBeaconSpeed[6] = (giBeaconSpeed2 % 10000) / 1000;
            gcBeaconSpeed[7] = (giBeaconSpeed2 % 1000) / 100;
            gcBeaconSpeed[8] = (giBeaconSpeed2 % 100) / 10;
            gcBeaconSpeed[9] = giBeaconSpeed2 % 10;
            
            gcBeaconSpeed[10] = giBeaconSpeed3 / 10000;
            gcBeaconSpeed[11] = (giBeaconSpeed3 % 10000) / 1000;
            gcBeaconSpeed[12] = (giBeaconSpeed3 % 1000) / 100;
            gcBeaconSpeed[13] = (giBeaconSpeed3 % 100) / 10;
            gcBeaconSpeed[14] = giBeaconSpeed3 % 10;
          }
          
          Switch_Staut_Old = Switch_Staut;
        }

        Field_Disable_Irq;      //关闭场中断        
        Row_InterruptClear;     //清行中断标志
        Row_Enable_Irq;         //开启行中断
    }
  
}
/**
**Function: PORTB23_IRQHandler
**Description: 行中断服务函数
**Calls:   AngleCalculate();
**         AngleControl(); 
**         DirectionControl
**         SetMotorVoltage();
**Called By: 无
**Table Accessed: 无
**Table Updated: 无
**Input:输入参数说明，包括每个参数的数据类型、作用、取值（范围）说明及参数间关系。
**Output: gfCurrentSpeedLeft
**        gfCurrentSpeedRight
**        gfCurrentSpeed
**Return: 无
**Others:无
**/
void PORTB23_IRQHandler(void)   //外部中断B23行中断
{
    if (Row_InterruptMark)      //PTB23触发中断
    {
        Row_InterruptClear;     //写1清中断标志位
        ++guHrefCount;          //行中断计数标志
        
        if(guFlagStand<14)
        {
          if(guOddEven == 1 && guHrefCount == *guP_Href)//guHrefCount == *guP_Href//行采集56.8us
          {
               ++guP_Href;
               GetImage();      //行中断号=所需采集的行号
               ++guHrefElect;
          }
          
          else if(guHrefCount==1||guHrefCount==131)//速度模糊计算//模糊计算54us
          {
            //TestPortUp; 
            Speed_FuzzyCalculate();
            //TestPortDown;
          }
          
          else if(guHrefCount==4||guHrefCount==134)//方向模糊计算//模糊计算51.23us
          {
            //TestPortUp;
            Direction_FuzzyCalculate();
            //TestPortDown;
          }
          
          else if(guHrefCount==7||guHrefCount==137)//方向控制和开关检测//方向控制8.24us
          {
            //TestPortUp;
            if(Switch_On)//开关检测
            {
              StartCount++;
            }
               
            if(StartCount>500)
            {
              StartCount=500;
            }
            DirectionControl();
            //TestPortDown;
          }
          
          else if(guHrefCount==14||guHrefCount==149)     //14行和149行测速//测速周期27.65us
          {
            //TestPortUp;
            GetSpeed();                                  //Period 8.5 ms
            SpeedControl();
            SetMotorVoltage();
            //TestPortDown;
          }
          
          if(guHrefCount%19==0)
          {
            ++guFlagStand;
          }
        }
        
        else
        {
          #ifdef Photograph
          Row_Disable_Irq;        //关闭行中断
          Field_InterruptClear;   //场中断清零
          Field_Disable_Irq;      //关场中断
          guFlagCtrl = 0;         //方向计算标志置执行//拍图/**/
          #elif defined StaticPhotograph
          Row_Disable_Irq;        //关闭行中断
          Field_InterruptClear;   //场中断清零
          Field_Disable_Irq;      //关场中断
          guFlagCtrl = 0;         //方向计算标志置执行//拍图/**/
          #else
          Row_Disable_Irq;        //关闭行中断
          Field_InterruptClear;   //场中断清零
          Field_Enable_Irq;       //开场中断/**/
          #endif
        }
    }             
}

/**
**Function: PORTD5_IRQHandler
**Description: 超声波外部中断服务函数
**Calls:无 
**Called By:无
**Table Accessed:无
**Table Updated:无
**Input:输入参数说明，包括每个参数的数据类型、作用、取值（范围）说明及参数间关系。
**Output:无 
**Return:无
**Others:无
**/
void PORTD5_IRQHandler(void)                         //外部中断  D5
{
  int i=0;
  
  if (Ultrasonic_InterruptMark)                      //PTD5触发中断
  {
    Ultrasonic_InterruptClear;                       //写1清中断标志位               
                
    if(PTD5_IN == 1)                                 //检测到IO口是高电平，那么就是上升沿
    {
      PIT_TCTRL1  &= ~PIT_TCTRL_TEN_MASK;            //停止定时器
      PIT_LDVAL1  = 0xFFFFFFFF;                      //32位定时器，装最大值即可
      PIT_TCTRL1  |= PIT_TCTRL_TEN_MASK;             //启动定时器
    }
        
    else
    {
      Ultrasonic = (0xFFFFFFFF - PIT_CVAL1) * 0.00017544;
      Ultrasonic_Dis[0] = Ultrasonic;
      PIT_TCTRL1  &= ~PIT_TCTRL_TEN_MASK;             //停止定时器
    }
  }
}