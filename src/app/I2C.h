/*********************Copyright(C) 2015-2016 TYUT JBD TEAM B.*******************
Copyright (C), 2015-2016, TYUT JBD TEAM B.
File name: I2C.h 
Author: TEAM B, Alopex
Version: 1.0               
Date: 2015.10.28(E)
      2015.12.31(C)
      2016.03.15(A)
Description: 
                1.I2C Communication Protocol(I2Cͨ��Э��)
                2.MPU6050 Gyroscope & Accelerometer(MPU6050�����Ǻͼ��ٶȼ�)
                3.MMA8451 Accelerometer(MMA8451���ٶȼ�)
Others: Noting
Function List:  1.I2C Communication Protocol(I2Cͨ��Э��)
                2.MPU6050 Gyroscope & Accelerometer(MPU6050�����Ǻͼ��ٶȼ�)
                3.MMA8451 Accelerometer(MMA8451���ٶȼ�)
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
#ifndef __I2C_H__
#define __I2C_H__

#define uint unsigned int
#define uchar unsigned char

//����IIC�豸��ַ
#define IIC_MPU6050_ADR  0xD0
#define IIC_MMA8451_ADR  0x38//0x1C//0x38
#define IIC_L3G4200_ADR  0xD2

//����MPU6050�ڲ��Ĵ�����ַ
#define	PWR_MGMT_1      0x6B	//��Դ��������ֵ��0x00(��������)
#define	SMPLRT_DIV	0x19	//�����ǲ����ʣ�����ֵ��0x07(125Hz)
#define	CONFIG		0x1A	//��ͨ�˲�Ƶ�ʣ�����ֵ��0x06(5Hz)
#define	GYRO_CONFIG	0x1B	//�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
#define	ACCEL_CONFIG	0x1C	//���ټ��Լ졢������Χ����ͨ�˲�Ƶ�ʣ�����ֵ��0x01(���Լ죬2G��5Hz)
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
#define	WHO_AM_I        0x75	//IIC��ַ�Ĵ���(Ĭ����ֵ0x68��ֻ��)

//����L3G�ڲ��Ĵ�����ַ
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

//����MMA8451�ڲ��Ĵ�����ַ
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

//�˿�����
#define INIT_SCL  gpio_init(PORTB, 9, GPO, 0)  //IIC��SCL
#define INIT_SDA  gpio_init(PORTB, 2, GPO, 0)  //IIC��SDA
#define SCLout    DDRB9 = 1                    //SCL���
#define SDAout    DDRB2 = 1                    //SDA���
#define SDA_H_    PTB2_OUT = 1                 //B2�ߵ�ƽ
#define SCL_H_    PTB9_OUT = 1                 //B9�ߵ�ƽ
#define SDA_L_    PTB2_OUT = 0                 //B2�͵�ƽ
#define SCL_L_    PTB9_OUT = 0                 //B9�͵�ƽ
#define SDAin     DDRB2 = 0                    //SDA����
#define SDAread   PTB2_IN                      //SDA��ȡ

//IIC��ʱ��
#define I2C_Delay IIC_TestDelay()

//Gyro��ֵ̬(Gyro��תֵΪ-,��תֵΪ+)
#define Gyro_Static -20

//��������
extern int giAcc_X;
extern int giAcc_Y;
extern int giAcc_Z;
extern int giGyro_X;
extern int giGyro_Y;
extern int giGyro_Z;
extern char ack;                 
extern int CY;                         

//��������
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