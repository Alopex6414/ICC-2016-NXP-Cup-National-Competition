/*********************Copyright(C) 2015-2016 TYUT JBD TEAM B.*******************
Copyright (C), 2015-2016, TYUT JBD TEAM B.
File name: main.c 
Author: TEAM B, Alopex
Version: 1.0               
Date: 2015.10.28(E)
      2015.12.31(C)
      2016.03.15(A)
Description: 
            All Modules Initialization(ģ���ʼ��)
            Keyboard Display Variables(������ʾ����)
            File System Initialization(�ļ�ϵͳ��ʼ��)
            TF Card Recode Parameters(TF����¼����)
            Oled Display Parameters(Oled��ʾ����)
Others: Noting
Function List: main()������
History:
<author> <time> <version > <desc>
 ������ 2015/10/28 1.0 �޸ĳ���
        2015/12/31 1.1 ���ע��
        2016/2/7   1.2 ���ע��
        2016/4/1   2.0 ���ע��
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
#ifndef __DECLARE_H__
#define __DECLARE_H__

#include  "common.h"


#include  "gpio.h"       //IO�ڲ���
#include  "uart.h"       //����
#include  "lptmr.h"      //�͹��Ķ�ʱ��(��ʱ)
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
**Declare�궨��
*/

#define ROW 102                              //����ͷͼ������//100
#define COL 220                              //����ͷͼ������
#define NUM 25                               //�ڰ��������

#define DeadLineROW 12                       //�ű�ɨ���ֹ�� ROW:12 (24cm)

/*
**ѡ��ģʽ(��ͼ������)
**The User Can Through The Macro Definition Select The Schema
*/

//#define Photograph                      //ѡ���Ƿ񴮿���ͼ(��������������ͼ)
//#define StaticPhotograph                //��̬��ͼ
//#define ParameterSentFour               //�����������������
#define Oled                            //Oled������ʾ
#define Oled_Reset                      //Oled��λ
//#define SD                              //SD��д����
//#define BlueTooth                       //��������
//#define Buzzer                          //������
#define LeftModel                       //�ű���ת��ģʽ
//#define RightModel                       //�ű���ת��ģʽ
//#define Limitation_Test                 //���޲���

/*
**Declare��������
*/

/*FATFS�ļ�ϵͳ*/
extern FIL     fdst;                        //�ļ�
extern FATFS   fs;                          //�ļ�ϵͳ
extern volatile int FlagSD;                //SD������

/*Keyboardģʽ*/
extern volatile unsigned char ModelChoice;//Keyboard ģʽѡ�� : '0' Keyboard���� , '1' ����Keyboard
extern volatile unsigned char BeaconStart;//�ű꿪ʼ��־: '0' δ�����ű�,������ , ��1�������ű�,����
extern volatile unsigned char BeaconStartOld;//ǰ���ű꿪ʼ��־

/*GetInfor*/
extern unsigned char guImgBuff[ROW+27][NUM];
extern int ChangeNum[ROW];
extern int ActEdge[ROW][NUM];
extern int giTopBotEdge[ROW][2];            //�߽�����

/*
**Declare��������
*/

void Init_all(void);
void MotorVoltage(void);                    //�������

#endif  