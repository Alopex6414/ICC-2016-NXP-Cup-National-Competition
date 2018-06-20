/*********************Copyright(C) 2015-2016 TYUT JBD TEAM B.*******************
Copyright (C), 2015-2016, TYUT JBD TEAM B.
File name: init.c 
Author: TEAM B, Alopex
Version: 1.0               
Date: 2015.10.28(E)
      2015.12.31(C)
      2016.03.15(A)
Description: 
            All Modules Initialization(���˿�ģ���ʼ��)
Others: Noting
Function List:  1.InitClk ��ʼ��ʱ��
                2.InitPort ��ʼ��IO��
                3.InitInterrupt ��ʼ���ⲿ�ж�
                4.InitAll ϵͳ��ʼ��
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
#ifndef __INIT_H__
#define __INIT_H__

/*
**UART
*/
#define UART_PORT_USR           UART4          //�û�UART
#define UART_BAUD_USR_SERIAL    256000         //�û�UART������(����)

/*
**CAMERA
*/
#define CAMERA_IN PTC5_IN                      //����ͷ�ź�����

/*
**PORT
*/
#define TestPortUp      PTA19_OUT=1
#define TestPortDown    PTA19_OUT=0

#endif