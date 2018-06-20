/*********************Copyright(C) 2015-2016 TYUT JBD TEAM B.*******************
Copyright (C), 2015-2016, TYUT JBD TEAM B.
File name: init.c 
Author: TEAM B, Alopex
Version: 1.0               
Date: 2015.10.28(E)
      2015.12.31(C)
      2016.03.15(A)
Description: 
            All Modules Initialization(各端口模块初始化)
Others: Noting
Function List:  1.InitClk 初始化时钟
                2.InitPort 初始化IO口
                3.InitInterrupt 初始化外部中断
                4.InitAll 系统初始化
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
#ifndef __INIT_H__
#define __INIT_H__

/*
**UART
*/
#define UART_PORT_USR           UART4          //用户UART
#define UART_BAUD_USR_SERIAL    256000         //用户UART波特率(串口)

/*
**CAMERA
*/
#define CAMERA_IN PTC5_IN                      //摄像头信号输入

/*
**PORT
*/
#define TestPortUp      PTA19_OUT=1
#define TestPortDown    PTA19_OUT=0

#endif