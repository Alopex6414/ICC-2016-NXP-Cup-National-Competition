/*********************Copyright(C) 2015-2016 TYUT JBD TEAM B.*******************
Copyright (C), 2015-2016, TYUT JBD TEAM B.
File name: func.c 
Author: TEAM B, Alopex
Version: 1.0               
Date: 2015.10.28(E)
      2015.12.31(C)
      2016.03.15(A)
Description: 
            Definition Function(���幦�ܺ���)
            Keyboard Display Function (������ʾ����)
            UART transform Function(���ڷ��ͺ���)
            TF Card Send Parameters Function(TF�����Ͳ�������)
            GetSpeed Function(��ȡ�����ٶȺ���)
            Clear Array(������麯��)
            UltrasonicTrig(��������������)
            Beacon Clock(�ű��ʱ��)
            Parameters Flash(flash�洢����)
Others: Noting
Function List:
            1.ThousandToSingleUp ������ʾ���κ���
            2.ParameterSent 3-6�������ڷ��ͺ���
            3.Calculate ������Ϣ���㺯��
            4.Getspeed ��ȡ��ǰ�����ٶ�
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
#ifndef __FUN_H__
#define __FUN_H__

#define Trig_H PTD2_OUT=1              //�����������ߵ�ƽ
#define Trig_L PTD2_OUT=0              //�����������͵�ƽ
#define Trig2_H PTB11_OUT=1            //�����������ߵ�ƽ
#define Trig2_L PTB11_OUT=0            //�����������͵�ƽ

/*
**������ʾǧ��ʮ��λ
*/
//������ʾ����
extern int ThousandUp;
extern int HundredUp;
extern int TenUp;
extern int SingleUp;
//������ʾ����
extern int ThousandDown;
extern int HundredDown;
extern int TenDown;
extern int SingleDown;
//���ڷ��ͱ�������
extern float gfSendWhite;              //���ڷ��ͱ�����ɫ����
extern float gfSendRed;                //���ڷ��ͱ�����ɫ����
extern float gfSendGreen;              //���ڷ��ͱ�����ɫ����
extern float gfSendBlue;               //���ڷ��ͱ�����ɫ����
//�������ٶ�
extern int giSpeedWheel;               //�����ٶ�
extern int giCurrentSpeed;             //��ǰ�ٶ�
extern int giSpeedSmooth[5];           //�ٶ��˲�
extern float gfSpeed_Filter;           //��Ȩ�˲��ٶ�
extern float gfSpeed_Filter_Old;       //ǰһ�μ�Ȩ�˲��ٶ�
/*
**SD������ʱ��
*/
extern int SDTIME;
extern int SD_Record;                  //SD����¼��ɱ�־
/*
**���ܼ���
*/
extern int StartCount;
/*
**�������
*/
extern volatile unsigned char guInfrared_front;//ǰ��������չ�
/*
**BeaconClock
*/
extern unsigned char guBeaconTimer;             //��ʱ��
extern float gfAllMeter;                        //��·��
extern float gfAllTime;                         //��ʱ��
extern float gfRealSpeed;                       //ƽ���ٶ�
extern int giBeaconExcept;                      //�����ű����,��������ͣ��
/*
**��������
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