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
#include "declare.h"

/**
**Function: InitClk
**Description: ��ʼ��ʱ��
**Calls:   ��
**Called By: InitAll
**Table Accessed: ��
**Table Updated: ��
**Input: ��
**Output: ��
**Return: ��
**Others:��
**/
void InitClk(void)
{
    SIM_SCGC5 = SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK;//ʹ��A B C D E�˿�
    SIM_SCGC5|= SIM_SCGC5_LPTIMER_MASK;          //ʹ���������ʱ��
    SIM_SCGC6|= SIM_SCGC6_PIT_MASK;              //ʹ��PITʱ��
}


/**
**Function: InitPort
**Description: IO��ʼ��
**Calls:   ��
**Called By: InitAll
**Table Accessed: ��
**Table Updated: ��
**Input: ��
**Output: ��
**Return: ��
**Others:��
**/
void InitPort(void)
{
    /*
    **ָʾ����
    */
    gpio_init(PORTA, 15, GPO, 0);                //ָʾ��  A15
    
    /*
    **���Motor
    */
    gpio_init(PORTC, 2, GPO, 0);                 //���ʹ�ܿ� C2
    gpio_init(PORTC, 1, GPO, 0);                 //���Ƶ������ת C1
    gpio_init(PORTC, 7, GPI_DOWN, 0);            //��������C7��������
    
    gpio_init(PORTD, 2, GPO, 0);                 //������Trig���� D2
    gpio_init(PORTB, 11, GPO, 0);                //������Trig���� B11
    gpio_init(PORTA, 19, GPO, 0);                //���Կ� A19
    gpio_init(PORTC, 5, GPI, 0);                 //����ͷģ���ź����� C5
    
    #ifdef Buzzer
    gpio_init(PORTB, 11, GPO, 0);                 //������B11
    #endif
}


/**
**Function: InitInterrupt
**Description: �ⲿ�жϳ�ʼ��
**Calls:   ��
**Called By: InitAll
**Table Accessed: ��
**Table Updated: ��
**Input: ��
**Output: ��
**Return: ��
**Others:��
**/
void InitInterrupt(void)
{
    Row_Exti_Init;                      //���жϣ�PORTB23 �˿��ⲿ�жϳ�ʼ�� ��˫���ش����жϣ��ڲ�����
    Row_Set_Priority;                   //���ж�������88���ж����ȼ�0
    Row_Disable_Irq;                    //�����ж�
    Field_Exti_Init;                    //���жϣ�PORTC3 �˿��ⲿ�жϳ�ʼ�� ��˫���ش����жϣ��ڲ�����
    Field_Set_Priority;                 //���ж�������89���ж����ȼ�1
    Field_Disable_Irq;                  //�س��ж�
    Ultrasonic_Exti_Init;               //�����жϣ�PORTD5�˿��ⲿ�жϳ�ʼ�� ��˫���ش����жϣ��ڲ�����
    Ultrasonic_Set_Priority;            //�����ж�������90���ж����ȼ�2
    Ultrasonic_Disable_Irq;             //�������ж�
}


/**
**Function: InitAll
**Description: ϵͳ��ʼ��
**Calls:   InitClk
**        InitPort
**        InitInterrupt
**Called By: main.c
**Table Accessed: ��
**Table Updated: ��
**Input: ��
**Output: ��
**Return: ��
**Others:��
**/
void InitAll(void)
{
    //ʱ�ӳ�ʼ��
    InitClk();
  
    //�ⲿ�жϳ�ʼ��
    InitInterrupt();
  
    //PWM��ʼ��
    FTM_PWM_init(FTM0, CH3, 15000, 50); //����ڳ�ʼ�� C4
    FTM_PWM_init(FTM2, CH0, 50,7320);   //���B18 50Hz//Center: 7270
   
    //IO��ʼ��
    InitPort();
    
    //I2C��ʼ��
    I2C_Init();
    
    //ADC��ʼ��
    //adc_init(ADC0,AD20);//ADC 0M1��ʼ��
    
    //DAC��ʼ��
    dac_once_init(DAC0,VDDA);//DAC0��ʼ��
    dac_once_init(DAC1,VDDA);//DAC1��ʼ��
    
    /*
    **�������SD����ͼ�Ͳ�ʹ��Oled
    */
    #ifdef SD
    #undef Oled
    #endif
    
    //Oled��ʼ��
    #ifdef Oled
    LCD_Init();
    #endif
    
    //���ڳ�ʼ��(���пڻ�����)
    uart_init(UART_PORT_USR,UART_BAUD_USR_SERIAL);  //UART4 E24 E25 Baud:256000
    
    //���������ʼ��
    //FTM_QUAD_init();
    
    //DMA����
    DMA_count_Init(DMA_CH1, PTB0, 65535, DMA_rising); 
}