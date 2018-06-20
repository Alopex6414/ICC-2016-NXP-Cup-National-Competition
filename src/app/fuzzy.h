/*********************Copyright(C) 2015-2016 TYUT JBD TEAM B.*******************
Copyright (C), 2015-2016, TYUT JBD TEAM B.
File name: fuzzy.c 
Author: TEAM B, Alopex
Version: 1.0               
Date: 2015.10.28(E)
      2015.12.31(C)
      2016.03.15(A)
Description: 
            FuzzyControl(ģ������)
            Fuzzy_Update(ģ������)
            Direction_FuzzyCalculate(����ģ������)
            Speed_FuzzyCalculate(�ٶ�ģ������)
Others: Noting
Function List: fuzzy control ģ������
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
#ifndef __FUZZY_H__
#define __FUZZY_H__

typedef enum
{
  Fuzzy_Rank7 = 7
}Fuzzy_Rank_e;                  //ģ��ö�ٷּ���

typedef struct
{
  Fuzzy_Rank_e Rank;            //ģ���ּ�����
  float fe;                     //ƫ��
  float fec;                    //ƫ��΢��(ƫ��仯��)
  float eRule[7];               //��������Ⱥ�������ֵ
  float ecRule[7];              //���仯�����Ⱥ�������ֵ
  float U1Rule[7];              //���������������ֵ
  int rule[7][7];               //ģ�������
}FuzzyStruct;                   //ģ���ṹ��

typedef struct
{
  float fuzzyP;
  float fuzzyI;
  float fuzzyD;
}FuzzyPID;                      //ģ��PID�ṹ��

/*��������*/
extern FuzzyPID gs_Derict_fuzzyPID;     //����ģ���ṹ��
extern FuzzyPID gs_Speed_fuzzyPID;      //�ٶ�ģ���ṹ��
/*��������*/
extern float Fuzzy_Update(FuzzyStruct* F_S,float ek,float ekc);
extern float FuzzyCtrl(FuzzyStruct* Fuzzy_S);
extern void Direction_FuzzyCalculate(void);
extern void Speed_FuzzyCalculate(void);
#endif
