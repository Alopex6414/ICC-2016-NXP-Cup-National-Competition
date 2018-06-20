/*********************Copyright(C) 2015-2016 TYUT JBD TEAM B.*******************
Copyright (C), 2015-2016, TYUT JBD TEAM B.
File name: Isr.c 
Author: TEAM B, Alopex
Version: 1.0               
Date: 2015.10.28(E)
      2015.12.31(C)
      2016.03.15(A)
Description: 
            Row Interrupt Function(行中断函数)
            Field Interrupt Function (场中断函数)
            Ultrasonic Interrupt Function(超声波中断函数)
            UART transform Function(串口发送函数)
Others: Noting
Function List:
            1.ThousandToSingleUp 键盘显示传参函数
            2.ParameterSent 3-6变量串口发送函数
            3.calculate 赛道信息计算函数
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
#ifndef __ISR_H
#define __ISR_H 

#include  "declare.h"

/*
**重新定义中断向量表

**先取消默认的中断向量元素宏定义       #undef  VECTOR_xxx
**在重新定义到自己编写的中断函数       #define VECTOR_xxx    xxx_IRQHandler
**例如：
**     #undef VECTOR_106
**     #define VECTOR_106    PORTD_IRQHandler   重新定义硬件上访中断服务函数
**
**     extern void PORTD_IRQHandler(void);    声明函数，然后在isr.c里定义
*/

#undef VECTOR_104
#define VECTOR_104    PORTB23_IRQHandler        /*B23行中断*/

#undef VECTOR_105
#define VECTOR_105    PORTC3_IRQHandler         /*C3场中断*/
   
#undef VECTOR_106
#define VECTOR_106    PORTD5_IRQHandler         /*D5超声波中断*/

/*
**Interrupt define
*/
#define Row_Exti_Init           exti_init(PORTB, 23, rising_up) //行中断，PORTB23 端口外部中断初始化 ，双边沿触发中断，内部上拉
#define Row_Set_Priority        set_irq_priority (88, 0)        //行中断向量号88，中断优先级0
#define Field_Exti_Init         exti_init(PORTC, 3, either_up)  //场中断，PORTC3 端口外部中断初始化 ，双边沿触发中断，内部上拉
#define Field_Set_Priority      set_irq_priority (89, 1)        //场中断向量号89，中断优先级1
#define Ultrasonic_Exti_Init    exti_init(PORTD, 5, either_up)  //声波中断，PORTD5 端口外部中断初始化 ，双边沿触发中断，内部上拉
#define Ultrasonic_Set_Priority set_irq_priority (90, 2)        //声波中断向量号90，中断优先级2

#define Row_InterruptMark               PORTB_ISFR & 1 << 23            //行中断触发标志
#define Row_InterruptClear              PORTB_ISFR = 1 << 23            //行中断清零
#define Field_InterruptMark             PORTC_ISFR & 1 << 3             //场中断触发标志
#define Field_InterruptClear            PORTC_ISFR = 1 << 3             //场中断清零
#define Ultrasonic_InterruptMark        PORTD_ISFR & 1 << 5             //声波中断触发标志
#define Ultrasonic_InterruptClear       PORTD_ISFR = 1 << 5             //声波中断清零
#define Row_Enable_Irq                  enable_irq(88)                  //开行中断
#define Row_Disable_Irq                 disable_irq(88)                 //关行中断
#define Field_Enable_Irq                enable_irq(89)                  //开场中断
#define Field_Disable_Irq               disable_irq(89)                 //关场中断
#define Ultrasonic_Enable_Irq           enable_irq(90)                  //开声波中断
#define Ultrasonic_Disable_Irq          disable_irq(90)                 //关声波中断

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
**Isr变量声明
*/
extern int DAC_Count;                            //DAC计数
extern int guAngleOffset;                        //偏移距离
extern unsigned char DAC_flag;                  //DAC标志
extern unsigned char guFlagStand;               //直立执行标志
extern unsigned char guFlagStand;               //直立执行标志
extern unsigned char guFlagCtrl;                //方向计算标志
extern unsigned char guOddEven;                 //奇偶场标志

extern unsigned char guModifyRownumber[];       //选行数组
extern unsigned char *guP_Href;                 //指向实际行号数组用于选择存储
extern unsigned char guHrefCount;               //行计数
extern unsigned char guHrefElect;               //数组存储行计数

extern unsigned char guRoadBlock;               //路障标志
extern unsigned char guRoadBlockflag;           //路障延时标志
extern unsigned char guRoadBlockCount;          //路障个数计数
extern unsigned char guAngleflag;               //偏移角度标志
extern unsigned char guStraight;                //红外检测到直走
extern unsigned int Ultrasonic;                 //超声波距离
extern unsigned int Ultrasonic_Dis[6];          //超声波距离数组

/*
**Isr函数声明
*/
extern void PORTB23_IRQHandler(void);           //外部中断  B23  行中断
extern void PORTC3_IRQHandler(void);            //外部中断  C3   场中断
extern void PORTD5_IRQHandler(void);            //外部中断  D5   超声波中断

#endif  //__ISR_H


/* End of "isr.h" */
