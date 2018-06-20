/*********************Copyright(C) 2015-2016 TYUT JBD TEAM B.*******************
Copyright (C), 2015-2016, TYUT JBD TEAM B.
File name: fuzzy.c 
Author: TEAM B, Alopex
Version: 1.0               
Date: 2015.10.28(E)
      2015.12.31(C)
      2016.03.15(A)
Description: 
            FuzzyControl(模糊控制)
            Fuzzy_Update(模糊计算)
            Direction_FuzzyCalculate(方向模糊计算)
            Speed_FuzzyCalculate(速度模糊计算)
Others: Noting
Function List: fuzzy control 模糊控制
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
#ifndef __FUZZY_H__
#define __FUZZY_H__

typedef enum
{
  Fuzzy_Rank7 = 7
}Fuzzy_Rank_e;                  //模糊枚举分级量

typedef struct
{
  Fuzzy_Rank_e Rank;            //模糊分级数量
  float fe;                     //偏差
  float fec;                    //偏差微分(偏差变化率)
  float eRule[7];               //误差隶属度函数中心值
  float ecRule[7];              //误差变化隶属度函数中心值
  float U1Rule[7];              //输出隶属函数中心值
  int rule[7][7];               //模糊规则表
}FuzzyStruct;                   //模糊结构体

typedef struct
{
  float fuzzyP;
  float fuzzyI;
  float fuzzyD;
}FuzzyPID;                      //模糊PID结构体

/*变量声明*/
extern FuzzyPID gs_Derict_fuzzyPID;     //方向模糊结构体
extern FuzzyPID gs_Speed_fuzzyPID;      //速度模糊结构体
/*函数声明*/
extern float Fuzzy_Update(FuzzyStruct* F_S,float ek,float ekc);
extern float FuzzyCtrl(FuzzyStruct* Fuzzy_S);
extern void Direction_FuzzyCalculate(void);
extern void Speed_FuzzyCalculate(void);
#endif
