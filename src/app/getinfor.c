/*********************Copyright(C) 2015-2016 TYUT JBD TEAM B.*******************
Copyright (C), 2015-2016, TYUT JBD TEAM B.
File name: Getinfor.c 
Author: TEAM B, Alopex
Version: 1.0               
Date: 2015.10.28(E)
      2015.12.31(C)
      2016.03.15(A)
Description: 
            GetImage Function(摄像头信息采集函数)
            ExtractChangeNum Function(提取跳变点函数)
            Seek_Beacon Function(寻找信标函数)
Others: Noting
Function List:
            1.GetImage 游程编码采集跳变
            2.ExtractChangeNum 提取每行跳变个数
            3.Seek_Beacon 寻找信标
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
#include  "declare.h"

/*
**摄像头采集图像数组(游程编码)
*/
unsigned char guImgBuff[ROW+27][NUM]  = {0};

/*
**公共变量(浮点型)
*/
float gf_Public_Variable_Red = 0;     //红色
float gf_Public_Variable_Purple = 0;  //紫色
float gf_Public_Variable_Celeste = 0; //蓝色
float gf_Public_Variable_Green = 0;   //绿色

/*
**过零启动
*/
char STARTFLAG = 0;                           //过零启动标志

/*
**中心值
*/
float gfAngleCenter = 0;                      //加速度计中心值（直立）
float gfAngleSpeedCenter = 0;                 //陀螺仪中心值（静止）
float gfAngleRenew[2] = {0};                  //过零启动判断用

/*
**数据融合
*/
float gfAngleSpeedIntegral = 0;               //陀螺仪积分值
float gfAngleSpeedCurrent = 0;                //陀螺仪归一化后值
float gfAngle = 0;                            //融合后输出角度
float gfGyroIntegral = 0;                     //陀螺仪积分角度
float gfAccIntegral = 0;                      //加速度计积分输出速度值

/*
**信息采集
*/
int giRoadEdgeSit[5]={0};                     //中心值
int ChangeNum[ROW]={0};                       //每一行跳变点数
int ActEdge[ROW][NUM]={0};                    //实际图像二维数组
int ActCol[COL][NUM]={0};                     //实际图像主列数组
float gfRoadSitError[5]={0};                  //当前与期望中心偏差
float gfRoadSitErrorGrad[5]={0};              //当前与期望中心偏差的微分

/*
**CurrentLoop
*/
int CurrentLoop_Value = 0;                    //电流环电压AD值
int CurrentLoop_Normal = 0;                   //电流环电压标准值

/*
**信标转向次序
*/
char gcBeaconOrder[15]={0};                   //0:左转 1:右转
int giBeaconOrder1=0;                         //信标前5个次序
int giBeaconOrder2=0;                         //信标后5个次序
int giBeaconOrder3=0;                         //信标最后5个次序
int giExpectCOL=0;                            //当前期望信标列号

/*
**寻找信标
*/
BeaconVariable gs_OriginalBeacon[3][3];         //信标原始白区域
BeaconVariable gs_Beacon[5];                    //信标结构体
BeaconVariable *gs_PTemp_Beacon=&gs_Beacon[0];  //信标结构体指针
BeaconVariable gs_Beacon_Temp;

int giBeacon_Row=0;                             //信标行号
int giBeacon_Col=0;                             //信标列号
int giBeacon_Row_Top=0;                         //信标顶端行号
int giBeacon_Row_Bottom=0;                      //信标底端行号
int giBeacon_Col_Left=0;                        //信标左边列号
int giBeacon_Col_Right=0;                       //信标右边列号
int giBeacon_Area_Sum=0;                        //信标面积
int giDarkField=0;                              //暗场标志
int giLostBeacon=0;                             //丢失信标标志
int giTurnBack=0;                               //发现信标后返回标志
int giCenterCoord[3][2]={0};                    //记录每次搜索到的信标白坐标(3次)
int giTopBotEdge[ROW][2]={0};                   //边界数组
int giAreaSum_Predict[3]={0};                   //信标面积预测(滤除干扰)
int giWhiteArea_MaxCol[3]={0};                  //白区域最长列长度
float gfRowFilter=0;                            //行号低通滤波值
float gfRowFilter_Old=0;                        //上次行号低通滤波值
float gfCurrentSkew=0;                          //由信标行号推算当前偏移
unsigned char guCircleLost=0;                  //进入圆圈丢失信标
unsigned char guLastCircleLost=0;              //上次丢失圆圈丢失信标
unsigned char guCirclePass=0;                  //进圈通过标志
unsigned char guBeacon_Dark_field=0;           //信标暗场标志
unsigned char guSidePass=0;                    //发现信标以后从侧面通过标志
unsigned char guBeaconPassflag=0;              //信标通过由0->1标志
unsigned char gu_Disturb_flag=0;               //白色干扰标志
unsigned char guBeaconCount=0;                 //经过信标个数
unsigned char guBeaconCountPlus=0;             //信标个数加一标志(消除D项影响)

/*信标行号与距离*/
int giNextDis=0;                                //当前距离
float gf_Beacon_Distance=0;                     //小车和信标距离(由行号计算)
float gf_Beacon_Distance_Old=0;                 //上次距离值(由行号计算)
float gf_Beacon_Distance_Bot=0;                 //小车和信标距离(由底端行号计算)

/*蜂鸣器*/
unsigned char BuzzerNoise_flag=0;

/**
**Function: GetImage
**Description: 摄像头IO采集游程编码跳变点
**Calls: 无
**Called By: main.c
**Table Accessed: 无
**Table Updated: 无
**Input: 
**Output: guImgBuff
**Return: 无
**Others: 游程编码采集图像跳变点,用I/O宏比函数
          快26指令周期,if...else...语句比问号表达式
          指令数更少
**/
void GetImage(void)
{
    unsigned char *PTemp = &guImgBuff[guHrefElect][0];
    unsigned char *PTemptest=0;
    unsigned char  bit = 0;
    unsigned char  i = 0,j = 0,k = 0,ktest = 0;//下标偶数为黑
    
    //清除guImgBuff数组
    ClearArrayChar(&guImgBuff[guHrefElect][0],&guImgBuff[guHrefElect][NUM-1]);
    
    for (i = 0; i < 130; ++i);//场消隐延时
    
    for (j = 1; j < COL; ++j)//j = 0 start//COL1 不采集
    {
        //bit = gpio_get(PORTB, 11);/*33指令周期*/
        bit = CAMERA_IN;/*25指令周期*//*输入PTx_IN*/
        
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        
        asm("nop");
        asm("nop");
        asm("nop");//18nop
        
        if(bit!=k)
        {
            *PTemp=j;
            ++PTemp;
            k=1-k;
        } 
        else
        {
            ++PTemptest;
            --PTemptest;
            ktest=1-ktest;
        }
    }
    *PTemp=COL;
}

/**
**Function: ExtractChangeNum
**Description: 提取Imgbuff数组里的跳变点
**Calls:   无
**Called By: isr.c
**Table Accessed: 无
**Table Updated: 无
**Input: guImgBuff
**Output: ChangNum
**Return: 无
**Others: guImgBuff数组->ActEdge数组
          行号正序,无矫正
          guImgBuff数组->ActCol数组
          列号正序,无矫正
**/
void ExtractChangeNum(void)
{
  int i=0,j=0,k=0;
  int liWhiteflag=0;
  int liWhiteflagOld=0;
  int *PTest=&ActEdge[0][0];
  int *PTestEnd=&ActEdge[0][0];
  int *PTestCol=&ActCol[0][0];//列编码数组
  unsigned char *PTemp=&guImgBuff[0][0];
  unsigned char *PEnd=&guImgBuff[0][0];
  
  ClearArrayInt(&ChangeNum[0],&ChangeNum[ROW-1]);//清除ChangeNum数组
  ClearArrayInt(&ActEdge[0][0],&ActEdge[ROW-1][NUM-1]);//清除ActEdge数组
  ClearArrayInt(&ActCol[0][0],&ActCol[COL-1][NUM-1]);//清除ActCol数组
  
  for(i=0;i<ROW;i++)
  {
    PTest=&ActEdge[i][0];
    PTemp=&guImgBuff[ROW-i-1][0];
    PEnd=&guImgBuff[ROW-i-1][NUM-1];
    
    for(k=0;PTemp<PEnd && (*PTemp!=0||*(PTemp+1)!=0);PTemp++)
    {
      if(k%2==0&&*(PTemp+1)!=0&&*(PTemp+1)-*PTemp<=0)
      {
        if(*(PTemp+2)==0&&*(PTemp+1)!=COL)
        {
          *PTest=*(PTemp+1);
        }
        PTemp++;
        k+=2;
      }
      
      else if(*PTemp!=0)
      {
        *PTest=*PTemp;
        PTest++;
        ChangeNum[i]++;
        k++;
      }
    }
    ChangeNum[i]--;
  }
  
  //行编码转换列编码(时间很长,大约需要8ms)
  /*for(j=1;j<=COL;j++)
  {
    PTestCol=&ActCol[j-1][0];
    
    for(i=0;i<ROW;i++)
    {
      liWhiteflag=0;
      PTest=&ActEdge[i][0];
      PTestEnd=&ActEdge[i][NUM-1];
      
      for(k=0;PTest < PTestEnd && (*PTest!=0 || *(PTest+1)!=0);k++,PTest++)
      {
        if(k%2==0 && *PTest!=0 && *PTest!=COL && *(PTest+1)!=0  && *PTest <= j && (*(PTest+1) > *PTest))
        {
          if(*PTest <= j && *(PTest+1) >= j)
          {
            liWhiteflag = 1;
            break;
          }
          else
          {
            liWhiteflag = 0;
          }
        }
        else if(*PTest > j)
        {
          break;
        } 
      }
      
      if(liWhiteflag != liWhiteflagOld)
      {
        *PTestCol = i;
        PTestCol++;
      }
      
      liWhiteflagOld = liWhiteflag;
    }
    *PTestCol = ROW;
  }*/
}

/**
**Function: CurrentLoopValue_Get
**Description: 获取当前电流环AD并转换为基准值
**Calls:   GetAD
**Called By: PORTD5_IRQHandler
**           PORTA13_IRQHandler
**Table Accessed: 无
**Table Updated: 无
**Input: 
**Output:
**Return:
**Others:
**/
void CurrentLoopValue_Get(void)
{
  CurrentLoop_Value = ad_once(ADC0, AD20, ADC_12bit);
  CurrentLoop_Normal = CurrentLoop_Value - 2052;
}

/**
**Function: AngleCalculate
**Description: 加速度计纠正计算角度值
**Calls:   GetAD
**Called By: PORTD5_IRQHandler
**           PORTA13_IRQHandler
**Table Accessed: 无
**Table Updated: 无
**Input: gfAngleCenter
**       gfAngleSpeedCenter
**Output: gfAngle
**        gfAngleSpeedCurrent
**Return: 无
**Others:无
**/
void AngleCalculate(void)
{
    float lfDeltaValue = 0;                  //角度融合值与就角度AD值偏差
    float lfAngleCurrent = 0;                //加速度计归一化值
    float lfAngleAD = 0;                     //加速度计AD值
    float lfAngleSpeedAD = 0;                //陀螺仪AD值
    
    GetAD(&lfAngleAD,&lfAngleSpeedAD);       //获取陀螺仪加速度计AD值
    gfSendWhite=lfAngleAD;                   //串口发送加速度计AD值(白色曲线)
    
    //ACT:2040
    lfAngleCurrent = (gfAngleCenter - lfAngleAD) * 180 / (1109 - 3239);//加速度计归一化
    gfAngleSpeedCurrent  = (lfAngleSpeedAD - gfAngleSpeedCenter) * 0.169;//陀螺仪归一化        
   
    gfSendRed = lfAngleCurrent+10000;   //串口发送加速度计归一化(红色曲线)
            
    gfAngleRenew[1] = gfAngleRenew[0];       //历史
    gfAngleRenew[0] = lfAngleCurrent;        //当前
    
    if(gfAngleRenew[1] > 0 && gfAngleRenew[0] < 0) 
    {
        STARTFLAG = 1;                                                 
    }                                        //首次角度在中心值前后变动则过零启动
    
    if(STARTFLAG == 1)                       //过零启动                        
    {
        gfAngle = gfAngleSpeedIntegral;
        gfSendGreen = gfAngle+10000;    //串口发送角度(绿色曲线)
        lfDeltaValue = (lfAngleCurrent - gfAngle) * 0.8;//0.9;
        gfAngleSpeedIntegral += (gfAngleSpeedCurrent + lfDeltaValue) * 0.001216;//积分时间常数
    }                                        
}

/**
**Function: Seek_Beacon
**Description: 寻找信标
**Calls:   无
**Called By: isr.c
**Table Accessed: 无
**Table Updated: 无
**Input: 无
**Output: Coordinate
**Return: 无
**Others:无
**/
void Seek_Beacon(void)
{
    int i=0,j=0,k=0;
    int i0=0,j0=0,k0=0;
    int i1=0,j1=0,k1=0;
    int liWhiteMaxTemp=0;                       //白最大行临时变量
    int liWhiteRow_Top=0;                       //从坐标点向上扫到最远白行
    int liWhiteRow_Bottom=0;                    //从坐标点向下扫到最近白行
    int liWhiteCol_Left=0;                      //白最多行左边界
    int liWhiteCol_Right=0;                     //白最多行右边界
    int liWhiteNUM_Top=0;                       //信标白顶端
    int liWhiteNUM_Bottom=0;                    //信标白底端
    int liWhiteNUM_SUM=0;                       //信标白面积
    int liWhite_Max=0;
    int liWhiteMaxRow=0;                        //最宽行行号
    int liWhiteMaxCol=0;                        //最宽行列号
    int liEdgeColWidth=0;                       //信标边界最后一列宽度
    int liEdgeWhiteRowCount=0;                  //信标在边缘时候列白计数
    int liEdgeWhiteCol=0;                       //信标在边缘时候中心列号
    int liNextDis=0;                            //下一场距离
    int liActEdgeTemporary[ROW][NUM]={0};       //ActEdge数组寻找信标时临时用数组
    int *PTemp=&ActEdge[0][0];
    int *PEnd=&ActEdge[0][0];
    int *PTempTest=&liActEdgeTemporary[0][0];   //临时数组首指针
    int *PTempTestEnd=&liActEdgeTemporary[0][0];//临时数组末指针
    int *PTempStore=&liActEdgeTemporary[0][0];  //存储指针
    int *PEdgeTemp=&giTopBotEdge[0][0];         //边沿数组首指针
    float *PTemp_f=&gfRoadSitError[0];          //浮点数组首指针
    float *PEnd_f=&gfRoadSitError[0];           //浮点数组末指针
    unsigned char luUplineflag=0;              //目标行的上一行
    unsigned char luDownlineflag=0;            //目标行的下一行
    unsigned char luUplineCount=0;             //目标行上行计数
    unsigned char luDownlineCount=0;           //目标行的下行计数
    BeaconVariable *ls_PTemp=&gs_Beacon[0];     //信标结构体指针(首地址)
    BeaconVariable *ls_PEnd=&gs_Beacon[4];      //信标结构体指针(末地址)
    
    /*寻找信标边界变量*/
    int liLastRowEdgeL=0;                       //上一行信标左边界
    int liLastRowEdgeR=0;                       //上一行信标右边界
    int liMemoryJump[NUM]={0};                  //存储跳变白跳变点
    int *liMemoryJumpPTemp=&liMemoryJump[0];    //存储跳变白首地址
    int *liMemoryJumpPEnd=&liMemoryJump[0];     //存储跳变白末地址
    
    /*Attention Please,These Code Was Just Test*/
    /*寻找信标临时变量*/
    int liTestWhiteMaxTemp=0;                   //临时变量白最多行临时变量
    int liTestWhite_Max=0;                      //临时变量白最大行
    int liTestWhiteMaxRow=0;                    //临时变量最宽行行号
    int liTestWhiteMaxCol=0;                    //临时变量最宽行列号
    int liTestWhiteRow_Top=0;                   //临时变量白区域顶端行号
    int liTestWhiteRow_Bottom=0;                //临时变量白区域底端行号
    int liTestWhiteCol_Left=0;                  //临时变量白区域左列号
    int liTestWhiteCol_Right=0;                 //临时变量白区域右列号
    int liTestWhiteNUM_Top=0;                   //临时变量白区域顶端白数量
    int liTestWhiteNUM_Bottom=0;                //临时变量白区域底端白数量
    int liTestWhiteNUM_SUM=0;                   //临时变量白区域白面积
    int liTestEdgeL=0;                          //临时变量白区域左边
    int liTestEdgeR=0;                          //临时变量白区域右边
    int liClearJump[NUM]={0};                   //清除跳变点数组
    int *liClearJumpPTemp=&liClearJump[0];      //清除跳变点首指针
    int *liClearJumpPEnd=&liClearJump[0];       //清除跳变点末指针
    BeaconVariable ls_BeaconArray[3];           //寻找存储图像上3个白区域
    
    /*信标结构体初值*/
    gs_Beacon[0].Center_Coord_Row=0;
    gs_Beacon[0].Center_Coord_Col=0;
    gs_Beacon[0].Row_Top=0;
    gs_Beacon[0].Row_Bottom=0;
    gs_Beacon[0].Col_Left=0;
    gs_Beacon[0].Col_Right=0;
    gs_Beacon[0].Area_Sum=0;
    gs_Beacon[0].Darkfield=0;
    
    /*ActEdge数组复制*/
    for(PTemp=&ActEdge[0][0],PEnd=&ActEdge[ROW-1][NUM-1],PTempTest=&liActEdgeTemporary[0][0];PTemp<PEnd;PTemp++,PTempTest++)
    {
      *PTempTest=*PTemp;
    }
    
    //清数组
    ClearArrayInt(&giTopBotEdge[0][0],&giTopBotEdge[ROW-1][1]);//边界数组
    
    //gf_Public_Variable_Purple=0;
    /*Attention Please,These Code Was Just Test*/
    
    /*寻找3个白区域*/
    /*白区域1*/
    for(i=0;i<ROW;i++)//找白最长的一行
    {
      PTempTest=&liActEdgeTemporary[i][0];//首地址
      PTempTestEnd=&liActEdgeTemporary[i][NUM-1];//末地址
      
      for(j=0;PTempTest<PTempTestEnd && (*PTempTest!=0||*(PTempTest+1)!=0);j++,PTempTest++)
      {
        if(j%2==0 && *PTempTest != 0 && *PTempTest != COL && *(PTempTest+1)!=0 && *(PTempTest+1) > *PTempTest)
        {
          liTestWhiteMaxTemp = *(PTempTest+1) - *PTempTest;//白行宽度
          if(liTestWhiteMaxTemp > liTestWhite_Max)
          {
            liTestWhite_Max = liTestWhiteMaxTemp;//最大白行
            liTestWhiteMaxRow = i;
            liTestWhiteMaxCol = (*PTempTest + *(PTempTest+1)) * 0.5;
            liTestWhiteCol_Left = *PTempTest;
            liTestWhiteCol_Right = *(PTempTest+1);
          }
        }
      }
    }
    
    liTestEdgeL = liTestWhiteCol_Left;
    liTestEdgeR = liTestWhiteCol_Right;
    
    //向上清除最大白区域
    for(i = liTestWhiteMaxRow;i < ROW;i++)
    {
      PTempTest=&liActEdgeTemporary[i][0];//首地址
      PTempTestEnd=&liActEdgeTemporary[i][NUM-1];//末地址
      PTempStore=&liActEdgeTemporary[i][0];
      liClearJumpPTemp = &liClearJump[0];//指向待清除跳变点
      liClearJumpPEnd = &liClearJump[0];//指向待清除跳变点
      ClearArrayInt(&liClearJump[0],&liClearJump[NUM-1]);//清除跳变点数组
      
      if(*PTempTest == COL || *PTempTest == 0)
      {
        break;                                  //只要出现全黑行停止扫描
      }
      
      for(j=0;PTempTest < PTempTestEnd && (*PTempTest!=0 || *(PTempTest+1)!=0);j++,PTempTest++)
      {
        if(j%2==0 && *PTempTest != 0 && *PTempTest != COL && *(PTempTest+1)!=0 && *(PTempTest+1) > *PTempTest)
        { 
          //清除邻近的白
          if((liTestEdgeL <= *PTempTest && liTestEdgeR >= *(PTempTest+1))
             ||(liTestEdgeL <= *PTempTest && liTestEdgeR <= *(PTempTest+1) && liTestEdgeR >= *PTempTest)
               ||(liTestEdgeL >= *PTempTest && liTestEdgeR >= *(PTempTest+1) && liTestEdgeL <= *(PTempTest+1))
                 ||(liTestEdgeL >= *PTempTest && liTestEdgeR <= *(PTempTest+1)))
          {
            PTempStore = PTempTest;//存储当前地址
            liTestWhiteRow_Top = i;//白区域顶行行号
            liTestWhiteNUM_Top += (*(PTempTest+1) - *PTempTest);//白区域上半部分面积
            *liClearJumpPTemp++ = *PTempTest;
            *liClearJumpPTemp++ = *(PTempTest+1);
            for(;PTempTest+2 <= PTempTestEnd;PTempTest++)
            {
              *PTempTest = *(PTempTest + 2);
            }
            liActEdgeTemporary[i][NUM-2]=0;
            liActEdgeTemporary[i][NUM-1]=0;
            PTempTest = PTempStore;
            PTempTest--;
            j--;
          }
        }
      }
      
      //清除跳变点数组
      liTestEdgeL = liClearJump[0];
      for(liClearJumpPTemp = &liClearJump[0],liClearJumpPEnd = &liClearJump[NUM-1];
          liClearJumpPTemp < liClearJumpPEnd && *liClearJumpPTemp != 0;liClearJumpPTemp++)
      {
        liTestEdgeR = *liClearJumpPTemp;
      }
      
    }
    
    liTestEdgeL = liTestWhiteCol_Left;
    liTestEdgeR = liTestWhiteCol_Right;
    
    //向下清除最大白区域   
    for(i = liTestWhiteMaxRow - 1;i > 0;i--)
    {
      PTempTest=&liActEdgeTemporary[i][0];//首地址
      PTempTestEnd=&liActEdgeTemporary[i][NUM-1];//末地址
      PTempStore=&liActEdgeTemporary[i][0];
      liClearJumpPTemp = &liClearJump[0];//指向待清除跳变点
      liClearJumpPEnd = &liClearJump[0];//指向待清除跳变点
      ClearArrayInt(&liClearJump[0],&liClearJump[NUM-1]);//清除跳变点数组
      
      if(*PTempTest == COL || *PTempTest == 0)
      {
        break;                                  //只要出现全黑行停止扫描
      }
      
      for(j=0;PTempTest < PTempTestEnd && (*PTempTest!=0 || *(PTempTest+1)!=0);j++,PTempTest++)
      {
        if(j%2==0 && *PTempTest != 0 && *PTempTest != COL && *(PTempTest+1)!=0 && *(PTempTest+1) > *PTempTest)
        { 
          if((liTestEdgeL <= *PTempTest && liTestEdgeR >= *(PTempTest+1))
             ||(liTestEdgeL <= *PTempTest && liTestEdgeR <= *(PTempTest+1) && liTestEdgeR >= *PTempTest)
               ||(liTestEdgeL >= *PTempTest && liTestEdgeR >= *(PTempTest+1) && liTestEdgeL <= *(PTempTest+1))
                 ||(liTestEdgeL >= *PTempTest && liTestEdgeR <= *(PTempTest+1)))
          {
            PTempStore = PTempTest;//存储当前地址
            liTestWhiteRow_Bottom = i;//白区域顶行行号
            liTestWhiteNUM_Bottom += (*(PTempTest+1) - *PTempTest);//白区域上半部分面积
            *liClearJumpPTemp++ = *PTempTest;
            *liClearJumpPTemp++ = *(PTempTest+1);
            for(;PTempTest+2 <= PTempTestEnd;PTempTest++)
            {
              *PTempTest = *(PTempTest + 2);
            }
            liActEdgeTemporary[i][NUM-2]=0;
            liActEdgeTemporary[i][NUM-1]=0;
            PTempTest = PTempStore;
            PTempTest--;
            j--;
          }
        }
      }

      //清除跳变点数组
      liTestEdgeL = liClearJump[0];
      for(liClearJumpPTemp = &liClearJump[0],liClearJumpPEnd = &liClearJump[NUM-1];
          liClearJumpPTemp < liClearJumpPEnd && *liClearJumpPTemp != 0;liClearJumpPTemp++)
      {
        liTestEdgeR = *liClearJumpPTemp;
      }
    }
    
    liTestWhiteNUM_SUM = liTestWhiteNUM_Top + liTestWhiteNUM_Bottom;
    if(liTestWhiteRow_Top != 0 && liTestWhiteRow_Bottom != 0 && liTestWhiteRow_Top >= liTestWhiteRow_Bottom)
    {
      liTestWhiteMaxRow = (liTestWhiteRow_Top + liTestWhiteRow_Bottom) * 0.5;
      ls_BeaconArray[0].Row_Top = liTestWhiteRow_Top;
      ls_BeaconArray[0].Row_Bottom = liTestWhiteRow_Bottom;
    }
    else if(liTestWhiteRow_Top == 0 && liTestWhiteRow_Bottom != 0 && liTestWhiteMaxRow >= liTestWhiteRow_Bottom)
    {
      ls_BeaconArray[0].Row_Top = liTestWhiteMaxRow;
      ls_BeaconArray[0].Row_Bottom = liTestWhiteRow_Bottom;
    }
    else if(liTestWhiteRow_Top != 0 && liTestWhiteRow_Bottom == 0 && liTestWhiteMaxRow <= liTestWhiteRow_Top)
    {
      ls_BeaconArray[0].Row_Top = liTestWhiteRow_Top;
      ls_BeaconArray[0].Row_Bottom = liTestWhiteMaxRow;
    }
    else if(liTestWhiteRow_Top == 0 && liTestWhiteRow_Bottom == 0)
    {
      ls_BeaconArray[0].Row_Top = liTestWhiteMaxRow;
      ls_BeaconArray[0].Row_Bottom = liTestWhiteMaxRow;
    }
    
    /*中心坐标*/
    ls_BeaconArray[0].Center_Coord_Row = liTestWhiteMaxRow;
    ls_BeaconArray[0].Center_Coord_Col = liTestWhiteMaxCol;
    ls_BeaconArray[0].Col_Left = liTestWhiteCol_Left;
    ls_BeaconArray[0].Col_Right = liTestWhiteCol_Right;
    ls_BeaconArray[0].Area_Sum = liTestWhiteNUM_SUM;
    gs_OriginalBeacon[0][0].Center_Coord_Row=liTestWhiteMaxRow;
    gs_OriginalBeacon[0][0].Center_Coord_Col=liTestWhiteMaxCol;
    gs_OriginalBeacon[0][0].Row_Top=ls_BeaconArray[0].Row_Top;
    gs_OriginalBeacon[0][0].Row_Bottom=ls_BeaconArray[0].Row_Bottom;
    gs_OriginalBeacon[0][0].Col_Left=ls_BeaconArray[0].Col_Left;
    gs_OriginalBeacon[0][0].Col_Right=ls_BeaconArray[0].Col_Right;
    gs_OriginalBeacon[0][0].Area_Sum=ls_BeaconArray[0].Area_Sum;
    giWhiteArea_MaxCol[0]=gs_OriginalBeacon[0][0].Row_Top - gs_OriginalBeacon[0][0].Row_Bottom;
    
    /*白区域2*/
    liTestWhiteMaxTemp=0;
    liTestWhite_Max=0;
    liTestWhiteMaxRow=0;
    liTestWhiteMaxCol=0;
    liTestWhiteRow_Top=0;
    liTestWhiteRow_Bottom=0;
    liTestWhiteCol_Left=0;
    liTestWhiteCol_Right=0;
    liTestWhiteNUM_Top=0;
    liTestWhiteNUM_Bottom=0;
    liTestWhiteNUM_SUM=0;
    
    for(i=0;i<ROW;i++)//找白最长的一行
    {
      PTempTest=&liActEdgeTemporary[i][0];//首地址
      PTempTestEnd=&liActEdgeTemporary[i][NUM-1];//末地址
      
      for(j=0;PTempTest<PTempTestEnd && (*PTempTest!=0||*(PTempTest+1)!=0);j++,PTempTest++)
      {
        if(j%2==0 && *PTempTest != 0 && *PTempTest != COL && *(PTempTest+1)!=0 && *(PTempTest+1) > *PTempTest)
        {
          liTestWhiteMaxTemp = *(PTempTest+1) - *PTempTest;//白行宽度
          if(liTestWhiteMaxTemp > liTestWhite_Max)
          {
            liTestWhite_Max = liTestWhiteMaxTemp;//最大白行
            liTestWhiteMaxRow = i;
            liTestWhiteMaxCol = (*PTempTest + *(PTempTest+1)) * 0.5;
            liTestWhiteCol_Left = *PTempTest;
            liTestWhiteCol_Right = *(PTempTest+1);
          }
        }
      }
    }
    
    liTestEdgeL = liTestWhiteCol_Left;
    liTestEdgeR = liTestWhiteCol_Right;
    
    //向上清除最大白区域
    for(i = liTestWhiteMaxRow;i < ROW;i++)
    {
      PTempTest=&liActEdgeTemporary[i][0];//首地址
      PTempTestEnd=&liActEdgeTemporary[i][NUM-1];//末地址
      PTempStore=&liActEdgeTemporary[i][0];
      liClearJumpPTemp = &liClearJump[0];//指向待清除跳变点
      liClearJumpPEnd = &liClearJump[0];//指向待清除跳变点
      ClearArrayInt(&liClearJump[0],&liClearJump[NUM-1]);//清除跳变点数组
      
      if(*PTempTest == COL || *PTempTest == 0)
      {
        break;                                  //只要出现全黑行停止扫描
      }
      
      for(j=0;PTempTest < PTempTestEnd && (*PTempTest!=0 || *(PTempTest+1)!=0);j++,PTempTest++)
      {
        if(j%2==0 && *PTempTest != 0 && *PTempTest != COL && *(PTempTest+1)!=0 && *(PTempTest+1) > *PTempTest)
        {          
          if((liTestEdgeL <= *PTempTest && liTestEdgeR >= *(PTempTest+1))
             ||(liTestEdgeL <= *PTempTest && liTestEdgeR <= *(PTempTest+1) && liTestEdgeR >= *PTempTest)
               ||(liTestEdgeL >= *PTempTest && liTestEdgeR >= *(PTempTest+1) && liTestEdgeL <= *(PTempTest+1))
                 ||(liTestEdgeL >= *PTempTest && liTestEdgeR <= *(PTempTest+1)))
          {
            PTempStore = PTempTest;//存储当前地址
            liTestWhiteRow_Top = i;//白区域顶行行号
            liTestWhiteNUM_Top += (*(PTempTest+1) - *PTempTest);//白区域上半部分面积
            *liClearJumpPTemp++ = *PTempTest;
            *liClearJumpPTemp++ = *(PTempTest+1);
            for(;PTempTest+2 <= PTempTestEnd;PTempTest++)
            {
              *PTempTest = *(PTempTest + 2);
            }
            liActEdgeTemporary[i][NUM-2]=0;
            liActEdgeTemporary[i][NUM-1]=0;
            PTempTest = PTempStore;
            PTempTest--;
            j--;
          }
        }
      }
      
      //清除跳变点数组
      liTestEdgeL = liClearJump[0];
      for(liClearJumpPTemp = &liClearJump[0],liClearJumpPEnd = &liClearJump[NUM-1];
          liClearJumpPTemp < liClearJumpPEnd && *liClearJumpPTemp != 0;liClearJumpPTemp++)
      {
        liTestEdgeR = *liClearJumpPTemp;
      }
    }
    
    liTestEdgeL = liTestWhiteCol_Left;
    liTestEdgeR = liTestWhiteCol_Right;
    
    //向下清除最大白区域   
    for(i = liTestWhiteMaxRow - 1;i > 0;i--)
    {
      PTempTest=&liActEdgeTemporary[i][0];//首地址
      PTempTestEnd=&liActEdgeTemporary[i][NUM-1];//末地址
      PTempStore=&liActEdgeTemporary[i][0];
      liClearJumpPTemp = &liClearJump[0];//指向待清除跳变点
      liClearJumpPEnd = &liClearJump[0];//指向待清除跳变点
      ClearArrayInt(&liClearJump[0],&liClearJump[NUM-1]);//清除跳变点数组
      
      if(*PTempTest == COL || *PTempTest == 0)
      {
        break;                                  //只要出现全黑行停止扫描
      }
      
      for(j=0;PTempTest < PTempTestEnd && (*PTempTest!=0 || *(PTempTest+1)!=0);j++,PTempTest++)
      {
        if(j%2==0 && *PTempTest != 0 && *PTempTest != COL && *(PTempTest+1)!=0 && *(PTempTest+1) > *PTempTest)
        {
          if((liTestEdgeL <= *PTempTest && liTestEdgeR >= *(PTempTest+1))
             ||(liTestEdgeL <= *PTempTest && liTestEdgeR <= *(PTempTest+1) && liTestEdgeR >= *PTempTest)
               ||(liTestEdgeL >= *PTempTest && liTestEdgeR >= *(PTempTest+1) && liTestEdgeL <= *(PTempTest+1))
                 ||(liTestEdgeL >= *PTempTest && liTestEdgeR <= *(PTempTest+1)))
          {
            PTempStore = PTempTest;//存储当前地址
            liTestWhiteRow_Bottom = i;//白区域顶行行号
            liTestWhiteNUM_Bottom += (*(PTempTest+1) - *PTempTest);//白区域上半部分面积
            *liClearJumpPTemp++ = *PTempTest;
            *liClearJumpPTemp++ = *(PTempTest+1);
            for(;PTempTest+2 <= PTempTestEnd;PTempTest++)
            {
              *PTempTest = *(PTempTest + 2);
            }
            liActEdgeTemporary[i][NUM-2]=0;
            liActEdgeTemporary[i][NUM-1]=0;
            PTempTest = PTempStore;
            PTempTest--;
            j--;
          }
        }
      }
      
      //清除跳变点数组
      liTestEdgeL = liClearJump[0];
      for(liClearJumpPTemp = &liClearJump[0],liClearJumpPEnd = &liClearJump[NUM-1];
          liClearJumpPTemp < liClearJumpPEnd && *liClearJumpPTemp != 0;liClearJumpPTemp++)
      {
        liTestEdgeR = *liClearJumpPTemp;
      }
    }
    
    liTestWhiteNUM_SUM = liTestWhiteNUM_Top + liTestWhiteNUM_Bottom;
    if(liTestWhiteRow_Top != 0 && liTestWhiteRow_Bottom != 0 && liTestWhiteRow_Top >= liTestWhiteRow_Bottom)
    {
      liTestWhiteMaxRow = (liTestWhiteRow_Top + liTestWhiteRow_Bottom) * 0.5;
      ls_BeaconArray[1].Row_Top = liTestWhiteRow_Top;
      ls_BeaconArray[1].Row_Bottom = liTestWhiteRow_Bottom;
    }
    else if(liTestWhiteRow_Top == 0 && liTestWhiteRow_Bottom != 0 && liTestWhiteMaxRow >= liTestWhiteRow_Bottom)
    {
      ls_BeaconArray[1].Row_Top = liTestWhiteMaxRow;
      ls_BeaconArray[1].Row_Bottom = liTestWhiteRow_Bottom;
    }
    else if(liTestWhiteRow_Top != 0 && liTestWhiteRow_Bottom == 0 && liTestWhiteMaxRow <= liTestWhiteRow_Top)
    {
      ls_BeaconArray[1].Row_Top = liTestWhiteRow_Top;
      ls_BeaconArray[1].Row_Bottom = liTestWhiteMaxRow;
    }
    else if(liTestWhiteRow_Top == 0 && liTestWhiteRow_Bottom == 0)
    {
      ls_BeaconArray[1].Row_Top = liTestWhiteMaxRow;
      ls_BeaconArray[1].Row_Bottom = liTestWhiteMaxRow;
    }
    
    /*中心坐标*/
    ls_BeaconArray[1].Center_Coord_Row = liTestWhiteMaxRow;
    ls_BeaconArray[1].Center_Coord_Col = liTestWhiteMaxCol;
    ls_BeaconArray[1].Col_Left = liTestWhiteCol_Left;
    ls_BeaconArray[1].Col_Right = liTestWhiteCol_Right;
    ls_BeaconArray[1].Area_Sum = liTestWhiteNUM_SUM;
    gs_OriginalBeacon[0][1].Center_Coord_Row=liTestWhiteMaxRow;
    gs_OriginalBeacon[0][1].Center_Coord_Col=liTestWhiteMaxCol;
    gs_OriginalBeacon[0][1].Row_Top=ls_BeaconArray[1].Row_Top;
    gs_OriginalBeacon[0][1].Row_Bottom=ls_BeaconArray[1].Row_Bottom;
    gs_OriginalBeacon[0][1].Col_Left=ls_BeaconArray[1].Col_Left;
    gs_OriginalBeacon[0][1].Col_Right=ls_BeaconArray[1].Col_Right;
    gs_OriginalBeacon[0][1].Area_Sum=ls_BeaconArray[1].Area_Sum;
    giWhiteArea_MaxCol[1]=gs_OriginalBeacon[0][1].Row_Top - gs_OriginalBeacon[0][1].Row_Bottom;
    
    /*白区域3*/
    liTestWhiteMaxTemp=0;
    liTestWhite_Max=0;
    liTestWhiteMaxRow=0;
    liTestWhiteMaxCol=0;
    liTestWhiteRow_Top=0;
    liTestWhiteRow_Bottom=0;
    liTestWhiteCol_Left=0;
    liTestWhiteCol_Right=0;
    liTestWhiteNUM_Top=0;
    liTestWhiteNUM_Bottom=0;
    liTestWhiteNUM_SUM=0;
    
    for(i=0;i<ROW;i++)//找白最长的一行
    {
      PTempTest=&liActEdgeTemporary[i][0];//首地址
      PTempTestEnd=&liActEdgeTemporary[i][NUM-1];//末地址
      
      for(j=0;PTempTest<PTempTestEnd && (*PTempTest!=0||*(PTempTest+1)!=0);j++,PTempTest++)
      {
        if(j%2==0 && *PTempTest != 0 && *PTempTest != COL && *(PTempTest+1)!=0 && *(PTempTest+1) > *PTempTest)
        {
          liTestWhiteMaxTemp = *(PTempTest+1) - *PTempTest;//白行宽度
          if(liTestWhiteMaxTemp > liTestWhite_Max)
          {
            liTestWhite_Max = liTestWhiteMaxTemp;//最大白行
            liTestWhiteMaxRow = i;
            liTestWhiteMaxCol = (*PTempTest + *(PTempTest+1)) * 0.5;
            liTestWhiteCol_Left = *PTempTest;
            liTestWhiteCol_Right = *(PTempTest+1);
          }
        }
      }
    }
    
    liTestEdgeL = liTestWhiteCol_Left;
    liTestEdgeR = liTestWhiteCol_Right;
    
    //向上清除最大白区域
    for(i = liTestWhiteMaxRow;i < ROW;i++)
    {
      PTempTest=&liActEdgeTemporary[i][0];//首地址
      PTempTestEnd=&liActEdgeTemporary[i][NUM-1];//末地址
      PTempStore=&liActEdgeTemporary[i][0];
      liClearJumpPTemp = &liClearJump[0];//指向待清除跳变点
      liClearJumpPEnd = &liClearJump[0];//指向待清除跳变点
      ClearArrayInt(&liClearJump[0],&liClearJump[NUM-1]);//清除跳变点数组
      
      if(*PTempTest == COL || *PTempTest == 0)
      {
        break;                                  //只要出现全黑行停止扫描
      }
      
      for(j=0;PTempTest < PTempTestEnd && (*PTempTest!=0 || *(PTempTest+1)!=0);j++,PTempTest++)
      {
        if(j%2==0 && *PTempTest != 0 && *PTempTest != COL && *(PTempTest+1)!=0 && *(PTempTest+1) > *PTempTest)
        {        
          if((liTestEdgeL <= *PTempTest && liTestEdgeR >= *(PTempTest+1))
             ||(liTestEdgeL <= *PTempTest && liTestEdgeR <= *(PTempTest+1) && liTestEdgeR >= *PTempTest)
               ||(liTestEdgeL >= *PTempTest && liTestEdgeR >= *(PTempTest+1) && liTestEdgeL <= *(PTempTest+1))
                 ||(liTestEdgeL >= *PTempTest && liTestEdgeR <= *(PTempTest+1)))
          {
            PTempStore = PTempTest;//存储当前地址
            liTestWhiteRow_Top = i;//白区域顶行行号
            liTestWhiteNUM_Top += (*(PTempTest+1) - *PTempTest);//白区域上半部分面积
            *liClearJumpPTemp++ = *PTempTest;
            *liClearJumpPTemp++ = *(PTempTest+1);
            for(;PTempTest+2 <= PTempTestEnd;PTempTest++)
            {
              *PTempTest = *(PTempTest + 2);
            }
            liActEdgeTemporary[i][NUM-2]=0;
            liActEdgeTemporary[i][NUM-1]=0;
            PTempTest = PTempStore;
            PTempTest--;
            j--;
          }
        }
      }
      
      //清除跳变点数组
      liTestEdgeL = liClearJump[0];
      for(liClearJumpPTemp = &liClearJump[0],liClearJumpPEnd = &liClearJump[NUM-1];
          liClearJumpPTemp < liClearJumpPEnd && *liClearJumpPTemp != 0;liClearJumpPTemp++)
      {
        liTestEdgeR = *liClearJumpPTemp;
      }
    }
    
    liTestEdgeL = liTestWhiteCol_Left;
    liTestEdgeR = liTestWhiteCol_Right;
    
    //向下清除最大白区域   
    for(i = liTestWhiteMaxRow - 1;i > 0;i--)
    {
      PTempTest=&liActEdgeTemporary[i][0];//首地址
      PTempTestEnd=&liActEdgeTemporary[i][NUM-1];//末地址
      PTempStore=&liActEdgeTemporary[i][0];
      liClearJumpPTemp = &liClearJump[0];//指向待清除跳变点
      liClearJumpPEnd = &liClearJump[0];//指向待清除跳变点
      ClearArrayInt(&liClearJump[0],&liClearJump[NUM-1]);//清除跳变点数组
      
      if(*PTempTest == COL || *PTempTest == 0)
      {
        break;                                  //只要出现全黑行停止扫描
      }
      
      for(j=0;PTempTest < PTempTestEnd && (*PTempTest!=0 || *(PTempTest+1)!=0);j++,PTempTest++)
      {
        if(j%2==0 && *PTempTest != 0 && *PTempTest != COL && *(PTempTest+1)!=0 && *(PTempTest+1) > *PTempTest)
        {
          if((liTestEdgeL <= *PTempTest && liTestEdgeR >= *(PTempTest+1))
             ||(liTestEdgeL <= *PTempTest && liTestEdgeR <= *(PTempTest+1) && liTestEdgeR >= *PTempTest)
               ||(liTestEdgeL >= *PTempTest && liTestEdgeR >= *(PTempTest+1) && liTestEdgeL <= *(PTempTest+1))
                 ||(liTestEdgeL >= *PTempTest && liTestEdgeR <= *(PTempTest+1)))
          {
            PTempStore = PTempTest;//存储当前地址
            liTestWhiteRow_Bottom = i;//白区域顶行行号
            liTestWhiteNUM_Bottom += (*(PTempTest+1) - *PTempTest);//白区域上半部分面积
            *liClearJumpPTemp++ = *PTempTest;
            *liClearJumpPTemp++ = *(PTempTest+1);
            for(;PTempTest+2 <= PTempTestEnd;PTempTest++)
            {
              *PTempTest = *(PTempTest + 2);
            }
            liActEdgeTemporary[i][NUM-2]=0;
            liActEdgeTemporary[i][NUM-1]=0;
            PTempTest = PTempStore;
            PTempTest--;
            j--;
          }
        }
      }

      //清除跳变点数组
      liTestEdgeL = liClearJump[0];
      for(liClearJumpPTemp = &liClearJump[0],liClearJumpPEnd = &liClearJump[NUM-1];
          liClearJumpPTemp < liClearJumpPEnd && *liClearJumpPTemp != 0;liClearJumpPTemp++)
      {
        liTestEdgeR = *liClearJumpPTemp;
      }
    }
    
    liTestWhiteNUM_SUM = liTestWhiteNUM_Top + liTestWhiteNUM_Bottom;
    if(liTestWhiteRow_Top != 0 && liTestWhiteRow_Bottom != 0 && liTestWhiteRow_Top >= liTestWhiteRow_Bottom)
    {
      liTestWhiteMaxRow = (liTestWhiteRow_Top + liTestWhiteRow_Bottom) * 0.5;
      ls_BeaconArray[2].Row_Top = liTestWhiteRow_Top;
      ls_BeaconArray[2].Row_Bottom = liTestWhiteRow_Bottom;
    }
    else if(liTestWhiteRow_Top == 0 && liTestWhiteRow_Bottom != 0 && liTestWhiteMaxRow >= liTestWhiteRow_Bottom)
    {
      ls_BeaconArray[2].Row_Top = liTestWhiteMaxRow;
      ls_BeaconArray[2].Row_Bottom = liTestWhiteRow_Bottom;
    }
    else if(liTestWhiteRow_Top != 0 && liTestWhiteRow_Bottom == 0 && liTestWhiteMaxRow <= liTestWhiteRow_Top)
    {
      ls_BeaconArray[2].Row_Top = liTestWhiteRow_Top;
      ls_BeaconArray[2].Row_Bottom = liTestWhiteMaxRow;
    }
    else if(liTestWhiteRow_Top == 0 && liTestWhiteRow_Bottom == 0)
    {
      ls_BeaconArray[2].Row_Top = liTestWhiteMaxRow;
      ls_BeaconArray[2].Row_Bottom = liTestWhiteMaxRow;
    }
    
    /*中心坐标*/
    ls_BeaconArray[2].Center_Coord_Row = liTestWhiteMaxRow;
    ls_BeaconArray[2].Center_Coord_Col = liTestWhiteMaxCol;
    ls_BeaconArray[2].Col_Left = liTestWhiteCol_Left;
    ls_BeaconArray[2].Col_Right = liTestWhiteCol_Right;
    ls_BeaconArray[2].Area_Sum = liTestWhiteNUM_SUM;
    gs_OriginalBeacon[0][2].Center_Coord_Row=liTestWhiteMaxRow;
    gs_OriginalBeacon[0][2].Center_Coord_Col=liTestWhiteMaxCol;
    gs_OriginalBeacon[0][2].Row_Top=ls_BeaconArray[2].Row_Top;
    gs_OriginalBeacon[0][2].Row_Bottom=ls_BeaconArray[2].Row_Bottom;
    gs_OriginalBeacon[0][2].Col_Left=ls_BeaconArray[2].Col_Left;
    gs_OriginalBeacon[0][2].Col_Right=ls_BeaconArray[2].Col_Right;
    gs_OriginalBeacon[0][2].Area_Sum=ls_BeaconArray[2].Area_Sum;
    giWhiteArea_MaxCol[2]=gs_OriginalBeacon[0][2].Row_Top - gs_OriginalBeacon[0][2].Row_Bottom;
    
    /*****************************Attention Above*******************************/
    
    /*liWhiteMaxTemp = 0;
    
    //找白最长的一列
    for(j=0;j<COL;j++)
    {
      PTemp=&ActCol[j][0];                     //每列首地址
      PEnd=&ActCol[j][NUM-1];                  //每列末地址
      
      for(k=0;PTemp < PEnd && (*PTemp!=0 || *(PTemp+1)!=0);k++,PTemp++)
      {
        if(k%2==0 && *PTemp!=0 && *PTemp!=ROW && *(PTemp+1)!=0 && (*(PTemp+1) > *PTemp))
        {
          liWhiteMaxTemp = *(PTemp+1) - *PTemp;
          
          if(liWhiteMaxTemp > liWhiteCol_Max)
          {
            liWhiteCol_Max = liWhiteMaxTemp;
            liWhiteCol_Max_Row = (*PTemp + *(PTemp+1)) * 0.5;
            liWhiteCol_Max_Col = j+1;
          }   
        }
      }
    }
    
    liWhiteMaxRow = liWhiteCol_Max_Row;
    liWhiteMaxCol = liWhiteCol_Max_Col;
    
    liWhiteMaxTemp = 0;
    
    for(PTemp=&ActEdge[liWhiteMaxRow][0],PEnd=&ActEdge[liWhiteMaxRow][NUM-1];PTemp < PEnd;PTemp++)
    {
      for(j=0;PTemp < PEnd && (*PTemp!=0 || *(PTemp+1)!=0);j++,PTemp++)
      {
        if(j%2==0 && *PTemp!=0 && *PTemp!=COL && *(PTemp+1)!=0 && (*(PTemp+1) > *PTemp))
        {
          if(liWhiteMaxCol >= *PTemp && liWhiteMaxCol <= *(PTemp+1))
          {
            liWhiteCol_Left = *PTemp;
            liWhiteCol_Right = *(PTemp+1);
            break;
          }
        }
      }
    }*/
    
    //找白最长的一行
    for(i=0;i<ROW;i++)
    {
      PTemp=&ActEdge[i][0];                     //每行首地址
      PEnd=&ActEdge[i][NUM-1];                  //每行末地址
      
      for(j=0;PTemp < PEnd && (*PTemp!=0 || *(PTemp+1)!=0);j++,PTemp++)
      {
        if(j%2==0 && *PTemp!=0 && *PTemp!=COL && *(PTemp+1)!=0 && (*(PTemp+1) > *PTemp))
        {
          liWhiteMaxTemp = *(PTemp+1) - *PTemp;
          
          if(liWhiteMaxTemp > liWhite_Max)
          {
            liWhite_Max = liWhiteMaxTemp;
            liWhiteMaxRow = i;
            liWhiteMaxCol = (*PTemp + *(PTemp+1)) * 0.5;
            liWhiteCol_Left = *PTemp;
            liWhiteCol_Right = *(PTemp+1);
            giTopBotEdge[i][0] = *PTemp;
            giTopBotEdge[i][1] = *(PTemp+1);
          }
        }
      }
    }
    
    //信标白区域列长度最长的为信标
    if(giWhiteArea_MaxCol[0] >= giWhiteArea_MaxCol[1]
        && giWhiteArea_MaxCol[0] >= giWhiteArea_MaxCol[2])//白区域1列长度最大
    {
      liWhiteMaxRow = liWhiteMaxRow;
      liWhiteMaxCol = liWhiteMaxCol;
      liWhiteCol_Left = liWhiteCol_Left;
      liWhiteCol_Right = liWhiteCol_Right;
    }
    
    else if(giWhiteArea_MaxCol[1] >= giWhiteArea_MaxCol[0]
              && giWhiteArea_MaxCol[1] >= giWhiteArea_MaxCol[2])//白区域2列长度最大
    {
      liWhiteMaxRow = gs_OriginalBeacon[0][1].Center_Coord_Row;
      liWhiteMaxCol = gs_OriginalBeacon[0][1].Center_Coord_Col;
      liWhiteCol_Left = gs_OriginalBeacon[0][1].Col_Left;
      liWhiteCol_Right = gs_OriginalBeacon[0][1].Col_Right;
    }
    
    else if(giWhiteArea_MaxCol[2] >= giWhiteArea_MaxCol[0]
              && giWhiteArea_MaxCol[2] >= giWhiteArea_MaxCol[1])//白区域3列长度最大
    {
      liWhiteMaxRow = gs_OriginalBeacon[0][2].Center_Coord_Row;
      liWhiteMaxCol = gs_OriginalBeacon[0][2].Center_Coord_Col;
      liWhiteCol_Left = gs_OriginalBeacon[0][2].Col_Left;
      liWhiteCol_Right = gs_OriginalBeacon[0][2].Col_Right;
    }
    
    //白最大行边界
    liLastRowEdgeL = liWhiteCol_Left;
    liLastRowEdgeR = liWhiteCol_Right;
    
    //从白色最大行向上扫
    for(i0 = liWhiteMaxRow;i0 < ROW;i0++)
    {
      PTemp=&ActEdge[i0][0];                     //每行首地址
      PEnd=&ActEdge[i0][NUM-1];                  //每行末地址
      PEdgeTemp=&giTopBotEdge[i0][0];            //边界首地址
      liMemoryJumpPTemp=&liMemoryJump[0];        //存储白跳变首地址
      liMemoryJumpPEnd=&liMemoryJump[0];         //存储白跳变末地址
      ClearArrayInt(&liMemoryJump[0],&liMemoryJump[NUM-1]);//清数组
      
      if(*PTemp==COL || *PTemp==0)               //只要出现全黑行停止扫描
      {
        break;
      }
      
      for(j0=0;PTemp < PEnd && (*PTemp!=0 || *(PTemp+1)!=0);j0++,PTemp++)
      {
        if(j0%2==0 && *PTemp!=0 && *PTemp!=COL && *(PTemp+1)!=0 && (*(PTemp+1) > *PTemp))
        {
          if((liLastRowEdgeL <= *PTemp && liLastRowEdgeR >= *(PTemp+1))
             ||(liLastRowEdgeL <= *PTemp && liLastRowEdgeR <= *(PTemp+1) && liLastRowEdgeR >= *PTemp)
               ||(liLastRowEdgeL >= *PTemp && liLastRowEdgeR >= *(PTemp+1) && liLastRowEdgeL <= *(PTemp+1))
                 ||(liLastRowEdgeL >= *PTemp && liLastRowEdgeR <= *(PTemp+1)))
          {
            liWhiteRow_Top = i0;
            liWhiteNUM_Top += (*(PTemp+1) - *PTemp);
            *liMemoryJumpPTemp++ = *PTemp;
            *liMemoryJumpPTemp++ = *(PTemp+1);
          }
        }
      }
      
      liLastRowEdgeL = liMemoryJump[0];
      for(liMemoryJumpPTemp = &liMemoryJump[0],liMemoryJumpPEnd = &liMemoryJump[NUM-1];
          liMemoryJumpPTemp < liMemoryJumpPEnd && *liMemoryJumpPTemp != 0;liMemoryJumpPTemp++)
      {
        liLastRowEdgeR = *liMemoryJumpPTemp;
      }
      *PEdgeTemp = liLastRowEdgeL;
      *++PEdgeTemp = liLastRowEdgeR;
    }
    
    //白最大行边界
    liLastRowEdgeL = liWhiteCol_Left;
    liLastRowEdgeR = liWhiteCol_Right;
    
    //从白色最大行向下扫
    for(i0 = liWhiteMaxRow;i0 > 0;i0--)
    {
      PTemp=&ActEdge[i0][0];                     //每行首地址
      PEnd=&ActEdge[i0][NUM-1];                  //每行末地址
      PEdgeTemp=&giTopBotEdge[i0][0];            //边界首地址
      liMemoryJumpPTemp=&liMemoryJump[0];        //存储白跳变首地址
      liMemoryJumpPEnd=&liMemoryJump[0];         //存储白跳变末地址
      ClearArrayInt(&liMemoryJump[0],&liMemoryJump[NUM-1]);//清数组
      
      if(*PTemp==COL || *PTemp==0)               //只要出现全黑行停止扫描
      {
        break;
      }
      
      for(j0=0;PTemp < PEnd && (*PTemp!=0 || *(PTemp+1)!=0);j0++,PTemp++)
      {
        if(j0%2==0 && *PTemp!=0 && *PTemp!=COL && *(PTemp+1)!=0 && (*(PTemp+1) > *PTemp))
        {
          if((liLastRowEdgeL <= *PTemp && liLastRowEdgeR >= *(PTemp+1))
             ||(liLastRowEdgeL <= *PTemp && liLastRowEdgeR <= *(PTemp+1) && liLastRowEdgeR >= *PTemp)
               ||(liLastRowEdgeL >= *PTemp && liLastRowEdgeR >= *(PTemp+1) && liLastRowEdgeL <= *(PTemp+1))
                 ||(liLastRowEdgeL >= *PTemp && liLastRowEdgeR <= *(PTemp+1)))
          {
            liWhiteRow_Bottom = i0;
            liWhiteNUM_Bottom += (*(PTemp+1) - *PTemp);
            *liMemoryJumpPTemp++ = *PTemp;
            *liMemoryJumpPTemp++ = *(PTemp+1);
          }
        }
      }
      
      liLastRowEdgeL = liMemoryJump[0];
      for(liMemoryJumpPTemp = &liMemoryJump[0],liMemoryJumpPEnd = &liMemoryJump[NUM-1];
          liMemoryJumpPTemp < liMemoryJumpPEnd && *liMemoryJumpPTemp != 0;liMemoryJumpPTemp++)
      {
        liLastRowEdgeR = *liMemoryJumpPTemp;
      }
      *PEdgeTemp = liLastRowEdgeL;
      *++PEdgeTemp = liLastRowEdgeR;
    }
    
    liWhiteNUM_SUM=liWhiteNUM_Top+liWhiteNUM_Bottom;//信标白大小
    gs_Beacon[0].Area_Sum=liWhiteNUM_SUM;           //信标面积
    
    //顶端和底端都有白
    if(liWhiteRow_Top!=0 && liWhiteRow_Bottom!=0 && liWhiteRow_Top >= liWhiteRow_Bottom)
    {      
      liWhiteMaxRow=(liWhiteRow_Top + liWhiteRow_Bottom)*0.5;
      
      //最亮场信标与暗场前半亮场信标比较
      
      //行比较
      if(liWhiteMaxRow - gs_Beacon[2].Center_Coord_Row >= -5
         && liWhiteMaxRow - gs_Beacon[2].Center_Coord_Row <= 5
          && gs_Beacon[2].Row_Bottom != 0 && gs_Beacon[2].Row_Top != 0
            && gs_Beacon[2].Row_Bottom >= liWhiteRow_Bottom
              && gs_Beacon[2].Row_Top <= liWhiteRow_Top)
      {
        liWhiteMaxRow = gs_Beacon[2].Center_Coord_Row;
      }
      
      else
      {
        liWhiteMaxRow = liWhiteMaxRow;
      }
      
      //列比较
      if(liWhiteMaxCol - gs_Beacon[2].Center_Coord_Col >= -5
         && liWhiteMaxCol - gs_Beacon[2].Center_Coord_Col <= 5
          && gs_Beacon[2].Col_Left != 0 && gs_Beacon[2].Col_Right != 0
           && gs_Beacon[2].Col_Left >= liWhiteCol_Left
             && gs_Beacon[2].Col_Right <= liWhiteCol_Right)
      {
        liWhiteMaxCol = gs_Beacon[2].Center_Coord_Col;
      }
      
      else
      {
        liWhiteMaxCol = liWhiteMaxCol;
      }
      
      //考虑到信标在摄像头边缘时信标列坐标//liEdgeColWidth
      if(liWhiteCol_Left <= liWhiteCol_Right && liWhiteCol_Right == COL)//左边缘小于右边缘,右边缘等于最大列号
      {
        //计算信标白最后一列宽度
        for(i0 = liWhiteMaxRow;i0 < ROW;i0++)//向上查找
        {
          PTemp=&ActEdge[i0][0];                     //每行首地址
          PEnd=&ActEdge[i0][NUM-1];                  //每行末地址
          
          if(*PTemp==COL || *PTemp==0)               //只要出现全黑行停止扫描
          {
            break;
          }
          
          for(k0=0;PTemp<PEnd && (*PTemp!=0 || *(PTemp+1)!=0);k0++,PTemp++)
          {
            if(k0%2==0 && *PTemp!=0 && *PTemp!=COL && *(PTemp+1)!=0 && (*(PTemp+1) > *PTemp))
            {
              if(*(PTemp+1) == COL)
              {
                liEdgeColWidth++;
              }
              
              else
              {
                liEdgeColWidth = liEdgeColWidth;
              }
            }
          }
        }
        
        for(i0 = liWhiteMaxRow;i0 >= 0;i0--)//向下查找
        {
          PTemp=&ActEdge[i0][0];                     //每行首地址
          PEnd=&ActEdge[i0][NUM-1];                  //每行末地址
          
          if(*PTemp==COL || *PTemp==0)               //只要出现全黑行停止扫描
          {
            break;
          }
          
          for(k0=0;PTemp<PEnd && (*PTemp!=0 || *(PTemp+1)!=0);k0++,PTemp++)
          {
            if(k0%2==0 && *PTemp!=0 && *PTemp!=COL && *(PTemp+1)!=0 && (*(PTemp+1) > *PTemp))
            {
              if(*(PTemp+1) == COL)
              {
                liEdgeColWidth++;
              }
              
              else
              {
                liEdgeColWidth = liEdgeColWidth;
              }
            }
          }
        }
        
        //寻找宽度相同的列号,计算中心
        for(i1 = liWhiteCol_Left;i1 <= liWhiteCol_Right;i1++)
        {
          liEdgeWhiteRowCount = 0;//每一列计数清零
          
          for(j1 = liWhiteMaxRow;j1 < ROW;j1++)
          {
            PTemp=&ActEdge[j1][0];                     //每行首地址
            PEnd=&ActEdge[j1][NUM-1];                  //每行末地址
            
            if(*PTemp==COL || *PTemp==0)               //只要出现全黑行停止扫描
            {
              break;
            }
            
            for(k1=0;PTemp<PEnd && (*PTemp!=0 || *(PTemp+1)!=0);k1++,PTemp++)
            {
              if(k1%2==0 && *PTemp!=0 && *PTemp!=COL && *(PTemp+1)!=0 && (*(PTemp+1) > *PTemp))
              {
                if(i1 >= *PTemp && i1 <= *(PTemp+1))//白色
                {
                  liEdgeWhiteRowCount++;
                }
                
                else
                {
                  liEdgeWhiteRowCount = liEdgeWhiteRowCount;
                }
              }
            }
          }
          
          for(j1 = liWhiteMaxRow;j1 >= 0;j1--)
          {
            PTemp=&ActEdge[j1][0];                     //每行首地址
            PEnd=&ActEdge[j1][NUM-1];                  //每行末地址
            
            if(*PTemp==COL || *PTemp==0)               //只要出现全黑行停止扫描
            {
              break;
            }
            
            for(k1=0;PTemp<PEnd && (*PTemp!=0 || *(PTemp+1)!=0);k1++,PTemp++)
            {
              if(k1%2==0 && *PTemp!=0 && *PTemp!=COL && *(PTemp+1)!=0 && (*(PTemp+1) > *PTemp))
              {
                if(i1 >= *PTemp && i1 <= *(PTemp+1))//白色
                {
                  liEdgeWhiteRowCount++;
                }
                
                else
                {
                  liEdgeWhiteRowCount = liEdgeWhiteRowCount;
                }
              }
            }
          }
          
          if(liEdgeColWidth !=0)
          {
            if(liEdgeWhiteRowCount > liEdgeColWidth)//找到比COL列大的列号
            {
              liEdgeWhiteCol = i1;
              break;//跳出搜索
            }
            
            else//COL列最大
            {
              liEdgeWhiteCol = COL;
            }            
          }
        }
      }
      
      if(liWhiteCol_Right == COL)
      {
        liWhiteMaxCol = (liEdgeWhiteCol + COL) * 0.5;
      }
      
      else
      {
        liWhiteMaxCol = liWhiteMaxCol;
      }
      
      gs_Beacon[0].Row_Top=liWhiteRow_Top;            //信标顶端行号
      gs_Beacon[0].Row_Bottom=liWhiteRow_Bottom;      //信标底端行号
      gs_Beacon[0].Col_Left=liWhiteCol_Left;          //信标左边沿列号
      gs_Beacon[0].Col_Right=liWhiteCol_Right;        //信标右边沿列号
    }
    
    //顶端没有白，底端有白
    else if(liWhiteRow_Top==0 && liWhiteRow_Bottom!=0 && liWhiteRow_Bottom <= liWhiteMaxRow)
    {
      gs_Beacon[0].Row_Top=liWhiteMaxRow;
      gs_Beacon[0].Row_Bottom=liWhiteRow_Bottom;
    }
    
    //顶端有白，底端没有白
    else if(liWhiteRow_Bottom==0 && liWhiteRow_Top!=0 && liWhiteRow_Top >= liWhiteMaxRow)
    {
      gs_Beacon[0].Row_Top=liWhiteRow_Top;
      gs_Beacon[0].Row_Bottom=liWhiteMaxRow;
    }
    
    //顶端和底端均没有白
    else if(liWhiteRow_Top==0 && liWhiteRow_Bottom==0)
    {
      gs_Beacon[0].Row_Top=liWhiteMaxRow;
      gs_Beacon[0].Row_Bottom=liWhiteMaxRow;
    }
    
    if(liWhiteMaxRow < 0)
    {
      liWhiteMaxRow = 0;
    }
    
    else if(liWhiteMaxRow > ROW-1)
    {
      liWhiteMaxRow = ROW-1;
    }  
    
    //寻找中心点上下两行是否为白
    //下一行
    if(liWhiteMaxRow>0 && liWhiteMaxRow< ROW-1)
    {
      //白最大行边界,中心行左右边界
      liLastRowEdgeL = liWhiteCol_Left;
      liLastRowEdgeR = liWhiteCol_Right;  
      
      PTemp=&ActEdge[liWhiteMaxRow-1][0];
      PEnd=&ActEdge[liWhiteMaxRow-1][NUM-1];
      
      for(k0=0;PTemp<PEnd && (*PTemp!=0 || *(PTemp+1)!=0);k0++,PTemp++)
      {
        if(k0%2==0 && *PTemp!=0 && *PTemp!=COL && *(PTemp+1)!=0 && (*(PTemp+1) > *PTemp))
        {
          /*if(liWhiteMaxCol >= *PTemp && liWhiteMaxCol <= *(PTemp+1)
             && *(PTemp+1) - *PTemp >= 3)
          {
            luDownlineflag=1;
            break;
          }
          
          else
          {
            luDownlineflag=0;
          }*/
          
          if((liLastRowEdgeL <= *PTemp && liLastRowEdgeR >= *(PTemp+1))
             ||(liLastRowEdgeL <= *PTemp && liLastRowEdgeR <= *(PTemp+1) && liLastRowEdgeR >= *PTemp)
               ||(liLastRowEdgeL >= *PTemp && liLastRowEdgeR >= *(PTemp+1) && liLastRowEdgeL <= *(PTemp+1))
                 ||(liLastRowEdgeL >= *PTemp && liLastRowEdgeR <= *(PTemp+1)))
          {
            luDownlineflag=1;
            break;
          }
          
          else
          {
            luDownlineflag=0;
          }
        }
      }
      
      //白最大行边界,中心行左右边界
      //上一行
      liLastRowEdgeL = liWhiteCol_Left;
      liLastRowEdgeR = liWhiteCol_Right; 
      
      PTemp=&ActEdge[liWhiteMaxRow+1][0];
      PEnd=&ActEdge[liWhiteMaxRow+1][NUM-1];
      
      for(k0=0;PTemp<PEnd && (*PTemp!=0 || *(PTemp+1)!=0);k0++,PTemp++)
      {
        if(k0%2==0 && *PTemp!=0 && *PTemp!=COL && *(PTemp+1)!=0 && (*(PTemp+1) > *PTemp))
        {
          /*if(liWhiteMaxCol >= *PTemp && liWhiteMaxCol <= *(PTemp+1)
             && *(PTemp+1) - *PTemp >= 3)
          {
            luUplineflag=1;
            break;
          }
          
          else
          {
            luUplineflag=0;
          }*/
          
          if((liLastRowEdgeL <= *PTemp && liLastRowEdgeR >= *(PTemp+1))
             ||(liLastRowEdgeL <= *PTemp && liLastRowEdgeR <= *(PTemp+1) && liLastRowEdgeR >= *PTemp)
               ||(liLastRowEdgeL >= *PTemp && liLastRowEdgeR >= *(PTemp+1) && liLastRowEdgeL <= *(PTemp+1))
                 ||(liLastRowEdgeL >= *PTemp && liLastRowEdgeR <= *(PTemp+1)))
          {
            luDownlineflag=1;
            break;
          }
          
          else
          {
            luDownlineflag=0;
          }
        }
      }
      
      //考虑信标半亮状态形状不规则,增加向上向下找白
      if(luUplineflag == 0 && luDownlineflag == 1)
      {        
         /*PTemp=&ActEdge[liWhiteMaxRow-2][0];
         PEnd=&ActEdge[liWhiteMaxRow-2][NUM-1];
         for(k0=0;PTemp<PEnd && (*PTemp!=0 || *(PTemp+1)!=0);k0++,PTemp++)
         { 
            if(k0%2==0 && *PTemp!=0 && *PTemp!=COL && *(PTemp+1)!=0 && (*(PTemp+1) > *PTemp))
            {
              if(liWhiteMaxCol >= *PTemp && liWhiteMaxCol <= *(PTemp+1)
                 && *(PTemp+1) - *PTemp >= 3)
              {
                luUplineflag=1;
                break;
              }
          
              else
              {
                luUplineflag=0;
              }
            }
          }*/
        
        if(liWhiteMaxRow - liWhiteRow_Bottom >= 2)
        {
          luUplineflag=1;
        }
        
        else
        {
          luUplineflag=0; 
        }
      }
      
      else if(luUplineflag == 1 && luDownlineflag == 0)
      {
         /*PTemp=&ActEdge[liWhiteMaxRow+2][0];
         PEnd=&ActEdge[liWhiteMaxRow+2][NUM-1];
         for(k0=0;PTemp<PEnd && (*PTemp!=0 || *(PTemp+1)!=0);k0++,PTemp++)
         { 
            if(k0%2==0 && *PTemp!=0 && *PTemp!=COL && *(PTemp+1)!=0 && (*(PTemp+1) > *PTemp))
            {
              if(liWhiteMaxCol >= *PTemp && liWhiteMaxCol <= *(PTemp+1)
                 && *(PTemp+1) - *PTemp >= 3)
              {
                luDownlineflag=1;
                break;
              }
          
              else
              {
                luDownlineflag=0;
              }
            }
          }*/
        
        if(liWhiteRow_Top - liWhiteMaxRow >= 2)
        {
          luDownlineflag=1;
        }
        
        else
        {
          luDownlineflag=0; 
        }
      }
      
      //远处信标比较小的情况(5行两个白)
     if((luUplineflag == 0 || luDownlineflag == 0)
              && liWhiteMaxRow >= 5 && liWhiteMaxRow <= ROW-6)
      {
        for(k=0;k<=4;k++)
        {
           PTemp=&ActEdge[liWhiteMaxRow+k+1][0];
           PEnd=&ActEdge[liWhiteMaxRow+k+1][NUM-1];
           for(k0=0;PTemp<PEnd && (*PTemp!=0 || *(PTemp+1)!=0);k0++,PTemp++)
           {
             if(k0%2==0 && *PTemp!=0 && *PTemp!=COL && *(PTemp+1)!=0 && (*(PTemp+1) > *PTemp))
             {
               if(liWhiteMaxCol >= *PTemp && liWhiteMaxCol <= *(PTemp+1)
                  && *(PTemp+1) - *PTemp >= 2)
               {
                 luUplineCount++;
               }
               
               else
               {
                 luUplineCount = luUplineCount;
               }
             }
           }
           
           PTemp=&ActEdge[liWhiteMaxRow-k-1][0];
           PEnd=&ActEdge[liWhiteMaxRow-k-1][NUM-1];
           for(k0=0;PTemp<PEnd && (*PTemp!=0 || *(PTemp+1)!=0);k0++,PTemp++)
           {
             if(k0%2==0 && *PTemp!=0 && *PTemp!=COL && *(PTemp+1)!=0 && (*(PTemp+1) > *PTemp))
             {
               if(liWhiteMaxCol >= *PTemp && liWhiteMaxCol <= *(PTemp+1)
                  && *(PTemp+1) - *PTemp >= 2)
               {
                 luDownlineCount++;
               }
               
               else
               {
                 luDownlineCount = luDownlineCount;
               }
             }
           }
        }
        
        if(luUplineCount > 5)
        {
          luUplineCount = 5;
        }
        
        if(luDownlineCount > 5)
        {
          luDownlineCount = 5;
        }
        
        if(luUplineCount + luDownlineCount >= 5)
        {
          luUplineflag = 1;
          luDownlineflag = 1;
        }
        
      }
      
    }
    
    //最近行情况    
    else if(liWhiteMaxRow == 0)
    {
      //白最大行边界,中心行左右边界
      liLastRowEdgeL = liWhiteCol_Left;
      liLastRowEdgeR = liWhiteCol_Right;
      
      PTemp=&ActEdge[liWhiteMaxRow+1][0];
      PEnd=&ActEdge[liWhiteMaxRow+1][NUM-1];
      
      for(k0=0;PTemp<PEnd && (*PTemp!=0 || *(PTemp+1)!=0);k0++,PTemp++)
      {
        if(k0%2==0 && *PTemp!=0 && *PTemp!=COL && *(PTemp+1)!=0 && (*(PTemp+1) > *PTemp))
        {
          /*if(liWhiteMaxCol >= *PTemp && liWhiteMaxCol <= *(PTemp+1)
             && *(PTemp+1) - *PTemp >= 3)
          {
            luUplineflag=1;
            break;
          }
          
          else
          {
            luUplineflag=0;
          }*/
          
          if((liLastRowEdgeL <= *PTemp && liLastRowEdgeR >= *(PTemp+1))
             ||(liLastRowEdgeL <= *PTemp && liLastRowEdgeR <= *(PTemp+1) && liLastRowEdgeR >= *PTemp)
               ||(liLastRowEdgeL >= *PTemp && liLastRowEdgeR >= *(PTemp+1) && liLastRowEdgeL <= *(PTemp+1))
                 ||(liLastRowEdgeL >= *PTemp && liLastRowEdgeR <= *(PTemp+1)))
          {
            luUplineflag=1;
            break;
          }
          
          else
          {
            luUplineflag=0;
          }
        }
      }
      
      //白最大行边界,中心行左右边界
      liLastRowEdgeL = liWhiteCol_Left;
      liLastRowEdgeR = liWhiteCol_Right;
      
      PTemp=&ActEdge[liWhiteMaxRow+2][0];
      PEnd=&ActEdge[liWhiteMaxRow+2][NUM-1];
      
      for(k0=0;PTemp<PEnd && (*PTemp!=0 || *(PTemp+1)!=0);k0++,PTemp++)
      {
        if(k0%2==0 && *PTemp!=0 && *PTemp!=COL && *(PTemp+1)!=0 && (*(PTemp+1) > *PTemp))
        {
          /*if(liWhiteMaxCol >= *PTemp && liWhiteMaxCol <= *(PTemp+1)
             && *(PTemp+1) - *PTemp >= 3)
          {
            luDownlineflag=1;
            break;
          }
          
          else
          {
            luDownlineflag=0;
          }*/
          
          if((liLastRowEdgeL <= *PTemp && liLastRowEdgeR >= *(PTemp+1))
             ||(liLastRowEdgeL <= *PTemp && liLastRowEdgeR <= *(PTemp+1) && liLastRowEdgeR >= *PTemp)
               ||(liLastRowEdgeL >= *PTemp && liLastRowEdgeR >= *(PTemp+1) && liLastRowEdgeL <= *(PTemp+1))
                 ||(liLastRowEdgeL >= *PTemp && liLastRowEdgeR <= *(PTemp+1)))
          {
            luDownlineflag=1;
            break;
          }
          
          else
          {
            luDownlineflag=0;
          }
        }
      }
    }
    
    //最远端情况
    else
    {
      //白最大行边界,中心行左右边界
      liLastRowEdgeL = liWhiteCol_Left;
      liLastRowEdgeR = liWhiteCol_Right;
      
      PTemp=&ActEdge[liWhiteMaxRow-1][0];
      PEnd=&ActEdge[liWhiteMaxRow-1][NUM-1];
      
      for(k0=0;PTemp<PEnd && (*PTemp!=0 || *(PTemp+1)!=0);k0++,PTemp++)
      {
        if(k0%2==0 && *PTemp!=0 && *PTemp!=COL && *(PTemp+1)!=0 && (*(PTemp+1) > *PTemp))
        {
          /*if(liWhiteMaxCol >= *PTemp && liWhiteMaxCol <= *(PTemp+1)
             && *(PTemp+1) - *PTemp >= 3)
          {
            luDownlineflag=1;
            break;
          }
          
          else
          {
            luDownlineflag=0;
          }*/
          
          if((liLastRowEdgeL <= *PTemp && liLastRowEdgeR >= *(PTemp+1))
             ||(liLastRowEdgeL <= *PTemp && liLastRowEdgeR <= *(PTemp+1) && liLastRowEdgeR >= *PTemp)
               ||(liLastRowEdgeL >= *PTemp && liLastRowEdgeR >= *(PTemp+1) && liLastRowEdgeL <= *(PTemp+1))
                 ||(liLastRowEdgeL >= *PTemp && liLastRowEdgeR <= *(PTemp+1)))
          {
            luDownlineflag=1;
            break;
          }
          
          else
          {
            luDownlineflag=0;
          }
        }
      }
      
      //白最大行边界,中心行左右边界
      liLastRowEdgeL = liWhiteCol_Left;
      liLastRowEdgeR = liWhiteCol_Right;
      
      PTemp=&ActEdge[liWhiteMaxRow-2][0];
      PEnd=&ActEdge[liWhiteMaxRow-2][NUM-1];
      
      for(k0=0;PTemp<PEnd && (*PTemp!=0 || *(PTemp+1)!=0);k0++,PTemp++)
      {
        if(k0%2==0 && *PTemp!=0 && *PTemp!=COL && *(PTemp+1)!=0 && (*(PTemp+1) > *PTemp))
        {
          /*if(liWhiteMaxCol >= *PTemp && liWhiteMaxCol <= *(PTemp+1)
             && *(PTemp+1) - *PTemp >= 3)
          {
            luUplineflag=1;
            break;
          }
          
          else
          {
            luUplineflag=0;
          }*/
          
          if((liLastRowEdgeL <= *PTemp && liLastRowEdgeR >= *(PTemp+1))
             ||(liLastRowEdgeL <= *PTemp && liLastRowEdgeR <= *(PTemp+1) && liLastRowEdgeR >= *PTemp)
               ||(liLastRowEdgeL >= *PTemp && liLastRowEdgeR >= *(PTemp+1) && liLastRowEdgeL <= *(PTemp+1))
                 ||(liLastRowEdgeL >= *PTemp && liLastRowEdgeR <= *(PTemp+1)))
          {
            luUplineflag=1;
            break;
          }
          
          else
          {
            luUplineflag=0;
          }
        }
      }
    }
    
    //本次搜索信标坐标
    giCenterCoord[0][0] = liWhiteMaxRow;
    giCenterCoord[0][1] = liWhiteMaxCol;
    
    //计算信标中心坐标位置
    if(liWhiteMaxRow != 0 && liWhiteMaxCol != 0)//liWhite_Max>=2 && luDownlineflag==1 && luUplineflag==1//liWhiteMaxRow != 0 && liWhiteMaxCol != 0
    {
      gs_Beacon[0].Center_Coord_Row = liWhiteMaxRow;
      gs_Beacon[0].Center_Coord_Col = liWhiteMaxCol;
      gs_Beacon[0].Darkfield = 0;
      guCircleLost = 0;
      giDarkField = 0;                                    //暗场标志清零
      giLostBeacon = 0;                                   //丢失信标标志清零
    }
    
    else
    {
      gs_Beacon[0].Center_Coord_Row = gs_Beacon[1].Center_Coord_Row;
      gs_Beacon[0].Center_Coord_Col = gs_Beacon[1].Center_Coord_Col;
      gs_Beacon[0].Row_Top = gs_Beacon[1].Row_Top;
      gs_Beacon[0].Row_Bottom = gs_Beacon[1].Row_Bottom;
      gs_Beacon[0].Darkfield = 1;
      giDarkField++;
      if(giDarkField > 3)
      {
        giDarkField = 3;                                //连续3暗场
      }
    }
    
    giBeacon_Row = gs_Beacon[0].Center_Coord_Row;
    giBeacon_Col = gs_Beacon[0].Center_Coord_Col;
    giBeacon_Row_Top = gs_Beacon[0].Row_Top;
    giBeacon_Row_Bottom = gs_Beacon[0].Row_Bottom;
    giBeacon_Col_Left = gs_Beacon[0].Col_Left;
    giBeacon_Col_Right = gs_Beacon[0].Col_Right;
    giBeacon_Area_Sum = gs_Beacon[0].Area_Sum;
    guBeacon_Dark_field = gs_Beacon[0].Darkfield;
    
    //信标行号列号低通滤波
    gfRowFilter = giBeacon_Row * 0.50 + gfRowFilter_Old * 0.50;
    gfRowFilter_Old = gfRowFilter;
    
    //信标距离
    gf_Beacon_Distance = 0.00004645 * pow(gs_Beacon[0].Center_Coord_Row,3) - 0.00694 * pow(gs_Beacon[0].Center_Coord_Row,2) + 0.3671 * gs_Beacon[0].Center_Coord_Row - 6.054;
        
    if(gf_Beacon_Distance < 0)
    {
      gf_Beacon_Distance = 0;
    }
    else if(gf_Beacon_Distance > 9.0)
    {
      gf_Beacon_Distance = 9.0;
    }
    
    //BeaconStart(起跑发车标志位)
    if(gs_Beacon[0].Center_Coord_Row != 0 && gs_Beacon[0].Center_Coord_Col != 0
        && gs_Beacon[0].Darkfield == 0 && BeaconStart == 0 && Switch_On)
    {
      BeaconStart = 1;
    }
     
    //信标数计数
    if(giBeaconExcept == 0)//未输入期望信标个数
    {
      //找到新的信标灯
      if(gs_Beacon[0].Center_Coord_Row != 0 && gs_Beacon[0].Center_Coord_Col != 0
          && gs_Beacon[1].Center_Coord_Row != 0 && gs_Beacon[1].Center_Coord_Col != 0
            && BeaconStart == 1 && BeaconStartOld == 1
              && gs_Beacon[0].Darkfield == 0 && ((gf_Beacon_Distance_Old <= 1.00 && gs_Beacon[1].Row_Bottom <= 30) || (guLastCircleLost == 1 && gs_Beacon[1].Darkfield == 1))
                && (gs_Beacon[0].Row_Bottom - gs_Beacon[1].Row_Bottom) >= 20)
      {
        guBeaconCount++;
        
        if(guBeaconCount > 100)
        {
          guBeaconCount = 100;
        }
        
        else if(guBeaconCount > giBeaconExcept && giBeaconExcept != 0)
        {
          guBeaconCount = giBeaconExcept;
        }
        
        guBeaconCountPlus = 1;
        BuzzerNoise_flag = 0;
        //Buzzer_noise;//寻找到新的信标,蜂鸣器bee
      }
      
      else
      {
        BuzzerNoise_flag++;
        if(BuzzerNoise_flag > 5)
        {
          BuzzerNoise_flag = 5;
        }
        if(BuzzerNoise_flag == 5)
        {
          //Buzzer_quite;//未找到新的信标
          BuzzerNoise_flag = 5;
        }
        guBeaconCountPlus = 0;
      }
    }
    
    else if(giBeaconExcept != 0 && guBeaconCount+1 < giBeaconExcept)//信标个数加一小于期望信标数
    {
      //找到新的信标灯(上次靠近信标1M,本次信标与车相距1M以上)
      if(gs_Beacon[0].Center_Coord_Row != 0 && gs_Beacon[0].Center_Coord_Col != 0
          && gs_Beacon[1].Center_Coord_Row != 0 && gs_Beacon[1].Center_Coord_Col != 0
            && BeaconStart == 1 && BeaconStartOld == 1
              && gs_Beacon[0].Darkfield == 0 && ((gf_Beacon_Distance_Old <= 1.00 && gs_Beacon[1].Row_Bottom <= 30) || (guLastCircleLost == 1 && gs_Beacon[1].Darkfield == 1))
                && (gs_Beacon[0].Row_Bottom - gs_Beacon[1].Row_Bottom) >= 20)
      {
        guBeaconCount++;
        guBeaconCountPlus = 1;
        BuzzerNoise_flag = 0;
        //Buzzer_noise;//寻找到新的信标,蜂鸣器biu
      }
      
      else
      {
        BuzzerNoise_flag++;
        if(BuzzerNoise_flag > 5)
        {
          BuzzerNoise_flag = 5;
        }
        if(BuzzerNoise_flag == 5)
        {
          //Buzzer_quite;//未找到新的信标
          BuzzerNoise_flag = 5;
        }
        guBeaconCountPlus = 0;
      }
    }
    
    else if(giBeaconExcept != 0 && guBeaconCount+1 == giBeaconExcept)//计数最后一个信标,暗场确定
    {
      if(gs_Beacon[0].Center_Coord_Row != 0 && gs_Beacon[0].Center_Coord_Col != 0
          && gs_Beacon[1].Center_Coord_Row != 0 && gs_Beacon[1].Center_Coord_Col != 0
            && BeaconStart == 1 && BeaconStartOld == 1
              && ((gs_Beacon[0].Darkfield == 0 && (gf_Beacon_Distance_Old <= 1.00 && gs_Beacon[1].Row_Bottom <= 30) && (gs_Beacon[0].Row_Bottom - gs_Beacon[1].Row_Bottom) >= 20)
                ||(gs_Beacon[0].Darkfield == 1 && gs_Beacon[1].Darkfield == 0 && guCircleLost == 1)))//由亮场到暗场或是两个信标距离满足条件
      {
        guBeaconCount++;
        guBeaconCountPlus = 1;
        BuzzerNoise_flag = 0;
        //Buzzer_noise;//寻找到新的信标,蜂鸣器biu
      }
      
      else
      {
        BuzzerNoise_flag++;
        if(BuzzerNoise_flag > 5)
        {
          BuzzerNoise_flag = 5;
        }
        if(BuzzerNoise_flag == 5)
        {
          //Buzzer_quite;//未找到新的信标
          BuzzerNoise_flag = 5;
        }
        guBeaconCountPlus = 0;
      }
    }
    
    else if(giBeaconExcept != 0 && guBeaconCount+1 > giBeaconExcept)//信标计数计满
    {
      guBeaconCount = giBeaconExcept;
    }
    
    //信标偏移量
    gfCurrentSkew = 0.01377 * pow(gs_Beacon[0].Center_Coord_Row,2) - 2.689 * gs_Beacon[0].Center_Coord_Row + 130.0;
    
    if(gfCurrentSkew < 0)//范围限制
    {
      gfCurrentSkew = 0;
    } 
    else if(gfCurrentSkew > 65)//60
    {
      gfCurrentSkew = 65;
    }
    
    if(gs_Beacon[0].Center_Coord_Row > 97)//行数大于80行偏移为0,注意每次拟合时改数据
    {
      gfCurrentSkew = 0;
    }
        
    giRoadEdgeSit[0] = gs_Beacon[0].Center_Coord_Col;
    
    //信标次序
    if(guBeaconCount < 15)//10信标内
    {
      if(gcBeaconOrder[guBeaconCount] == 0)
      {
        gfRoadSitError[0] = giRoadEdgeSit[0]-COL/2-gfCurrentSkew;
        giExpectCOL = COL/2+gfCurrentSkew;
      }
      else if(gcBeaconOrder[guBeaconCount] == 1)
      {
        gfRoadSitError[0] = giRoadEdgeSit[0]-COL/2+gfCurrentSkew;
        giExpectCOL = COL/2-gfCurrentSkew;
      }
    }
    
    else//超过10信标
    {
      #ifdef LeftModel
      gfRoadSitError[0] = giRoadEdgeSit[0]-COL/2-gfCurrentSkew;
      #elif defined RightModel
      gfRoadSitError[0] = giRoadEdgeSit[0]-COL/2+gfCurrentSkew;
      #else
      gfRoadSitError[0] = giRoadEdgeSit[0]-COL/2-gfCurrentSkew;
      #endif
    }
    
    /*信标坐标平滑*/
    //gfRoadSitError[0] = gfRoadSitError[0] * 0.90 + gfRoadSitError[1] * 0.10;
    gfRoadSitErrorGrad[0] = gfRoadSitError[0] - gfRoadSitError[2];
    
    //信标在视野远处3场丢失信标
    if(giBeacon_Row > 68)
    {
      if(guCirclePass == 3)        //接近找到信标后再发现3次暗场清标志
      {
        giLostBeacon = 1;
        guSidePass = 0;           //连续暗场3次后清除侧边通过标识
        guCirclePass = 0;
        giTurnBack = 0;
        gfCurrentSkew = 0;
        guRoadBlockCount = 0;
      }
      
      //两次信标之间没有暗场,清除TurnBack标志
      else if((gs_Beacon[0].Center_Coord_Row - gs_Beacon[1].Center_Coord_Row >= 10
              || gs_Beacon[0].Center_Coord_Row - gs_Beacon[2].Center_Coord_Row >= 10)
              && guSidePass == 1)//侧向通过标志置位并且信标行号有突然变大
      {
        guSidePass = 0;//侧向通过标志清零
        guCirclePass = 0;//侧向通过标志清零
        giTurnBack = 0;//giTurnBack 标志清零
      }
    }
    //信标在视野近处1场丢失信标
    else
    {
      if(giDarkField >= 1)        //接近找到信标后再发现3次暗场清标志
      {
        giLostBeacon = 1;
        if(guCirclePass == 1)
        {
          guCircleLost = 1;
        }
        guSidePass = 0;           //连续暗场3次后清除侧边通过标识
        guCirclePass = 0;
        giTurnBack = 0;
        gfCurrentSkew = 0;
        guRoadBlockCount = 0;
      }
      
      //两次信标之间没有暗场,清除TurnBack标志
      else if((gs_Beacon[0].Center_Coord_Row - gs_Beacon[1].Center_Coord_Row >= 10
              || gs_Beacon[0].Center_Coord_Row - gs_Beacon[2].Center_Coord_Row >= 10)
              && guSidePass == 1)//侧向通过标志置位并且信标行号有突然变大
      {
        guSidePass = 0;//侧向通过标志清零
        guCirclePass = 0;//侧向通过标志清零
        giTurnBack = 0;//giTurnBack 标志清零
      }
    }
    
    if(gs_Beacon[0].Center_Coord_Row <= 68 && gs_Beacon[0].Row_Bottom <= 68
       && gs_Beacon[0].Center_Coord_Row != 0 && gs_Beacon[0].Row_Bottom != 0
         && giLostBeacon == 0)//ROW 68 1.5M
    {
      guSidePass = 1;                                     //距离信标比较近,侧向通过标志
    }
    
    if(gs_Beacon[0].Center_Coord_Row <= 50 && gs_Beacon[0].Row_Bottom <= 50
       && gs_Beacon[0].Center_Coord_Row != 0 && gs_Beacon[0].Row_Bottom != 0
         && giLostBeacon == 0)//ROW 50 1.0M
    {
      guCirclePass = 1;                                  //距离信标比较近,侧向通过标志
    }
    
    if(guSidePass == 1)                                   //如果接近信标
    {
      gf_Beacon_Distance_Bot = 1.752 * gs_Beacon[0].Row_Bottom + 14.19;
      
      if(gf_Beacon_Distance_Bot > 100)
      {
        gf_Beacon_Distance_Bot = 100;
      }
      else if(gf_Beacon_Distance_Bot < 0)
      {
        gf_Beacon_Distance_Bot = 0;
      }
      
      liNextDis = gf_Beacon_Distance_Bot - giSpeedWheel * 0.2;
      giNextDis = liNextDis;
      
      if(guBeaconCount < 15)//10信标内
      {
        if(gcBeaconSpeed[guBeaconCount] == 0)
        {
          if(gf_Beacon_Distance <= 1.0 && gs_Beacon[0].Row_Bottom > 0 && gs_Beacon[0].Row_Bottom <= 10)
          {
            /*if((gcBeaconOrder[guBeaconCount] == 0 && gs_Beacon[0].Center_Coord_Col <= giExpectCOL)
                ||(gcBeaconOrder[guBeaconCount] == 1 && gs_Beacon[0].Center_Coord_Col >= giExpectCOL))
            {
              giTurnBack = 1;
            }*/
            giTurnBack = 1;
          }
        }
      }
      
      else
      {
        if(gf_Beacon_Distance <= 1.0 && gs_Beacon[0].Row_Bottom > 0 && gs_Beacon[0].Row_Bottom <= 10)
        {
          giTurnBack = 1;
        }
      }
      
    }
    
    //信标发车标志
    BeaconStartOld = BeaconStart;
    
    //记录距离
    gf_Beacon_Distance_Old = gf_Beacon_Distance;
    
    //圆圈丢失信标
    guLastCircleLost = guCircleLost;
    
    for(PTemp=&giRoadEdgeSit[4],PEnd=&giRoadEdgeSit[0];PTemp > PEnd;PTemp--)
    {
      *PTemp = *(PTemp-1);
    }
    
    for(PTemp_f=&gfRoadSitError[4],PEnd_f=&gfRoadSitError[0];PTemp_f > PEnd_f;PTemp_f--)
    {
      *PTemp_f = *(PTemp_f-1);
    }
    
    for(PTemp_f=&gfRoadSitErrorGrad[4],PEnd_f=&gfRoadSitErrorGrad[0];PTemp_f > PEnd_f;PTemp_f--)
    {
      *PTemp_f = *(PTemp_f-1);
    }
    
    for(i=2;i>0;i--)
    {
      gs_OriginalBeacon[i][0] = gs_OriginalBeacon[i-1][0];
      gs_OriginalBeacon[i][1] = gs_OriginalBeacon[i-1][1];
      gs_OriginalBeacon[i][2] = gs_OriginalBeacon[i-1][2];
      
      giCenterCoord[i][0] = giCenterCoord[i-1][0];
      giCenterCoord[i][1] = giCenterCoord[i-1][0];
    }
    
    for(ls_PTemp=&gs_Beacon[4],ls_PEnd=&gs_Beacon[0];ls_PTemp > ls_PEnd;ls_PTemp--)
    {
      ls_PTemp -> Center_Coord_Row = (ls_PTemp-1) -> Center_Coord_Row;
      ls_PTemp -> Center_Coord_Col = (ls_PTemp-1) -> Center_Coord_Col;
      ls_PTemp -> Row_Bottom = (ls_PTemp-1) -> Row_Bottom;
      ls_PTemp -> Row_Top = (ls_PTemp-1) -> Row_Top;
      ls_PTemp -> Col_Left = (ls_PTemp-1) -> Col_Left;
      ls_PTemp -> Col_Right = (ls_PTemp-1) -> Col_Right;
      ls_PTemp -> Area_Sum = (ls_PTemp-1) -> Area_Sum;
      ls_PTemp -> Darkfield = (ls_PTemp-1) -> Darkfield;
    }
}


