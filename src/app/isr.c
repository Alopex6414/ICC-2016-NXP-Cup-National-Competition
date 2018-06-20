/*********************Copyright(C) 2015-2016 TYUT JBD TEAM B.*******************
Copyright (C), 2015-2016, TYUT JBD TEAM B.
File name: Isr.c 
Author: TEAM B, Alopex
Version: 1.0               
Date: 2015.10.28(E)
      2015.12.31(C)
      2016.03.15(A)
Description: 
            Row Interrupt Function(���жϺ���)
            Field Interrupt Function (���жϺ���)
            Ultrasonic Interrupt Function(�������жϺ���)
            UART transform Function(���ڷ��ͺ���)
Others: Noting
Function List:
            1.ThousandToSingleUp ������ʾ���κ���
            2.ParameterSent 3-6�������ڷ��ͺ���
            3.calculate ������Ϣ���㺯��
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
#include  "declare.h"

int DAC_Count = 0;                      //DAC����
int guAngleOffset = 0;                  //ƫ�ƾ���
unsigned char DAC_flag = 0;            //DAC��־
unsigned char guFlagStand = 0;         //ֱ��ִ�б�־
unsigned char guFlagCtrl = 1;          //��������־
unsigned char guOddEven = 0;           //��ż����־
unsigned char guRoadBlock = 0;         //·�ϱ�־
unsigned char guRoadBlockflag = 0;     //·����ʱ��־
unsigned char guRoadBlockCount = 0;    //·�ϸ�������
unsigned char guAngleflag = 0;         //ƫ�ƽǶȱ�־
unsigned char guStraight = 0;          //�����⵽,����ͷû��⵽ֱ�߱�־
unsigned char Switch_Staut_Old = 0;    //�ϴο���״̬
unsigned int Ultrasonic = 0;           //����������
unsigned int Ultrasonic_Dis[6]={0};    //��������������

unsigned char guModifyRownumber[] ={30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,
64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,
98,99,100,102,104,106,108,110,112,114,116,118,121,124,127,130,133,136,140,144,148,152,156,161,166,173,
180,188,198,208,218,228,238,245};//2cmѡ��
unsigned char *guP_Href = &guModifyRownumber[0];       //ָ��ʵ���к���������ѡ��洢
unsigned char guHrefCount = 0;         //�м���
unsigned char guHrefElect = 0;         //����洢�м���


/**
**Function: PORTC3_IRQHandler
**Description: ���жϷ�����
**Calls:   AngleCalculate();
**         AngleControl(); 
**         DirectionControl
**         SetMotorVoltage();
**Called By: ��
**Table Accessed: ��
**Table Updated: ��
**Input:�������˵��������ÿ���������������͡����á�ȡֵ����Χ��˵�����������ϵ��
**Output: gfCurrentSpeedRight
**Return: ��
**Others:��
**/
void PORTC3_IRQHandler(void)//�ⲿ�ж�C3���ж�
{
    if (Field_InterruptMark) //PTC3�����ж�
    {
        Field_InterruptClear; //д1���жϱ�־λ
        
        guFlagStand = 1;
        
        if(guOddEven==0)
        {
            guOddEven=1;
        }
        else
        {
            #ifdef Photograph
            guFlagCtrl=guFlagCtrl;//0(��ͼ:guFlagCtrl �ܳ�:0)
            #elif defined StaticPhotograph
            guFlagCtrl=guFlagCtrl;//0(��ͼ:guFlagCtrl �ܳ�:0)
            #else
            guFlagCtrl=0;//0(��ͼ:guFlagCtrl �ܳ�:0)
            #endif
            guOddEven=0;
        }
        
        guP_Href = &guModifyRownumber[0];
        guHrefElect = 0;
        guHrefCount = 0;
        
        if(guOddEven == 0)
        {
          ExtractChangeNum();
          Seek_Beacon();
          BeaconClock();
          Ultrasonic_Trig();
          FindRoadBlock();
          SendSD();
          //TestPortUp;
          /*giGyro_Z = MPU_Get_Z_Gyro();
          giGyro_Z -= Gyro_Static;
          
          if(giLostBeacon == 1)
          {
            gfGyroIntegral += giGyro_Z * 0.00201422;
          }
          
          else
          {
            gfGyroIntegral = 0;
          }*/
          
          //gfSendGreen = giGyro_Z;
          //TestPortDown;
          
          if(Switch_Staut == 0 && Switch_Staut_Old == 1)//�����ɿ�����
          {
            f_close(&fdst);//ͬ�����ݲ��ر��ļ�
            FlagSD=SDTIME;
            SD_Record = 1;//SD����¼��ɺ���1
            dac_once_convert(DAC0,4000);//DAC0ת��
            dac_once_convert(DAC1,4000);//DAC1ת��
          }
          else if(Switch_Staut == 1 && Switch_Staut_Old == 0)//�����ɹص���
          {
            /*����ű����gcBeaconOrder����*/
            gcBeaconOrder[0] = giBeaconOrder1 / 10000;
            gcBeaconOrder[1] = (giBeaconOrder1 % 10000) / 1000;
            gcBeaconOrder[2] = (giBeaconOrder1 % 1000) / 100;
            gcBeaconOrder[3] = (giBeaconOrder1 % 100) / 10;
            gcBeaconOrder[4] = giBeaconOrder1 % 10;
            
            gcBeaconOrder[5] = giBeaconOrder2 / 10000;
            gcBeaconOrder[6] = (giBeaconOrder2 % 10000) / 1000;
            gcBeaconOrder[7] = (giBeaconOrder2 % 1000) / 100;
            gcBeaconOrder[8] = (giBeaconOrder2 % 100) / 10;
            gcBeaconOrder[9] = giBeaconOrder2 % 10;
            
            gcBeaconOrder[10] = giBeaconOrder3 / 10000;
            gcBeaconOrder[11] = (giBeaconOrder3 % 10000) / 1000;
            gcBeaconOrder[12] = (giBeaconOrder3 % 1000) / 100;
            gcBeaconOrder[13] = (giBeaconOrder3 % 100) / 10;
            gcBeaconOrder[14] = giBeaconOrder3 % 10;
            
            /*����ű��ٶȵ�gcBeaconSpeed����*/
            gcBeaconSpeed[0] = giBeaconSpeed1 / 10000;
            gcBeaconSpeed[1] = (giBeaconSpeed1 % 10000) / 1000;
            gcBeaconSpeed[2] = (giBeaconSpeed1 % 1000) / 100;
            gcBeaconSpeed[3] = (giBeaconSpeed1 % 100) / 10;
            gcBeaconSpeed[4] = giBeaconSpeed1 % 10;
            
            gcBeaconSpeed[5] = giBeaconSpeed2 / 10000;
            gcBeaconSpeed[6] = (giBeaconSpeed2 % 10000) / 1000;
            gcBeaconSpeed[7] = (giBeaconSpeed2 % 1000) / 100;
            gcBeaconSpeed[8] = (giBeaconSpeed2 % 100) / 10;
            gcBeaconSpeed[9] = giBeaconSpeed2 % 10;
            
            gcBeaconSpeed[10] = giBeaconSpeed3 / 10000;
            gcBeaconSpeed[11] = (giBeaconSpeed3 % 10000) / 1000;
            gcBeaconSpeed[12] = (giBeaconSpeed3 % 1000) / 100;
            gcBeaconSpeed[13] = (giBeaconSpeed3 % 100) / 10;
            gcBeaconSpeed[14] = giBeaconSpeed3 % 10;
          }
          
          Switch_Staut_Old = Switch_Staut;
        }

        Field_Disable_Irq;      //�رճ��ж�        
        Row_InterruptClear;     //�����жϱ�־
        Row_Enable_Irq;         //�������ж�
    }
  
}
/**
**Function: PORTB23_IRQHandler
**Description: ���жϷ�����
**Calls:   AngleCalculate();
**         AngleControl(); 
**         DirectionControl
**         SetMotorVoltage();
**Called By: ��
**Table Accessed: ��
**Table Updated: ��
**Input:�������˵��������ÿ���������������͡����á�ȡֵ����Χ��˵�����������ϵ��
**Output: gfCurrentSpeedLeft
**        gfCurrentSpeedRight
**        gfCurrentSpeed
**Return: ��
**Others:��
**/
void PORTB23_IRQHandler(void)   //�ⲿ�ж�B23���ж�
{
    if (Row_InterruptMark)      //PTB23�����ж�
    {
        Row_InterruptClear;     //д1���жϱ�־λ
        ++guHrefCount;          //���жϼ�����־
        
        if(guFlagStand<14)
        {
          if(guOddEven == 1 && guHrefCount == *guP_Href)//guHrefCount == *guP_Href//�вɼ�56.8us
          {
               ++guP_Href;
               GetImage();      //���жϺ�=����ɼ����к�
               ++guHrefElect;
          }
          
          else if(guHrefCount==1||guHrefCount==131)//�ٶ�ģ������//ģ������54us
          {
            //TestPortUp; 
            Speed_FuzzyCalculate();
            //TestPortDown;
          }
          
          else if(guHrefCount==4||guHrefCount==134)//����ģ������//ģ������51.23us
          {
            //TestPortUp;
            Direction_FuzzyCalculate();
            //TestPortDown;
          }
          
          else if(guHrefCount==7||guHrefCount==137)//������ƺͿ��ؼ��//�������8.24us
          {
            //TestPortUp;
            if(Switch_On)//���ؼ��
            {
              StartCount++;
            }
               
            if(StartCount>500)
            {
              StartCount=500;
            }
            DirectionControl();
            //TestPortDown;
          }
          
          else if(guHrefCount==14||guHrefCount==149)     //14�к�149�в���//��������27.65us
          {
            //TestPortUp;
            GetSpeed();                                  //Period 8.5 ms
            SpeedControl();
            SetMotorVoltage();
            //TestPortDown;
          }
          
          if(guHrefCount%19==0)
          {
            ++guFlagStand;
          }
        }
        
        else
        {
          #ifdef Photograph
          Row_Disable_Irq;        //�ر����ж�
          Field_InterruptClear;   //���ж�����
          Field_Disable_Irq;      //�س��ж�
          guFlagCtrl = 0;         //��������־��ִ��//��ͼ/**/
          #elif defined StaticPhotograph
          Row_Disable_Irq;        //�ر����ж�
          Field_InterruptClear;   //���ж�����
          Field_Disable_Irq;      //�س��ж�
          guFlagCtrl = 0;         //��������־��ִ��//��ͼ/**/
          #else
          Row_Disable_Irq;        //�ر����ж�
          Field_InterruptClear;   //���ж�����
          Field_Enable_Irq;       //�����ж�/**/
          #endif
        }
    }             
}

/**
**Function: PORTD5_IRQHandler
**Description: �������ⲿ�жϷ�����
**Calls:�� 
**Called By:��
**Table Accessed:��
**Table Updated:��
**Input:�������˵��������ÿ���������������͡����á�ȡֵ����Χ��˵�����������ϵ��
**Output:�� 
**Return:��
**Others:��
**/
void PORTD5_IRQHandler(void)                         //�ⲿ�ж�  D5
{
  int i=0;
  
  if (Ultrasonic_InterruptMark)                      //PTD5�����ж�
  {
    Ultrasonic_InterruptClear;                       //д1���жϱ�־λ               
                
    if(PTD5_IN == 1)                                 //��⵽IO���Ǹߵ�ƽ����ô����������
    {
      PIT_TCTRL1  &= ~PIT_TCTRL_TEN_MASK;            //ֹͣ��ʱ��
      PIT_LDVAL1  = 0xFFFFFFFF;                      //32λ��ʱ����װ���ֵ����
      PIT_TCTRL1  |= PIT_TCTRL_TEN_MASK;             //������ʱ��
    }
        
    else
    {
      Ultrasonic = (0xFFFFFFFF - PIT_CVAL1) * 0.00017544;
      Ultrasonic_Dis[0] = Ultrasonic;
      PIT_TCTRL1  &= ~PIT_TCTRL_TEN_MASK;             //ֹͣ��ʱ��
    }
  }
}