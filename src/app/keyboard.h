/*********************Copyright(C) 2015-2016 TYUT JBD TEAM B.*******************
Copyright (C), 2015-2016, TYUT JBD TEAM B.
File name: keyboard.c 
Author: TEAM B, Alopex
Version: 1.0               
Date: 2015.10.28(E)
      2015.12.31(C)
      2016.03.15(A)
Description: 
            Keyboard Input Display Function(����������ʾ����)
Others: Noting
Function List:
            1.keyboardinput ���������������
            2.keyboardadscreen ������ʾ��������
            3.keydoBeacon �ű��������ɨ�躯��
            4.gCommandBeacon �ű�����������뺯��
            5.�����˸��,С����'.'������ǰ�������...
History:
<author> <time> <version > <desc>
 ������ 2015/10/28 1.0 �޸ĳ���
        2015/12/31 1.1 ���ע��
        2016/2/7   1.2 ���ע��
        2016/4/1   2.0 ���ע��
        2016/4/8   2.1 '.'�˸��
        2016/6/30  3.0 ��ɫ����
        2016/8/10  4.0 �����հ�(CSU)

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
**��������
*/
//#define VerticalKeyboard        //�������
#define HorizontalKeyboard      //������

/*
**���̺걣��
*/
#ifdef VerticalKeyboard
#undef HorizontalKeyboard
#elif defined HorizontalKeyboard
#undef VerticalKeyboard
#endif

/*
**ʹ�ö˿ڶ���,I/O��,KeyBoard
*/
#define INIT_KB_STB  gpio_init(PORTB, 11, GPO, 0)       //STB���
#define INIT_KB_CLK  gpio_init(PORTB, 10, GPO, 0)       //CLK���
#define INIT_KB_DO   gpio_init(PORTB,  3, GPO, 0)       //DIO���
#define INIT_KB_DI   gpio_init(PORTB,  3, GPI, 0)       //DIO����
#define KB_STBH      GPIO_SET(PORTB, 11, 1)             //STB�ߵ�ƽ
#define KB_STBL      GPIO_SET(PORTB, 11, 0)             //STB�͵�ƽ
#define KB_CLKH      GPIO_SET(PORTB, 10, 1)             //CLK�ߵ�ƽ
#define KB_CLKL      GPIO_SET(PORTB, 10, 0)             //CLK�͵�ƽ
#define KB_DOH       GPIO_SET(PORTB,  3, 1)             //DO�ߵ�ƽ
#define KB_DOL       GPIO_SET(PORTB,  3, 0)             //DO�͵�ƽ
#define KB_DI_Get    GPIO_Get(PORTB,  3)                //��ȡDIֵ

/*
**���������
*/
#define WRITE_MODE_A     0x40                          //0100 0000 д�����ģʽA����ַ�Զ���һ
#define WRITE_MODE_B     0x44                          //0100 0100 д�����ģʽB��ѡ������ַ
#define READ_MODE        0x42                          //0100 0010 ��ȡ��������
#define START_ADD        0xc0                          //1100 0000 ��ʼ��ַ��C0Ϊʼ��ַ��
#define LED_CTRL         0x88                          //1000 1000 �������ʾ���ơ�����λ��000~111��8���Զȵ�����������4λ��0�أ�1��
#define DATA_COUNT       16
#define PRESS_DELAY      4000                          //1900//4000
                                                                             
/*
**���ܺ�������
*/
void KeyScan(void);                                   //��ɨ�躯��
void InEnter(void);
void KeyPortInit(void);
int StrToInt(unsigned char *str);
unsigned char ReadKeyValue(int bitnum);              //����ֵ
unsigned char KeyDecode(unsigned char KeyCode);
unsigned char LedEncode(unsigned char LedCode); 

/*
**ȫ�ֱ�������
*/
extern int ValueTest;
extern int ConClick;                                 //���������־
extern int PointFlag;                                //С�����־
extern int EnterFlag;                                //�س������±�־
extern unsigned char KeyValue[8];
extern unsigned char ShowText[8];                   //��ǰ��ʾ����
extern unsigned char InputString[5];
extern unsigned char KeyCode;

/*
**�ⲿ��������
*/
extern unsigned char LedEncode(unsigned char LedCode);
extern int gCommand(unsigned char ch1,unsigned char ch2,unsigned char ch3);
extern int gCommand_Beacon(unsigned char ch1,unsigned char ch2,unsigned char ch3,unsigned char ch4);
extern void keyboardinput(void);
extern void keyboardadscreen(void);
extern void delayms1(int ms);

#endif