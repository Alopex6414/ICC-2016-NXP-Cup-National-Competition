/*********************Copyright(C) 2015-2016 TYUT JBD TEAM B.*******************
Copyright (C), 2015-2016, TYUT JBD TEAM B.
File name: Oled.c 
Author: TEAM B, Alopex
Version: 1.0               
Date: 2015.10.28(E)
      2015.12.31(C)
      2016.03.15(A)
Description: 
            All Modules Initialization(模块初始化)
            Keyboard Display Variables(键盘显示变量)
Others: Noting
Function List: TFT液晶屏
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
#ifndef	_TFT_H_
#define _TFT_H_


#define	LCD_H	240		//高
#define LCD_W	320		//宽

#define   H_VIEW

#define   CS             PTE0_OUT
#define   SOUT           PTE1_OUT
#define   SCL            PTE2_OUT
#define   IN             PTE3_OUT
#define   DC             PTE4_OUT
#define   RESET          PTE5_OUT

/******常用颜色*****/
#define RED	    0XF800    //红色
#define GREEN   0X07E0    //绿色
#define BLUE    0X001F    //蓝色
#define BRED    0XF81F
#define GRED    0XFFE0    //灰色
#define GBLUE   0X07FF    //
#define BLACK   0X0000    //黑色
#define WHITE   0XFFFF    //白色
#define YELLOW  0xFFE0    //黄色
typedef	struct
{
	u16 x;
	u16 y;
}Site_t;

typedef	struct
{
	u16 W;		//宽
	u16 H;		//高
}Size_t;

extern void TFT_Init(void);
extern void TFT_WriteCom(u8 command);
extern void TFT_WriteData(u8 data);
extern void TFT_WriteData16Bit(u16 data);
extern void TFT_Reset(void);
extern void TFT_PTLON(Site_t site,Size_t size);
extern void TFT_SetXY(u16 x,u16 y);
extern void TFT_DrawPoint(u16 x,u16 y,u16 color);
extern void TFT_Rectangle(Site_t site,Size_t size,u16 RGB);
extern u16 TFT_Char(Site_t site,const u8 ascii, u16 Color, u16 bkColor);
extern void TFT_Str(Site_t site,u8 *Str, u16 Color, u16 bkColor);
extern void TFT_Num(Site_t site,u32 num, u16 Color, u16 bkColor);
extern void TFT_ImgRGB(Site_t site,Size_t size,u16 *img);
extern void TFT_ImgRGB_Buff(Site_t site,Size_t size,u8 *img);
extern void TFT_ImgRGB_ActBuff(Site_t site,Size_t size,u8 n,unsigned char *img);
extern void TFT_Clear(u16 color);
extern void addset(u16 x,u16 y);
#endif