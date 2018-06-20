/*********************Copyright(C) 2015-2016 TYUT JBD TEAM B.*******************
Copyright (C), 2015-2016, TYUT JBD TEAM B.
File name: I2C.c 
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
#include "declare.h"

/**
**Function: IIC
**Description: IICͨ�Ż�ȡ����������ٶȼƵ�ֵ
**Calls: getinfor.c
**Called By: 
**Output: ��
**Return: ��
**Others: ��
**/
char ack=0;                     //Ӧ���ź�
int CY=0;                       //���ݽ���
int giAcc_X=0;                  //X����ٶ�
int giAcc_Y=0;                  //Y����ٶ�
int giAcc_Z=0;                  //Z����ٶ�
int giGyro_X=0;                 //X����ٶ�
int giGyro_Y=0;                 //Y����ٶ�
int giGyro_Z=0;                 //Z����ٶ�

/*
**IIC��ʱ����
*/
void IIC_TestDelay(void)
{  
    asm("nop");//1//����0.2ms,��ʱ0.01us//1
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
**IIC��ʼ�ź�
*/
void IIC_Start(void)
{
    //SCLout;
    SDAout;		
    SDA_H_;                     //����SDA
    SCL_H_;                     //����SCL
    I2C_Delay;                  //I2C��ʱ
    SDA_L_;                     //����SDA
    I2C_Delay;                  //I2C��ʱ
    SCL_L_;                     //����SCL
}

/*
**IICֹͣ�ź�
*/
void IIC_Stop(void)
{
    //SCLout;
    SDAout;		
    SDA_L_;                     //����SDA
    SCL_H_;                     //����SCL
    I2C_Delay;                  //I2C��ʱ
    SDA_H_;                     //����SDA
    I2C_Delay;                  //I2C��ʱ
}

/*
**Ӧ���ź�
*/
void IIC_SendACK(int ack)
{
    //SCLout;
    SDAout;
    if(ack==0)
    {
    	SDA_L_;                 //����SDA
    }
    else
    {
    	SDA_H_;                 //����SDA
    }
    SCL_H_;                     //����SCL
    I2C_Delay;                  //I2C��ʱ
    SCL_L_;                     //����SCL
    I2C_Delay;                  //I2C��ʱ
}

/*
**����Ӧ���ź�
*/
uchar IIC_RecvACK(void)
{
    SDAin;
    //SCLout;
    SCL_H_;                    //����ʱ����
    I2C_Delay;
    CY = SDAread;              //��Ӧ���ź�
    SCL_L_;                    //����ʱ����
    I2C_Delay;
    return CY;
}

/*
**��IIC���߷�������
*/
uchar IIC_SendByte(uchar data)
{                
    uchar i;
    //SCLout;
    SDAout;	
    for (i=0; i<8; i++)         //8λ������
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
**��IIC���߽�������
*/
uchar IIC_RecvByte(void)
{
    uchar i;
    uchar dat = 0;
    //SCLout;
    SDAout;
	
    SDA_H_;
    SDAin;                     //ʹ���ڲ�����,׼����ȡ����,
    for (i=0; i<8; i++)        //8λ������
    {
        dat <<= 1;
        SCL_H_;                //����ʱ����
        I2C_Delay;
        dat |= SDAread;        //������               
        SCL_L_;                //����ʱ����
        I2C_Delay;
    }
    return dat;
}

/*
**дIIC�豸�Ĵ���
*/
void IIC_WriteReg(uint8 dev_addr,uint8 reg_addr , uint8 data)
{		
    IIC_Start();                     //��ʼ�ź�
    IIC_SendByte(dev_addr);          //�����豸��ַ+д�ź�     
    IIC_SendByte(reg_addr);          //�ڲ��Ĵ�����ַ
    IIC_SendByte(data);              //�ڲ��Ĵ�������
    IIC_Stop();                      //����ֹͣ�ź�
}

/*
**��IIC�豸�Ĵ���
*/
uchar IIC_ReadByte(uint8 dev_addr,uint8 reg_addr)
{  
    uchar data;
    IIC_Start();                      //��ʼ�ź�
    IIC_SendByte(dev_addr);           //�����豸��ַ+д�ź�
    IIC_SendByte(reg_addr);           //���ʹ洢��Ԫ��ַ����0��ʼ	
    IIC_Start();                      //��ʼ�ź�
    IIC_SendByte(dev_addr+1);         //�����豸��ַ+���ź�
    data=IIC_RecvByte();              //�����Ĵ�������
    IIC_SendACK(1);   
    IIC_Stop();                       //ֹͣ�ź�
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
**�ϳ�����
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
    return (H<<8)+L;   //�ϳ�����
}

/*
**���MMA X����ٶ�
*/
int16 MMA_Get_X_Acc(void)
{
    int16 temp;
    temp=IIC_GetData(IIC_MMA8451_ADR,OUT_X_MSB);
    return temp;
}

/*
**���MMA Y����ٶ�
*/
int16 MMA_Get_Y_Acc(void)
{
    int16 temp;
    temp=IIC_GetData(IIC_MMA8451_ADR,OUT_Y_MSB);
    return temp;
}

/*
**���MMA Z����ٶ�
*/
int16 MMA_Get_Z_Acc(void)
{
    int16 temp;
    temp=IIC_GetData(IIC_MMA8451_ADR,OUT_Z_MSB);
    return temp;
}

/*
**���MPU X����ٶ�
*/
int16 MPU_Get_X_Acc(void)
{
    int16 temp;
    temp=IIC_GetData(IIC_MPU6050_ADR,ACCEL_XOUT_H);
    return temp;
}

/*
**���MPU Y����ٶ�
*/
int16 MPU_Get_Y_Acc(void)
{
    int16 temp;
    temp=IIC_GetData(IIC_MPU6050_ADR,ACCEL_YOUT_H);
    return temp;
}

/*
**���MPU Z����ٶ�
*/
int16 MPU_Get_Z_Acc(void)
{
    int16 temp;
    temp=IIC_GetData(IIC_MPU6050_ADR,ACCEL_ZOUT_H);
    return temp;
}

/*
**���MPU Z����ٶ�
*/
int16 MPU_Get_Z_Gyro(void)
{
    int16 temp;
    temp=IIC_GetData(IIC_MPU6050_ADR,GYRO_ZOUT_H);
    return temp;
}

/*
**���MPU Y����ٶ�
*/
int16 MPU_Get_Y_Gyro(void)
{
    int16 temp;
    temp=IIC_GetData(IIC_MPU6050_ADR,GYRO_YOUT_H);
    return temp;
}

/*
**���MPU X����ٶ�
*/
int16 MPU_Get_X_Gyro(void)
{
    int16 temp;
    temp=IIC_GetData(IIC_MPU6050_ADR,GYRO_XOUT_H);
    return temp;
}


/*
**MMA8451��ʼ��
*/
void MMA8451_Init(void)
{ 
    IIC_WriteReg(IIC_MMA8451_ADR,XYZ_DATA_CFG, 0x00);           //0Ϊ+-2g��1Ϊ+-4g��2Ϊ+-8g
    IIC_WriteReg(IIC_MMA8451_ADR,HP_FILTER_CUTOFF, 0x10);       // ʹ�ܵ�ͨ�˲�
    IIC_WriteReg(IIC_MMA8451_ADR,CTR_REG1 , 0x01);              //0x2aΪ���ƼĴ���1 ����mma8451
    IIC_WriteReg(IIC_MMA8451_ADR,CTR_REG2 , 0x02);
}

/*
**MPU6050��ʼ��
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
**IIC��ʼ��
*/
void I2C_Init(void)
{
    INIT_SCL;                                        //IIC��SCL
    INIT_SDA;                                        //IIC��SDA
    MPU6050_Init();
    MMA8451_Init();
}
