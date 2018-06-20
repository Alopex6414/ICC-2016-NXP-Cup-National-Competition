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
#include "declare.h"

FuzzyPID gs_Derict_fuzzyPID;/*����ģ���ṹ��*/
FuzzyPID gs_Speed_fuzzyPID;/*�ٶ�ģ���ṹ��*/

/**
**Function: FuzzyControl
**Description: ģ������
**Calls: nothing
**Called By: isr.c
**Table Accessed: nothing
**Table Updated: nothing
**Input: Fuzzy Struct
**Output: nothing
**Return: nothing
**Others:nothing
**/
float FuzzyControl(FuzzyStruct* Fuzzy_S)
{
  int num = 0;
  int pe = 0;//ƫ�������ֶ�
  int pec = 0;//ƫ��仯�������ֶ�
  int rank = 0;//ģ���ּ�����
  int i = 0;
  float kp = 0.0;//ģ�����
  float eFuzzy[2]  = {0.0, 0.0};//ƫ��������
  float ecFuzzy[2] = {0.0, 0.0};//ƫ��仯��������
  float U1Fuzzy[7] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};//���������
  
  rank = Fuzzy_S -> Rank;
  
  switch (rank)
  {
    case 7:
          {
            /*ƫ������������*/
            if(Fuzzy_S -> fe < Fuzzy_S -> eRule[0])
	    {
              eFuzzy[0] = 1.0;
              eFuzzy[1] = 1.0 - eFuzzy[0];
	      pe = 0;
	    }
            
            else if(Fuzzy_S -> fe < Fuzzy_S -> eRule[1])
            {
              eFuzzy[0] = (Fuzzy_S -> eRule[1] - Fuzzy_S -> fe) / (Fuzzy_S -> eRule[1] - Fuzzy_S -> eRule[0]);
              eFuzzy[1] = 1.0 - eFuzzy[0];
	      pe = 0;
            }
            
            else if(Fuzzy_S -> fe < Fuzzy_S -> eRule[2])
            {
              eFuzzy[0] = (Fuzzy_S -> eRule[2] -Fuzzy_S -> fe) / (Fuzzy_S -> eRule[2] - Fuzzy_S -> eRule[1]);
              eFuzzy[1] = 1.0 - eFuzzy[0];
	      pe = 1;
            }
            
            else if(Fuzzy_S -> fe < Fuzzy_S -> eRule[3])
            {
              eFuzzy[0] = (Fuzzy_S -> eRule[3] - Fuzzy_S -> fe) / (Fuzzy_S -> eRule[3] - Fuzzy_S -> eRule[2]);
              eFuzzy[1] = 1.0 - eFuzzy[0];
	      pe = 2;
            }
            
            else if(Fuzzy_S -> fe < Fuzzy_S -> eRule[4])
            {
              eFuzzy[0] = (Fuzzy_S -> eRule[4] - Fuzzy_S -> fe) / (Fuzzy_S -> eRule[4] - Fuzzy_S -> eRule[3]);
              eFuzzy[1] = 1.0 - eFuzzy[0];
	      pe = 3;
            }
            
            else if(Fuzzy_S -> fe < Fuzzy_S -> eRule[5])
            {
              eFuzzy[0] = (Fuzzy_S -> eRule[5] - Fuzzy_S -> fe) / (Fuzzy_S -> eRule[5] - Fuzzy_S -> eRule[4]);
              eFuzzy[1] = 1.0 - eFuzzy[0];
	      pe = 4;
            }
            
            else if(Fuzzy_S -> fe < Fuzzy_S -> eRule[6])
            {
              eFuzzy[0] = (Fuzzy_S -> eRule[6] - Fuzzy_S -> fe) / (Fuzzy_S -> eRule[6] - Fuzzy_S -> eRule[5]);
              eFuzzy[1] = 1.0 - eFuzzy[0];
	      pe = 5;
            }
            
            else
            {
              eFuzzy[0] =1.0;
              eFuzzy[1] = 1.0 - eFuzzy[0];
	      pe = 6;
            }

            /*ƫ��仯������������*/
            if(Fuzzy_S -> fec < Fuzzy_S -> ecRule[0])
	    {
              ecFuzzy[0] =1.0;
              ecFuzzy[1] = 1.0 - ecFuzzy[0];
	      pec = 0;
	    }
            
            else if(Fuzzy_S -> fec < Fuzzy_S -> ecRule[1])
            {
              ecFuzzy[0] = (Fuzzy_S -> ecRule[1] - Fuzzy_S -> fec) / (Fuzzy_S -> ecRule[1] - Fuzzy_S -> ecRule[0]);
              ecFuzzy[1] = 1.0 - ecFuzzy[0];
	      pec = 0;
            }
            
            else if(Fuzzy_S -> fec < Fuzzy_S -> ecRule[2])
            {
              ecFuzzy[0] = (Fuzzy_S -> ecRule[2] - Fuzzy_S -> fec) / (Fuzzy_S -> ecRule[2] - Fuzzy_S -> ecRule[1]);
              ecFuzzy[1] = 1.0 - ecFuzzy[0];
              pec = 1;
            }
            
            else if(Fuzzy_S -> fec < Fuzzy_S -> ecRule[3])
            {
              ecFuzzy[0] = (Fuzzy_S -> ecRule[3] - Fuzzy_S -> fec) / (Fuzzy_S -> ecRule[3] - Fuzzy_S -> ecRule[2]);
              ecFuzzy[1] = 1.0 - ecFuzzy[0];
              pec = 2;
            }
            
            else if(Fuzzy_S -> fec < Fuzzy_S -> ecRule[4])
            {
               ecFuzzy[0] = (Fuzzy_S -> ecRule[4] - Fuzzy_S -> fec) / (Fuzzy_S -> ecRule[4] - Fuzzy_S -> ecRule[3]);
	       ecFuzzy[1] = 1.0 - ecFuzzy[0];
               pec = 3;
            }
            
            else if(Fuzzy_S -> fec < Fuzzy_S -> ecRule[5])
            {
              ecFuzzy[0] = (Fuzzy_S -> ecRule[5] - Fuzzy_S -> fec) / (Fuzzy_S -> ecRule[5] - Fuzzy_S -> ecRule[4]);
	      ecFuzzy[1] = 1.0 - ecFuzzy[0];
              pec = 4;
            }
            
            else if(Fuzzy_S -> fec < Fuzzy_S -> ecRule[6])
            {
              ecFuzzy[0] = (Fuzzy_S -> ecRule[6] - Fuzzy_S -> fec) / (Fuzzy_S -> ecRule[6] - Fuzzy_S -> ecRule[5]);
	      ecFuzzy[1] = 1.0 - ecFuzzy[0];
              pec = 5;
            }
            
            else
            {
              ecFuzzy[0] = 1.0;
              ecFuzzy[1] = 1.0 - ecFuzzy[0];
	      pec = 6;
            }
            break;  
          }
          default: break;
  }
  
  /*��ѯģ�������*/
  if(pe < (rank-1) && pec < (rank-1))        // e��de/dt��û�дﵽ��Ե
  {
    num =Fuzzy_S -> rule[pec][pe];
    U1Fuzzy[num] += eFuzzy[0] * ecFuzzy[0];
                
    num =Fuzzy_S -> rule[pec][pe+1];
    U1Fuzzy[num] += eFuzzy[1] * ecFuzzy[0];	
                
    num =Fuzzy_S -> rule[pec+1][pe];
    U1Fuzzy[num] += eFuzzy[0] * ecFuzzy[1];

    num =Fuzzy_S -> rule[pec+1][pe+1];
    U1Fuzzy[num] += eFuzzy[1] * ecFuzzy[1];
   }
   else if(pe == (rank-1) && pec < (rank-1))  // e�ﵽ��Ե
  {
    num =Fuzzy_S -> rule[pec][pe];
    U1Fuzzy[num] += eFuzzy[0] * ecFuzzy[0];
                
    num =Fuzzy_S -> rule[pec+1][pe];
    U1Fuzzy[num] += eFuzzy[0] * ecFuzzy[1];
   }
   else if(pe < (rank-1) && pec == (rank-1))  // de/dt�ﵽ��Ե
  {
    num =Fuzzy_S -> rule[pec][pe];
    U1Fuzzy[num] += eFuzzy[0] * ecFuzzy[0];	
                
    num =Fuzzy_S -> rule[pec][pe+1];
    U1Fuzzy[num] += eFuzzy[1] * ecFuzzy[0];
   }
   else				       // e��de/dtͬʱ�ﵽ��Ե
  {
    num = Fuzzy_S -> rule[pec][pe];
    U1Fuzzy[num] += eFuzzy[0] * ecFuzzy[0];		
  }
  
  /*������ķ���ģ��*/
  for(i=0;i<rank;i++)
  kp+=U1Fuzzy[i]*Fuzzy_S->U1Rule[i];
  
  return(kp);
}

/**
**Function: Fuzzy_Update
**Description: ģ�����Ƽ���
**Calls: nothing
**Called By: isr.c
**Table Accessed: nothing
**Table Updated: nothing
**Input: Fuzzy Struct
**Output: nothing
**Return: nothing
**Others:nothing
**/
float Fuzzy_Update(FuzzyStruct* F_S,float ek,float ekc)
{
  float value=0;
  F_S -> fe = ek;
  F_S -> fec = ekc;
  value = FuzzyControl(F_S);
  return value;
}

/**
**Function: Direction_FuzzyCalculate
**Description: ����ģ�����Ƽ���
**Calls: nothing
**Called By: isr.c
**Table Accessed: nothing
**Table Updated: nothing
**Input: Fuzzy Struct
**Output: nothing
**Return: nothing
**Others:nothing
**/
void Direction_FuzzyCalculate(void)
{
  /*fuzzy_P*/
  FuzzyStruct fuzzy_P =
 {
   Fuzzy_Rank7,
   0,
   0,
   {-24,-16,-8,0,8,16,24},
   {60,70,80,90,100,110,120},
   {giDp*1.3,giDp*1.2,giDp*1.1,giDp,giDp*1.1,giDp*1.2,giDp*1.3},
   {
    {4,3,3,3,3,3,4},
    {4,4,3,3,3,4,4},
    {5,4,3,3,3,4,5},
    {6,5,4,3,4,5,6},//6 5 4 3 4 5 6
    {6,5,4,4,4,5,6},
    {6,5,5,4,5,5,6},
    {6,6,5,4,5,6,6}
   },
 };
 
 /*fuzzy_D*/
 FuzzyStruct fuzzy_D =
 {
   Fuzzy_Rank7,
   0,
   0,
   {-24,-16,-8,0,8,16,24},
   {-24,-16,-8,0,8,16,24},
   {giDd,giDd*0.90,giDd*0.85,giDd*0.80,giDd*0.85,giDd*0.90,giDd},
   {
    {6,6,6,6,5,4,3},
    {5,5,6,6,6,5,4},
    {4,5,6,6,6,6,5},
    {5,6,6,6,6,6,5},
    {5,6,6,6,6,5,4},
    {4,5,6,6,6,5,5},
    {3,4,5,6,6,6,6}
   },
 };
 
 gs_Derict_fuzzyPID.fuzzyP = 0;
 gs_Derict_fuzzyPID.fuzzyD = 0;
 
 gs_Derict_fuzzyPID.fuzzyP = Fuzzy_Update(&fuzzy_P,gfRoadSitError[0],giCurrentSpeed);
 gs_Derict_fuzzyPID.fuzzyD = Fuzzy_Update(&fuzzy_D,gfRoadSitError[0],gfRoadSitError[0]-gfRoadSitError[2]);
  
}

/**
**Function: Speed_FuzzyCalculate
**Description: �ٶ�ģ�����Ƽ���
**Calls: nothing
**Called By: isr.c
**Table Accessed: nothing
**Table Updated: nothing
**Input: Fuzzy Struct
**Output: nothing
**Return: nothing
**Others:nothing
**/
void Speed_FuzzyCalculate(void)
{
  /*fuzzy_P*/
  FuzzyStruct fuzzy_P =
 {
   Fuzzy_Rank7,
   0,
   0,
   {-24,-12,-6,0,6,12,24},
   {20,40,60,80,100,120,150},
   {giSp,giSp*0.60,giSp*0.40,giSp*0.20,giSp*0.40,giSp*0.60,giSp},
   {
    {4,3,3,3,3,3,4},
    {4,4,3,3,3,4,4},
    {5,4,3,3,3,4,5},
    {5,5,4,3,4,5,5},
    {6,5,4,3,4,5,6},
    {6,5,4,3,4,5,6},
    {6,6,5,4,5,6,6}
   },
 };
 
 /*fuzzy_I*/
 FuzzyStruct fuzzy_I =
 {
   Fuzzy_Rank7,
   0,
   0,
   {-24,-12,-6,0,6,12,24},
   {20,40,60,80,100,120,150},
   {giSi,giSi*0.75,giSi*0.5,giSi*0.25,giSi*0.5,giSi*0.75,giSi},
   {
    {5,6,6,6,6,6,5},
    {5,5,6,6,6,5,5},
    {4,5,6,6,6,5,4},
    {4,4,5,6,5,4,4},
    {3,4,5,6,5,4,3},
    {3,4,5,6,5,4,3},
    {3,3,6,6,6,3,3}
   },
 };
 
 gs_Speed_fuzzyPID.fuzzyP = 0;
 gs_Speed_fuzzyPID.fuzzyI = 0;
 
 gs_Speed_fuzzyPID.fuzzyP = Fuzzy_Update(&fuzzy_P,gfSpeedErr[0],gfDesirSpeed);
 gs_Speed_fuzzyPID.fuzzyI = Fuzzy_Update(&fuzzy_I,gfSpeedErr[0],gfDesirSpeed);
 
}

