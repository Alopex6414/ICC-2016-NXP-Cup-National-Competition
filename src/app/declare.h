/*********************Copyright(C) 2015-2016 TYUT JBD TEAM B.*******************
Copyright (C), 2015-2016, TYUT JBD TEAM B.
File name: main.c 
Author: TEAM B, Alopex
Version: 1.0               
Date: 2015.10.28(E)
      2015.12.31(C)
      2016.03.15(A)
Description: 
            All Modules Initialization(模块初始化)
            Keyboard Display Variables(键盘显示变量)
            File System Initialization(文件系统初始化)
            TF Card Recode Parameters(TF卡记录参数)
            Oled Display Parameters(Oled显示参数)
Others: Noting
Function List: main()主函数
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
#ifndef __DECLARE_H__
#define __DECLARE_H__

#include  "common.h"


#include  "gpio.h"       //IO口操作
#include  "uart.h"       //串口
#include  "lptmr.h"      //低功耗定时器(延时)
#include  "FTM.h" 
#include  "adc.h"
#include  "dac.h"
#include  "delay.h"
#include  "dma.h"
#include  "exti.h"
#include  "PIT.h"
#include  "mcg.h"
#include  "wdog.h"

#include  "fun.h"
#include  "fuzzy.h"
#include  "keyboard.h"
#include  "ctrl.h"
#include  "getinfor.h"
#include  "init.h"
#include  "I2C.h"
#include  "Oled.h"
#include  "bootloader.h"
#include  "TFT.h"
#include  "ff.h"
#include  "isr.h"

#include "math.h"
#include "stdlib.h"
#include "float.h"
#include "stdio.h"
#include "time.h"

/*
**Declare宏定义
*/

#define ROW 102                              //摄像头图像行数//100
#define COL 220                              //摄像头图像列数
#define NUM 25                               //黑白跳变个数

#define DeadLineROW 12                       //信标扫描截止行 ROW:12 (24cm)

/*
**选择模式(拍图或正常)
**The User Can Through The Macro Definition Select The Schema
*/

//#define Photograph                      //选择是否串口拍图(屏蔽正常，打开拍图)
//#define StaticPhotograph                //静态拍图
//#define ParameterSentFour               //传红白蓝绿四条曲线
#define Oled                            //Oled参数显示
#define Oled_Reset                      //Oled复位
//#define SD                              //SD卡写参数
//#define BlueTooth                       //蓝牙发送
//#define Buzzer                          //蜂鸣器
#define LeftModel                       //信标左转向模式
//#define RightModel                       //信标右转向模式
//#define Limitation_Test                 //极限测试

/*
**Declare变量声明
*/

/*FATFS文件系统*/
extern FIL     fdst;                        //文件
extern FATFS   fs;                          //文件系统
extern volatile int FlagSD;                //SD卡计数

/*Keyboard模式*/
extern volatile unsigned char ModelChoice;//Keyboard 模式选择 : '0' Keyboard输入 , '1' 跳过Keyboard
extern volatile unsigned char BeaconStart;//信标开始标志: '0' 未发现信标,不起跑 , ‘1’发现信标,起跑
extern volatile unsigned char BeaconStartOld;//前次信标开始标志

/*GetInfor*/
extern unsigned char guImgBuff[ROW+27][NUM];
extern int ChangeNum[ROW];
extern int ActEdge[ROW][NUM];
extern int giTopBotEdge[ROW][2];            //边界数组

/*
**Declare函数声明
*/

void Init_all(void);
void MotorVoltage(void);                    //驱动输出

#endif  