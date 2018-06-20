#ifndef __INCLUDE_H__
#define __INCLUDE_H__

#include  "common.h"

/*
 * Include 用户自定义的头文件
 */
//extern u8 pit_flag=0;
#include  "gpio.h"       //IO口操作
#include  "uart.h"      //串口
#include  "lptmr.h"     //低功耗定时器(延时)
#include   "FTM.h" 
#include   "adc.h"
#include    "delay.h"
#include    "dma.h"
#include    "exti.h"
#include    "PIT.h"
#include    "mcg.h"
#include    "wdog.h"
#endif  //__INCLUDE_H__
