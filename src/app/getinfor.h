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
#ifndef __GETINFOR_H__
#define __GETINFOR_H__
/*
**Getinfor变量声明
*/
struct BeaconVariable
{
  int Center_Coord_Row;//行号
  int Center_Coord_Col;//列号
  int Row_Top;//信标顶端行
  int Row_Bottom;//信标低端行
  int Col_Left;//信标左边列
  int Col_Right;//信标右边列
  int Area_Sum;//面积
  unsigned char Darkfield;//暗场标志
};
typedef struct BeaconVariable BeaconVariable;
extern char STARTFLAG;
extern char gcBeaconOrder[15];                   //0:左转 1:右转
extern int giRoadEdgeSit[5];                     //中心值
extern struct BeaconVariable gs_OriginalBeacon[3][3];//信标原始白区域
extern struct BeaconVariable gs_Beacon[5];       //信标结构体
extern struct BeaconVariable *gs_PTemp_Beacon;   //信标结构体指针
extern struct BeaconVariable gs_Beacon_Temp;
extern int giBeacon_Row;                         //信标行号
extern int giBeacon_Col;                         //信标列号
extern int giBeacon_Row_Top;                     //信标顶端行号
extern int giBeacon_Row_Bottom;                  //信标底端行号
extern int giBeacon_Col_Left;                    //信标左边列号
extern int giBeacon_Col_Right;                   //信标右边列号
extern int giBeacon_Area_Sum;                    //信标面积
extern int giDarkField;                          //暗场标志
extern int giLostBeacon;                         //丢失信标标志
extern int giTurnBack;                           //回转标志
extern int giCenterCoord[3][2];                  //记录每次搜索到的信标白坐标(3次)
extern int giPredictRow;                         //暗场预测行坐标
extern int giPredictCol;                         //暗场预测列坐标
extern int giAreaSum_Predict[3];                 //信标面积预测(滤除干扰)
extern int giBeaconOrder1;                       //信标前5个次序
extern int giBeaconOrder2;                       //信标后5个次序
extern int giBeaconOrder3;                       //信标最后5个次序
extern int giExpectCOL;                          //当前期望信标列号
extern int CurrentLoop_Value;                    //电流环电压AD值
extern int CurrentLoop_Normal;                   //电流环电压标准值
extern int giNextDis;                            //当前距离
extern int giWhiteArea_MaxCol[3];                //白区域最长列长度
extern float gf_Public_Variable_Red;             //红色
extern float gf_Public_Variable_Purple;          //紫色
extern float gf_Public_Variable_Celeste;         //蓝色
extern float gf_Public_Variable_Green;           //绿色
extern float gfRoadSitError[5];                  //当前与期望中心偏差
extern float gfRoadSitErrorGrad[5];              //当前与期望中心偏差的微分
extern float gfGyroIntegral;                     //陀螺仪积分角度
extern float gfRowFilter;                        //行号低通滤波值
extern float gfRowFilter_Old;                    //上次行号低通滤波值
extern float gfCurrentSkew;                      //由信标行号推算当前偏移
extern float gf_Beacon_Distance;                 //小车和信标距离(由行号计算)
extern float gf_Beacon_Distance_Old;             //上次距离值
extern float gf_Beacon_Distance_Bot;             //小车和信标距离(由底端行号计算)
extern float gfAccIntegral;                      //加速度计积分输出速度值
extern unsigned char guCircleLost;              //进入圆圈丢失信标
extern unsigned char guLastCircleLost;          //上次丢失圆圈丢失信标
extern unsigned char guCirclePass;              //进圈通过标志
extern unsigned char guBeacon_Dark_field;       //信标暗场标志
extern unsigned char guSidePass;                //发现信标以后从侧面通过标志
extern unsigned char guBeaconPassflag;          //信标通过由0->1标志
extern unsigned char gu_Disturb_flag;           //白色干扰标志
extern unsigned char guBeaconCount;             //经过信标个数
extern unsigned char guBeaconCountPlus;         //信标个数加一标志

/*
**Getinfor函数声明
*/
extern void GetImage(void);                     //游程编码采集摄像头二值化跳变点
extern void ExtractChangeNum(void);             //提取图像的跳变点
extern void CurrentLoopValue_Get(void);         //获取电流环AD
extern void AngleCalculate(void);               //互补滤波计算角度
extern void Seek_Beacon(void);                  //寻找信标函数
#endif
