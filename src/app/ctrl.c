/*********************Copyright(C) 2015-2016 TYUT JBD TEAM B.*******************
Copyright (C), 2015-2016, TYUT JBD TEAM B.
File name: ctrl.c 
Author: TEAM B, Alopex
Version: 1.0               
Date: 2015.10.28(E)
      2015.12.31(C)
      2016.03.15(A)

Description: 
            Control File Include AngleControl SpeedControl DirectionControl(控制文件包含角度控制、速度控制、方向控制)
            MotorControl(电机控制)
Others: Noting
Function List:
              1.DirectionControl 方向PWM输出舵机控制
              2.SpeedControl 电机速度控制
              3.SetMotorVoltage 电机驱动输出
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
**SpeedControl
*/
int giSp = 0;                           //速度P参数
int giSi = 0;                           //速度I参数 
int giSd = 0;                           //速度D参数
int giMeterSetValue = 500;              //距离设定值(距离停车)(调试时候用)
int giRoadBlockOffset = 0;              //路障偏移量
float gfMeter_Count=0;                  //路程计算
float gf_AlterSpeed=0;                  //当前期望速度
float gf_AlterSpeed_Old=0;              //前次期望速度
float gfSpeedErrI=0;                    //速度I积分
float gfDesirSpeed_Mid=0;               //期望中速
float gfDesirSpeed_Low=0;               //期望低速
float gfDesirSpeed_High=0;              //期望高速
float gfDesirSpeed=0;                   //当前期望速度
float gfSpeedPWM=0;                     //速度PWM
float gfDirectionPWM=0;                 //方向PWM
float gfSpeedErr[4]={0};                //速度偏差数组
float gfSpeedErrGrad[4]={0};            //速度偏差变化率
double gfCurrentSpeedPWM=0;             //当前速度PWM
double gfLastSpeedPWM=0;                //上次速度PWM
long int giMeter_Sum=0;                 //速度积分路程
long int giMeter_Sum_RoadBlock=0;       //路障速度积分路程

/*
**信标转向速度
*/
char gcBeaconSpeed[15]={0};             //0:低速 1:中低速
int giBeaconSpeed1=0;                   //每个信标的转圈期望速度(前5个信标)
int giBeaconSpeed2=0;                   //每个信标的转圈期望速度(后5个信标)
int giBeaconSpeed3=0;                   //每个信标的转圈期望速度(最后5个信标)

/*
**DirectionControl
*/
int giDp = 0;                           //方向P参数
int giDd = 0;                           //方向D参数 
int giDi = 0;                           //方向I参数
int giDk = 0;                           //方向K参数
int giAddOffset = 0;                    //方向偏差补偿
float gfDirectionErrI = 0;              //方向偏差I
float gfDirection_Pwm = 0;              //方向PWM
float gfDirectionPwmOld=0;              //上次方向PWM
float gfDirectionPWM_Normalize=0;       //方向归一化

/*
**路障相关
*/
int giBlockSpeed=0;                      //判断为障碍时的速度
int giRoadBlockDis=0;                    //遇到路障时候距离路障的距离
int giDirectStraight=0;                  //直走标志(当前舵机角度范围比较小)

/*
**SetMotorVoltag
*/
unsigned char StartStop=0;             //起跑线检测停车开关
unsigned char MotorStree=0;            //电机转动方向: 0:正转 1:反转
unsigned char guSpeedflag=0;           //速度控制标志(enable)

/**
**Function: DirectionControl
**Description: 方向控制
**Calls:   无
**Called By: PORTD5_IRQHandler
**           PORTA13_IRQHandler
**Table Accessed: 无
**Table Updated: 无
**Input: gfPosition
**       gfCurrentSpeed
**       gfCurrentSpeedLeft
**       giDp
**       giDd
**       giDk
**Output: gfDirectionPwmL
**        gfDirectionPwmR
**Return: 无
**Others:无
**/
void DirectionControl(void)
{   
    int i = 0;
    int liSpeedWheel=0;//giSpeedWheel
    double lfDirectionPWM = 0;
    
    //方向积分项(积分分离,方向偏差10内方向偏差积分)
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
    
    //积分与速度有关,低速状态下积分慢,高速状态下积分快
    if(gfRoadSitError[0] >= -25 && gfRoadSitError[0] <= 25)
    {
      gfDirectionErrI += giDi * gfRoadSitError[0] * liSpeedWheel * 0.00001;
    }
    
    else
    {
      gfDirectionErrI = 0;
    }
    
    gfDirection_Pwm = giDp * gfRoadSitError[0] * 0.1 + gfDirectionErrI + giDd * (gfRoadSitError[0] - gfRoadSitError[2]) * 0.1;
   
    if(guBeaconCountPlus==1)//信标个数加一时,D项计算错误，需要去除
    {
      gfDirection_Pwm = giDp * gfRoadSitError[0] * 0.1;
    }
    
    if(guSidePass == 1)
    {
      gfDirection_Pwm = gs_Derict_fuzzyPID.fuzzyP * gfRoadSitError[0] * 0.1 + gfDirectionErrI + giDd * (gfRoadSitError[0] - gfRoadSitError[2]) * 0.1;
    }
    
    /*直走标志判定*/
    if(gfDirection_Pwm > -200 && gfDirection_Pwm < 200
        && gfDirectionPwmOld > -200 && gfDirectionPwmOld < 200)
    {
      giDirectStraight = 1;
    }
    
    else
    {
      giDirectStraight = 0;
    }
    
    
    /*路障处理*/    
    if(guRoadBlock == 1)//路障标志
    {    
      if(guRoadBlockflag == 0)
      {
        giMeter_Sum_RoadBlock = 0;
        giBlockSpeed = giCurrentSpeed;//遇到路障时当前速度
        giRoadBlockDis = Ultrasonic_Dis[0];//遇到路障时当前距离
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
          giRoadBlockDis = 0;           //距离清零
          guRoadBlock = 0;              //路障标志清零
          giRoadBlockOffset = 0;        //路障偏移清零
          giMeter_Sum_RoadBlock = 0;    //路程计数清零
          guRoadBlockflag = 0;          //路障时间标志清零
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
    
    /*转回标志转向*/
    if(giTurnBack == 1 && giDp != 0)//回转标志后向左转
    {
        if(guBeaconCount < 15)//10信标内
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
    
    /*丢失信标处理*/
    if(giLostBeacon == 1 && giDp != 0)//丢失信标后向左转
    {
      if(guCircleLost == 1)
      {
        if(guBeaconCount < 15)//10信标内
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
    
    /*测试舵机的转向极限*/
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
    
    gfDirectionPWM = Motor_Center + gfDirection_Pwm;//当前舵机转向
    
    lfDirectionPWM = Motor_Center + gfDirection_Pwm;
    
    gfDirectionPwmOld = gfDirection_Pwm;//方向PWM记录值
    
    FTM_PWM_Duty(FTM2,CH0,lfDirectionPWM);
}

/**
**Function: SpeedControl
**Description: 速度控制偏角计算
**Calls:   无
**Called By: PORTB2_IRQHandler
**           PORTA13_IRQHandler
**Table Accessed: 无
**Table Updated: 无
**Input: giDesirSpeed
**       gfCurrentSpeed
**       giSpExpedite
**       giSpConstant
**       giSi
**       giSd
**Output: SpeedControl 
**Return: 无
**Others:无
**/
void SpeedControl(void)
{ 
  int i=0;
  float lfSpeedLow=0;           //当前给定转圈低速
  float lf_AlterSpeed=0;        //当前速度期望值
  float lf_AlterSpeed_Now=0;    //加入位置的速度期望值
  double lfSpeedPWM=0;        //速度PWM
    
  giCurrentSpeed = giSpeedWheel;//计算当前速度(当前速度等于轮速)
  giMeter_Sum += giCurrentSpeed;//距离积分
  
  if(giMeter_Sum > OneMeter)//4050脉冲是1M(待测)
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
  
  //信标期望低速(0:低速 1:中低速)
  if(guBeaconCount < 15)//10信标内
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
  
  else//超过10信标
  {
    lfSpeedLow = gfDesirSpeed_Low;
  }
  
  /*速度给定计算(行号距离)*/
  if(gfDesirSpeed_Low <= gfDesirSpeed_Mid && gfDesirSpeed_High >= gfDesirSpeed_Mid
      && gfDesirSpeed_Low != 0 && gfDesirSpeed_Mid != 0 && gfDesirSpeed_High != 0)
  {
    if(gfRowFilter > 30 && gfRowFilter <= 60)//信标当前行号在30 - 60范围之内(0.6M-1M)
    {        
      lf_AlterSpeed = gfDesirSpeed_Mid - (gfDesirSpeed_Mid - lfSpeedLow) * ((60 - gfRowFilter) * 0.034);
    }
    
    else if(gfRowFilter > 60 && gfRowFilter <= 75)//信标当前行号在60 - 75行范围内(1M-2M)
    {
      lf_AlterSpeed = gfDesirSpeed_High - (gfDesirSpeed_High - gfDesirSpeed_Mid) * ((75 - gfRowFilter) * 0.067);
    }
    
    else if(gfRowFilter > 75)//信标当前行号在75行以后(2M以上)
    {
      lf_AlterSpeed = gfDesirSpeed_High;
    }
      
    else//信标当前行号在30行以内(0.6M以内)
    {
      lf_AlterSpeed = lfSpeedLow;
    }
      
    //速度期望限制
    if(lf_AlterSpeed > gfDesirSpeed_High)
    {
      lf_AlterSpeed = gfDesirSpeed_High;
    }
      
    else if(lf_AlterSpeed < gfDesirSpeed_Low)
    {
      lf_AlterSpeed = gfDesirSpeed_Low;
    }
      
  }
  
  //信标丢失转圈时低速
  if(giTurnBack == 1 || giLostBeacon == 1)//返回标志或者丢失信标
  {
    lf_AlterSpeed = lfSpeedLow;//低速
  }
  else if(guRoadBlock == 1)//或者路障标志
  {
    lf_AlterSpeed = gfDesirSpeed_Low;//低速
  }
  
  //舵机角度归一化(角度期望速度)
  if(gfDirection_Pwm < 0)
  {
    gfDirectionPWM_Normalize = gfDirection_Pwm / Motor_Limit_L;
  }
  
  else
  {
    gfDirectionPWM_Normalize = gfDirection_Pwm / Motor_Limit_R;
  }
  
  //位置期望速度(角度推算)
  lf_AlterSpeed_Now = lf_AlterSpeed - (lf_AlterSpeed - lfSpeedLow) * gfDirectionPWM_Normalize;
  
  //期望速度低通滤波(加速平滑,平滑期望速度)
  if(guRoadBlock == 1)//路障标志
  {
    lf_AlterSpeed_Now = lf_AlterSpeed_Now;
  }
  else
  {
    if(giBeacon_Row > 75)  //信标在视野远方
    {
      lf_AlterSpeed_Now = gf_AlterSpeed_Old * 0.95 + lf_AlterSpeed_Now * 0.05;
    }
  }
  
  //位置期望速度限幅
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
  
  //左右轮速度偏差
  gfSpeedErr[0] = gfDesirSpeed - giSpeedWheel;
  gfSpeedErrGrad[0] = gfSpeedErr[0] - gfSpeedErr[3];
    
  if(Switch_On && StartCount >= 200)         //按键发车之后
  {
    if(gfSpeedErr[0] <= 30 && gfSpeedErr[0] >= -30)
    {
      gfSpeedErrI += gs_Speed_fuzzyPID.fuzzyI * gfSpeedErr[0] * 0.1;
    }
    
    else
    {
      gfSpeedErrI = 0;
    }
    
    //积分限幅
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
  
  //电机PWM输出滤波
  if(guRoadBlock == 1)//路障标志
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
    else if(giBeacon_Row > 60 && giBeacon_Row <= 75)  //近处开始减速
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
  
  //增量限幅
  if(lfSpeedPWM>99999)
  {
    lfSpeedPWM=99999;
  }
  
  else if(lfSpeedPWM<-99999)
  {
    lfSpeedPWM=-99999;
  }
  
  //PWM计算
  gfCurrentSpeedPWM = lfSpeedPWM;
  
  //上次PWM记录
  gfLastSpeedPWM = lfSpeedPWM;
  
  //期望速度记录
  gf_AlterSpeed_Old = lf_AlterSpeed_Now;
  
  //数组存储速度偏差
  for(i=3;i>0;i--)
  {
    gfSpeedErr[i]=gfSpeedErr[i-1];
    gfSpeedErrGrad[i]=gfSpeedErrGrad[i-1];
  }
}

/*
**Function: SetMotorVoltage
**Description: 输出PWM控制
**Calls:   无
**Called By: PORTD5_IRQHandler
**           PORTA13_IRQHandler
**Table Accessed: 无
**Table Updated: 无
**Input: gfAnglePWM
**       gfDirectionPwmL
**       gfDirectionPwmR
**Output: 电机控制量
**Return: 无
**Others:无
**/
void SetMotorVoltage(void) 
{ 
    double lfPWM = 0;
    
    //如果距离停车为0M,设置为300M
    if(giMeterSetValue == 0)
    {
      giMeterSetValue = 500;
    }
    
    if(Switch_Off || (giBeaconExcept != 0 && guBeaconCount == giBeaconExcept) || BeaconStart == 0)//开关关或者丢失信标标志
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
      
      else if(Switch_On && BeaconStart == 0)//开关开启但是没检测到开始亮灯
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
    
    else if(Switch_On && StartCount >= 200 && (giBeaconExcept == 0 || guBeaconCount < giBeaconExcept))//开关开并且计数标志
    {
      DriveSwitch_On;/*enable*/
    }
    
    lfPWM = gfCurrentSpeedPWM;
        
    if(lfPWM <= 0)//电机正反转
    {
      MotorInversion;//电机反转
      MotorStree = 1;
      //lfPWM = 100000 + lfPWM;
      lfPWM = -lfPWM;
    }
        
    else
    {
      MotorForeward;//电机正转
      MotorStree = 0;
      //lfPWM = lfPWM;
      lfPWM = 100000 - lfPWM;
    }
        
    //PWM输出限幅
    if(lfPWM > 99999)
    {
      lfPWM = 99999;
    }
        
    if (lfPWM < 0)     
    {
      lfPWM = 0;
    }
    
    gfSpeedPWM = (99999 - lfPWM) * 0.001;//速度PWM
        
    FTM_PWM_Duty(FTM0,CH3,lfPWM);//电机PWM

}