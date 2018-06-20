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
#ifndef __ISR_H
#define __ISR_H 

#include  "declare.h"

/*
**���¶����ж�������

**��ȡ��Ĭ�ϵ��ж�����Ԫ�غ궨��       #undef  VECTOR_xxx
**�����¶��嵽�Լ���д���жϺ���       #define VECTOR_xxx    xxx_IRQHandler
**���磺
**     #undef VECTOR_106
**     #define VECTOR_106    PORTD_IRQHandler   ���¶���Ӳ���Ϸ��жϷ�����
**
**     extern void PORTD_IRQHandler(void);    ����������Ȼ����isr.c�ﶨ��
*/

#undef VECTOR_104
#define VECTOR_104    PORTB23_IRQHandler        /*B23���ж�*/

#undef VECTOR_105
#define VECTOR_105    PORTC3_IRQHandler         /*C3���ж�*/
   
#undef VECTOR_106
#define VECTOR_106    PORTD5_IRQHandler         /*D5�������ж�*/

/*
**Interrupt define
*/
#define Row_Exti_Init           exti_init(PORTB, 23, rising_up) //���жϣ�PORTB23 �˿��ⲿ�жϳ�ʼ�� ��˫���ش����жϣ��ڲ�����
#define Row_Set_Priority        set_irq_priority (88, 0)        //���ж�������88���ж����ȼ�0
#define Field_Exti_Init         exti_init(PORTC, 3, either_up)  //���жϣ�PORTC3 �˿��ⲿ�жϳ�ʼ�� ��˫���ش����жϣ��ڲ�����
#define Field_Set_Priority      set_irq_priority (89, 1)        //���ж�������89���ж����ȼ�1
#define Ultrasonic_Exti_Init    exti_init(PORTD, 5, either_up)  //�����жϣ�PORTD5 �˿��ⲿ�жϳ�ʼ�� ��˫���ش����жϣ��ڲ�����
#define Ultrasonic_Set_Priority set_irq_priority (90, 2)        //�����ж�������90���ж����ȼ�2

#define Row_InterruptMark               PORTB_ISFR & 1 << 23            //���жϴ�����־
#define Row_InterruptClear              PORTB_ISFR = 1 << 23            //���ж�����
#define Field_InterruptMark             PORTC_ISFR & 1 << 3             //���жϴ�����־
#define Field_InterruptClear            PORTC_ISFR = 1 << 3             //���ж�����
#define Ultrasonic_InterruptMark        PORTD_ISFR & 1 << 5             //�����жϴ�����־
#define Ultrasonic_InterruptClear       PORTD_ISFR = 1 << 5             //�����ж�����
#define Row_Enable_Irq                  enable_irq(88)                  //�����ж�
#define Row_Disable_Irq                 disable_irq(88)                 //�����ж�
#define Field_Enable_Irq                enable_irq(89)                  //�����ж�
#define Field_Disable_Irq               disable_irq(89)                 //�س��ж�
#define Ultrasonic_Enable_Irq           enable_irq(90)                  //�������ж�
#define Ultrasonic_Disable_Irq          disable_irq(90)                 //�������ж�

/*
**Buzzer
*/
#define Buzzer_noise   PTB11_OUT=1 
#define Buzzer_quite   PTB11_OUT=0  

/*
**UltrasonicDistance
*/
#define UltrasonicDistance 180
#define Interval 25

/*
**Isr��������
*/
extern int DAC_Count;                            //DAC����
extern int guAngleOffset;                        //ƫ�ƾ���
extern unsigned char DAC_flag;                  //DAC��־
extern unsigned char guFlagStand;               //ֱ��ִ�б�־
extern unsigned char guFlagStand;               //ֱ��ִ�б�־
extern unsigned char guFlagCtrl;                //��������־
extern unsigned char guOddEven;                 //��ż����־

extern unsigned char guModifyRownumber[];       //ѡ������
extern unsigned char *guP_Href;                 //ָ��ʵ���к���������ѡ��洢
extern unsigned char guHrefCount;               //�м���
extern unsigned char guHrefElect;               //����洢�м���

extern unsigned char guRoadBlock;               //·�ϱ�־
extern unsigned char guRoadBlockflag;           //·����ʱ��־
extern unsigned char guRoadBlockCount;          //·�ϸ�������
extern unsigned char guAngleflag;               //ƫ�ƽǶȱ�־
extern unsigned char guStraight;                //�����⵽ֱ��
extern unsigned int Ultrasonic;                 //����������
extern unsigned int Ultrasonic_Dis[6];          //��������������

/*
**Isr��������
*/
extern void PORTB23_IRQHandler(void);           //�ⲿ�ж�  B23  ���ж�
extern void PORTC3_IRQHandler(void);            //�ⲿ�ж�  C3   ���ж�
extern void PORTD5_IRQHandler(void);            //�ⲿ�ж�  D5   �������ж�

#endif  //__ISR_H


/* End of "isr.h" */
