/*********************Copyright(C) 2015-2016 TYUT JBD TEAM B.*******************
Copyright (C), 2015-2016, TYUT JBD TEAM B.
File name: func.c 
Author: TEAM B, Alopex
Version: 1.0               
Date: 2015.10.28(E)
      2015.12.31(C)
      2016.03.15(A)
Description: 
            Definition Function(定义功能函数)
            Keyboard Display Function (键盘显示函数)
            UART transform Function(串口发送函数)
            TF Card Send Parameters Function(TF卡发送参数函数)
            GetSpeed Function(获取两轮速度函数)
            Clear Array(清空数组函数)
            UltrasonicTrig(超声波触发函数)
            Beacon Clock(信标计时器)
            Parameters Flash(flash存储参数)
Others: Noting
Function List:
            1.ThousandToSingleUp 键盘显示传参函数
            2.ParameterSent 3-6变量串口发送函数
            3.Calculate 赛道信息计算函数
            4.Getspeed 获取当前两轮速度
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
#ifndef __FUN_H__
#define __FUN_H__

#define Trig_H PTD2_OUT=1              //超声波触发高电平
#define Trig_L PTD2_OUT=0              //超声波触发低电平
#define Trig2_H PTB11_OUT=1            //超声波触发高电平
#define Trig2_L PTB11_OUT=0            //超声波触发低电平

/*
**键盘显示千百十个位
*/
//键盘显示上排
extern int ThousandUp;
extern int HundredUp;
extern int TenUp;
extern int SingleUp;
//键盘显示下排
extern int ThousandDown;
extern int HundredDown;
extern int TenDown;
extern int SingleDown;
//串口发送变量曲线
extern float gfSendWhite;              //串口发送变量白色曲线
extern float gfSendRed;                //串口发送变量红色曲线
extern float gfSendGreen;              //串口发送变量绿色曲线
extern float gfSendBlue;               //串口发送变量蓝色曲线
//左右轮速度
extern int giSpeedWheel;               //后轮速度
extern int giCurrentSpeed;             //当前速度
extern int giSpeedSmooth[5];           //速度滤波
extern float gfSpeed_Filter;           //加权滤波速度
extern float gfSpeed_Filter_Old;       //前一次加权滤波速度
/*
**SD卡计数时间
*/
extern int SDTIME;
extern int SD_Record;                  //SD卡记录完成标志
/*
**起跑计数
*/
extern int StartCount;
/*
**红外接收
*/
extern volatile unsigned char guInfrared_front;//前方红外接收管
/*
**BeaconClock
*/
extern unsigned char guBeaconTimer;             //计时器
extern float gfAllMeter;                        //总路程
extern float gfAllTime;                         //总时间
extern float gfRealSpeed;                       //平均速度
extern int giBeaconExcept;                      //期望信标个数,个数到了停车
/*
**函数声明
*/
extern void ThousandToSingleUp(int value);
extern void ThousandToSingleDown(int value);
extern void SingleToDecimals(float value);
extern void ParameterSent(void);
extern void SendPIEX(void);
extern void SendPIEX1(void);
extern void ClearArrayChar(unsigned char *PTemp, unsigned char *PEnd);
extern void ClearArrayInt(int *PTemp, int *PEnd);
extern void GetSpeed(void);
extern void SendSD(void);
extern void Ultrasonic_Trig(void);
extern void Parametershoutcut(void);
extern void FindRoadBlock(void);
extern void BeaconClock(void);

#endif