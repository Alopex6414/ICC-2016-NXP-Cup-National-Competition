/*********************Copyright(C) 2015-2016 TYUT JBD TEAM B.*******************
Copyright (C), 2015-2016, TYUT JBD TEAM B.
File name: func.c 
Author: TEAM B, Alopex
Version: 1.0               
Date: 2015.10.28(E)
      2015.12.31(C)
      2016.03.15(A)
Description: 
            Definition Function(���幦�ܺ���)
            Keyboard Display Function (������ʾ����)
            UART transform Function(���ڷ��ͺ���)
            TF Card Send Parameters Function(TF�����Ͳ�������)
            GetSpeed Function(��ȡ�����ٶȺ���)
            Clear Array(������麯��)
            UltrasonicTrig(��������������)
            Beacon Clock(�ű��ʱ��)
            Parameters Flash(flash�洢����)
Others: Noting
Function List:
            1.ThousandToSingleUp ������ʾ���κ���
            2.ParameterSent 3-6�������ڷ��ͺ���
            3.Calculate ������Ϣ���㺯��
            4.Getspeed ��ȡ��ǰ�����ٶ�
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

/*
**������ʾǧ��ʮ��λ
*/
int ThousandUp = 0;
int HundredUp = 0;
int TenUp = 0;  
int SingleUp = 0;                   //����

int ThousandDown = 0;
int HundredDown = 0;
int TenDown = 0;
int SingleDown = 0;                 //����

/*
**���ڷ��ͺ���
*/
float gfSendWhite = 0;              //���ڷ��ͱ�����ɫ����
float gfSendRed = 0;                //���ڷ��ͱ�����ɫ����
float gfSendGreen = 0;              //���ڷ��ͱ�����ɫ����
float gfSendBlue = 0;               //���ڷ��ͱ�����ɫ����

/*
**SD������ʱ��
*/
int SDTIME=0;
int SD_Record = 0;//SD����¼��ɱ�־

/*
**���ܼ���
*/
int StartCount = 0;

/*
**�������
*/
volatile unsigned char guInfrared_front = 0;//ǰ��������չ�

/*
**BeaconClock
*/
int giBeaconExcept = 0;//�����ű����,��������ͣ��
float gfAllMeter = 0;//��·��
float gfAllTime = 0;//��ʱ��
float gfRealSpeed = 0;//ƽ���ٶ�
unsigned char guBeaconTimer = 0;//��ʱ��

/*
**DMA����
*/
int giSpeedWheel=0;                    //�����ٶ�
int giCurrentSpeed=0;                  //��ǰ�ٶ�
int giSpeedSmooth[5]={0};              //�ٶ��˲�
float gfSpeed_Filter=0;                //��Ȩ�˲��ٶ�
float gfSpeed_Filter_Old=0;            //ǰһ�μ�Ȩ�˲��ٶ�

/*
**Function: ThousandToSingleUp ThousandToSingleDown
**Description: ������ʾǧ��ʮ��λ����
**Calls:   ��
**Called By: 
**Table Accessed: ��
**Table Updated: ��
**Input:  value ��Ҫ��ʾ�ı���
**Output: ��
**Return: ��
**Others:��
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
**Description: ���ں���
**Calls:   ��
**Called By: main.c
**Table Accessed: ��
**Table Updated: ��
**Input:  gfSendWhite
**        gfSendRed
**        gfSendGreen
**        gfSendBlue
**Output: ��
**Return: ��
**Others:��
**/
void ParameterSent(void)
{
    signed int temp;
    unsigned char temp1, temp2;
    
    uart_putchar(UART_PORT_USR,0xff);
    uart_putchar(UART_PORT_USR,0xff);
    
    /*�ٶȷ���*/
    /*gfSendWhite = giBeacon_Row;
    gfSendRed   = giBeacon_Col;
    gfSendGreen = giExpectCOL;*/
    //gfSendBlue  = giSpeedWheelR;
      
    /*��ɫ����(1)*/
    temp = (signed int)gfSendWhite;  
    temp1 = (temp) >> 8;
    temp2 = (temp)&0x00ff;
    uart_putchar(UART_PORT_USR, temp1);
    uart_putchar(UART_PORT_USR, temp2);
    
    /*��ɫ����(2)*/
    temp = (signed int)gfSendRed; 
    temp1 = (temp) >> 8;
    temp2 = (temp)&0x00ff;
    uart_putchar(UART_PORT_USR, temp1);
    uart_putchar(UART_PORT_USR, temp2);
    
    /*��ɫ����(3)*/
    temp = (signed int)gfSendGreen;        
    temp1 = (temp) >> 8;
    temp2 = (temp)&0x00ff;
    uart_putchar(UART_PORT_USR, temp1);
    uart_putchar(UART_PORT_USR, temp2);

    /*��ɫ����(4)*/
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
**Description: ��̬��λ�����ͺ���
**Calls:   ��
**Called By: main.c
**Table Accessed: ��
**Table Updated: ��
**Input: guBuff
**Output: ��
**Return: ��
**Others:��
**/
void SendPIEX(void)           //������������Ϣ
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
**Description: ��̬��λ�����ͺ���
**Calls:   ��
**Called By: main.c
**Table Accessed: ��
**Table Updated: ��
**Input:  guBuff
**        giRoadEdgeL
**        giRoadEdgeR 
**        giRoadLocation
**Output: ��
**Return: ��
**Others:��
**/
void SendPIEX1(void)           //��������Ϣ
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
**Description: �������
**Calls:   ��
**Called By: main.c
**Table Accessed: ��
**Table Updated: ��
**Input: ��Ҫ����������׵�ַ
**       ��Ҫ���������β��ַ
**Output: ��
**Return: ��
**Others:��
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
**Description: �������
**Calls:   ��
**Called By: main.c
**Table Accessed: ��
**Table Updated: ��
**Input: ��Ҫ����������׵�ַ
**       ��Ҫ���������β��ַ
**Output: ��
**Return: ��
**Others:��
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
**Description: �������
**Calls:   ��
**Called By: main.c
**Table Accessed: ��
**Table Updated: ��
**Input: ��Ҫ����������׵�ַ
**       ��Ҫ���������β��ַ
**Output: ��
**Return: ��
**Others:��
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
**Description: SD��д����
**Calls:   ��
**Called By: main.c
**Table Accessed: ��
**Table Updated: ��
**Input:  
**Output: ��
**Return: ��
**Others:��
**/
void SendSD(void)           //��������Ϣ
{
  int Num=26,SN=0;
        
  SN=((Num+1)*25-ROW*6);     //Parameters(63/2=31)
  
  /*
  **���򲿷ֲ���(�������)
  **0���ű�������
  **1���ű�������
  **2������ƫ��(e)
  **3������ƫ��仯(ec)
  **4�������к�
  **5������P
  **6������D
  **7�����PWM
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
  **�ٶȲ��ֲ���(�ٶ����)
  **8�������ٶ�
  **9��ʵ���ٶ�
  **10���ٶ�P
  **11���ٶ�I
  **12���ٶ�PWM
  **13������ת����
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
  **�������ֲ���(��־λ)
  **14����������ǰ����
  **15��·�ϱ�־
  **16��·������
  **17��·��ƫ����
  **18�������ǻ��ֽǶ�
  **19���ű����
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
  **���Ӳ�������(����)
  **40����������ǰ����
  **42��·�ϱ�־
  **44��·������
  **46��·��ƫ����
  **48�������ǻ��ֽǶ�
  **50���ű����
  **52����������ǰ����
  **54��·�ϱ�־
  **56��·������
  **58��·��ƫ����
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
**Description: DMA����,�ٶ��˲�
**Calls:   ��
**Called By: isr.c
**Table Accessed: ��
**Table Updated: ��
**Input: ��
**Output: ��
**Return: ��
**Others:��
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
  
  //giSpeedSmooth[0]=GetQUAD();//��ǰ�ٶȻ�ȡ
  giSpeedSmooth[0]=DMA_count_get(DMA_CH1);//��ȡ�ٶ�
  DMA_count_reset(DMA_CH1);//��������
  DMA_count_reset(DMA_CH1);//��������*/
  
  //�ٶ��˲�(Speed Filter)
  
  //��λ��Ȩƽ�������˲�
  //���鸴��
  for(PTemp_Array=&liSpeedArray[0],PTemp_Smoot=&giSpeedSmooth[0],i=5;i>0;i--)
  {
    *PTemp_Array++ = *PTemp_Smoot++;
  }
  
  liSpeedTempMax=liSpeedTempMin=liSpeedArray[0];//�����Сֵ����
  
  //�ҳ�5�ε������Сֵ
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
  
  //�������д���ȡ�����������Сֵ��3���ٶ�ֵ
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
  
  //��Ȩƽ���˲�,һ�׵�ͨ�˲�
  gfSpeed_Filter = 0.60 * (0.50 * liSpeedArrayModify[0] + 0.34 * liSpeedArrayModify[1] + 0.16 * liSpeedArrayModify[2]) + 0.40 * gfSpeed_Filter_Old;
  giSpeedWheel = gfSpeed_Filter;
  gfSpeed_Filter_Old = gfSpeed_Filter;//�ϴ��ٶ��˲�ֵ
    
  for(i=4,PTemp_Smoot=&giSpeedSmooth[0];i>0;i--)//history
  {
    *(PTemp_Smoot + i) = *(PTemp_Smoot + i - 1);
  }
}

/**
**Function: Ultrasonic_Trig
**Description: ����������
**Calls:   ��
**Called By: isr.c
**Table Accessed: ��
**Table Updated: ��
**Input: ��
**Output: ��
**Return: ��
**Others:��
**/
void Ultrasonic_Trig(void)
{
  int i=0;
  
  /*����������*/
  Trig_H;
  Trig2_H;
  for(i=0;i<400;i++);
  Trig_L;
  Trig2_L;
  
  /*��ʱ��װ��*/
  PIT_TCTRL1  &= ~PIT_TCTRL_TEN_MASK;       //ֹͣ��ʱ��
  PIT_CVAL1  = 0xFFFFFFFF;                  //32λ��ʱ����װ���ֵ���� 
}

/**
**Function: FindRoadBlock
**Description: ·�ϼ��
**Calls:   ��
**Called By: isr.c
**Table Accessed: ��
**Table Updated: ��
**Input: ��
**Output: ��
**Return: ��
**Others:��
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
**Description: ���ݵ��粻��ʧ������.�������ϴε�ֵ��
**Calls: isr.c
**Called By: 
**Output: ��
**Return: ��
**Others: ��
**/
int times = 0;

int Parameterdistinction(int para)                  
{
  uint16 sectorNo = 0,offset = 0;
  int n=0;
  
  sectorNo = 255 - times;
  
  if(ModelChoice == 0)/*��������ģʽ*/
  {
    if(para != -2)/*������������*/
    {
      para = (uint32)para;
      flash_erase_sector(sectorNo);
      flash_write(sectorNo,offset,para);
      n = para; 
    }
    
    else/*����С����'.'*/
    {
      n = (int)(*(uint32 *)((uint32)(((sectorNo)<<11) + (offset))));
    }
  }
  
  else/*��������ģʽ*/
  {
    n = (int)(*(uint32 *)((uint32)(((sectorNo)<<11) + (offset))));
  }
  
  times++;
  
  return n;
}

void Parametershoutcut(void)
{
  if(ModelChoice == 0)/*��������ģʽ*/
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
  
  else/*��������ģʽ*/
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
**Description: �ű��ʱ��
**Calls:   ��
**Called By: isr.c
**Table Accessed: ��
**Table Updated: ��
**Input: ��
**Output: ��
**Return: ��
**Others:��
**/
void BeaconClock(void)
{
  
  /*С������ǰ��������*/
  if(Switch_On && (StartCount < 200 || BeaconStart == 0) && (giBeaconExcept == 0 || guBeaconCount < giBeaconExcept))
  {
    guBeaconTimer = 0;
    gfAllMeter = 0;
    gfAllTime = 0;
    gfRealSpeed = 0;
    giMeter_Sum = 0;
  }
  
  /*С������ʱ��ʼ����*/
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
  
  /*30�����жϼ�ʱ1s*/
  if(guBeaconTimer > 30)
  {
    guBeaconTimer = 0;
    gfAllTime++;
  }
  
  /*����120s��Ϊ120s*/
  if(gfAllTime > 120)
  {
    gfAllTime = 120;
  }
  
  gfAllTime = (int)gfAllTime + guBeaconTimer * 0.033;
  
}
