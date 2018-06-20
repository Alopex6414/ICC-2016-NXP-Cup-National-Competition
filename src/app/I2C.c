/*********************Copyright(C) 2015-2016 TYUT JBD TEAM B.*******************
Copyright (C), 2015-2016, TYUT JBD TEAM B.
File name: I2C.c 
Author: TEAM B, Alopex
Version: 1.0               
Date: 2015.10.28(E)
      2015.12.31(C)
      2016.03.15(A)
Description: 
                1.I2C Communication Protocol(I2C通信协议)
                2.MPU6050 Gyroscope & Accelerometer(MPU6050陀螺仪和加速度计)
                3.MMA8451 Accelerometer(MMA8451加速度计)
Others: Noting
Function List:  1.I2C Communication Protocol(I2C通信协议)
                2.MPU6050 Gyroscope & Accelerometer(MPU6050陀螺仪和加速度计)
                3.MMA8451 Accelerometer(MMA8451加速度计)
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

/**
**Function: IIC
**Description: IIC通信获取陀螺仪与加速度计的值
**Calls: getinfor.c
**Called By: 
**Output: 无
**Return: 无
**Others: 无
**/
char ack=0;                     //应答信号
int CY=0;                       //数据接收
int giAcc_X=0;                  //X轴加速度
int giAcc_Y=0;                  //Y轴加速度
int giAcc_Z=0;                  //Z轴加速度
int giGyro_X=0;                 //X轴角速度
int giGyro_Y=0;                 //Y轴角速度
int giGyro_Z=0;                 //Z轴角速度

/*
**IIC延时测试
*/
void IIC_TestDelay(void)
{  
    asm("nop");//1//周期0.2ms,延时0.01us//1
    asm("nop");//2
    asm("nop");//3
    asm("nop");//4
    asm("nop");//5
    asm("nop");//6
    asm("nop");//7
    asm("nop");//8
    asm("nop");//9
    asm("nop");//10
}
/*
**IIC起始信号
*/
void IIC_Start(void)
{
    //SCLout;
    SDAout;		
    SDA_H_;                     //拉高SDA
    SCL_H_;                     //拉高SCL
    I2C_Delay;                  //I2C延时
    SDA_L_;                     //拉低SDA
    I2C_Delay;                  //I2C延时
    SCL_L_;                     //拉低SCL
}

/*
**IIC停止信号
*/
void IIC_Stop(void)
{
    //SCLout;
    SDAout;		
    SDA_L_;                     //拉低SDA
    SCL_H_;                     //拉高SCL
    I2C_Delay;                  //I2C延时
    SDA_H_;                     //拉高SDA
    I2C_Delay;                  //I2C延时
}

/*
**应答信号
*/
void IIC_SendACK(int ack)
{
    //SCLout;
    SDAout;
    if(ack==0)
    {
    	SDA_L_;                 //拉低SDA
    }
    else
    {
    	SDA_H_;                 //拉高SDA
    }
    SCL_H_;                     //拉高SCL
    I2C_Delay;                  //I2C延时
    SCL_L_;                     //拉低SCL
    I2C_Delay;                  //I2C延时
}

/*
**接收应答信号
*/
uchar IIC_RecvACK(void)
{
    SDAin;
    //SCLout;
    SCL_H_;                    //拉高时钟线
    I2C_Delay;
    CY = SDAread;              //读应答信号
    SCL_L_;                    //拉低时钟线
    I2C_Delay;
    return CY;
}

/*
**向IIC总线发送数据
*/
uchar IIC_SendByte(uchar data)
{                
    uchar i;
    //SCLout;
    SDAout;	
    for (i=0; i<8; i++)         //8位计数器
    {   
        if((data<<i)&0x80)
       	{
        	SDA_H_;	
       	}
        else
        {
        	SDA_L_;
        }
        
        I2C_Delay;
        SCL_H_;
        I2C_Delay;
        SCL_L_;
        I2C_Delay;
    }
   
    I2C_Delay;
    SDA_H_;
    I2C_Delay;
    SCL_H_;
    I2C_Delay;
    SDAin;
    if(SDAread==1)
    ack=0;
    else ack=1;
    SCL_L_;
    I2C_Delay;
    return data;
}

/*
**从IIC总线接收数据
*/
uchar IIC_RecvByte(void)
{
    uchar i;
    uchar dat = 0;
    //SCLout;
    SDAout;
	
    SDA_H_;
    SDAin;                     //使能内部上拉,准备读取数据,
    for (i=0; i<8; i++)        //8位计数器
    {
        dat <<= 1;
        SCL_H_;                //拉高时钟线
        I2C_Delay;
        dat |= SDAread;        //读数据               
        SCL_L_;                //拉低时钟线
        I2C_Delay;
    }
    return dat;
}

/*
**写IIC设备寄存器
*/
void IIC_WriteReg(uint8 dev_addr,uint8 reg_addr , uint8 data)
{		
    IIC_Start();                     //起始信号
    IIC_SendByte(dev_addr);          //发送设备地址+写信号     
    IIC_SendByte(reg_addr);          //内部寄存器地址
    IIC_SendByte(data);              //内部寄存器数据
    IIC_Stop();                      //发送停止信号
}

/*
**读IIC设备寄存器
*/
uchar IIC_ReadByte(uint8 dev_addr,uint8 reg_addr)
{  
    uchar data;
    IIC_Start();                      //起始信号
    IIC_SendByte(dev_addr);           //发送设备地址+写信号
    IIC_SendByte(reg_addr);           //发送存储单元地址，从0开始	
    IIC_Start();                      //起始信号
    IIC_SendByte(dev_addr+1);         //发送设备地址+读信号
    data=IIC_RecvByte();              //读出寄存器数据
    IIC_SendACK(1);   
    IIC_Stop();                       //停止信号
    return data; 
}

int16 IIC_ReadWord(uint8 dev_addr,uint8 reg_addr)
{
    int16 h,l;
    IIC_Start();
    IIC_SendByte(dev_addr); 
    IIC_SendByte(reg_addr);
    IIC_Start();	
    IIC_SendByte(dev_addr+1); 
    h= IIC_RecvByte();
    l= IIC_RecvByte();
    IIC_SendACK(1);   
    IIC_Stop();
    return (h<<8)+l;
}

/*
**合成数据
*/
int SendHigh = 0;
int SendLow = 0;

int IIC_GetData(uint8 dev_addr,uint8 reg_addr)
{
    char H,L;
    H=IIC_ReadByte(dev_addr,reg_addr);
    L=IIC_ReadByte(dev_addr,reg_addr+1);
    SendHigh = H;
    SendLow = L;
    return (H<<8)+L;   //合成数据
}

/*
**获得MMA X轴加速度
*/
int16 MMA_Get_X_Acc(void)
{
    int16 temp;
    temp=IIC_GetData(IIC_MMA8451_ADR,OUT_X_MSB);
    return temp;
}

/*
**获得MMA Y轴加速度
*/
int16 MMA_Get_Y_Acc(void)
{
    int16 temp;
    temp=IIC_GetData(IIC_MMA8451_ADR,OUT_Y_MSB);
    return temp;
}

/*
**获得MMA Z轴加速度
*/
int16 MMA_Get_Z_Acc(void)
{
    int16 temp;
    temp=IIC_GetData(IIC_MMA8451_ADR,OUT_Z_MSB);
    return temp;
}

/*
**获得MPU X轴加速度
*/
int16 MPU_Get_X_Acc(void)
{
    int16 temp;
    temp=IIC_GetData(IIC_MPU6050_ADR,ACCEL_XOUT_H);
    return temp;
}

/*
**获得MPU Y轴加速度
*/
int16 MPU_Get_Y_Acc(void)
{
    int16 temp;
    temp=IIC_GetData(IIC_MPU6050_ADR,ACCEL_YOUT_H);
    return temp;
}

/*
**获得MPU Z轴加速度
*/
int16 MPU_Get_Z_Acc(void)
{
    int16 temp;
    temp=IIC_GetData(IIC_MPU6050_ADR,ACCEL_ZOUT_H);
    return temp;
}

/*
**获得MPU Z轴角速度
*/
int16 MPU_Get_Z_Gyro(void)
{
    int16 temp;
    temp=IIC_GetData(IIC_MPU6050_ADR,GYRO_ZOUT_H);
    return temp;
}

/*
**获得MPU Y轴角速度
*/
int16 MPU_Get_Y_Gyro(void)
{
    int16 temp;
    temp=IIC_GetData(IIC_MPU6050_ADR,GYRO_YOUT_H);
    return temp;
}

/*
**获得MPU X轴角速度
*/
int16 MPU_Get_X_Gyro(void)
{
    int16 temp;
    temp=IIC_GetData(IIC_MPU6050_ADR,GYRO_XOUT_H);
    return temp;
}


/*
**MMA8451初始化
*/
void MMA8451_Init(void)
{ 
    IIC_WriteReg(IIC_MMA8451_ADR,XYZ_DATA_CFG, 0x00);           //0为+-2g，1为+-4g，2为+-8g
    IIC_WriteReg(IIC_MMA8451_ADR,HP_FILTER_CUTOFF, 0x10);       // 使能低通滤波
    IIC_WriteReg(IIC_MMA8451_ADR,CTR_REG1 , 0x01);              //0x2a为控制寄存器1 激活mma8451
    IIC_WriteReg(IIC_MMA8451_ADR,CTR_REG2 , 0x02);
}

/*
**MPU6050初始化
*/
void MPU6050_Init(void)
{
   IIC_WriteReg(IIC_MPU6050_ADR,PWR_MGMT_1, 0x00);  
   IIC_WriteReg(IIC_MPU6050_ADR,SMPLRT_DIV, 0x00);   
   IIC_WriteReg(IIC_MPU6050_ADR,CONFIG, 0x00);          //0.98ms delay      
   IIC_WriteReg(IIC_MPU6050_ADR,GYRO_CONFIG, 0x18);     //+-2000dps
   IIC_WriteReg(IIC_MPU6050_ADR,ACCEL_CONFIG, 0x09);    //+-4g
}

/*
**IIC初始化
*/
void I2C_Init(void)
{
    INIT_SCL;                                        //IIC的SCL
    INIT_SDA;                                        //IIC的SDA
    MPU6050_Init();
    MMA8451_Init();
}
