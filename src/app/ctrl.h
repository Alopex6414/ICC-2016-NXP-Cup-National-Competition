/*********************Copyright(C) 2015-2016 TYUT JBD TEAM B.*******************
Copyright (C), 2015-2016, TYUT JBD TEAM B.
File name: ctrl.c 
Author: TEAM B, Alopex
Version: 1.0               
Date: 2015.10.28(E)
      2015.12.31(C)
      2016.03.15(A)
Description: 
            Control File Include AngleControl SpeedControl DirectionControl(控制文件包含角度控制、速度控制、方向控制)
            MotorControl(电机控制)
Others: Noting
Function List:
              1.DirectionControl 方向PWM输出舵机控制
              2.SpeedControl 电机速度控制
              3.SetMotorVoltage 电机驱动输出
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
#ifndef __CTRL_H__
#define __CTRL_H__

/*舵机中心值及左右极限*/
#define Motor_Center    7320                    //舵机中心值7320
#define Motor_Limit_L   -950                    //舵机左极限
#define Motor_Limit_R    950                    //舵机右极限
#define Motor_Limit      950                    //舵机极限值

/*1M光码盘脉冲个数*/
#define OneMeter 2340.0                         //1M光码盘脉冲个数

/*Switch开关状态*/
#define Switch_Staut PTC7_IN                    //开关状态
#define Switch_On PTC7_IN == 1                  //开关开启
#define Switch_Off PTC7_IN == 0                 //开关关闭

/*Drive驱动有关IO*/
#define DriveSwitch_On  GPIO_SET(PORTC, 2, 1)  //驱动使能打开
#define DriveSwitch_Off GPIO_SET(PORTC, 2, 0)  //驱动使能关闭
#define MotorForeward  GPIO_SET(PORTC, 1, 1)   //电机正转
#define MotorInversion GPIO_SET(PORTC, 1, 0)   //电机反转

/*
**方向控制
*/
extern int giDp;                           //方向P参数
extern int giDi;                           //方向I参数
extern int giDd;                           //方向D参数 
extern int giDI;                           //方向I参数 
extern int giDk;                           //方向K参数
extern int giAddOffset;                    //方向偏差补偿
extern float gfDirectionErrI;              //方向偏差I
extern float gfDirectionPWM_Normalize;     //方向归一化

/*
**路障相关
*/
extern int giDirectStraight;                //直走标志(当前舵机角度范围比较小)
extern unsigned char guDirectStraight_Flag;//当前舵机状态(舵机角度不大,方向大致在中心值)

/*
**速度控制
*/
extern int giSp;                           //速度P参数
extern int giSi;                           //速度I参数 
extern int giSd;                           //速度D参数
extern int giMeterSetValue;                //距离设定值(距离停车)
extern int giRoadBlockDis;                 //遇到路障时候距离路障的距离
extern int giRoadBlockOffset;              //路障偏移量
extern float gfMeter_Count;                //路程计算
extern float gf_AlterSpeed;                //当前期望速度
extern float gf_AlterSpeed_Old;            //前次期望速度
extern float gfSpeedErrI;                  //速度I积分
extern float gfDesirSpeed_Mid;             //期望速度
extern float gfDesirSpeed_Low;             //期望低速
extern float gfDesirSpeed_High;            //期望高速
extern float gfDesirSpeed;                 //当前期望速度
extern float gfSpeedPWM;                   //速度PWM
extern float gfDirection_Pwm;              //方向PWM
extern float gfDirectionPWM;               //方向PWM
extern float gfDirectionPwmOld;            //上次方向PWM
extern float gfSpeedErr[4];                //速度偏差
extern float gfSpeedErrGrad[4];            //速度偏差变化率
extern double gfCurrentSpeedPWM;           //当前速度PWM
extern double gfLastSpeedPWM;              //上次速度PWM
extern long int giMeter_Sum;               //速度积分路程
extern long int giMeter_Sum_RoadBlock;     //路障速度积分路程

/*
**信标转向速度
*/
extern char gcBeaconSpeed[15];              //0:低速 1:中低速
extern int giBeaconSpeed1;                  //每个信标的转圈期望速度(前5个信标)
extern int giBeaconSpeed2;                  //每个信标的转圈期望速度(后5个信标)
extern int giBeaconSpeed3;                  //每个信标的转圈期望速度(最后5个信标)

/*
**SetMotorVoltag
*/
extern unsigned char StartStop;            //起跑线检测停车开关
extern unsigned char MotorStree;           //电机转动方向: 0:正转 1:反转
extern unsigned char guSpeedflag;          //速度控制标志(enable)

/*
**函数声明
*/
extern void DirectionControl(void);
extern void SpeedControl(void);
extern void SetMotorVoltage(void);

/*1M=4050*/
#endif  