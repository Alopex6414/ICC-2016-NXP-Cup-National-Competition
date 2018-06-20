/*********************Copyright(C) 2015-2016 TYUT JBD TEAM B.*******************
Copyright (C), 2015-2016, TYUT JBD TEAM B.
File name: ctrl.c 
Author: TEAM B, Alopex
Version: 1.0               
Date: 2015.10.28(E)
      2015.12.31(C)
      2016.03.15(A)

Description: 
            Control File Include AngleControl SpeedControl DirectionControl(�����ļ������Ƕȿ��ơ��ٶȿ��ơ��������)
            MotorControl(�������)
Others: Noting
Function List:
              1.DirectionControl ����PWM����������
              2.SpeedControl ����ٶȿ���
              3.SetMotorVoltage ����������
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
**SpeedControl
*/
int giSp = 0;                           //�ٶ�P����
int giSi = 0;                           //�ٶ�I���� 
int giSd = 0;                           //�ٶ�D����
int giMeterSetValue = 500;              //�����趨ֵ(����ͣ��)(����ʱ����)
int giRoadBlockOffset = 0;              //·��ƫ����
float gfMeter_Count=0;                  //·�̼���
float gf_AlterSpeed=0;                  //��ǰ�����ٶ�
float gf_AlterSpeed_Old=0;              //ǰ�������ٶ�
float gfSpeedErrI=0;                    //�ٶ�I����
float gfDesirSpeed_Mid=0;               //��������
float gfDesirSpeed_Low=0;               //��������
float gfDesirSpeed_High=0;              //��������
float gfDesirSpeed=0;                   //��ǰ�����ٶ�
float gfSpeedPWM=0;                     //�ٶ�PWM
float gfDirectionPWM=0;                 //����PWM
float gfSpeedErr[4]={0};                //�ٶ�ƫ������
float gfSpeedErrGrad[4]={0};            //�ٶ�ƫ��仯��
double gfCurrentSpeedPWM=0;             //��ǰ�ٶ�PWM
double gfLastSpeedPWM=0;                //�ϴ��ٶ�PWM
long int giMeter_Sum=0;                 //�ٶȻ���·��
long int giMeter_Sum_RoadBlock=0;       //·���ٶȻ���·��

/*
**�ű�ת���ٶ�
*/
char gcBeaconSpeed[15]={0};             //0:���� 1:�е���
int giBeaconSpeed1=0;                   //ÿ���ű��תȦ�����ٶ�(ǰ5���ű�)
int giBeaconSpeed2=0;                   //ÿ���ű��תȦ�����ٶ�(��5���ű�)
int giBeaconSpeed3=0;                   //ÿ���ű��תȦ�����ٶ�(���5���ű�)

/*
**DirectionControl
*/
int giDp = 0;                           //����P����
int giDd = 0;                           //����D���� 
int giDi = 0;                           //����I����
int giDk = 0;                           //����K����
int giAddOffset = 0;                    //����ƫ���
float gfDirectionErrI = 0;              //����ƫ��I
float gfDirection_Pwm = 0;              //����PWM
float gfDirectionPwmOld=0;              //�ϴη���PWM
float gfDirectionPWM_Normalize=0;       //�����һ��

/*
**·�����
*/
int giBlockSpeed=0;                      //�ж�Ϊ�ϰ�ʱ���ٶ�
int giRoadBlockDis=0;                    //����·��ʱ�����·�ϵľ���
int giDirectStraight=0;                  //ֱ�߱�־(��ǰ����Ƕȷ�Χ�Ƚ�С)

/*
**SetMotorVoltag
*/
unsigned char StartStop=0;             //�����߼��ͣ������
unsigned char MotorStree=0;            //���ת������: 0:��ת 1:��ת
unsigned char guSpeedflag=0;           //�ٶȿ��Ʊ�־(enable)

/**
**Function: DirectionControl
**Description: �������
**Calls:   ��
**Called By: PORTD5_IRQHandler
**           PORTA13_IRQHandler
**Table Accessed: ��
**Table Updated: ��
**Input: gfPosition
**       gfCurrentSpeed
**       gfCurrentSpeedLeft
**       giDp
**       giDd
**       giDk
**Output: gfDirectionPwmL
**        gfDirectionPwmR
**Return: ��
**Others:��
**/
void DirectionControl(void)
{   
    int i = 0;
    int liSpeedWheel=0;//giSpeedWheel
    double lfDirectionPWM = 0;
    
    //���������(���ַ���,����ƫ��10�ڷ���ƫ�����)
    if(giSpeedWheel < 0)
    {
      liSpeedWheel = 0;
    }
    else if(giSpeedWheel > gfDesirSpeed_High)
    {
      liSpeedWheel = gfDesirSpeed_High;
    }
    else
    {
      liSpeedWheel = giSpeedWheel;
    }
    
    //�������ٶ��й�,����״̬�»�����,����״̬�»��ֿ�
    if(gfRoadSitError[0] >= -25 && gfRoadSitError[0] <= 25)
    {
      gfDirectionErrI += giDi * gfRoadSitError[0] * liSpeedWheel * 0.00001;
    }
    
    else
    {
      gfDirectionErrI = 0;
    }
    
    gfDirection_Pwm = giDp * gfRoadSitError[0] * 0.1 + gfDirectionErrI + giDd * (gfRoadSitError[0] - gfRoadSitError[2]) * 0.1;
   
    if(guBeaconCountPlus==1)//�ű������һʱ,D����������Ҫȥ��
    {
      gfDirection_Pwm = giDp * gfRoadSitError[0] * 0.1;
    }
    
    if(guSidePass == 1)
    {
      gfDirection_Pwm = gs_Derict_fuzzyPID.fuzzyP * gfRoadSitError[0] * 0.1 + gfDirectionErrI + giDd * (gfRoadSitError[0] - gfRoadSitError[2]) * 0.1;
    }
    
    /*ֱ�߱�־�ж�*/
    if(gfDirection_Pwm > -200 && gfDirection_Pwm < 200
        && gfDirectionPwmOld > -200 && gfDirectionPwmOld < 200)
    {
      giDirectStraight = 1;
    }
    
    else
    {
      giDirectStraight = 0;
    }
    
    
    /*·�ϴ���*/    
    if(guRoadBlock == 1)//·�ϱ�־
    {    
      if(guRoadBlockflag == 0)
      {
        giMeter_Sum_RoadBlock = 0;
        giBlockSpeed = giCurrentSpeed;//����·��ʱ��ǰ�ٶ�
        giRoadBlockDis = Ultrasonic_Dis[0];//����·��ʱ��ǰ����
        giRoadBlockOffset = -0.002236 * pow(giRoadBlockDis,3) + 0.7605 * pow(giRoadBlockDis,2) - 87.7 * giRoadBlockDis + 3755.5;//3555.5
        
        if(giRoadBlockOffset < 150)
        {
          giRoadBlockOffset = 150;
        }
        else if(giRoadBlockOffset > Motor_Limit)
        {
          giRoadBlockOffset = Motor_Limit;
        }
        
        guRoadBlockflag = 1;
      }
        
      else if(guRoadBlockflag == 1)
      {
        giMeter_Sum_RoadBlock += giCurrentSpeed;
        
        if(giMeter_Sum_RoadBlock >= giRoadBlockDis * OneMeter * 0.0090 || guSidePass == 1 || giLostBeacon == 1)
        {
          giRoadBlockDis = 0;           //��������
          guRoadBlock = 0;              //·�ϱ�־����
          giRoadBlockOffset = 0;        //·��ƫ������
          giMeter_Sum_RoadBlock = 0;    //·�̼�������
          guRoadBlockflag = 0;          //·��ʱ���־����
        }
      }
      
      if(guBeaconCount < 15)
      {
        if(gcBeaconOrder[guBeaconCount] == 0)
        {
          gfDirection_Pwm = -giRoadBlockOffset;
        }
        else if(gcBeaconOrder[guBeaconCount] == 1)
        {
          gfDirection_Pwm = giRoadBlockOffset;
        }
      }
    
      else
      {
        #ifdef LeftModel
        gfDirection_Pwm = -giRoadBlockOffset;
        #elif defined RightModel
        gfDirection_Pwm = giRoadBlockOffset;
        #else
        gfDirection_Pwm = -giRoadBlockOffset;
        #endif
      }
    }
    
    /*ת�ر�־ת��*/
    if(giTurnBack == 1 && giDp != 0)//��ת��־������ת
    {
        if(guBeaconCount < 15)//10�ű���
        {
          if(gcBeaconOrder[guBeaconCount] == 0)
          {
            gfDirection_Pwm = Motor_Limit_L;
          }
          else if(gcBeaconOrder[guBeaconCount] == 1)
          {
            gfDirection_Pwm = Motor_Limit_R;
          }
        }
        
        else
        {
          #ifdef LeftModel
          gfDirection_Pwm = Motor_Limit_L;
          #elif defined RightModel
          gfDirection_Pwm = Motor_Limit_R;
          #else
          gfDirection_Pwm = Motor_Limit_L;
          #endif
        }
    }
    
    /*��ʧ�ű괦��*/
    if(giLostBeacon == 1 && giDp != 0)//��ʧ�ű������ת
    {
      if(guCircleLost == 1)
      {
        if(guBeaconCount < 15)//10�ű���
        {
          if(gcBeaconOrder[guBeaconCount] == 0)
          {
            gfDirection_Pwm = Motor_Limit_L;
          }
          else if(gcBeaconOrder[guBeaconCount] == 1)
          {
            gfDirection_Pwm = Motor_Limit_R;
          }
        }
        
        else
        {
          #ifdef LeftModel
          gfDirection_Pwm = Motor_Limit_L;
          #elif defined RightModel
          gfDirection_Pwm = Motor_Limit_R;
          #else
          gfDirection_Pwm = Motor_Limit_L;
          #endif
        }

      }
      
      else
      {
        if(gfDirectionPwmOld <= 0)
        {
          gfDirection_Pwm = Motor_Limit_L;
        }
        else
        {
          gfDirection_Pwm = Motor_Limit_R;
        }
      }
      guStraight = 0;    
    }
    
    /*���Զ����ת����*/
    #ifdef Limitation_Test
    gfDirection_Pwm = 0;
    #endif
    
    if(gfDirection_Pwm < Motor_Limit_L)
    {
      gfDirection_Pwm = Motor_Limit_L;
    }
    
    else if(gfDirection_Pwm > Motor_Limit_R)
    {
      gfDirection_Pwm = Motor_Limit_R;
    }
    
    gfDirectionPWM = Motor_Center + gfDirection_Pwm;//��ǰ���ת��
    
    lfDirectionPWM = Motor_Center + gfDirection_Pwm;
    
    gfDirectionPwmOld = gfDirection_Pwm;//����PWM��¼ֵ
    
    FTM_PWM_Duty(FTM2,CH0,lfDirectionPWM);
}

/**
**Function: SpeedControl
**Description: �ٶȿ���ƫ�Ǽ���
**Calls:   ��
**Called By: PORTB2_IRQHandler
**           PORTA13_IRQHandler
**Table Accessed: ��
**Table Updated: ��
**Input: giDesirSpeed
**       gfCurrentSpeed
**       giSpExpedite
**       giSpConstant
**       giSi
**       giSd
**Output: SpeedControl 
**Return: ��
**Others:��
**/
void SpeedControl(void)
{ 
  int i=0;
  float lfSpeedLow=0;           //��ǰ����תȦ����
  float lf_AlterSpeed=0;        //��ǰ�ٶ�����ֵ
  float lf_AlterSpeed_Now=0;    //����λ�õ��ٶ�����ֵ
  double lfSpeedPWM=0;        //�ٶ�PWM
    
  giCurrentSpeed = giSpeedWheel;//���㵱ǰ�ٶ�(��ǰ�ٶȵ�������)
  giMeter_Sum += giCurrentSpeed;//�������
  
  if(giMeter_Sum > OneMeter)//4050������1M(����)
  {
    gfMeter_Count++;
    gfAllMeter++;
    giMeter_Sum = 0;
  }
  
  else
  {
    gfMeter_Count = (int)gfMeter_Count + giMeter_Sum / OneMeter;
    gfAllMeter = (int)gfAllMeter + giMeter_Sum / OneMeter;
  }
  
  if(gfMeter_Count > 100)
  {
    gfMeter_Count = 100;//100M
  }
  
  //�ű���������(0:���� 1:�е���)
  if(guBeaconCount < 15)//10�ű���
  {
    if(gcBeaconSpeed[guBeaconCount] == 0)
    {
      lfSpeedLow = gfDesirSpeed_Low;
    }
    else if(gcBeaconSpeed[guBeaconCount] == 1)
    {
      lfSpeedLow = (gfDesirSpeed_Low + gfDesirSpeed_Mid) * 0.5;
    }
    else
    {
      lfSpeedLow = gfDesirSpeed_Low;
    }
  }
  
  else//����10�ű�
  {
    lfSpeedLow = gfDesirSpeed_Low;
  }
  
  /*�ٶȸ�������(�кž���)*/
  if(gfDesirSpeed_Low <= gfDesirSpeed_Mid && gfDesirSpeed_High >= gfDesirSpeed_Mid
      && gfDesirSpeed_Low != 0 && gfDesirSpeed_Mid != 0 && gfDesirSpeed_High != 0)
  {
    if(gfRowFilter > 30 && gfRowFilter <= 60)//�ű굱ǰ�к���30 - 60��Χ֮��(0.6M-1M)
    {        
      lf_AlterSpeed = gfDesirSpeed_Mid - (gfDesirSpeed_Mid - lfSpeedLow) * ((60 - gfRowFilter) * 0.034);
    }
    
    else if(gfRowFilter > 60 && gfRowFilter <= 75)//�ű굱ǰ�к���60 - 75�з�Χ��(1M-2M)
    {
      lf_AlterSpeed = gfDesirSpeed_High - (gfDesirSpeed_High - gfDesirSpeed_Mid) * ((75 - gfRowFilter) * 0.067);
    }
    
    else if(gfRowFilter > 75)//�ű굱ǰ�к���75���Ժ�(2M����)
    {
      lf_AlterSpeed = gfDesirSpeed_High;
    }
      
    else//�ű굱ǰ�к���30������(0.6M����)
    {
      lf_AlterSpeed = lfSpeedLow;
    }
      
    //�ٶ���������
    if(lf_AlterSpeed > gfDesirSpeed_High)
    {
      lf_AlterSpeed = gfDesirSpeed_High;
    }
      
    else if(lf_AlterSpeed < gfDesirSpeed_Low)
    {
      lf_AlterSpeed = gfDesirSpeed_Low;
    }
      
  }
  
  //�ű궪ʧתȦʱ����
  if(giTurnBack == 1 || giLostBeacon == 1)//���ر�־���߶�ʧ�ű�
  {
    lf_AlterSpeed = lfSpeedLow;//����
  }
  else if(guRoadBlock == 1)//����·�ϱ�־
  {
    lf_AlterSpeed = gfDesirSpeed_Low;//����
  }
  
  //����Ƕȹ�һ��(�Ƕ������ٶ�)
  if(gfDirection_Pwm < 0)
  {
    gfDirectionPWM_Normalize = gfDirection_Pwm / Motor_Limit_L;
  }
  
  else
  {
    gfDirectionPWM_Normalize = gfDirection_Pwm / Motor_Limit_R;
  }
  
  //λ�������ٶ�(�Ƕ�����)
  lf_AlterSpeed_Now = lf_AlterSpeed - (lf_AlterSpeed - lfSpeedLow) * gfDirectionPWM_Normalize;
  
  //�����ٶȵ�ͨ�˲�(����ƽ��,ƽ�������ٶ�)
  if(guRoadBlock == 1)//·�ϱ�־
  {
    lf_AlterSpeed_Now = lf_AlterSpeed_Now;
  }
  else
  {
    if(giBeacon_Row > 75)  //�ű�����ҰԶ��
    {
      lf_AlterSpeed_Now = gf_AlterSpeed_Old * 0.95 + lf_AlterSpeed_Now * 0.05;
    }
  }
  
  //λ�������ٶ��޷�
  if(lf_AlterSpeed_Now > gfDesirSpeed_High)
  {
    lf_AlterSpeed_Now = gfDesirSpeed_High;
  }
  
  else if(lf_AlterSpeed_Now < gfDesirSpeed_Low)
  {
    lf_AlterSpeed_Now = gfDesirSpeed_Low;
  }
  
  gf_AlterSpeed = lf_AlterSpeed;//lf_AlterSpeed
  gfDesirSpeed = lf_AlterSpeed_Now;//lf_AlterSpeed
  
  //�������ٶ�ƫ��
  gfSpeedErr[0] = gfDesirSpeed - giSpeedWheel;
  gfSpeedErrGrad[0] = gfSpeedErr[0] - gfSpeedErr[3];
    
  if(Switch_On && StartCount >= 200)         //��������֮��
  {
    if(gfSpeedErr[0] <= 30 && gfSpeedErr[0] >= -30)
    {
      gfSpeedErrI += gs_Speed_fuzzyPID.fuzzyI * gfSpeedErr[0] * 0.1;
    }
    
    else
    {
      gfSpeedErrI = 0;
    }
    
    //�����޷�
    if(gfSpeedErrI > 50000)
    {
      gfSpeedErrI = 50000;
    }
    
    else if(gfSpeedErrI < -50000)
    {
      gfSpeedErrI = -50000;
    }
    
  }
  
  lfSpeedPWM = gs_Speed_fuzzyPID.fuzzyP * gfSpeedErr[0] * 10.0 +  gfSpeedErrI + giSd * (gfSpeedErr[0] - gfSpeedErr[2]) * 1;
  
  //���PWM����˲�
  if(guRoadBlock == 1)//·�ϱ�־
  {
    lfSpeedPWM = lfSpeedPWM;
    guSpeedflag = 0;
  }
  else
  {
    if(giBeacon_Row > 75)
    {
      lfSpeedPWM = gfLastSpeedPWM * 0.95 + lfSpeedPWM * 0.05;
      guSpeedflag = 1;
    }
    else if(giBeacon_Row > 60 && giBeacon_Row <= 75)  //������ʼ����
    {
      lfSpeedPWM = gfLastSpeedPWM * 0.7 + lfSpeedPWM * 0.3;
      guSpeedflag = 1;
    }
    else if(giBeacon_Row > 30 && giBeacon_Row <= 60)
    {
      lfSpeedPWM = gfLastSpeedPWM * 0.5 + lfSpeedPWM * 0.5;
      guSpeedflag = 0;
    }
    else
    {
      lfSpeedPWM = gfLastSpeedPWM * 0.5 + lfSpeedPWM * 0.5;
      guSpeedflag = 0;
    }
  }
  
  //�����޷�
  if(lfSpeedPWM>99999)
  {
    lfSpeedPWM=99999;
  }
  
  else if(lfSpeedPWM<-99999)
  {
    lfSpeedPWM=-99999;
  }
  
  //PWM����
  gfCurrentSpeedPWM = lfSpeedPWM;
  
  //�ϴ�PWM��¼
  gfLastSpeedPWM = lfSpeedPWM;
  
  //�����ٶȼ�¼
  gf_AlterSpeed_Old = lf_AlterSpeed_Now;
  
  //����洢�ٶ�ƫ��
  for(i=3;i>0;i--)
  {
    gfSpeedErr[i]=gfSpeedErr[i-1];
    gfSpeedErrGrad[i]=gfSpeedErrGrad[i-1];
  }
}

/*
**Function: SetMotorVoltage
**Description: ���PWM����
**Calls:   ��
**Called By: PORTD5_IRQHandler
**           PORTA13_IRQHandler
**Table Accessed: ��
**Table Updated: ��
**Input: gfAnglePWM
**       gfDirectionPwmL
**       gfDirectionPwmR
**Output: ���������
**Return: ��
**Others:��
**/
void SetMotorVoltage(void) 
{ 
    double lfPWM = 0;
    
    //�������ͣ��Ϊ0M,����Ϊ300M
    if(giMeterSetValue == 0)
    {
      giMeterSetValue = 500;
    }
    
    if(Switch_Off || (giBeaconExcept != 0 && guBeaconCount == giBeaconExcept) || BeaconStart == 0)//���عػ��߶�ʧ�ű��־
    {
      DriveSwitch_Off;/*disable*/
      
      if(Switch_Off)
      {
        StartCount = 0;
        BeaconStart = 0;
        guBeaconCount = 0;
        guBeaconTimer = 0;
        gfAllMeter = 0;
        gfAllTime = 0;
        gfRealSpeed = 0;
        giMeter_Sum = 0;
      }
      
      else if(Switch_On && BeaconStart == 0)//���ؿ�������û��⵽��ʼ����
      {
        if(SDTIME == 1)
        {
          StartCount = 0;
        }
        else
        {
          StartCount = 200;
        }
        BeaconStart = 0;
        guBeaconCount = 0;
        guBeaconTimer = 0;
        gfAllMeter = 0;
        gfAllTime = 0;
        gfRealSpeed = 0;
        giMeter_Sum = 0;
      }
    }
    
    else if(Switch_On && StartCount >= 200 && (giBeaconExcept == 0 || guBeaconCount < giBeaconExcept))//���ؿ����Ҽ�����־
    {
      DriveSwitch_On;/*enable*/
    }
    
    lfPWM = gfCurrentSpeedPWM;
        
    if(lfPWM <= 0)//�������ת
    {
      MotorInversion;//�����ת
      MotorStree = 1;
      //lfPWM = 100000 + lfPWM;
      lfPWM = -lfPWM;
    }
        
    else
    {
      MotorForeward;//�����ת
      MotorStree = 0;
      //lfPWM = lfPWM;
      lfPWM = 100000 - lfPWM;
    }
        
    //PWM����޷�
    if(lfPWM > 99999)
    {
      lfPWM = 99999;
    }
        
    if (lfPWM < 0)     
    {
      lfPWM = 0;
    }
    
    gfSpeedPWM = (99999 - lfPWM) * 0.001;//�ٶ�PWM
        
    FTM_PWM_Duty(FTM0,CH3,lfPWM);//���PWM

}