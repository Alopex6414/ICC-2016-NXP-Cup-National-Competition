/*********************Copyright(C) 2015-2016 TYUT JBD TEAM B.*******************
Copyright (C), 2015-2016, TYUT JBD TEAM B.
File name: func.c 
Author: TEAM B, Alopex
Version: 1.0               
Date: 2015.10.28(E)
      2015.12.31(C)
      2016.03.15(A)
Description: 
            Definition Function(定义功能函数)
            Keyboard Display Function (键盘显示函数)
            UART transform Function(串口发送函数)
            TF Card Send Parameters Function(TF卡发送参数函数)
            GetSpeed Function(获取两轮速度函数)
            Clear Array(清空数组函数)
            UltrasonicTrig(超声波触发函数)
            Beacon Clock(信标计时器)
            Parameters Flash(flash存储参数)
Others: Noting
Function List:
            1.ThousandToSingleUp 键盘显示传参函数
            2.ParameterSent 3-6变量串口发送函数
            3.Calculate 赛道信息计算函数
            4.Getspeed 获取当前两轮速度
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

/*
**键盘显示千百十个位
*/
int ThousandUp = 0;
int HundredUp = 0;
int TenUp = 0;  
int SingleUp = 0;                   //上排

int ThousandDown = 0;
int HundredDown = 0;
int TenDown = 0;
int SingleDown = 0;                 //下排

/*
**串口发送函数
*/
float gfSendWhite = 0;              //串口发送变量白色曲线
float gfSendRed = 0;                //串口发送变量红色曲线
float gfSendGreen = 0;              //串口发送变量绿色曲线
float gfSendBlue = 0;               //串口发送变量蓝色曲线

/*
**SD卡计数时间
*/
int SDTIME=0;
int SD_Record = 0;//SD卡记录完成标志

/*
**起跑计数
*/
int StartCount = 0;

/*
**红外接收
*/
volatile unsigned char guInfrared_front = 0;//前方红外接收管

/*
**BeaconClock
*/
int giBeaconExcept = 0;//期望信标个数,个数到了停车
float gfAllMeter = 0;//总路程
float gfAllTime = 0;//总时间
float gfRealSpeed = 0;//平均速度
unsigned char guBeaconTimer = 0;//计时器

/*
**DMA测速
*/
int giSpeedWheel=0;                    //后轮速度
int giCurrentSpeed=0;                  //当前速度
int giSpeedSmooth[5]={0};              //速度滤波
float gfSpeed_Filter=0;                //加权滤波速度
float gfSpeed_Filter_Old=0;            //前一次加权滤波速度

/*
**Function: ThousandToSingleUp ThousandToSingleDown
**Description: 键盘显示千百十个位函数
**Calls:   无
**Called By: 
**Table Accessed: 无
**Table Updated: 无
**Input:  value 需要显示的变量
**Output: 无
**Return: 无
**Others:无
**/
void ThousandToSingleUp(int value)
{
    ThousandUp = value / 1000;        
    HundredUp = (value % 1000) / 100;
    TenUp = (value % 100) / 10;
    SingleUp = (value % 100) % 10;
}

void ThousandToSingleDown(int value)
{
    ThousandDown = value / 1000;        
    HundredDown = (value % 1000) / 100;
    TenDown = (value % 100) / 10;
    SingleDown = (value % 100) % 10;
}

/**
**Function: ParameterSent
**Description: 串口函数
**Calls:   无
**Called By: main.c
**Table Accessed: 无
**Table Updated: 无
**Input:  gfSendWhite
**        gfSendRed
**        gfSendGreen
**        gfSendBlue
**Output: 无
**Return: 无
**Others:无
**/
void ParameterSent(void)
{
    signed int temp;
    unsigned char temp1, temp2;
    
    uart_putchar(UART_PORT_USR,0xff);
    uart_putchar(UART_PORT_USR,0xff);
    
    /*速度发送*/
    /*gfSendWhite = giBeacon_Row;
    gfSendRed   = giBeacon_Col;
    gfSendGreen = giExpectCOL;*/
    //gfSendBlue  = giSpeedWheelR;
      
    /*白色曲线(1)*/
    temp = (signed int)gfSendWhite;  
    temp1 = (temp) >> 8;
    temp2 = (temp)&0x00ff;
    uart_putchar(UART_PORT_USR, temp1);
    uart_putchar(UART_PORT_USR, temp2);
    
    /*红色曲线(2)*/
    temp = (signed int)gfSendRed; 
    temp1 = (temp) >> 8;
    temp2 = (temp)&0x00ff;
    uart_putchar(UART_PORT_USR, temp1);
    uart_putchar(UART_PORT_USR, temp2);
    
    /*绿色曲线(3)*/
    temp = (signed int)gfSendGreen;        
    temp1 = (temp) >> 8;
    temp2 = (temp)&0x00ff;
    uart_putchar(UART_PORT_USR, temp1);
    uart_putchar(UART_PORT_USR, temp2);

    /*蓝色曲线(4)*/
    temp = (signed int)gfSendBlue;    
    temp1 = (temp) >> 8;
    temp2 = (temp)&0x00ff;
    uart_putchar(UART_PORT_USR, temp1);
    uart_putchar(UART_PORT_USR, temp2);

    uart_putchar(UART_PORT_USR, 6);
    /*uart_putchar(UART_PORT_USR, 0x66);
    uart_putchar(UART_PORT_USR, 0x66);*/

}

/**
**Function: SendPIEX
**Description: 静态上位机发送函数
**Calls:   无
**Called By: main.c
**Table Accessed: 无
**Table Updated: 无
**Input: guBuff
**Output: 无
**Return: 无
**Others:无
**/
void SendPIEX(void)           //传赛道黑线信息
{
    unsigned char  i, j, i0, buff1 = 100;
    unsigned char  mask = 0;
    
    for (i = 0; i < ROW; i++)
    {
      for (i0 = 0; i0 < guImgBuff[i][0]; i0++)
      {
	buff1 = 0;
        uart_putchar(UART_PORT_USR, buff1);
      }
      for (j = 1; j < NUM&&guImgBuff[i][j]!=0; j++)
      {
        mask = guImgBuff[i][j]-guImgBuff[i][j-1];          
        for (i0 = 0; i0 < mask; i0++)
	{
          buff1 = (j%2) * 128;
          uart_putchar(UART_PORT_USR, buff1);
	}
       }
     }

}

/**
**Function: SendPIEX1
**Description: 动态上位机发送函数
**Calls:   无
**Called By: main.c
**Table Accessed: 无
**Table Updated: 无
**Input:  guBuff
**        giRoadEdgeL
**        giRoadEdgeR 
**        giRoadLocation
**Output: 无
**Return: 无
**Others:无
**/
void SendPIEX1(void)           //传参数信息
{
    unsigned char  i, j=0;
    uart_putchar(UART_PORT_USR, 0xFF);
    uart_putchar(UART_PORT_USR, 0xFF);
    
    for (i = ROW; i > 0; i--)
    {
      for (j = 0; j < NUM; j++)
      {
        uart_putchar(UART_PORT_USR, guImgBuff[i-1][j]);
      }	
    }
    
    for (i = ROW; i > 0; i--)
    {	
      uart_putchar(UART_PORT_USR, giTopBotEdge[i-1][0]);
      uart_putchar(UART_PORT_USR, giTopBotEdge[i-1][1]);         
      uart_putchar(UART_PORT_USR, giTopBotEdge[i-1][0]);
      uart_putchar(UART_PORT_USR, giTopBotEdge[i-1][1]);
      uart_putchar(UART_PORT_USR, giTopBotEdge[i-1][0]);
      uart_putchar(UART_PORT_USR, giTopBotEdge[i-1][1]);
    }
    
}

/**
**Function: ClearArrayChar
**Description: 清空数据
**Calls:   无
**Called By: main.c
**Table Accessed: 无
**Table Updated: 无
**Input: 需要清零的数组首地址
**       需要清零的数组尾地址
**Output: 无
**Return: 无
**Others:无
**/
void ClearArrayChar(unsigned char *PTemp, unsigned char *PEnd)
{
  while (PTemp <= PEnd)
  {
    *PTemp++ = 0;
  }
}

/**
**Function: ClearArrayInt
**Description: 清空数据
**Calls:   无
**Called By: main.c
**Table Accessed: 无
**Table Updated: 无
**Input: 需要清零的数组首地址
**       需要清零的数组尾地址
**Output: 无
**Return: 无
**Others:无
**/
void ClearArrayInt(int *PTemp, int *PEnd)
{
  while (PTemp <= PEnd)
  {
    *PTemp++ = 0;
  }
}

/**
**Function: ClearArrayByte
**Description: 清空数据
**Calls:   无
**Called By: main.c
**Table Accessed: 无
**Table Updated: 无
**Input: 需要清零的数组首地址
**       需要清零的数组尾地址
**Output: 无
**Return: 无
**Others:无
**/
void ClearArrayByte(byte *PTemp, byte *PEnd)
{
  while (PTemp <= PEnd)
  {
    *PTemp++ = 0;
  }
}

/**
**Function: SendSD
**Description: SD卡写参数
**Calls:   无
**Called By: main.c
**Table Accessed: 无
**Table Updated: 无
**Input:  
**Output: 无
**Return: 无
**Others:无
**/
void SendSD(void)           //传参数信息
{
  int Num=26,SN=0;
        
  SN=((Num+1)*25-ROW*6);     //Parameters(63/2=31)
  
  /*
  **方向部分参量(方向相关)
  **0、信标行坐标
  **1、信标列坐标
  **2、方向偏差(e)
  **3、方向偏差变化(ec)
  **4、期望列号
  **5、方向P
  **6、方向D
  **7、舵机PWM
  */
  guImgBuff[ROW+Num][NUM-SN]=((int)(giBeacon_Row))>>8;
  guImgBuff[ROW+Num][NUM-SN+1]=((int)(giBeacon_Col))>>8;
  guImgBuff[ROW+Num][NUM-SN+2]=((int)(gfRoadSitError[0]+200))>>8;
  guImgBuff[ROW+Num][NUM-SN+3]=((int)((gfRoadSitError[0]-gfRoadSitError[2])+200))>>8;
  guImgBuff[ROW+Num][NUM-SN+4]=((int)(giExpectCOL))>>8;
  guImgBuff[ROW+Num][NUM-SN+5]=((int)(gs_Derict_fuzzyPID.fuzzyP))>>8;
  guImgBuff[ROW+Num][NUM-SN+6]=((int)(gs_Derict_fuzzyPID.fuzzyD))>>8;
  guImgBuff[ROW+Num][NUM-SN+7]=((int)(gfDirection_Pwm+1000))>>8;
  
  guImgBuff[ROW+Num][NUM-SN+20]=((int)(giBeacon_Row))&0xff;
  guImgBuff[ROW+Num][NUM-SN+21]=((int)(giBeacon_Col))&0xff;
  guImgBuff[ROW+Num][NUM-SN+22]=((int)(gfRoadSitError[0]+200))&0xff;
  guImgBuff[ROW+Num][NUM-SN+23]=((int)((gfRoadSitError[0]-gfRoadSitError[2])+200))&0xff;
  guImgBuff[ROW+Num][NUM-SN+24]=((int)(giExpectCOL))&0xff;
  guImgBuff[ROW+Num][NUM-SN+25]=((int)(gs_Derict_fuzzyPID.fuzzyP))&0xff;
  guImgBuff[ROW+Num][NUM-SN+26]=((int)(gs_Derict_fuzzyPID.fuzzyD))&0xff;
  guImgBuff[ROW+Num][NUM-SN+27]=((int)(gfDirection_Pwm+1000))&0xff;
  
  /*
  **速度部分参量(速度相关)
  **8、期望速度
  **9、实际速度
  **10、速度P
  **11、速度I
  **12、速度PWM
  **13、正反转方向
  */
  guImgBuff[ROW+Num][NUM-SN+8]=((int)(gfDesirSpeed))>>8;
  guImgBuff[ROW+Num][NUM-SN+9]=((int)(giSpeedWheel))>>8;
  guImgBuff[ROW+Num][NUM-SN+10]=((int)(gfGyroIntegral))>>8;
  guImgBuff[ROW+Num][NUM-SN+11]=((int)(guSidePass*100))>>8;
  guImgBuff[ROW+Num][NUM-SN+12]=((int)(gfSpeedPWM))>>8;      
  guImgBuff[ROW+Num][NUM-SN+13]=((int)(MotorStree))>>8;//giGyro_Z
  
  guImgBuff[ROW+Num][NUM-SN+28]=((int)(gfDesirSpeed))&0xff;
  guImgBuff[ROW+Num][NUM-SN+29]=((int)(giSpeedWheel))&0xff;
  guImgBuff[ROW+Num][NUM-SN+30]=((int)(gfGyroIntegral))&0xff;
  guImgBuff[ROW+Num][NUM-SN+31]=((int)(guSidePass*100))&0xff;
  guImgBuff[ROW+Num][NUM-SN+32]=((int)(gfSpeedPWM))&0xff;      
  guImgBuff[ROW+Num][NUM-SN+33]=((int)(MotorStree))&0xff;//giGyro_Z
  
  /*
  **其他部分参量(标志位)
  **14、超声波当前距离
  **15、路障标志
  **16、路障米数
  **17、路障偏移量
  **18、陀螺仪积分角度
  **19、信标计数
  */
  guImgBuff[ROW+Num][NUM-SN+14]=((int)(Ultrasonic_Dis[0]))>>8;
  guImgBuff[ROW+Num][NUM-SN+15]=((int)(guRoadBlock*100))>>8;
  guImgBuff[ROW+Num][NUM-SN+16]=((int)(guCirclePass))>>8;
  guImgBuff[ROW+Num][NUM-SN+17]=((int)(giTurnBack))>>8;
  guImgBuff[ROW+Num][NUM-SN+18]=((int)(gs_Beacon[0].Row_Bottom))>>8;      
  guImgBuff[ROW+Num][NUM-SN+19]=((int)(guBeaconCount))>>8;
  
  guImgBuff[ROW+Num][NUM-SN+34]=((int)(Ultrasonic_Dis[0]))&0xff;
  guImgBuff[ROW+Num][NUM-SN+35]=((int)(guRoadBlock*100))&0xff;
  guImgBuff[ROW+Num][NUM-SN+36]=((int)(guCirclePass))&0xff;
  guImgBuff[ROW+Num][NUM-SN+37]=((int)(giTurnBack))&0xff;
  guImgBuff[ROW+Num][NUM-SN+38]=((int)(gs_Beacon[0].Row_Bottom))&0xff;      
  guImgBuff[ROW+Num][NUM-SN+39]=((int)(guBeaconCount))&0xff;
  
  /*
  **附加参量参量(变量)
  **40、超声波当前距离
  **42、路障标志
  **44、路障米数
  **46、路障偏移量
  **48、陀螺仪积分角度
  **50、信标计数
  **52、超声波当前距离
  **54、路障标志
  **56、路障米数
  **58、路障偏移量
  */
  guImgBuff[ROW+Num][NUM-SN+40]=((int)(giDirectStraight))>>8;   //parameter 21
  guImgBuff[ROW+Num][NUM-SN+41]=((int)(giDirectStraight))&0xff;
                                  
  guImgBuff[ROW+Num][NUM-SN+42]=((int)(guSidePass))>>8;         //parameter 22
  guImgBuff[ROW+Num][NUM-SN+43]=((int)(guSidePass))&0xff;
  
  guImgBuff[ROW+Num][NUM-SN+44]=((int)(giRoadBlockOffset))>>8;  //parameter 23
  guImgBuff[ROW+Num][NUM-SN+45]=((int)(giRoadBlockOffset))&0xff;
  
  guImgBuff[ROW+Num][NUM-SN+46]=((int)(giRoadBlockDis))>>8;  //parameter 24
  guImgBuff[ROW+Num][NUM-SN+47]=((int)(giRoadBlockDis))&0xff;
  
  guImgBuff[ROW+Num][NUM-SN+48]=((int)(giMeter_Sum_RoadBlock))>>8;      //parameter 25
  guImgBuff[ROW+Num][NUM-SN+49]=((int)(giMeter_Sum_RoadBlock))&0xff;
  
  guImgBuff[ROW+Num][NUM-SN+50]=((int)(0))>>8;                  //parameter 26
  guImgBuff[ROW+Num][NUM-SN+51]=((int)(0))&0xff;
  
  guImgBuff[ROW+Num][NUM-SN+52]=((int)(0))>>8;                  //parameter 27
  guImgBuff[ROW+Num][NUM-SN+53]=((int)(0))&0xff;                                
                                  
  guImgBuff[ROW+Num][NUM-SN+54]=((int)(0))>>8;                  //parameter 28
  guImgBuff[ROW+Num][NUM-SN+55]=((int)(0))&0xff;
  
  guImgBuff[ROW+Num][NUM-SN+56]=((int)(0))>>8;                  //parameter 29
  guImgBuff[ROW+Num][NUM-SN+57]=((int)(0))&0xff;
  
  guImgBuff[ROW+Num][NUM-SN+58]=((int)(0))>>8;                  //parameter 30
  guImgBuff[ROW+Num][NUM-SN+59]=((int)(0))&0xff; 
 }


/**
**Function: GetSpeed
**Description: DMA测速,速度滤波
**Calls:   无
**Called By: isr.c
**Table Accessed: 无
**Table Updated: 无
**Input: 无
**Output: 无
**Return: 无
**Others:无
**/
void GetSpeed(void)
{
  int i=0;
  int liSpeedArray[5]={0};
  int liSpeedArrayModify[3]={0};
  int liSpeedTempMax=0;
  int liSpeedTempMin=0;
  int liTempMaxFlag=0;
  int liTempMinFlag=0;
  int *PTemp_Array=&liSpeedArray[0];
  int *PTemp_Array_Modify=&liSpeedArrayModify[2];
  int *PTemp_Smoot=&giSpeedSmooth[0];
  
  //giSpeedSmooth[0]=GetQUAD();//当前速度获取
  giSpeedSmooth[0]=DMA_count_get(DMA_CH1);//获取速度
  DMA_count_reset(DMA_CH1);//计数清零
  DMA_count_reset(DMA_CH1);//计数清零*/
  
  //速度滤波(Speed Filter)
  
  //中位加权平均递推滤波
  //数组复制
  for(PTemp_Array=&liSpeedArray[0],PTemp_Smoot=&giSpeedSmooth[0],i=5;i>0;i--)
  {
    *PTemp_Array++ = *PTemp_Smoot++;
  }
  
  liSpeedTempMax=liSpeedTempMin=liSpeedArray[0];//最大最小值变量
  
  //找出5次的最大最小值
  for(PTemp_Array=&liSpeedArray[0];PTemp_Array<=&liSpeedArray[4];PTemp_Array++)
  {
    if(liSpeedTempMax < *PTemp_Array)
    {
      liSpeedTempMax = *PTemp_Array;
    }
    
    if(liSpeedTempMin > *PTemp_Array)
    {
      liSpeedTempMin = *PTemp_Array;
    }
  }
  
  //从数组中次序取出除了最大最小值的3次速度值
  for(PTemp_Array=&liSpeedArray[4],PTemp_Array_Modify=&liSpeedArrayModify[2];PTemp_Array>=&liSpeedArray[0];PTemp_Array--)
  {
    if(*PTemp_Array == liSpeedTempMax && liTempMaxFlag == 0)
    {    
      liTempMaxFlag = 1;
    }
    
    else if(*PTemp_Array == liSpeedTempMin && liTempMinFlag == 0)
    {
      liTempMinFlag = 1;
    }
    
    else if(PTemp_Array_Modify >= &liSpeedArrayModify[0])
    {
      *PTemp_Array_Modify = *PTemp_Array;
      PTemp_Array_Modify--;
    }
  }
  
  //加权平均滤波,一阶低通滤波
  gfSpeed_Filter = 0.60 * (0.50 * liSpeedArrayModify[0] + 0.34 * liSpeedArrayModify[1] + 0.16 * liSpeedArrayModify[2]) + 0.40 * gfSpeed_Filter_Old;
  giSpeedWheel = gfSpeed_Filter;
  gfSpeed_Filter_Old = gfSpeed_Filter;//上次速度滤波值
    
  for(i=4,PTemp_Smoot=&giSpeedSmooth[0];i>0;i--)//history
  {
    *(PTemp_Smoot + i) = *(PTemp_Smoot + i - 1);
  }
}

/**
**Function: Ultrasonic_Trig
**Description: 超声波触发
**Calls:   无
**Called By: isr.c
**Table Accessed: 无
**Table Updated: 无
**Input: 无
**Output: 无
**Return: 无
**Others:无
**/
void Ultrasonic_Trig(void)
{
  int i=0;
  
  /*超声波触发*/
  Trig_H;
  Trig2_H;
  for(i=0;i<400;i++);
  Trig_L;
  Trig2_L;
  
  /*计时器装载*/
  PIT_TCTRL1  &= ~PIT_TCTRL_TEN_MASK;       //停止定时器
  PIT_CVAL1  = 0xFFFFFFFF;                  //32位定时器，装最大值即可 
}

/**
**Function: FindRoadBlock
**Description: 路障检测
**Calls:   无
**Called By: isr.c
**Table Accessed: 无
**Table Updated: 无
**Input: 无
**Output: 无
**Return: 无
**Others:无
**/
void FindRoadBlock(void)
{
  int i = 0;
  unsigned int *PTemp = &Ultrasonic_Dis[0];
  
  if(guRoadBlock == 0 && guSidePass == 0 && giTurnBack == 0 
      && giLostBeacon == 0 && giDirectStraight == 1 && giDp != 0)
  {
    if(Ultrasonic_Dis[0] <= UltrasonicDistance 
        && Ultrasonic_Dis[1] <= UltrasonicDistance
          && Ultrasonic_Dis[2] <= UltrasonicDistance
            //&& Ultrasonic_Dis[3] <= UltrasonicDistance
              && ((Ultrasonic_Dis[0] < Ultrasonic_Dis[1] && (Ultrasonic_Dis[1] - Ultrasonic_Dis[0]) < Interval)
                    &&(Ultrasonic_Dis[1] < Ultrasonic_Dis[2] && (Ultrasonic_Dis[2] - Ultrasonic_Dis[1]) < Interval)
                      &&(Ultrasonic_Dis[0] < Ultrasonic_Dis[2] && (Ultrasonic_Dis[2] - Ultrasonic_Dis[0]) < 2*Interval)
                        //&&(Ultrasonic_Dis[0] < Ultrasonic_Dis[3] && (Ultrasonic_Dis[3] - Ultrasonic_Dis[0]) < 3*Interval)
                  )
                && Ultrasonic_Dis[0] > 30
                  && Ultrasonic_Dis[1] > 30
                    && Ultrasonic_Dis[2] > 30
                      //&& Ultrasonic_Dis[3] > 30
                        && giCurrentSpeed > 30
        )
    {
      guRoadBlock = 1;
    }
  }
  
  if(guRoadBlock == 1)
  {
    //Buzzer_noise;
  }
  else
  {
    //Buzzer_quite;
  }
  
  for(i=5,PTemp=&Ultrasonic_Dis[0];i>0;i--)
  {
    *(PTemp+i)=*(PTemp+i-1);
  }
}

/**
**Function: FlashMemory
**Description: 数据掉电不丢失，按‘.’返回上次的值。
**Calls: isr.c
**Called By: 
**Output: 无
**Return: 无
**Others: 无
**/
int times = 0;

int Parameterdistinction(int para)                  
{
  uint16 sectorNo = 0,offset = 0;
  int n=0;
  
  sectorNo = 255 - times;
  
  if(ModelChoice == 0)/*键盘输入模式*/
  {
    if(para != -2)/*输入正常参数*/
    {
      para = (uint32)para;
      flash_erase_sector(sectorNo);
      flash_write(sectorNo,offset,para);
      n = para; 
    }
    
    else/*输入小数点'.'*/
    {
      n = (int)(*(uint32 *)((uint32)(((sectorNo)<<11) + (offset))));
    }
  }
  
  else/*跳过键盘模式*/
  {
    n = (int)(*(uint32 *)((uint32)(((sectorNo)<<11) + (offset))));
  }
  
  times++;
  
  return n;
}

void Parametershoutcut(void)
{
  if(ModelChoice == 0)/*键盘输入模式*/
  {
    gfDesirSpeed_High = Parameterdistinction(gfDesirSpeed_High);
    gfDesirSpeed_Mid = Parameterdistinction(gfDesirSpeed_Mid);
    gfDesirSpeed_Low = Parameterdistinction(gfDesirSpeed_Low);
    
    giSp = Parameterdistinction(giSp);
    giSi = Parameterdistinction(giSi);
    //giSd = Parameterdistinction(giSd);
    
    giDp = Parameterdistinction(giDp);
    //giDi = Parameterdistinction(giDi);
    giDd = Parameterdistinction(giDd);
    //giDk = Parameterdistinction(giDk);

    SDTIME = Parameterdistinction(SDTIME);
    giBeaconExcept = Parameterdistinction(giBeaconExcept);
    //giMeterSetValue = Parameterdistinction(giMeterSetValue);
    
    giBeaconOrder1 = Parameterdistinction(giBeaconOrder1);
    giBeaconOrder2 = Parameterdistinction(giBeaconOrder2);
    giBeaconOrder3 = Parameterdistinction(giBeaconOrder3);
    //giBeaconSpeed1 = Parameterdistinction(giBeaconSpeed1);
    //giBeaconSpeed2 = Parameterdistinction(giBeaconSpeed2);
    //giBeaconSpeed3 = Parameterdistinction(giBeaconSpeed3);
  }
  
  else/*跳过键盘模式*/
  {
    gfDesirSpeed_High = (int)(*(uint32 *)((uint32)((255 - times++)<<11)));
    gfDesirSpeed_Mid  = (int)(*(uint32 *)((uint32)((255 - times++)<<11)));
    gfDesirSpeed_Low  = (int)(*(uint32 *)((uint32)((255 - times++)<<11)));
    
    giSp              = (int)(*(uint32 *)((uint32)((255 - times++)<<11)));
    giSi              = (int)(*(uint32 *)((uint32)((255 - times++)<<11)));
    //giSd              = (int)(*(uint32 *)((uint32)((255 - times++)<<11)));
    
    giDp              = (int)(*(uint32 *)((uint32)((255 - times++)<<11)));
    //giDi              = (int)(*(uint32 *)((uint32)((255 - times++)<<11)));
    giDd              = (int)(*(uint32 *)((uint32)((255 - times++)<<11)));
    //giDk              = (int)(*(uint32 *)((uint32)((255 - times++)<<11)));

    SDTIME            = (int)(*(uint32 *)((uint32)((255 - times++)<<11)));
    giBeaconExcept    = (int)(*(uint32 *)((uint32)((255 - times++)<<11)));
    //giMeterSetValue   = (int)(*(uint32 *)((uint32)((255 - times++)<<11)));
    
    giBeaconOrder1    = (int)(*(uint32 *)((uint32)((255 - times++)<<11)));
    giBeaconOrder2    = (int)(*(uint32 *)((uint32)((255 - times++)<<11)));
    giBeaconOrder3    = (int)(*(uint32 *)((uint32)((255 - times++)<<11)));
    //giBeaconSpeed1    = (int)(*(uint32 *)((uint32)((255 - times++)<<11)));
    //giBeaconSpeed2    = (int)(*(uint32 *)((uint32)((255 - times++)<<11)));
    //giBeaconSpeed3    = (int)(*(uint32 *)((uint32)((255 - times++)<<11)));
  }

  times = 0;
  FTFL_FPROT0 = 0x7Fu;
}

/**
**Function: BeaconClock
**Description: 信标计时器
**Calls:   无
**Called By: isr.c
**Table Accessed: 无
**Table Updated: 无
**Input: 无
**Output: 无
**Return: 无
**Others:无
**/
void BeaconClock(void)
{
  
  /*小车起跑前计数清零*/
  if(Switch_On && (StartCount < 200 || BeaconStart == 0) && (giBeaconExcept == 0 || guBeaconCount < giBeaconExcept))
  {
    guBeaconTimer = 0;
    gfAllMeter = 0;
    gfAllTime = 0;
    gfRealSpeed = 0;
    giMeter_Sum = 0;
  }
  
  /*小车起跑时候开始计数*/
  else if(Switch_On && StartCount >= 200 && BeaconStart == 1 && (giBeaconExcept == 0 || guBeaconCount < giBeaconExcept))
  {
    guBeaconTimer++;
    gfRealSpeed = gfAllMeter / gfAllTime;
  }
  
  else if(Switch_Off || (giBeaconExcept != 0 && guBeaconCount == giBeaconExcept))
  {
    guBeaconTimer = guBeaconTimer;
    gfAllMeter = gfAllMeter;
    gfRealSpeed = gfRealSpeed;
  }
  
  /*30个场中断计时1s*/
  if(guBeaconTimer > 30)
  {
    guBeaconTimer = 0;
    gfAllTime++;
  }
  
  /*超过120s计为120s*/
  if(gfAllTime > 120)
  {
    gfAllTime = 120;
  }
  
  gfAllTime = (int)gfAllTime + guBeaconTimer * 0.033;
  
}
