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
#include "declare.h"

/**
**Function: InitClk
**Description: 初始化时钟
**Calls:   无
**Called By: InitAll
**Table Accessed: 无
**Table Updated: 无
**Input: 无
**Output: 无
**Return: 无
**Others:无
**/
void InitClk(void)
{
    SIM_SCGC5 = SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK;//使能A B C D E端口
    SIM_SCGC5|= SIM_SCGC5_LPTIMER_MASK;          //使能脉冲计数时钟
    SIM_SCGC6|= SIM_SCGC6_PIT_MASK;              //使能PIT时钟
}


/**
**Function: InitPort
**Description: IO初始化
**Calls:   无
**Called By: InitAll
**Table Accessed: 无
**Table Updated: 无
**Input: 无
**Output: 无
**Return: 无
**Others:无
**/
void InitPort(void)
{
    /*
    **指示开关
    */
    gpio_init(PORTA, 15, GPO, 0);                //指示灯  A15
    
    /*
    **电机Motor
    */
    gpio_init(PORTC, 2, GPO, 0);                 //电机使能口 C2
    gpio_init(PORTC, 1, GPO, 0);                 //控制电机正反转 C1
    gpio_init(PORTC, 7, GPI_DOWN, 0);            //驱动开关C7输入下拉
    
    gpio_init(PORTD, 2, GPO, 0);                 //超声波Trig触发 D2
    gpio_init(PORTB, 11, GPO, 0);                //超声波Trig触发 B11
    gpio_init(PORTA, 19, GPO, 0);                //测试口 A19
    gpio_init(PORTC, 5, GPI, 0);                 //摄像头模拟信号输入 C5
    
    #ifdef Buzzer
    gpio_init(PORTB, 11, GPO, 0);                 //蜂鸣器B11
    #endif
}


/**
**Function: InitInterrupt
**Description: 外部中断初始化
**Calls:   无
**Called By: InitAll
**Table Accessed: 无
**Table Updated: 无
**Input: 无
**Output: 无
**Return: 无
**Others:无
**/
void InitInterrupt(void)
{
    Row_Exti_Init;                      //行中断，PORTB23 端口外部中断初始化 ，双边沿触发中断，内部上拉
    Row_Set_Priority;                   //行中断向量号88，中断优先级0
    Row_Disable_Irq;                    //关行中断
    Field_Exti_Init;                    //场中断，PORTC3 端口外部中断初始化 ，双边沿触发中断，内部上拉
    Field_Set_Priority;                 //场中断向量号89，中断优先级1
    Field_Disable_Irq;                  //关场中断
    Ultrasonic_Exti_Init;               //声波中断，PORTD5端口外部中断初始化 ，双边沿触发中断，内部上拉
    Ultrasonic_Set_Priority;            //声波中断向量号90，中断优先级2
    Ultrasonic_Disable_Irq;             //关声波中断
}


/**
**Function: InitAll
**Description: 系统初始化
**Calls:   InitClk
**        InitPort
**        InitInterrupt
**Called By: main.c
**Table Accessed: 无
**Table Updated: 无
**Input: 无
**Output: 无
**Return: 无
**Others:无
**/
void InitAll(void)
{
    //时钟初始化
    InitClk();
  
    //外部中断初始化
    InitInterrupt();
  
    //PWM初始化
    FTM_PWM_init(FTM0, CH3, 15000, 50); //电机口初始化 C4
    FTM_PWM_init(FTM2, CH0, 50,7320);   //舵机B18 50Hz//Center: 7270
   
    //IO初始化
    InitPort();
    
    //I2C初始化
    I2C_Init();
    
    //ADC初始化
    //adc_init(ADC0,AD20);//ADC 0M1初始化
    
    //DAC初始化
    dac_once_init(DAC0,VDDA);//DAC0初始化
    dac_once_init(DAC1,VDDA);//DAC1初始化
    
    /*
    **如果定义SD卡拍图就不使用Oled
    */
    #ifdef SD
    #undef Oled
    #endif
    
    //Oled初始化
    #ifdef Oled
    LCD_Init();
    #endif
    
    //串口初始化(串行口或蓝牙)
    uart_init(UART_PORT_USR,UART_BAUD_USR_SERIAL);  //UART4 E24 E25 Baud:256000
    
    //正交解码初始化
    //FTM_QUAD_init();
    
    //DMA测速
    DMA_count_Init(DMA_CH1, PTB0, 65535, DMA_rising); 
}