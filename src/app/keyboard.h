/*********************Copyright(C) 2015-2016 TYUT JBD TEAM B.*******************
Copyright (C), 2015-2016, TYUT JBD TEAM B.
File name: keyboard.c 
Author: TEAM B, Alopex
Version: 1.0               
Date: 2015.10.28(E)
      2015.12.31(C)
      2016.03.15(A)
Description: 
            Keyboard Input Display Function(键盘输入显示函数)
Others: Noting
Function List:
            1.keyboardinput 键盘输入参数函数
            2.keyboardadscreen 键盘显示变量函数
            3.keydoBeacon 信标参数键盘扫描函数
            4.gCommandBeacon 信标命令参数输入函数
            5.参数退格键,小数点'.'消除当前错误参数...
History:
<author> <time> <version > <desc>
 何雨行 2015/10/28 1.0 修改程序
        2015/12/31 1.1 添加注释
        2016/2/7   1.2 添加注释
        2016/4/1   2.0 添加注释
        2016/4/8   2.1 '.'退格键
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
#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__
          
#include "common.h"

/*
**键盘类型
*/
//#define VerticalKeyboard        //竖版键盘
#define HorizontalKeyboard      //横版键盘

/*
**键盘宏保护
*/
#ifdef VerticalKeyboard
#undef HorizontalKeyboard
#elif defined HorizontalKeyboard
#undef VerticalKeyboard
#endif

/*
**使用端口定义,I/O宏,KeyBoard
*/
#define INIT_KB_STB  gpio_init(PORTB, 11, GPO, 0)       //STB输出
#define INIT_KB_CLK  gpio_init(PORTB, 10, GPO, 0)       //CLK输出
#define INIT_KB_DO   gpio_init(PORTB,  3, GPO, 0)       //DIO输出
#define INIT_KB_DI   gpio_init(PORTB,  3, GPI, 0)       //DIO输入
#define KB_STBH      GPIO_SET(PORTB, 11, 1)             //STB高电平
#define KB_STBL      GPIO_SET(PORTB, 11, 0)             //STB低电平
#define KB_CLKH      GPIO_SET(PORTB, 10, 1)             //CLK高电平
#define KB_CLKL      GPIO_SET(PORTB, 10, 0)             //CLK低电平
#define KB_DOH       GPIO_SET(PORTB,  3, 1)             //DO高电平
#define KB_DOL       GPIO_SET(PORTB,  3, 0)             //DO低电平
#define KB_DI_Get    GPIO_Get(PORTB,  3)                //读取DI值

/*
**定义命令常量
*/
#define WRITE_MODE_A     0x40                          //0100 0000 写数码管模式A，地址自动加一
#define WRITE_MODE_B     0x44                          //0100 0100 写数码管模式B，选定定地址
#define READ_MODE        0x42                          //0100 0010 读取键盘命令
#define START_ADD        0xc0                          //1100 0000 开始地址。C0为始地址。
#define LED_CTRL         0x88                          //1000 1000 数码管显示控制。后三位从000~111共8级辉度递增。倒数第4位：0关；1开
#define DATA_COUNT       16
#define PRESS_DELAY      4000                          //1900//4000
                                                                             
/*
**功能函数声明
*/
void KeyScan(void);                                   //键扫描函数
void InEnter(void);
void KeyPortInit(void);
int StrToInt(unsigned char *str);
unsigned char ReadKeyValue(int bitnum);              //读键值
unsigned char KeyDecode(unsigned char KeyCode);
unsigned char LedEncode(unsigned char LedCode); 

/*
**全局变量声明
*/
extern int ValueTest;
extern int ConClick;                                 //连续点击标志
extern int PointFlag;                                //小数点标志
extern int EnterFlag;                                //回车键按下标志
extern unsigned char KeyValue[8];
extern unsigned char ShowText[8];                   //当前显示内容
extern unsigned char InputString[5];
extern unsigned char KeyCode;

/*
**外部函数声明
*/
extern unsigned char LedEncode(unsigned char LedCode);
extern int gCommand(unsigned char ch1,unsigned char ch2,unsigned char ch3);
extern int gCommand_Beacon(unsigned char ch1,unsigned char ch2,unsigned char ch3,unsigned char ch4);
extern void keyboardinput(void);
extern void keyboardadscreen(void);
extern void delayms1(int ms);

#endif