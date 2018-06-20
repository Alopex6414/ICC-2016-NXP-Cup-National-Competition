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
            Online Modify Parameters(实时改变参数)
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
#ifndef __OLED_H__
#define __OLED_H__

/*
**SPI端口定义
**CK:时钟
**DI:数据
**RST:复位
**DC:数据读写
*/
#define INIT_OL_CK  gpio_init(PORTB, 21, GPO, 0)   //SPI的CK
#define INIT_OL_DI  gpio_init(PORTB, 22, GPO, 0)   //SPI的DI
#define INIT_OL_RST gpio_init(PORTB, 19, GPO, 0)   //SPI的RST
#define INIT_OL_DC  gpio_init(PORTB, 20, GPO, 0)   //SPI的DC
#define OL_CK_H     GPIO_SET(PORTB, 21, 1)         //CK高电平
#define OL_CK_L     GPIO_SET(PORTB, 21, 0)         //CK低电平
#define OL_DI_H     GPIO_SET(PORTB, 22, 1)         //DI高电平
#define OL_DI_L     GPIO_SET(PORTB, 22, 0)         //DI低电平
#define OL_RST_H    GPIO_SET(PORTB, 19, 1)         //RST高电平
#define OL_RST_L    GPIO_SET(PORTB, 19, 0)         //RST低电平
#define OL_DC_H     GPIO_SET(PORTB, 20, 1)         //DC高电平
#define OL_DC_L     GPIO_SET(PORTB, 20, 0)         //DC低电平

/*
**独立按键
*/
#define INIT_KEY_L  gpio_init(PORTC, 6, GPI, 0)    //'L'左键
#define INIT_KEY_R  gpio_init(PORTD, 7, GPI, 0)    //'R'右键
#define INIT_KEY_U  gpio_init(PORTD, 3, GPI, 0)    //'U'上键
#define INIT_KEY_D  gpio_init(PORTD, 4, GPI, 0)    //'D'下键
#define INIT_KEY_E  gpio_init(PORTD, 6, GPI, 0)    //'E'确定键
#define INIT_KEY_Q  gpio_init(PORTD, 1, GPI, 0)    //'Q'取消键
#define KEY_L_VAL   GPIO_Get(PORTC, 6)             //'L'键值
#define KEY_R_VAL   GPIO_Get(PORTD, 7)             //'R'键值
#define KEY_U_VAL   GPIO_Get(PORTD, 3)             //'U'键值
#define KEY_D_VAL   GPIO_Get(PORTD, 4)             //'D'键值
#define KEY_E_VAL   GPIO_Get(PORTD, 6)             //'E'键值
#define KEY_Q_VAL   GPIO_Get(PORTD, 1)             //'Q'键值


/*
**字节和字定义
*/
#define byte u8                                    //u8类型byte
#define word u16                                   //u16类型word

/*
**Oled坐标
*/
#define XLevelL		0x00                            //X轴低位
#define XLevelH		0x10                            //X轴高位
#define XLevel		((XLevelH&0x0F)*16+XLevelL)     //X轴坐标
#define Max_Column	128                             //最大列数
#define Max_Row		64                              //最大行数
#define	Brightness	0xCF                            //亮度
#define X_WIDTH 128                                     //X轴宽度
#define Y_WIDTH 64                                      //Y轴宽度

/*
**页数宏(PointChange)
*/
#define Pagination      5                               //页数
#define Oled_DELAY      2000                            //Oled按键延时
#define Page_DELAY      4000                            //Oled翻页延时

/*
**变量声明
*/
extern int SwitchPage;                                  //换页标志
extern int SwitchRow;                                   //选择行标志
extern int ChoiceStatus;                                //参数选择状态: 0:取消选择 1:选中行 2:修改参数

/*
**函数声明
*/
extern void LCD_Init(void);//LCD初始化
extern void IndependentKey(void);//独立按键初始化
extern void LCD_CLS(void); //LCD清屏
extern void LCD_Reset(void);//LCD复位
extern void DrawPicture(void);//128x64图像
extern void Stand_Dis(void);//LCD标准行显示
extern void Image_Dis(void);//远处图像显示
extern void Image_Dis2(void);//近处图像显示
extern void LCD_P6x8Str(byte x,byte y,byte ch[]);//6*8字符显示
extern void LCD_P8x16Str(byte x,byte y,byte ch[]);//8*16字符显示
extern void LCD_Print(byte x, byte y, byte ch[]);//LCD绘图
extern void LCD_PutPixel(byte x,byte y);//LCD绘制像素点
extern void LCD_Rectangle(byte x1,byte y1,byte x2,byte y2,byte gif);//LCD绘制矩形
extern void LCD_Fill(byte dat);//LCD充满
extern void PointChange(void);//小数点换页
extern void DisplayLED(int Page);//LCD显示
extern int LCDEncode(int LedCode);//LCD编码
extern int OledCMD(byte Str[],byte Mod);
extern void OledInput(void);

#endif