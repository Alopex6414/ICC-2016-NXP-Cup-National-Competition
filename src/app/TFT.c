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
#include "common.h"
#include "gpio.h"
#include "SPI.h"
#include "TFT.h"
#include "delay.h"
#include "font.h"

extern unsigned char guImgBuff[110][25];
   
     
void TFT_Init()
{
    
    gpio_init  (PORTE, 4, GPO, 1);
    gpio_init  (PORTE, 5, GPO, 1);
    spi_init(SPI1,MASTER);
    
    TFT_Reset();
    
    TFT_WriteCom(0xCF);
    TFT_WriteData(0x00);
    TFT_WriteData(0x81);
    TFT_WriteData(0x30);

     
    TFT_WriteCom(0xED);
    TFT_WriteData(0x64);
    TFT_WriteData(0x03);
    TFT_WriteData(0x12);
    TFT_WriteData(0x81);

     
    TFT_WriteCom(0xE8);
    TFT_WriteData(0x85);
    TFT_WriteData(0x10);
    TFT_WriteData(0x78);

     
    TFT_WriteCom(0xCB);
    TFT_WriteData(0x39);
    TFT_WriteData(0x2C);
    TFT_WriteData(0x00);
    TFT_WriteData(0x34);
    TFT_WriteData(0x02);

     
    TFT_WriteCom(0xF7);
    TFT_WriteData(0x20);

     
    TFT_WriteCom(0xEA);
    TFT_WriteData(0x00);
    TFT_WriteData(0x00);

     
    TFT_WriteCom(0xB1);
    TFT_WriteData(0x00);
    TFT_WriteData(0x1B);

     
    TFT_WriteCom(0xB6);
    TFT_WriteData(0x0A);
    TFT_WriteData(0xA2);

    TFT_WriteCom(0x36);
    TFT_WriteData(0x68);    //横屏

    TFT_WriteCom(0X2A);
    TFT_WriteData(0x00);  //start
    TFT_WriteData(0x00);
    TFT_WriteData(0x01);  //end
    TFT_WriteData(0x3F);

    TFT_WriteCom(0X2B);
    TFT_WriteData(0x00);   //start
    TFT_WriteData(0x00);
    TFT_WriteData(0x00);   //end
    TFT_WriteData(0xEF); 
    
    TFT_WriteCom(0xC0);
    TFT_WriteData(0x35);

     
    TFT_WriteCom(0xC1);
    TFT_WriteData(0x11);

    TFT_WriteCom(0xC5);
    TFT_WriteData(0x45);
    TFT_WriteData(0x45);

    TFT_WriteCom(0xC7);
    TFT_WriteData(0xA2);

    TFT_WriteCom(0xF2);
    TFT_WriteData(0x00);

    TFT_WriteCom(0x26);
    TFT_WriteData(0x01);
    
    
    TFT_WriteCom(0xE0); //Set Gamma
    TFT_WriteData(0x0F);
    TFT_WriteData(0x26);
    TFT_WriteData(0x24);
    TFT_WriteData(0x0B);
    TFT_WriteData(0x0E);
    TFT_WriteData(0x09);
    TFT_WriteData(0x54);
    TFT_WriteData(0xA8);
    TFT_WriteData(0x46);
    TFT_WriteData(0x0C);
    TFT_WriteData(0x17);
    TFT_WriteData(0x09);
    TFT_WriteData(0x0F);
    TFT_WriteData(0x07);
    TFT_WriteData(0x00);
    TFT_WriteCom(0XE1); //Set Gamma
    TFT_WriteData(0x00);
    TFT_WriteData(0x19);
    TFT_WriteData(0x1B);
    TFT_WriteData(0x04);
    TFT_WriteData(0x10);
    TFT_WriteData(0x07);
    TFT_WriteData(0x2A);
    TFT_WriteData(0x47);
    TFT_WriteData(0x39);
    TFT_WriteData(0x03);
    TFT_WriteData(0x06);
    TFT_WriteData(0x06);
    TFT_WriteData(0x30);
    TFT_WriteData(0x38);
    TFT_WriteData(0x0F);
     


     
    TFT_WriteCom(0x3a); // Memory Access Control
    TFT_WriteData(0x55);
    TFT_WriteCom(0x11); //Exit Sleep
     
    TFT_WriteCom(0x29); //display on
    TFT_WriteCom(0x2c);
   
    
}

void TFT_WriteCom(u8 command)
{
    
    DC=0;
    SPI_Send(SPI1,command);
}

void TFT_WriteData(u8 data)
{
    
    DC=1;
    SPI_Send(SPI1,data);
}

void TFT_WriteData16Bit(u16 data)
{
    
    DC=1;
    SPI_Send(SPI1,(u8)(data>>8));  //高8位 
    SPI_Send(SPI1,(u8)data);
}
void TFT_Reset()
{
    RESET=0;
    delayms(10);
    RESET=1;
    delayms(10);
}


void TFT_Clear(u16 color)
{
    int ii,jj;
    TFT_SetXY(0,0);
    for(ii=0;ii<320;ii++)
        for(jj=0;jj<240;jj++)
        {
            TFT_WriteData16Bit(color);
        }
}



/*************************************************
函数名：TFT_PTLON
功能：设置lcd显示区域，在此区域写点数据自动换行
入口参数：site起点     size大小    16位结构体变量
返回值：无
*************************************************/

void TFT_PTLON(Site_t site,Size_t size)
{
    TFT_WriteCom(0X2A);
    TFT_WriteData(site.x >> 8); //start
    TFT_WriteData(site.x - ((site.x >> 8) << 8));
    TFT_WriteData((site.x + size.W - 1) >> 8); //end
    TFT_WriteData((site.x + size.W - 1) - (((site.x + size.W - 1) >> 8) << 8));

    TFT_WriteCom(0X2B);
    TFT_WriteData(site.y >> 8); //start
    TFT_WriteData(site.y - ((site.y >> 8) << 8));
    TFT_WriteData((site.y + size.H - 1) >> 8); //end
    TFT_WriteData((site.y + size.H - 1) - (((site.y + size.H - 1) >> 8) << 8));

    TFT_WriteCom(0x2c);
}

/*************************************************
函数名：TFT_Set_XY
功能：设置lcd显示起始点
入口参数：xy坐标
返回值：无
*************************************************/
void TFT_SetXY(u16 x,u16 y)
{
    TFT_WriteCom(0x2a);
	TFT_WriteData16Bit(x);
	
    TFT_WriteCom(0x2b);
	TFT_WriteData16Bit(y);

	TFT_WriteCom(0x2c);
}
/*************************************************
函数名：TFT_DrawPoint
功能：画一个点
入口参数：无
返回值：无
*************************************************/
void TFT_DrawPoint(u16 x,u16 y,u16 color)
{
	TFT_SetXY(x,y);
	TFT_WriteData16Bit(color);
}
/******************************************************
 * 函数名：LCD_Rectangle
 * 描述  ：在指定坐标处显示指定长宽的矩形
 * 输入  : -x1      横向矩形起点 0~319
           -y1      纵向矩形起点 0~239
		   -len     横向长度 0~319
		   -wid     纵向宽度 0~239
		   -rgb565  填充颜色
 * 输出  ：无
 * 举例  ：	LCD_Rectangle(10,10,120,160,0);
		    LCD_Rectangle(200,200,120,40,0);
 * 注意  ：	已测试
*********************************************************/    
void TFT_Rectangle(Site_t site,Size_t size,u16 RGB)
{                    
	u32 n, temp;
	
   	TFT_PTLON(site,size); 				//开窗
	
	temp = (u32)size.W * size.H;  
	
    				                  //写内存
   	
    for(n=0; n<temp; n++)
	{
		TFT_WriteData16Bit(RGB);		
	} 
}
/************************************************************
 * 函数名：TFT_Char
 * 描述  ：在液晶坐标(x,y)中,显示一个8*16大小的字符
 * 输入  : 	-x 0~(239-16)
 *         	-y 0~(319-8)
 *      	-acsii    要写入的字符
 *			-Color    字符颜色
 *			-bkColor  背景颜色
 * 输出  ：无
 * 举例  ：无
 * 注意  ：
******************************************************************/    
u16 TFT_Char(Site_t site,const u8 ascii, u16 Color, u16 bkColor)
{       
#define MAX_CHAR_POSX (LCD_W-8)
#define MAX_CHAR_POSY (LCD_H-16)
    
    u8 temp, t, pos;   
	Size_t size={8,16};
	
    if(site.x > MAX_CHAR_POSX || site.y > MAX_CHAR_POSY)
	{
        return 0;
	}
       
    TFT_PTLON(site,size); 
	
    for (pos=0;pos<16;pos++)
    {
        temp=asc16[((ascii-0x20)*16)+pos];
        
        for(t=0; t<8; t++)
        {
            
            if(temp & 0x80)              
            {
                TFT_WriteData16Bit(Color);
            }
            else 	
            {
                TFT_WriteData16Bit(bkColor);  
            }            
            temp <<= 1;	
        }
    }
    return 1;
#undef MAX_CHAR_POSX
#undef MAX_CHAR_POSY   
}

/************************************************************
 * 函数名：TFT_Str
 * 描述  ：在液晶坐标(x,y)中,显示一个8*16大小的字符串
 * 输入  : 	-x 0~(239-16)
 *         	-y 0~(319-8)
 *      	-acsii    要写入的字符串
 *			-Color    字符颜色
 *			-bkColor  背景颜色
 * 输出  ：无
 * 举例  ：无
 * 注意  ：
******************************************************************/    

void TFT_Str(Site_t site,u8 *Str, u16 Color, u16 bkColor)
{ 
#define MAX_CHAR_POSX (LCD_W-8)
#define MAX_CHAR_POSY (LCD_H-16)    
    while(*Str != '\0')
    {       
        if(site.x > MAX_CHAR_POSX ) 
        {	
					//换行
           site.x = 0;
           site.y += 16;
        }
        if(site.y > MAX_CHAR_POSY ) 
        {	
					//一屏
           site.y = 0;
		   site.x = 0;   
        }
        
        TFT_Char(site, *Str,Color, bkColor);
        site.x += 8;
        Str ++ ;
    }
#undef MAX_CHAR_POSX
#undef MAX_CHAR_POSY   
}
/************************************************************
 * 函数名：TFT_Num
 * 描述  ：在液晶坐标(x,y)中,显示一个8*16大小的数字
 * 输入  : 	-x 0~(239-16)
 *         	-y 0~(319-8)
 *      	-acsii    要写入的字符
 *			-Color    字符颜色
 *			-bkColor  背景颜色
 * 输出  ：无
 * 举例  ：无
 * 注意  ：
******************************************************************/    

void TFT_Num(Site_t site,u32 num, u16 Color, u16 bkColor)	  
{      
    u32 res=num;   	   
    u8 t=0;
	Site_t sitetemp;
	sitetemp.y=site.y;
	
    if( num == 0 )
    {
        TFT_Char(site,'0', Color, bkColor);
        return;
    }	
    while( res )  /*得到数字长度t*/
    {
        res/=10;
        t++;
    }
    
    while(num)
    {
		sitetemp.x=site.x+(8*(t--)-8);
        TFT_Char(sitetemp,(num%10)+'0', Color, bkColor);
        num /= 10 ;
    }			     
}

/************************************************************
 * 函数名：TFT_ImgRGB
 * 描述  ：在液晶坐标(x,y)中,显示图像
 * 输入  : 	-x 0~(239-16)
 *         	-y 0~(319-8)
 *      	-acsii    要写入的字符
 *			-Color    字符颜色
 *			-bkColor  背景颜色
 * 输出  ：无
 * 举例  ：无
 * 注意  ：
******************************************************************/    

void TFT_ImgRGB	(Site_t site,Size_t size,u16 *img)
{
	u32 n, temp;
	
   	TFT_PTLON(site,size); 				//开窗
	temp = (u32)size.W * size.H;  
	
   	for(n=0; n<temp; n++)
	{
		TFT_WriteData16Bit( *img );		
		img++;
	}
}
/************************************************************
 * 函数名：TFT_ImgRGB_Buff
 * 描述  ：在液晶坐标(x,y)中,显示压缩图像
 * 输入  : 	-x 0~(239-16)
 *         	-y 0~(319-8)
 *      	-acsii    要写入的字符
 *			-Color    字符颜色
 *			-bkColor  背景颜色
 * 输出  ：无
 * 举例  ：无
 * 注意  ：
******************************************************************/   
void TFT_ImgRGB_Buff(Site_t site,Size_t size,u8 *img)
{
	u32 n=0, temp=0;
    u8 k=0,mask=0x01;
    u8 i=0,j=0;
    
   	TFT_PTLON(site,size); 				//开窗
	temp = (u32)size.W * size.H;  
	
   	
    n=size.W/8;
    
    for(i=0;i<size.H;i++)
    {
        for(j=0;j<n;j++)
       {
           for(k=0;k<=7;k++)
		{
            TFT_WriteData16Bit( ((img[(i/2)*n+j]>>k)&mask)*0X07FF );//GBLUE 	0X07FF
           
        }	
          
       }
       
        
    }
}
/************************************************************
 * 函数名：TFT_ImgRGB_Buff
 * 描述  ：在液晶坐标(x,y)中,显示压缩图像
 * 输入  : 	-x 0~(239-16)
 *         	-y 0~(319-8)
 *      	-acsii    要写入的字符
 *			-Color    字符颜色
 *			-bkColor  背景颜色
 * 输出  ：无
 * 举例  ：无
 * 注意  ：
******************************************************************/   
void TFT_ImgRGB_ActBuff(Site_t site,Size_t size,u8 n,unsigned char *img)
{
    u8 i=0,j=0, k=0,x=0,y=0;
    
   	TFT_PTLON(site,size); 				//开窗
	  
    for(i=0;i<size.H;i++)
    { 
        
        for(k=0;k<guImgBuff[i/2][0];k++)
	    {
            TFT_WriteData16Bit( 0x0000 );//GBLUE 	0X07FF
        }
        
       for(j=1;j<n&&guImgBuff[i/2][j]!=0;j++)
       {   
           x = guImgBuff[i/2][j]-guImgBuff[i/2][j-1];
           y = j%2;
           // 
           for(k=0;k<x;k++)
		   {
               TFT_WriteData16Bit( y*0X07FF );//GBLUE 	0X07FF
           }	
       }
    }
    
}


 