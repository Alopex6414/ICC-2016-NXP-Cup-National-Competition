/*********************Copyright(C) 2015-2016 TYUT JBD TEAM B.*******************
Copyright (C), 2015-2016, TYUT JBD TEAM B.
File name: Oled.c 
Author: TEAM B, Alopex
Version: 1.0               
Date: 2015.10.28(E)
      2015.12.31(C)
      2016.03.15(A)
Description: 
            All Modules Initialization(ģ���ʼ��)
            Keyboard Display Variables(������ʾ����)
Others: Noting
Function List: TFTҺ����
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
#ifndef	_TFT_H_
#define _TFT_H_


#define	LCD_H	240		//��
#define LCD_W	320		//��

#define   H_VIEW

#define   CS             PTE0_OUT
#define   SOUT           PTE1_OUT
#define   SCL            PTE2_OUT
#define   IN             PTE3_OUT
#define   DC             PTE4_OUT
#define   RESET          PTE5_OUT

/******������ɫ*****/
#define RED	    0XF800    //��ɫ
#define GREEN   0X07E0    //��ɫ
#define BLUE    0X001F    //��ɫ
#define BRED    0XF81F
#define GRED    0XFFE0    //��ɫ
#define GBLUE   0X07FF    //
#define BLACK   0X0000    //��ɫ
#define WHITE   0XFFFF    //��ɫ
#define YELLOW  0xFFE0    //��ɫ
typedef	struct
{
	u16 x;
	u16 y;
}Site_t;

typedef	struct
{
	u16 W;		//��
	u16 H;		//��
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