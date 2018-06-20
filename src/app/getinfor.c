/*********************Copyright(C) 2015-2016 TYUT JBD TEAM B.*******************
Copyright (C), 2015-2016, TYUT JBD TEAM B.
File name: Getinfor.c 
Author: TEAM B, Alopex
Version: 1.0               
Date: 2015.10.28(E)
      2015.12.31(C)
      2016.03.15(A)
Description: 
            GetImage Function(����ͷ��Ϣ�ɼ�����)
            ExtractChangeNum Function(��ȡ����㺯��)
            Seek_Beacon Function(Ѱ���ű꺯��)
Others: Noting
Function List:
            1.GetImage �γ̱���ɼ�����
            2.ExtractChangeNum ��ȡÿ���������
            3.Seek_Beacon Ѱ���ű�
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
#include  "declare.h"

/*
**����ͷ�ɼ�ͼ������(�γ̱���)
*/
unsigned char guImgBuff[ROW+27][NUM]  = {0};

/*
**��������(������)
*/
float gf_Public_Variable_Red = 0;     //��ɫ
float gf_Public_Variable_Purple = 0;  //��ɫ
float gf_Public_Variable_Celeste = 0; //��ɫ
float gf_Public_Variable_Green = 0;   //��ɫ

/*
**��������
*/
char STARTFLAG = 0;                           //����������־

/*
**����ֵ
*/
float gfAngleCenter = 0;                      //���ٶȼ�����ֵ��ֱ����
float gfAngleSpeedCenter = 0;                 //����������ֵ����ֹ��
float gfAngleRenew[2] = {0};                  //���������ж���

/*
**�����ں�
*/
float gfAngleSpeedIntegral = 0;               //�����ǻ���ֵ
float gfAngleSpeedCurrent = 0;                //�����ǹ�һ����ֵ
float gfAngle = 0;                            //�ںϺ�����Ƕ�
float gfGyroIntegral = 0;                     //�����ǻ��ֽǶ�
float gfAccIntegral = 0;                      //���ٶȼƻ�������ٶ�ֵ

/*
**��Ϣ�ɼ�
*/
int giRoadEdgeSit[5]={0};                     //����ֵ
int ChangeNum[ROW]={0};                       //ÿһ���������
int ActEdge[ROW][NUM]={0};                    //ʵ��ͼ���ά����
int ActCol[COL][NUM]={0};                     //ʵ��ͼ����������
float gfRoadSitError[5]={0};                  //��ǰ����������ƫ��
float gfRoadSitErrorGrad[5]={0};              //��ǰ����������ƫ���΢��

/*
**CurrentLoop
*/
int CurrentLoop_Value = 0;                    //��������ѹADֵ
int CurrentLoop_Normal = 0;                   //��������ѹ��׼ֵ

/*
**�ű�ת�����
*/
char gcBeaconOrder[15]={0};                   //0:��ת 1:��ת
int giBeaconOrder1=0;                         //�ű�ǰ5������
int giBeaconOrder2=0;                         //�ű��5������
int giBeaconOrder3=0;                         //�ű����5������
int giExpectCOL=0;                            //��ǰ�����ű��к�

/*
**Ѱ���ű�
*/
BeaconVariable gs_OriginalBeacon[3][3];         //�ű�ԭʼ������
BeaconVariable gs_Beacon[5];                    //�ű�ṹ��
BeaconVariable *gs_PTemp_Beacon=&gs_Beacon[0];  //�ű�ṹ��ָ��
BeaconVariable gs_Beacon_Temp;

int giBeacon_Row=0;                             //�ű��к�
int giBeacon_Col=0;                             //�ű��к�
int giBeacon_Row_Top=0;                         //�ű궥���к�
int giBeacon_Row_Bottom=0;                      //�ű�׶��к�
int giBeacon_Col_Left=0;                        //�ű�����к�
int giBeacon_Col_Right=0;                       //�ű��ұ��к�
int giBeacon_Area_Sum=0;                        //�ű����
int giDarkField=0;                              //������־
int giLostBeacon=0;                             //��ʧ�ű��־
int giTurnBack=0;                               //�����ű�󷵻ر�־
int giCenterCoord[3][2]={0};                    //��¼ÿ�����������ű������(3��)
int giTopBotEdge[ROW][2]={0};                   //�߽�����
int giAreaSum_Predict[3]={0};                   //�ű����Ԥ��(�˳�����)
int giWhiteArea_MaxCol[3]={0};                  //��������г���
float gfRowFilter=0;                            //�кŵ�ͨ�˲�ֵ
float gfRowFilter_Old=0;                        //�ϴ��кŵ�ͨ�˲�ֵ
float gfCurrentSkew=0;                          //���ű��к����㵱ǰƫ��
unsigned char guCircleLost=0;                  //����ԲȦ��ʧ�ű�
unsigned char guLastCircleLost=0;              //�ϴζ�ʧԲȦ��ʧ�ű�
unsigned char guCirclePass=0;                  //��Ȧͨ����־
unsigned char guBeacon_Dark_field=0;           //�ű갵����־
unsigned char guSidePass=0;                    //�����ű��Ժ�Ӳ���ͨ����־
unsigned char guBeaconPassflag=0;              //�ű�ͨ����0->1��־
unsigned char gu_Disturb_flag=0;               //��ɫ���ű�־
unsigned char guBeaconCount=0;                 //�����ű����
unsigned char guBeaconCountPlus=0;             //�ű������һ��־(����D��Ӱ��)

/*�ű��к������*/
int giNextDis=0;                                //��ǰ����
float gf_Beacon_Distance=0;                     //С�����ű����(���кż���)
float gf_Beacon_Distance_Old=0;                 //�ϴξ���ֵ(���кż���)
float gf_Beacon_Distance_Bot=0;                 //С�����ű����(�ɵ׶��кż���)

/*������*/
unsigned char BuzzerNoise_flag=0;

/**
**Function: GetImage
**Description: ����ͷIO�ɼ��γ̱��������
**Calls: ��
**Called By: main.c
**Table Accessed: ��
**Table Updated: ��
**Input: 
**Output: guImgBuff
**Return: ��
**Others: �γ̱���ɼ�ͼ�������,��I/O��Ⱥ���
          ��26ָ������,if...else...�����ʺű��ʽ
          ָ��������
**/
void GetImage(void)
{
    unsigned char *PTemp = &guImgBuff[guHrefElect][0];
    unsigned char *PTemptest=0;
    unsigned char  bit = 0;
    unsigned char  i = 0,j = 0,k = 0,ktest = 0;//�±�ż��Ϊ��
    
    //���guImgBuff����
    ClearArrayChar(&guImgBuff[guHrefElect][0],&guImgBuff[guHrefElect][NUM-1]);
    
    for (i = 0; i < 130; ++i);//��������ʱ
    
    for (j = 1; j < COL; ++j)//j = 0 start//COL1 ���ɼ�
    {
        //bit = gpio_get(PORTB, 11);/*33ָ������*/
        bit = CAMERA_IN;/*25ָ������*//*����PTx_IN*/
        
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
**Description: ��ȡImgbuff������������
**Calls:   ��
**Called By: isr.c
**Table Accessed: ��
**Table Updated: ��
**Input: guImgBuff
**Output: ChangNum
**Return: ��
**Others: guImgBuff����->ActEdge����
          �к�����,�޽���
          guImgBuff����->ActCol����
          �к�����,�޽���
**/
void ExtractChangeNum(void)
{
  int i=0,j=0,k=0;
  int liWhiteflag=0;
  int liWhiteflagOld=0;
  int *PTest=&ActEdge[0][0];
  int *PTestEnd=&ActEdge[0][0];
  int *PTestCol=&ActCol[0][0];//�б�������
  unsigned char *PTemp=&guImgBuff[0][0];
  unsigned char *PEnd=&guImgBuff[0][0];
  
  ClearArrayInt(&ChangeNum[0],&ChangeNum[ROW-1]);//���ChangeNum����
  ClearArrayInt(&ActEdge[0][0],&ActEdge[ROW-1][NUM-1]);//���ActEdge����
  ClearArrayInt(&ActCol[0][0],&ActCol[COL-1][NUM-1]);//���ActCol����
  
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
  
  //�б���ת���б���(ʱ��ܳ�,��Լ��Ҫ8ms)
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
**Description: ��ȡ��ǰ������AD��ת��Ϊ��׼ֵ
**Calls:   GetAD
**Called By: PORTD5_IRQHandler
**           PORTA13_IRQHandler
**Table Accessed: ��
**Table Updated: ��
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
**Description: ���ٶȼƾ�������Ƕ�ֵ
**Calls:   GetAD
**Called By: PORTD5_IRQHandler
**           PORTA13_IRQHandler
**Table Accessed: ��
**Table Updated: ��
**Input: gfAngleCenter
**       gfAngleSpeedCenter
**Output: gfAngle
**        gfAngleSpeedCurrent
**Return: ��
**Others:��
**/
void AngleCalculate(void)
{
    float lfDeltaValue = 0;                  //�Ƕ��ں�ֵ��ͽǶ�ADֵƫ��
    float lfAngleCurrent = 0;                //���ٶȼƹ�һ��ֵ
    float lfAngleAD = 0;                     //���ٶȼ�ADֵ
    float lfAngleSpeedAD = 0;                //������ADֵ
    
    GetAD(&lfAngleAD,&lfAngleSpeedAD);       //��ȡ�����Ǽ��ٶȼ�ADֵ
    gfSendWhite=lfAngleAD;                   //���ڷ��ͼ��ٶȼ�ADֵ(��ɫ����)
    
    //ACT:2040
    lfAngleCurrent = (gfAngleCenter - lfAngleAD) * 180 / (1109 - 3239);//���ٶȼƹ�һ��
    gfAngleSpeedCurrent  = (lfAngleSpeedAD - gfAngleSpeedCenter) * 0.169;//�����ǹ�һ��        
   
    gfSendRed = lfAngleCurrent+10000;   //���ڷ��ͼ��ٶȼƹ�һ��(��ɫ����)
            
    gfAngleRenew[1] = gfAngleRenew[0];       //��ʷ
    gfAngleRenew[0] = lfAngleCurrent;        //��ǰ
    
    if(gfAngleRenew[1] > 0 && gfAngleRenew[0] < 0) 
    {
        STARTFLAG = 1;                                                 
    }                                        //�״νǶ�������ֵǰ��䶯���������
    
    if(STARTFLAG == 1)                       //��������                        
    {
        gfAngle = gfAngleSpeedIntegral;
        gfSendGreen = gfAngle+10000;    //���ڷ��ͽǶ�(��ɫ����)
        lfDeltaValue = (lfAngleCurrent - gfAngle) * 0.8;//0.9;
        gfAngleSpeedIntegral += (gfAngleSpeedCurrent + lfDeltaValue) * 0.001216;//����ʱ�䳣��
    }                                        
}

/**
**Function: Seek_Beacon
**Description: Ѱ���ű�
**Calls:   ��
**Called By: isr.c
**Table Accessed: ��
**Table Updated: ��
**Input: ��
**Output: Coordinate
**Return: ��
**Others:��
**/
void Seek_Beacon(void)
{
    int i=0,j=0,k=0;
    int i0=0,j0=0,k0=0;
    int i1=0,j1=0,k1=0;
    int liWhiteMaxTemp=0;                       //���������ʱ����
    int liWhiteRow_Top=0;                       //�����������ɨ����Զ����
    int liWhiteRow_Bottom=0;                    //�����������ɨ���������
    int liWhiteCol_Left=0;                      //���������߽�
    int liWhiteCol_Right=0;                     //��������ұ߽�
    int liWhiteNUM_Top=0;                       //�ű�׶���
    int liWhiteNUM_Bottom=0;                    //�ű�׵׶�
    int liWhiteNUM_SUM=0;                       //�ű�����
    int liWhite_Max=0;
    int liWhiteMaxRow=0;                        //������к�
    int liWhiteMaxCol=0;                        //������к�
    int liEdgeColWidth=0;                       //�ű�߽����һ�п��
    int liEdgeWhiteRowCount=0;                  //�ű��ڱ�Եʱ���а׼���
    int liEdgeWhiteCol=0;                       //�ű��ڱ�Եʱ�������к�
    int liNextDis=0;                            //��һ������
    int liActEdgeTemporary[ROW][NUM]={0};       //ActEdge����Ѱ���ű�ʱ��ʱ������
    int *PTemp=&ActEdge[0][0];
    int *PEnd=&ActEdge[0][0];
    int *PTempTest=&liActEdgeTemporary[0][0];   //��ʱ������ָ��
    int *PTempTestEnd=&liActEdgeTemporary[0][0];//��ʱ����ĩָ��
    int *PTempStore=&liActEdgeTemporary[0][0];  //�洢ָ��
    int *PEdgeTemp=&giTopBotEdge[0][0];         //����������ָ��
    float *PTemp_f=&gfRoadSitError[0];          //����������ָ��
    float *PEnd_f=&gfRoadSitError[0];           //��������ĩָ��
    unsigned char luUplineflag=0;              //Ŀ���е���һ��
    unsigned char luDownlineflag=0;            //Ŀ���е���һ��
    unsigned char luUplineCount=0;             //Ŀ�������м���
    unsigned char luDownlineCount=0;           //Ŀ���е����м���
    BeaconVariable *ls_PTemp=&gs_Beacon[0];     //�ű�ṹ��ָ��(�׵�ַ)
    BeaconVariable *ls_PEnd=&gs_Beacon[4];      //�ű�ṹ��ָ��(ĩ��ַ)
    
    /*Ѱ���ű�߽����*/
    int liLastRowEdgeL=0;                       //��һ���ű���߽�
    int liLastRowEdgeR=0;                       //��һ���ű��ұ߽�
    int liMemoryJump[NUM]={0};                  //�洢����������
    int *liMemoryJumpPTemp=&liMemoryJump[0];    //�洢������׵�ַ
    int *liMemoryJumpPEnd=&liMemoryJump[0];     //�洢�����ĩ��ַ
    
    /*Attention Please,These Code Was Just Test*/
    /*Ѱ���ű���ʱ����*/
    int liTestWhiteMaxTemp=0;                   //��ʱ�������������ʱ����
    int liTestWhite_Max=0;                      //��ʱ�����������
    int liTestWhiteMaxRow=0;                    //��ʱ����������к�
    int liTestWhiteMaxCol=0;                    //��ʱ����������к�
    int liTestWhiteRow_Top=0;                   //��ʱ���������򶥶��к�
    int liTestWhiteRow_Bottom=0;                //��ʱ����������׶��к�
    int liTestWhiteCol_Left=0;                  //��ʱ�������������к�
    int liTestWhiteCol_Right=0;                 //��ʱ�������������к�
    int liTestWhiteNUM_Top=0;                   //��ʱ���������򶥶˰�����
    int liTestWhiteNUM_Bottom=0;                //��ʱ����������׶˰�����
    int liTestWhiteNUM_SUM=0;                   //��ʱ��������������
    int liTestEdgeL=0;                          //��ʱ�������������
    int liTestEdgeR=0;                          //��ʱ�����������ұ�
    int liClearJump[NUM]={0};                   //������������
    int *liClearJumpPTemp=&liClearJump[0];      //����������ָ��
    int *liClearJumpPEnd=&liClearJump[0];       //��������ĩָ��
    BeaconVariable ls_BeaconArray[3];           //Ѱ�Ҵ洢ͼ����3��������
    
    /*�ű�ṹ���ֵ*/
    gs_Beacon[0].Center_Coord_Row=0;
    gs_Beacon[0].Center_Coord_Col=0;
    gs_Beacon[0].Row_Top=0;
    gs_Beacon[0].Row_Bottom=0;
    gs_Beacon[0].Col_Left=0;
    gs_Beacon[0].Col_Right=0;
    gs_Beacon[0].Area_Sum=0;
    gs_Beacon[0].Darkfield=0;
    
    /*ActEdge���鸴��*/
    for(PTemp=&ActEdge[0][0],PEnd=&ActEdge[ROW-1][NUM-1],PTempTest=&liActEdgeTemporary[0][0];PTemp<PEnd;PTemp++,PTempTest++)
    {
      *PTempTest=*PTemp;
    }
    
    //������
    ClearArrayInt(&giTopBotEdge[0][0],&giTopBotEdge[ROW-1][1]);//�߽�����
    
    //gf_Public_Variable_Purple=0;
    /*Attention Please,These Code Was Just Test*/
    
    /*Ѱ��3��������*/
    /*������1*/
    for(i=0;i<ROW;i++)//�Ұ����һ��
    {
      PTempTest=&liActEdgeTemporary[i][0];//�׵�ַ
      PTempTestEnd=&liActEdgeTemporary[i][NUM-1];//ĩ��ַ
      
      for(j=0;PTempTest<PTempTestEnd && (*PTempTest!=0||*(PTempTest+1)!=0);j++,PTempTest++)
      {
        if(j%2==0 && *PTempTest != 0 && *PTempTest != COL && *(PTempTest+1)!=0 && *(PTempTest+1) > *PTempTest)
        {
          liTestWhiteMaxTemp = *(PTempTest+1) - *PTempTest;//���п��
          if(liTestWhiteMaxTemp > liTestWhite_Max)
          {
            liTestWhite_Max = liTestWhiteMaxTemp;//������
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
    
    //���������������
    for(i = liTestWhiteMaxRow;i < ROW;i++)
    {
      PTempTest=&liActEdgeTemporary[i][0];//�׵�ַ
      PTempTestEnd=&liActEdgeTemporary[i][NUM-1];//ĩ��ַ
      PTempStore=&liActEdgeTemporary[i][0];
      liClearJumpPTemp = &liClearJump[0];//ָ�����������
      liClearJumpPEnd = &liClearJump[0];//ָ�����������
      ClearArrayInt(&liClearJump[0],&liClearJump[NUM-1]);//������������
      
      if(*PTempTest == COL || *PTempTest == 0)
      {
        break;                                  //ֻҪ����ȫ����ֹͣɨ��
      }
      
      for(j=0;PTempTest < PTempTestEnd && (*PTempTest!=0 || *(PTempTest+1)!=0);j++,PTempTest++)
      {
        if(j%2==0 && *PTempTest != 0 && *PTempTest != COL && *(PTempTest+1)!=0 && *(PTempTest+1) > *PTempTest)
        { 
          //����ڽ��İ�
          if((liTestEdgeL <= *PTempTest && liTestEdgeR >= *(PTempTest+1))
             ||(liTestEdgeL <= *PTempTest && liTestEdgeR <= *(PTempTest+1) && liTestEdgeR >= *PTempTest)
               ||(liTestEdgeL >= *PTempTest && liTestEdgeR >= *(PTempTest+1) && liTestEdgeL <= *(PTempTest+1))
                 ||(liTestEdgeL >= *PTempTest && liTestEdgeR <= *(PTempTest+1)))
          {
            PTempStore = PTempTest;//�洢��ǰ��ַ
            liTestWhiteRow_Top = i;//���������к�
            liTestWhiteNUM_Top += (*(PTempTest+1) - *PTempTest);//�������ϰ벿�����
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
      
      //������������
      liTestEdgeL = liClearJump[0];
      for(liClearJumpPTemp = &liClearJump[0],liClearJumpPEnd = &liClearJump[NUM-1];
          liClearJumpPTemp < liClearJumpPEnd && *liClearJumpPTemp != 0;liClearJumpPTemp++)
      {
        liTestEdgeR = *liClearJumpPTemp;
      }
      
    }
    
    liTestEdgeL = liTestWhiteCol_Left;
    liTestEdgeR = liTestWhiteCol_Right;
    
    //���������������   
    for(i = liTestWhiteMaxRow - 1;i > 0;i--)
    {
      PTempTest=&liActEdgeTemporary[i][0];//�׵�ַ
      PTempTestEnd=&liActEdgeTemporary[i][NUM-1];//ĩ��ַ
      PTempStore=&liActEdgeTemporary[i][0];
      liClearJumpPTemp = &liClearJump[0];//ָ�����������
      liClearJumpPEnd = &liClearJump[0];//ָ�����������
      ClearArrayInt(&liClearJump[0],&liClearJump[NUM-1]);//������������
      
      if(*PTempTest == COL || *PTempTest == 0)
      {
        break;                                  //ֻҪ����ȫ����ֹͣɨ��
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
            PTempStore = PTempTest;//�洢��ǰ��ַ
            liTestWhiteRow_Bottom = i;//���������к�
            liTestWhiteNUM_Bottom += (*(PTempTest+1) - *PTempTest);//�������ϰ벿�����
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

      //������������
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
    
    /*��������*/
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
    
    /*������2*/
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
    
    for(i=0;i<ROW;i++)//�Ұ����һ��
    {
      PTempTest=&liActEdgeTemporary[i][0];//�׵�ַ
      PTempTestEnd=&liActEdgeTemporary[i][NUM-1];//ĩ��ַ
      
      for(j=0;PTempTest<PTempTestEnd && (*PTempTest!=0||*(PTempTest+1)!=0);j++,PTempTest++)
      {
        if(j%2==0 && *PTempTest != 0 && *PTempTest != COL && *(PTempTest+1)!=0 && *(PTempTest+1) > *PTempTest)
        {
          liTestWhiteMaxTemp = *(PTempTest+1) - *PTempTest;//���п��
          if(liTestWhiteMaxTemp > liTestWhite_Max)
          {
            liTestWhite_Max = liTestWhiteMaxTemp;//������
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
    
    //���������������
    for(i = liTestWhiteMaxRow;i < ROW;i++)
    {
      PTempTest=&liActEdgeTemporary[i][0];//�׵�ַ
      PTempTestEnd=&liActEdgeTemporary[i][NUM-1];//ĩ��ַ
      PTempStore=&liActEdgeTemporary[i][0];
      liClearJumpPTemp = &liClearJump[0];//ָ�����������
      liClearJumpPEnd = &liClearJump[0];//ָ�����������
      ClearArrayInt(&liClearJump[0],&liClearJump[NUM-1]);//������������
      
      if(*PTempTest == COL || *PTempTest == 0)
      {
        break;                                  //ֻҪ����ȫ����ֹͣɨ��
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
            PTempStore = PTempTest;//�洢��ǰ��ַ
            liTestWhiteRow_Top = i;//���������к�
            liTestWhiteNUM_Top += (*(PTempTest+1) - *PTempTest);//�������ϰ벿�����
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
      
      //������������
      liTestEdgeL = liClearJump[0];
      for(liClearJumpPTemp = &liClearJump[0],liClearJumpPEnd = &liClearJump[NUM-1];
          liClearJumpPTemp < liClearJumpPEnd && *liClearJumpPTemp != 0;liClearJumpPTemp++)
      {
        liTestEdgeR = *liClearJumpPTemp;
      }
    }
    
    liTestEdgeL = liTestWhiteCol_Left;
    liTestEdgeR = liTestWhiteCol_Right;
    
    //���������������   
    for(i = liTestWhiteMaxRow - 1;i > 0;i--)
    {
      PTempTest=&liActEdgeTemporary[i][0];//�׵�ַ
      PTempTestEnd=&liActEdgeTemporary[i][NUM-1];//ĩ��ַ
      PTempStore=&liActEdgeTemporary[i][0];
      liClearJumpPTemp = &liClearJump[0];//ָ�����������
      liClearJumpPEnd = &liClearJump[0];//ָ�����������
      ClearArrayInt(&liClearJump[0],&liClearJump[NUM-1]);//������������
      
      if(*PTempTest == COL || *PTempTest == 0)
      {
        break;                                  //ֻҪ����ȫ����ֹͣɨ��
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
            PTempStore = PTempTest;//�洢��ǰ��ַ
            liTestWhiteRow_Bottom = i;//���������к�
            liTestWhiteNUM_Bottom += (*(PTempTest+1) - *PTempTest);//�������ϰ벿�����
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
      
      //������������
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
    
    /*��������*/
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
    
    /*������3*/
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
    
    for(i=0;i<ROW;i++)//�Ұ����һ��
    {
      PTempTest=&liActEdgeTemporary[i][0];//�׵�ַ
      PTempTestEnd=&liActEdgeTemporary[i][NUM-1];//ĩ��ַ
      
      for(j=0;PTempTest<PTempTestEnd && (*PTempTest!=0||*(PTempTest+1)!=0);j++,PTempTest++)
      {
        if(j%2==0 && *PTempTest != 0 && *PTempTest != COL && *(PTempTest+1)!=0 && *(PTempTest+1) > *PTempTest)
        {
          liTestWhiteMaxTemp = *(PTempTest+1) - *PTempTest;//���п��
          if(liTestWhiteMaxTemp > liTestWhite_Max)
          {
            liTestWhite_Max = liTestWhiteMaxTemp;//������
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
    
    //���������������
    for(i = liTestWhiteMaxRow;i < ROW;i++)
    {
      PTempTest=&liActEdgeTemporary[i][0];//�׵�ַ
      PTempTestEnd=&liActEdgeTemporary[i][NUM-1];//ĩ��ַ
      PTempStore=&liActEdgeTemporary[i][0];
      liClearJumpPTemp = &liClearJump[0];//ָ�����������
      liClearJumpPEnd = &liClearJump[0];//ָ�����������
      ClearArrayInt(&liClearJump[0],&liClearJump[NUM-1]);//������������
      
      if(*PTempTest == COL || *PTempTest == 0)
      {
        break;                                  //ֻҪ����ȫ����ֹͣɨ��
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
            PTempStore = PTempTest;//�洢��ǰ��ַ
            liTestWhiteRow_Top = i;//���������к�
            liTestWhiteNUM_Top += (*(PTempTest+1) - *PTempTest);//�������ϰ벿�����
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
      
      //������������
      liTestEdgeL = liClearJump[0];
      for(liClearJumpPTemp = &liClearJump[0],liClearJumpPEnd = &liClearJump[NUM-1];
          liClearJumpPTemp < liClearJumpPEnd && *liClearJumpPTemp != 0;liClearJumpPTemp++)
      {
        liTestEdgeR = *liClearJumpPTemp;
      }
    }
    
    liTestEdgeL = liTestWhiteCol_Left;
    liTestEdgeR = liTestWhiteCol_Right;
    
    //���������������   
    for(i = liTestWhiteMaxRow - 1;i > 0;i--)
    {
      PTempTest=&liActEdgeTemporary[i][0];//�׵�ַ
      PTempTestEnd=&liActEdgeTemporary[i][NUM-1];//ĩ��ַ
      PTempStore=&liActEdgeTemporary[i][0];
      liClearJumpPTemp = &liClearJump[0];//ָ�����������
      liClearJumpPEnd = &liClearJump[0];//ָ�����������
      ClearArrayInt(&liClearJump[0],&liClearJump[NUM-1]);//������������
      
      if(*PTempTest == COL || *PTempTest == 0)
      {
        break;                                  //ֻҪ����ȫ����ֹͣɨ��
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
            PTempStore = PTempTest;//�洢��ǰ��ַ
            liTestWhiteRow_Bottom = i;//���������к�
            liTestWhiteNUM_Bottom += (*(PTempTest+1) - *PTempTest);//�������ϰ벿�����
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

      //������������
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
    
    /*��������*/
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
    
    //�Ұ����һ��
    for(j=0;j<COL;j++)
    {
      PTemp=&ActCol[j][0];                     //ÿ���׵�ַ
      PEnd=&ActCol[j][NUM-1];                  //ÿ��ĩ��ַ
      
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
    
    //�Ұ����һ��
    for(i=0;i<ROW;i++)
    {
      PTemp=&ActEdge[i][0];                     //ÿ���׵�ַ
      PEnd=&ActEdge[i][NUM-1];                  //ÿ��ĩ��ַ
      
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
    
    //�ű�������г������Ϊ�ű�
    if(giWhiteArea_MaxCol[0] >= giWhiteArea_MaxCol[1]
        && giWhiteArea_MaxCol[0] >= giWhiteArea_MaxCol[2])//������1�г������
    {
      liWhiteMaxRow = liWhiteMaxRow;
      liWhiteMaxCol = liWhiteMaxCol;
      liWhiteCol_Left = liWhiteCol_Left;
      liWhiteCol_Right = liWhiteCol_Right;
    }
    
    else if(giWhiteArea_MaxCol[1] >= giWhiteArea_MaxCol[0]
              && giWhiteArea_MaxCol[1] >= giWhiteArea_MaxCol[2])//������2�г������
    {
      liWhiteMaxRow = gs_OriginalBeacon[0][1].Center_Coord_Row;
      liWhiteMaxCol = gs_OriginalBeacon[0][1].Center_Coord_Col;
      liWhiteCol_Left = gs_OriginalBeacon[0][1].Col_Left;
      liWhiteCol_Right = gs_OriginalBeacon[0][1].Col_Right;
    }
    
    else if(giWhiteArea_MaxCol[2] >= giWhiteArea_MaxCol[0]
              && giWhiteArea_MaxCol[2] >= giWhiteArea_MaxCol[1])//������3�г������
    {
      liWhiteMaxRow = gs_OriginalBeacon[0][2].Center_Coord_Row;
      liWhiteMaxCol = gs_OriginalBeacon[0][2].Center_Coord_Col;
      liWhiteCol_Left = gs_OriginalBeacon[0][2].Col_Left;
      liWhiteCol_Right = gs_OriginalBeacon[0][2].Col_Right;
    }
    
    //������б߽�
    liLastRowEdgeL = liWhiteCol_Left;
    liLastRowEdgeR = liWhiteCol_Right;
    
    //�Ӱ�ɫ���������ɨ
    for(i0 = liWhiteMaxRow;i0 < ROW;i0++)
    {
      PTemp=&ActEdge[i0][0];                     //ÿ���׵�ַ
      PEnd=&ActEdge[i0][NUM-1];                  //ÿ��ĩ��ַ
      PEdgeTemp=&giTopBotEdge[i0][0];            //�߽��׵�ַ
      liMemoryJumpPTemp=&liMemoryJump[0];        //�洢�������׵�ַ
      liMemoryJumpPEnd=&liMemoryJump[0];         //�洢������ĩ��ַ
      ClearArrayInt(&liMemoryJump[0],&liMemoryJump[NUM-1]);//������
      
      if(*PTemp==COL || *PTemp==0)               //ֻҪ����ȫ����ֹͣɨ��
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
    
    //������б߽�
    liLastRowEdgeL = liWhiteCol_Left;
    liLastRowEdgeR = liWhiteCol_Right;
    
    //�Ӱ�ɫ���������ɨ
    for(i0 = liWhiteMaxRow;i0 > 0;i0--)
    {
      PTemp=&ActEdge[i0][0];                     //ÿ���׵�ַ
      PEnd=&ActEdge[i0][NUM-1];                  //ÿ��ĩ��ַ
      PEdgeTemp=&giTopBotEdge[i0][0];            //�߽��׵�ַ
      liMemoryJumpPTemp=&liMemoryJump[0];        //�洢�������׵�ַ
      liMemoryJumpPEnd=&liMemoryJump[0];         //�洢������ĩ��ַ
      ClearArrayInt(&liMemoryJump[0],&liMemoryJump[NUM-1]);//������
      
      if(*PTemp==COL || *PTemp==0)               //ֻҪ����ȫ����ֹͣɨ��
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
    
    liWhiteNUM_SUM=liWhiteNUM_Top+liWhiteNUM_Bottom;//�ű�״�С
    gs_Beacon[0].Area_Sum=liWhiteNUM_SUM;           //�ű����
    
    //���˺͵׶˶��а�
    if(liWhiteRow_Top!=0 && liWhiteRow_Bottom!=0 && liWhiteRow_Top >= liWhiteRow_Bottom)
    {      
      liWhiteMaxRow=(liWhiteRow_Top + liWhiteRow_Bottom)*0.5;
      
      //�������ű��밵��ǰ�������ű�Ƚ�
      
      //�бȽ�
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
      
      //�бȽ�
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
      
      //���ǵ��ű�������ͷ��Եʱ�ű�������//liEdgeColWidth
      if(liWhiteCol_Left <= liWhiteCol_Right && liWhiteCol_Right == COL)//���ԵС���ұ�Ե,�ұ�Ե��������к�
      {
        //�����ű�����һ�п��
        for(i0 = liWhiteMaxRow;i0 < ROW;i0++)//���ϲ���
        {
          PTemp=&ActEdge[i0][0];                     //ÿ���׵�ַ
          PEnd=&ActEdge[i0][NUM-1];                  //ÿ��ĩ��ַ
          
          if(*PTemp==COL || *PTemp==0)               //ֻҪ����ȫ����ֹͣɨ��
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
        
        for(i0 = liWhiteMaxRow;i0 >= 0;i0--)//���²���
        {
          PTemp=&ActEdge[i0][0];                     //ÿ���׵�ַ
          PEnd=&ActEdge[i0][NUM-1];                  //ÿ��ĩ��ַ
          
          if(*PTemp==COL || *PTemp==0)               //ֻҪ����ȫ����ֹͣɨ��
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
        
        //Ѱ�ҿ����ͬ���к�,��������
        for(i1 = liWhiteCol_Left;i1 <= liWhiteCol_Right;i1++)
        {
          liEdgeWhiteRowCount = 0;//ÿһ�м�������
          
          for(j1 = liWhiteMaxRow;j1 < ROW;j1++)
          {
            PTemp=&ActEdge[j1][0];                     //ÿ���׵�ַ
            PEnd=&ActEdge[j1][NUM-1];                  //ÿ��ĩ��ַ
            
            if(*PTemp==COL || *PTemp==0)               //ֻҪ����ȫ����ֹͣɨ��
            {
              break;
            }
            
            for(k1=0;PTemp<PEnd && (*PTemp!=0 || *(PTemp+1)!=0);k1++,PTemp++)
            {
              if(k1%2==0 && *PTemp!=0 && *PTemp!=COL && *(PTemp+1)!=0 && (*(PTemp+1) > *PTemp))
              {
                if(i1 >= *PTemp && i1 <= *(PTemp+1))//��ɫ
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
            PTemp=&ActEdge[j1][0];                     //ÿ���׵�ַ
            PEnd=&ActEdge[j1][NUM-1];                  //ÿ��ĩ��ַ
            
            if(*PTemp==COL || *PTemp==0)               //ֻҪ����ȫ����ֹͣɨ��
            {
              break;
            }
            
            for(k1=0;PTemp<PEnd && (*PTemp!=0 || *(PTemp+1)!=0);k1++,PTemp++)
            {
              if(k1%2==0 && *PTemp!=0 && *PTemp!=COL && *(PTemp+1)!=0 && (*(PTemp+1) > *PTemp))
              {
                if(i1 >= *PTemp && i1 <= *(PTemp+1))//��ɫ
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
            if(liEdgeWhiteRowCount > liEdgeColWidth)//�ҵ���COL�д���к�
            {
              liEdgeWhiteCol = i1;
              break;//��������
            }
            
            else//COL�����
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
      
      gs_Beacon[0].Row_Top=liWhiteRow_Top;            //�ű궥���к�
      gs_Beacon[0].Row_Bottom=liWhiteRow_Bottom;      //�ű�׶��к�
      gs_Beacon[0].Col_Left=liWhiteCol_Left;          //�ű�������к�
      gs_Beacon[0].Col_Right=liWhiteCol_Right;        //�ű��ұ����к�
    }
    
    //����û�аף��׶��а�
    else if(liWhiteRow_Top==0 && liWhiteRow_Bottom!=0 && liWhiteRow_Bottom <= liWhiteMaxRow)
    {
      gs_Beacon[0].Row_Top=liWhiteMaxRow;
      gs_Beacon[0].Row_Bottom=liWhiteRow_Bottom;
    }
    
    //�����аף��׶�û�а�
    else if(liWhiteRow_Bottom==0 && liWhiteRow_Top!=0 && liWhiteRow_Top >= liWhiteMaxRow)
    {
      gs_Beacon[0].Row_Top=liWhiteRow_Top;
      gs_Beacon[0].Row_Bottom=liWhiteMaxRow;
    }
    
    //���˺͵׶˾�û�а�
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
    
    //Ѱ�����ĵ����������Ƿ�Ϊ��
    //��һ��
    if(liWhiteMaxRow>0 && liWhiteMaxRow< ROW-1)
    {
      //������б߽�,���������ұ߽�
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
      
      //������б߽�,���������ұ߽�
      //��һ��
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
      
      //�����ű����״̬��״������,�������������Ұ�
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
      
      //Զ���ű�Ƚ�С�����(5��������)
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
    
    //��������    
    else if(liWhiteMaxRow == 0)
    {
      //������б߽�,���������ұ߽�
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
      
      //������б߽�,���������ұ߽�
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
    
    //��Զ�����
    else
    {
      //������б߽�,���������ұ߽�
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
      
      //������б߽�,���������ұ߽�
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
    
    //���������ű�����
    giCenterCoord[0][0] = liWhiteMaxRow;
    giCenterCoord[0][1] = liWhiteMaxCol;
    
    //�����ű���������λ��
    if(liWhiteMaxRow != 0 && liWhiteMaxCol != 0)//liWhite_Max>=2 && luDownlineflag==1 && luUplineflag==1//liWhiteMaxRow != 0 && liWhiteMaxCol != 0
    {
      gs_Beacon[0].Center_Coord_Row = liWhiteMaxRow;
      gs_Beacon[0].Center_Coord_Col = liWhiteMaxCol;
      gs_Beacon[0].Darkfield = 0;
      guCircleLost = 0;
      giDarkField = 0;                                    //������־����
      giLostBeacon = 0;                                   //��ʧ�ű��־����
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
        giDarkField = 3;                                //����3����
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
    
    //�ű��к��кŵ�ͨ�˲�
    gfRowFilter = giBeacon_Row * 0.50 + gfRowFilter_Old * 0.50;
    gfRowFilter_Old = gfRowFilter;
    
    //�ű����
    gf_Beacon_Distance = 0.00004645 * pow(gs_Beacon[0].Center_Coord_Row,3) - 0.00694 * pow(gs_Beacon[0].Center_Coord_Row,2) + 0.3671 * gs_Beacon[0].Center_Coord_Row - 6.054;
        
    if(gf_Beacon_Distance < 0)
    {
      gf_Beacon_Distance = 0;
    }
    else if(gf_Beacon_Distance > 9.0)
    {
      gf_Beacon_Distance = 9.0;
    }
    
    //BeaconStart(���ܷ�����־λ)
    if(gs_Beacon[0].Center_Coord_Row != 0 && gs_Beacon[0].Center_Coord_Col != 0
        && gs_Beacon[0].Darkfield == 0 && BeaconStart == 0 && Switch_On)
    {
      BeaconStart = 1;
    }
     
    //�ű�������
    if(giBeaconExcept == 0)//δ���������ű����
    {
      //�ҵ��µ��ű��
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
        //Buzzer_noise;//Ѱ�ҵ��µ��ű�,������bee
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
          //Buzzer_quite;//δ�ҵ��µ��ű�
          BuzzerNoise_flag = 5;
        }
        guBeaconCountPlus = 0;
      }
    }
    
    else if(giBeaconExcept != 0 && guBeaconCount+1 < giBeaconExcept)//�ű������һС�������ű���
    {
      //�ҵ��µ��ű��(�ϴο����ű�1M,�����ű��복���1M����)
      if(gs_Beacon[0].Center_Coord_Row != 0 && gs_Beacon[0].Center_Coord_Col != 0
          && gs_Beacon[1].Center_Coord_Row != 0 && gs_Beacon[1].Center_Coord_Col != 0
            && BeaconStart == 1 && BeaconStartOld == 1
              && gs_Beacon[0].Darkfield == 0 && ((gf_Beacon_Distance_Old <= 1.00 && gs_Beacon[1].Row_Bottom <= 30) || (guLastCircleLost == 1 && gs_Beacon[1].Darkfield == 1))
                && (gs_Beacon[0].Row_Bottom - gs_Beacon[1].Row_Bottom) >= 20)
      {
        guBeaconCount++;
        guBeaconCountPlus = 1;
        BuzzerNoise_flag = 0;
        //Buzzer_noise;//Ѱ�ҵ��µ��ű�,������biu
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
          //Buzzer_quite;//δ�ҵ��µ��ű�
          BuzzerNoise_flag = 5;
        }
        guBeaconCountPlus = 0;
      }
    }
    
    else if(giBeaconExcept != 0 && guBeaconCount+1 == giBeaconExcept)//�������һ���ű�,����ȷ��
    {
      if(gs_Beacon[0].Center_Coord_Row != 0 && gs_Beacon[0].Center_Coord_Col != 0
          && gs_Beacon[1].Center_Coord_Row != 0 && gs_Beacon[1].Center_Coord_Col != 0
            && BeaconStart == 1 && BeaconStartOld == 1
              && ((gs_Beacon[0].Darkfield == 0 && (gf_Beacon_Distance_Old <= 1.00 && gs_Beacon[1].Row_Bottom <= 30) && (gs_Beacon[0].Row_Bottom - gs_Beacon[1].Row_Bottom) >= 20)
                ||(gs_Beacon[0].Darkfield == 1 && gs_Beacon[1].Darkfield == 0 && guCircleLost == 1)))//���������������������ű������������
      {
        guBeaconCount++;
        guBeaconCountPlus = 1;
        BuzzerNoise_flag = 0;
        //Buzzer_noise;//Ѱ�ҵ��µ��ű�,������biu
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
          //Buzzer_quite;//δ�ҵ��µ��ű�
          BuzzerNoise_flag = 5;
        }
        guBeaconCountPlus = 0;
      }
    }
    
    else if(giBeaconExcept != 0 && guBeaconCount+1 > giBeaconExcept)//�ű��������
    {
      guBeaconCount = giBeaconExcept;
    }
    
    //�ű�ƫ����
    gfCurrentSkew = 0.01377 * pow(gs_Beacon[0].Center_Coord_Row,2) - 2.689 * gs_Beacon[0].Center_Coord_Row + 130.0;
    
    if(gfCurrentSkew < 0)//��Χ����
    {
      gfCurrentSkew = 0;
    } 
    else if(gfCurrentSkew > 65)//60
    {
      gfCurrentSkew = 65;
    }
    
    if(gs_Beacon[0].Center_Coord_Row > 97)//��������80��ƫ��Ϊ0,ע��ÿ�����ʱ������
    {
      gfCurrentSkew = 0;
    }
        
    giRoadEdgeSit[0] = gs_Beacon[0].Center_Coord_Col;
    
    //�ű����
    if(guBeaconCount < 15)//10�ű���
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
    
    else//����10�ű�
    {
      #ifdef LeftModel
      gfRoadSitError[0] = giRoadEdgeSit[0]-COL/2-gfCurrentSkew;
      #elif defined RightModel
      gfRoadSitError[0] = giRoadEdgeSit[0]-COL/2+gfCurrentSkew;
      #else
      gfRoadSitError[0] = giRoadEdgeSit[0]-COL/2-gfCurrentSkew;
      #endif
    }
    
    /*�ű�����ƽ��*/
    //gfRoadSitError[0] = gfRoadSitError[0] * 0.90 + gfRoadSitError[1] * 0.10;
    gfRoadSitErrorGrad[0] = gfRoadSitError[0] - gfRoadSitError[2];
    
    //�ű�����ҰԶ��3����ʧ�ű�
    if(giBeacon_Row > 68)
    {
      if(guCirclePass == 3)        //�ӽ��ҵ��ű���ٷ���3�ΰ������־
      {
        giLostBeacon = 1;
        guSidePass = 0;           //��������3�κ�������ͨ����ʶ
        guCirclePass = 0;
        giTurnBack = 0;
        gfCurrentSkew = 0;
        guRoadBlockCount = 0;
      }
      
      //�����ű�֮��û�а���,���TurnBack��־
      else if((gs_Beacon[0].Center_Coord_Row - gs_Beacon[1].Center_Coord_Row >= 10
              || gs_Beacon[0].Center_Coord_Row - gs_Beacon[2].Center_Coord_Row >= 10)
              && guSidePass == 1)//����ͨ����־��λ�����ű��к���ͻȻ���
      {
        guSidePass = 0;//����ͨ����־����
        guCirclePass = 0;//����ͨ����־����
        giTurnBack = 0;//giTurnBack ��־����
      }
    }
    //�ű�����Ұ����1����ʧ�ű�
    else
    {
      if(giDarkField >= 1)        //�ӽ��ҵ��ű���ٷ���3�ΰ������־
      {
        giLostBeacon = 1;
        if(guCirclePass == 1)
        {
          guCircleLost = 1;
        }
        guSidePass = 0;           //��������3�κ�������ͨ����ʶ
        guCirclePass = 0;
        giTurnBack = 0;
        gfCurrentSkew = 0;
        guRoadBlockCount = 0;
      }
      
      //�����ű�֮��û�а���,���TurnBack��־
      else if((gs_Beacon[0].Center_Coord_Row - gs_Beacon[1].Center_Coord_Row >= 10
              || gs_Beacon[0].Center_Coord_Row - gs_Beacon[2].Center_Coord_Row >= 10)
              && guSidePass == 1)//����ͨ����־��λ�����ű��к���ͻȻ���
      {
        guSidePass = 0;//����ͨ����־����
        guCirclePass = 0;//����ͨ����־����
        giTurnBack = 0;//giTurnBack ��־����
      }
    }
    
    if(gs_Beacon[0].Center_Coord_Row <= 68 && gs_Beacon[0].Row_Bottom <= 68
       && gs_Beacon[0].Center_Coord_Row != 0 && gs_Beacon[0].Row_Bottom != 0
         && giLostBeacon == 0)//ROW 68 1.5M
    {
      guSidePass = 1;                                     //�����ű�ȽϽ�,����ͨ����־
    }
    
    if(gs_Beacon[0].Center_Coord_Row <= 50 && gs_Beacon[0].Row_Bottom <= 50
       && gs_Beacon[0].Center_Coord_Row != 0 && gs_Beacon[0].Row_Bottom != 0
         && giLostBeacon == 0)//ROW 50 1.0M
    {
      guCirclePass = 1;                                  //�����ű�ȽϽ�,����ͨ����־
    }
    
    if(guSidePass == 1)                                   //����ӽ��ű�
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
      
      if(guBeaconCount < 15)//10�ű���
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
    
    //�ű귢����־
    BeaconStartOld = BeaconStart;
    
    //��¼����
    gf_Beacon_Distance_Old = gf_Beacon_Distance;
    
    //ԲȦ��ʧ�ű�
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


