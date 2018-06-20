/*********************Copyright(C) 2015-2016 TYUT JBD TEAM B.*******************
Copyright (C), 2015-2016, TYUT JBD TEAM B.
File name: I2C.h 
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
#ifndef __I2C_H__
#define __I2C_H__

#define uint unsigned int
#define uchar unsigned char

//定义IIC设备地址
#define IIC_MPU6050_ADR  0xD0
#define IIC_MMA8451_ADR  0x38//0x1C//0x38
#define IIC_L3G4200_ADR  0xD2

//定义MPU6050内部寄存器地址
#define	PWR_MGMT_1      0x6B	//电源管理，典型值：0x00(正常启用)
#define	SMPLRT_DIV	0x19	//陀螺仪采样率，典型值：0x07(125Hz)
#define	CONFIG		0x1A	//低通滤波频率，典型值：0x06(5Hz)
#define	GYRO_CONFIG	0x1B	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define	ACCEL_CONFIG	0x1C	//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40
#define	TEMP_OUT_H      0x41
#define	TEMP_OUT_L	0x42
#define	GYRO_XOUT_H	0x43
#define	GYRO_XOUT_L	0x44	
#define	GYRO_YOUT_H	0x45
#define	GYRO_YOUT_L	0x46
#define	GYRO_ZOUT_H	0x47
#define	GYRO_ZOUT_L	0x48
#define	WHO_AM_I        0x75	//IIC地址寄存器(默认数值0x68，只读)

//定义L3G内部寄存器地址
#define OUT_X_L 0x28
#define OUT_X_H 0x29
#define OUT_Y_L 0x2A
#define OUT_Y_H 0x2B
#define OUT_Z_L 0x2C
#define OUT_Z_H 0x2D
#define CTRL_REG1 0x20
#define CTRL_REG2 0x21
#define CTRL_REG3 0x22
#define CTRL_REG4 0x23
#define CTRL_REG5 0x24

//定义MMA8451内部寄存器地址
#define OUT_X_MSB 0x01
#define OUT_X_LSB 0x02
#define OUT_Y_MSB 0x03
#define OUT_Y_LSB 0x04
#define OUT_Z_MSB 0x05
#define OUT_Z_LSB 0x06
#define XYZ_DATA_CFG 0x0E
#define HP_FILTER_CUTOFF 0x0F
#define CTR_REG1 0x2A
#define CTR_REG2 0x2B

//端口设置
#define INIT_SCL  gpio_init(PORTB, 9, GPO, 0)  //IIC的SCL
#define INIT_SDA  gpio_init(PORTB, 2, GPO, 0)  //IIC的SDA
#define SCLout    DDRB9 = 1                    //SCL输出
#define SDAout    DDRB2 = 1                    //SDA输出
#define SDA_H_    PTB2_OUT = 1                 //B2高电平
#define SCL_H_    PTB9_OUT = 1                 //B9高电平
#define SDA_L_    PTB2_OUT = 0                 //B2低电平
#define SCL_L_    PTB9_OUT = 0                 //B9低电平
#define SDAin     DDRB2 = 0                    //SDA输入
#define SDAread   PTB2_IN                      //SDA读取

//IIC延时宏
#define I2C_Delay IIC_TestDelay()

//Gyro静态值(Gyro左转值为-,右转值为+)
#define Gyro_Static -20

//变量声明
extern int giAcc_X;
extern int giAcc_Y;
extern int giAcc_Z;
extern int giGyro_X;
extern int giGyro_Y;
extern int giGyro_Z;
extern char ack;                 
extern int CY;                         

//函数声明
extern void IIC_TestDelay(void);
extern void IIC_Start(void);
extern void IIC_Stop(void);
extern void IIC_SendACK(int ack);
extern uchar IIC_SendByte(uchar data);
extern uchar IIC_RecvByte(void);
extern void IIC_WriteReg(uint8 dev_addr,uint8 reg_addr , uint8 data);
extern uchar IIC_ReadByte(uint8 dev_addr,uint8 reg_addr);
extern int16 IIC_ReadWord(uint8 dev_addr,uint8 reg_addr);
extern int IIC_GetData(uint8 dev_addr,uint8 reg_addr);
extern void MMA8451_Init(void);
extern void MPU6050_Init(void);
extern void I2C_Init(void);
extern int16 MPU_Get_Z_Gyro(void);

#endif