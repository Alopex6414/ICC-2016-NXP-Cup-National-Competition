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
#include "common.h"
#include "declare.h"

/*
**创建文件系统
*/
FIL     fdst;                            //文件
FATFS   fs;                              //文件系统

/*
**模式选择和标志位
*/
volatile int FlagSD = 0;                //SD卡计数
volatile unsigned char ModelChoice = 0;//Keyboard 模式选择 : '0' Keyboard输入 , '1' 跳过Keyboard
volatile unsigned char BeaconStart = 0;//信标开始标志: '0' 未发现信标,不起跑 , ‘1’发现信标,起跑
volatile unsigned char BeaconStartOld = 0;//前次信标开始标志

void main(void)
{
    /*
    **创建文件系统
    */
    uint32 size, sizetmp;
    int i;
    FRESULT res;
    unsigned char  *SDtemp1=&guImgBuff[0][0];//SD卡指针
    
    /*
    **SD卡创建文件
    */
    res = f_mount(0,&fs);  //挂载文件系统
    res = f_open(&fdst, "0:/Blue.txt", FA_CREATE_ALWAYS | FA_WRITE | FA_READ);
      
    /*
    **关中断,函数初始化
    */
    DisableInterrupts; 
    InitAll();
        
    /*检测Switch开关是否被按下*/
    if(Switch_On)/*Switch按下*/
    {
      ModelChoice = 1;/*跳过Keyboard*/
    }
    
    else
    {
      ModelChoice = 0;/*进入Keyboard*/
    }
    
    IndependentKey();
    OledInput();
    EnableInterrupts;
    
    /*拆分信标次序到gcBeaconOrder数组*/
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
    
    /*拆分信标速度到gcBeaconSpeed数组*/
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
    
    /*Oled清屏*/
    LCD_CLS();
    
    /*PIT计数器装入初值*/                    
    PIT_MCR &= ~PIT_MCR_MDIS_MASK;                            //PIT计数   
    PIT_LDVAL1 = 0xFFFFFFFF;                                  //PIT1
    PIT_TFLG1 |= PIT_TFLG_TIF_MASK;
    
    /*E11超声波中断*/
    Ultrasonic_InterruptClear;                                //清超声波中断
    Ultrasonic_Enable_Irq;                                    //开超声波中断
    
    /*D15场中断*/
    Field_InterruptClear;                                     //清场中断
    Field_Enable_Irq;                                         //开场中断
    
    /*指示灯*/
    GPIO_SET(PORTA, 15, 0);                                   //指示灯灭
         
    for (;;)
    {
        #ifdef ParameterSentFour //传参数
        ParameterSent();
        #endif
        if(guFlagCtrl==0)
        {
          FlagSD++;
          
          /*
          **SD卡写图象
          */
          if(FlagSD<SDTIME)
          {
            SDtemp1=&guImgBuff[ROW][0];
            
            f_puts("AAAA", &fdst);                          //帧头AAAA            
            f_write(&fdst,guImgBuff,(ROW+27)*NUM,&sizetmp); //写图像,27行可发曲线
          }
          
          #ifdef Photograph
          SendPIEX1();           /*动态*/
          Row_InterruptClear;    //行中断清零
          Row_Disable_Irq;       //关闭行中断
          Field_InterruptClear;  //场中断清零
          Field_Enable_Irq;      //开场中断*/
          #elif defined StaticPhotograph
          SendPIEX();            /*静态*/
          Row_InterruptClear;    //行中断清零
          Row_Disable_Irq;       //关闭行中断
          Field_InterruptClear;  //场中断清零
          Field_Enable_Irq;      //开场中断*/
          #endif          
          
          guFlagCtrl=1;
        }
        
        if(FlagSD==SDTIME)
        {
          f_close(&fdst);//同步数据并关闭文件
          FlagSD=SDTIME;
          GPIO_SET(PORTA, 15, 1);//A15指示灯
          SD_Record = 1;//SD卡记录完成后置1
        }
        
        if(FlagSD>=SDTIME)
        {
          //DAC:亮暗显示完成TFCard记录
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
          
          dac_once_convert(DAC0,DAC_Count);//DAC0转换
          dac_once_convert(DAC1,DAC_Count);//DAC1转换
          FlagSD=SDTIME+1;
        }
        
        #ifdef Oled_Reset       /*Oled复位Reset*/
        if(KEY_L_VAL==1)
        {
          if(ChoiceStatus != 1 && ChoiceStatus != 2)
          {
            LCD_Reset();          //Oled复位
            delayms1(Page_DELAY);//按压延时
          }
        }
        #endif
        
        #ifdef Oled
        PointChange();/*小数点Oled换页*/
        DisplayLED(SwitchPage);/*Oled显示*/
        #endif
        
        giGyro_Z = MPU_Get_Z_Gyro();
        giAcc_Z = MPU_Get_Acc_Z();
    }
}