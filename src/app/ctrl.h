/*********************Copyright(C) 2015-2016 TYUT JBD TEAM B.*******************
Copyright (C), 2015-2016, TYUT JBD TEAM B.
File name: ctrl.c 
Author: TEAM B, Alopex
Version: 1.0               
Date: 2015.10.28(E)
      2015.12.31(C)
      2016.03.15(A)
Description: 
            Control File Include AngleControl SpeedControl DirectionControl(�����ļ������Ƕȿ��ơ��ٶȿ��ơ��������)
            MotorControl(�������)
Others: Noting
Function List:
              1.DirectionControl ����PWM����������
              2.SpeedControl ����ٶȿ���
              3.SetMotorVoltage ����������
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
#ifndef __CTRL_H__
#define __CTRL_H__

/*�������ֵ�����Ҽ���*/
#define Motor_Center    7320                    //�������ֵ7320
#define Motor_Limit_L   -950                    //�������
#define Motor_Limit_R    950                    //����Ҽ���
#define Motor_Limit      950                    //�������ֵ

/*1M�������������*/
#define OneMeter 2340.0                         //1M�������������

/*Switch����״̬*/
#define Switch_Staut PTC7_IN                    //����״̬
#define Switch_On PTC7_IN == 1                  //���ؿ���
#define Switch_Off PTC7_IN == 0                 //���عر�

/*Drive�����й�IO*/
#define DriveSwitch_On  GPIO_SET(PORTC, 2, 1)  //����ʹ�ܴ�
#define DriveSwitch_Off GPIO_SET(PORTC, 2, 0)  //����ʹ�ܹر�
#define MotorForeward  GPIO_SET(PORTC, 1, 1)   //�����ת
#define MotorInversion GPIO_SET(PORTC, 1, 0)   //�����ת

/*
**�������
*/
extern int giDp;                           //����P����
extern int giDi;                           //����I����
extern int giDd;                           //����D���� 
extern int giDI;                           //����I���� 
extern int giDk;                           //����K����
extern int giAddOffset;                    //����ƫ���
extern float gfDirectionErrI;              //����ƫ��I
extern float gfDirectionPWM_Normalize;     //�����һ��

/*
**·�����
*/
extern int giDirectStraight;                //ֱ�߱�־(��ǰ����Ƕȷ�Χ�Ƚ�С)
extern unsigned char guDirectStraight_Flag;//��ǰ���״̬(����ǶȲ���,�������������ֵ)

/*
**�ٶȿ���
*/
extern int giSp;                           //�ٶ�P����
extern int giSi;                           //�ٶ�I���� 
extern int giSd;                           //�ٶ�D����
extern int giMeterSetValue;                //�����趨ֵ(����ͣ��)
extern int giRoadBlockDis;                 //����·��ʱ�����·�ϵľ���
extern int giRoadBlockOffset;              //·��ƫ����
extern float gfMeter_Count;                //·�̼���
extern float gf_AlterSpeed;                //��ǰ�����ٶ�
extern float gf_AlterSpeed_Old;            //ǰ�������ٶ�
extern float gfSpeedErrI;                  //�ٶ�I����
extern float gfDesirSpeed_Mid;             //�����ٶ�
extern float gfDesirSpeed_Low;             //��������
extern float gfDesirSpeed_High;            //��������
extern float gfDesirSpeed;                 //��ǰ�����ٶ�
extern float gfSpeedPWM;                   //�ٶ�PWM
extern float gfDirection_Pwm;              //����PWM
extern float gfDirectionPWM;               //����PWM
extern float gfDirectionPwmOld;            //�ϴη���PWM
extern float gfSpeedErr[4];                //�ٶ�ƫ��
extern float gfSpeedErrGrad[4];            //�ٶ�ƫ��仯��
extern double gfCurrentSpeedPWM;           //��ǰ�ٶ�PWM
extern double gfLastSpeedPWM;              //�ϴ��ٶ�PWM
extern long int giMeter_Sum;               //�ٶȻ���·��
extern long int giMeter_Sum_RoadBlock;     //·���ٶȻ���·��

/*
**�ű�ת���ٶ�
*/
extern char gcBeaconSpeed[15];              //0:���� 1:�е���
extern int giBeaconSpeed1;                  //ÿ���ű��תȦ�����ٶ�(ǰ5���ű�)
extern int giBeaconSpeed2;                  //ÿ���ű��תȦ�����ٶ�(��5���ű�)
extern int giBeaconSpeed3;                  //ÿ���ű��תȦ�����ٶ�(���5���ű�)

/*
**SetMotorVoltag
*/
extern unsigned char StartStop;            //�����߼��ͣ������
extern unsigned char MotorStree;           //���ת������: 0:��ת 1:��ת
extern unsigned char guSpeedflag;          //�ٶȿ��Ʊ�־(enable)

/*
**��������
*/
extern void DirectionControl(void);
extern void SpeedControl(void);
extern void SetMotorVoltage(void);

/*1M=4050*/
#endif  