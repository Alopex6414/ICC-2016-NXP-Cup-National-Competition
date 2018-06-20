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
#include "common.h"
#include "declare.h"

/*
**�����ļ�ϵͳ
*/
FIL     fdst;                            //�ļ�
FATFS   fs;                              //�ļ�ϵͳ

/*
**ģʽѡ��ͱ�־λ
*/
volatile int FlagSD = 0;                //SD������
volatile unsigned char ModelChoice = 0;//Keyboard ģʽѡ�� : '0' Keyboard���� , '1' ����Keyboard
volatile unsigned char BeaconStart = 0;//�ű꿪ʼ��־: '0' δ�����ű�,������ , ��1�������ű�,����
volatile unsigned char BeaconStartOld = 0;//ǰ���ű꿪ʼ��־

void main(void)
{
    /*
    **�����ļ�ϵͳ
    */
    uint32 size, sizetmp;
    int i;
    FRESULT res;
    unsigned char  *SDtemp1=&guImgBuff[0][0];//SD��ָ��
    
    /*
    **SD�������ļ�
    */
    res = f_mount(0,&fs);  //�����ļ�ϵͳ
    res = f_open(&fdst, "0:/Blue.txt", FA_CREATE_ALWAYS | FA_WRITE | FA_READ);
      
    /*
    **���ж�,������ʼ��
    */
    DisableInterrupts; 
    InitAll();
        
    /*���Switch�����Ƿ񱻰���*/
    if(Switch_On)/*Switch����*/
    {
      ModelChoice = 1;/*����Keyboard*/
    }
    
    else
    {
      ModelChoice = 0;/*����Keyboard*/
    }
    
    IndependentKey();
    OledInput();
    EnableInterrupts;
    
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
    
    /*Oled����*/
    LCD_CLS();
    
    /*PIT������װ���ֵ*/                    
    PIT_MCR &= ~PIT_MCR_MDIS_MASK;                            //PIT����   
    PIT_LDVAL1 = 0xFFFFFFFF;                                  //PIT1
    PIT_TFLG1 |= PIT_TFLG_TIF_MASK;
    
    /*E11�������ж�*/
    Ultrasonic_InterruptClear;                                //�峬�����ж�
    Ultrasonic_Enable_Irq;                                    //���������ж�
    
    /*D15���ж�*/
    Field_InterruptClear;                                     //�峡�ж�
    Field_Enable_Irq;                                         //�����ж�
    
    /*ָʾ��*/
    GPIO_SET(PORTA, 15, 0);                                   //ָʾ����
         
    for (;;)
    {
        #ifdef ParameterSentFour //������
        ParameterSent();
        #endif
        if(guFlagCtrl==0)
        {
          FlagSD++;
          
          /*
          **SD��дͼ��
          */
          if(FlagSD<SDTIME)
          {
            SDtemp1=&guImgBuff[ROW][0];
            
            f_puts("AAAA", &fdst);                          //֡ͷAAAA            
            f_write(&fdst,guImgBuff,(ROW+27)*NUM,&sizetmp); //дͼ��,27�пɷ�����
          }
          
          #ifdef Photograph
          SendPIEX1();           /*��̬*/
          Row_InterruptClear;    //���ж�����
          Row_Disable_Irq;       //�ر����ж�
          Field_InterruptClear;  //���ж�����
          Field_Enable_Irq;      //�����ж�*/
          #elif defined StaticPhotograph
          SendPIEX();            /*��̬*/
          Row_InterruptClear;    //���ж�����
          Row_Disable_Irq;       //�ر����ж�
          Field_InterruptClear;  //���ж�����
          Field_Enable_Irq;      //�����ж�*/
          #endif          
          
          guFlagCtrl=1;
        }
        
        if(FlagSD==SDTIME)
        {
          f_close(&fdst);//ͬ�����ݲ��ر��ļ�
          FlagSD=SDTIME;
          GPIO_SET(PORTA, 15, 1);//A15ָʾ��
          SD_Record = 1;//SD����¼��ɺ���1
        }
        
        if(FlagSD>=SDTIME)
        {
          //DAC:������ʾ���TFCard��¼
          if(DAC_flag == 0)
          {
            DAC_Count++;
          }
          
          else
          {
            DAC_Count--;
          }
          
          if(DAC_Count > 4095)
          {
            DAC_Count = 4095;
            DAC_flag = 1;
          }
          
          else if(DAC_Count < 2400)
          {
            DAC_Count = 2400;
            DAC_flag = 0;
          }
          
          dac_once_convert(DAC0,DAC_Count);//DAC0ת��
          dac_once_convert(DAC1,DAC_Count);//DAC1ת��
          FlagSD=SDTIME+1;
        }
        
        #ifdef Oled_Reset       /*Oled��λReset*/
        if(KEY_L_VAL==1)
        {
          if(ChoiceStatus != 1 && ChoiceStatus != 2)
          {
            LCD_Reset();          //Oled��λ
            delayms1(Page_DELAY);//��ѹ��ʱ
          }
        }
        #endif
        
        #ifdef Oled
        PointChange();/*С����Oled��ҳ*/
        DisplayLED(SwitchPage);/*Oled��ʾ*/
        #endif
        
        giGyro_Z = MPU_Get_Z_Gyro();
        giAcc_Z = MPU_Get_Acc_Z();
    }
}