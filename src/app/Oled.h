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
            Online Modify Parameters(ʵʱ�ı����)
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
#ifndef __OLED_H__
#define __OLED_H__

/*
**SPI�˿ڶ���
**CK:ʱ��
**DI:����
**RST:��λ
**DC:���ݶ�д
*/
#define INIT_OL_CK  gpio_init(PORTB, 21, GPO, 0)   //SPI��CK
#define INIT_OL_DI  gpio_init(PORTB, 22, GPO, 0)   //SPI��DI
#define INIT_OL_RST gpio_init(PORTB, 19, GPO, 0)   //SPI��RST
#define INIT_OL_DC  gpio_init(PORTB, 20, GPO, 0)   //SPI��DC
#define OL_CK_H     GPIO_SET(PORTB, 21, 1)         //CK�ߵ�ƽ
#define OL_CK_L     GPIO_SET(PORTB, 21, 0)         //CK�͵�ƽ
#define OL_DI_H     GPIO_SET(PORTB, 22, 1)         //DI�ߵ�ƽ
#define OL_DI_L     GPIO_SET(PORTB, 22, 0)         //DI�͵�ƽ
#define OL_RST_H    GPIO_SET(PORTB, 19, 1)         //RST�ߵ�ƽ
#define OL_RST_L    GPIO_SET(PORTB, 19, 0)         //RST�͵�ƽ
#define OL_DC_H     GPIO_SET(PORTB, 20, 1)         //DC�ߵ�ƽ
#define OL_DC_L     GPIO_SET(PORTB, 20, 0)         //DC�͵�ƽ

/*
**��������
*/
#define INIT_KEY_L  gpio_init(PORTC, 6, GPI, 0)    //'L'���
#define INIT_KEY_R  gpio_init(PORTD, 7, GPI, 0)    //'R'�Ҽ�
#define INIT_KEY_U  gpio_init(PORTD, 3, GPI, 0)    //'U'�ϼ�
#define INIT_KEY_D  gpio_init(PORTD, 4, GPI, 0)    //'D'�¼�
#define INIT_KEY_E  gpio_init(PORTD, 6, GPI, 0)    //'E'ȷ����
#define INIT_KEY_Q  gpio_init(PORTD, 1, GPI, 0)    //'Q'ȡ����
#define KEY_L_VAL   GPIO_Get(PORTC, 6)             //'L'��ֵ
#define KEY_R_VAL   GPIO_Get(PORTD, 7)             //'R'��ֵ
#define KEY_U_VAL   GPIO_Get(PORTD, 3)             //'U'��ֵ
#define KEY_D_VAL   GPIO_Get(PORTD, 4)             //'D'��ֵ
#define KEY_E_VAL   GPIO_Get(PORTD, 6)             //'E'��ֵ
#define KEY_Q_VAL   GPIO_Get(PORTD, 1)             //'Q'��ֵ


/*
**�ֽں��ֶ���
*/
#define byte u8                                    //u8����byte
#define word u16                                   //u16����word

/*
**Oled����
*/
#define XLevelL		0x00                            //X���λ
#define XLevelH		0x10                            //X���λ
#define XLevel		((XLevelH&0x0F)*16+XLevelL)     //X������
#define Max_Column	128                             //�������
#define Max_Row		64                              //�������
#define	Brightness	0xCF                            //����
#define X_WIDTH 128                                     //X����
#define Y_WIDTH 64                                      //Y����

/*
**ҳ����(PointChange)
*/
#define Pagination      5                               //ҳ��
#define Oled_DELAY      2000                            //Oled������ʱ
#define Page_DELAY      4000                            //Oled��ҳ��ʱ

/*
**��������
*/
extern int SwitchPage;                                  //��ҳ��־
extern int SwitchRow;                                   //ѡ���б�־
extern int ChoiceStatus;                                //����ѡ��״̬: 0:ȡ��ѡ�� 1:ѡ���� 2:�޸Ĳ���

/*
**��������
*/
extern void LCD_Init(void);//LCD��ʼ��
extern void IndependentKey(void);//����������ʼ��
extern void LCD_CLS(void); //LCD����
extern void LCD_Reset(void);//LCD��λ
extern void DrawPicture(void);//128x64ͼ��
extern void Stand_Dis(void);//LCD��׼����ʾ
extern void Image_Dis(void);//Զ��ͼ����ʾ
extern void Image_Dis2(void);//����ͼ����ʾ
extern void LCD_P6x8Str(byte x,byte y,byte ch[]);//6*8�ַ���ʾ
extern void LCD_P8x16Str(byte x,byte y,byte ch[]);//8*16�ַ���ʾ
extern void LCD_Print(byte x, byte y, byte ch[]);//LCD��ͼ
extern void LCD_PutPixel(byte x,byte y);//LCD�������ص�
extern void LCD_Rectangle(byte x1,byte y1,byte x2,byte y2,byte gif);//LCD���ƾ���
extern void LCD_Fill(byte dat);//LCD����
extern void PointChange(void);//С���㻻ҳ
extern void DisplayLED(int Page);//LCD��ʾ
extern int LCDEncode(int LedCode);//LCD����
extern int OledCMD(byte Str[],byte Mod);
extern void OledInput(void);

#endif