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
#ifndef __GETINFOR_H__
#define __GETINFOR_H__
/*
**Getinfor��������
*/
struct BeaconVariable
{
  int Center_Coord_Row;//�к�
  int Center_Coord_Col;//�к�
  int Row_Top;//�ű궥����
  int Row_Bottom;//�ű�Ͷ���
  int Col_Left;//�ű������
  int Col_Right;//�ű��ұ���
  int Area_Sum;//���
  unsigned char Darkfield;//������־
};
typedef struct BeaconVariable BeaconVariable;
extern char STARTFLAG;
extern char gcBeaconOrder[15];                   //0:��ת 1:��ת
extern int giRoadEdgeSit[5];                     //����ֵ
extern struct BeaconVariable gs_OriginalBeacon[3][3];//�ű�ԭʼ������
extern struct BeaconVariable gs_Beacon[5];       //�ű�ṹ��
extern struct BeaconVariable *gs_PTemp_Beacon;   //�ű�ṹ��ָ��
extern struct BeaconVariable gs_Beacon_Temp;
extern int giBeacon_Row;                         //�ű��к�
extern int giBeacon_Col;                         //�ű��к�
extern int giBeacon_Row_Top;                     //�ű궥���к�
extern int giBeacon_Row_Bottom;                  //�ű�׶��к�
extern int giBeacon_Col_Left;                    //�ű�����к�
extern int giBeacon_Col_Right;                   //�ű��ұ��к�
extern int giBeacon_Area_Sum;                    //�ű����
extern int giDarkField;                          //������־
extern int giLostBeacon;                         //��ʧ�ű��־
extern int giTurnBack;                           //��ת��־
extern int giCenterCoord[3][2];                  //��¼ÿ�����������ű������(3��)
extern int giPredictRow;                         //����Ԥ��������
extern int giPredictCol;                         //����Ԥ��������
extern int giAreaSum_Predict[3];                 //�ű����Ԥ��(�˳�����)
extern int giBeaconOrder1;                       //�ű�ǰ5������
extern int giBeaconOrder2;                       //�ű��5������
extern int giBeaconOrder3;                       //�ű����5������
extern int giExpectCOL;                          //��ǰ�����ű��к�
extern int CurrentLoop_Value;                    //��������ѹADֵ
extern int CurrentLoop_Normal;                   //��������ѹ��׼ֵ
extern int giNextDis;                            //��ǰ����
extern int giWhiteArea_MaxCol[3];                //��������г���
extern float gf_Public_Variable_Red;             //��ɫ
extern float gf_Public_Variable_Purple;          //��ɫ
extern float gf_Public_Variable_Celeste;         //��ɫ
extern float gf_Public_Variable_Green;           //��ɫ
extern float gfRoadSitError[5];                  //��ǰ����������ƫ��
extern float gfRoadSitErrorGrad[5];              //��ǰ����������ƫ���΢��
extern float gfGyroIntegral;                     //�����ǻ��ֽǶ�
extern float gfRowFilter;                        //�кŵ�ͨ�˲�ֵ
extern float gfRowFilter_Old;                    //�ϴ��кŵ�ͨ�˲�ֵ
extern float gfCurrentSkew;                      //���ű��к����㵱ǰƫ��
extern float gf_Beacon_Distance;                 //С�����ű����(���кż���)
extern float gf_Beacon_Distance_Old;             //�ϴξ���ֵ
extern float gf_Beacon_Distance_Bot;             //С�����ű����(�ɵ׶��кż���)
extern float gfAccIntegral;                      //���ٶȼƻ�������ٶ�ֵ
extern unsigned char guCircleLost;              //����ԲȦ��ʧ�ű�
extern unsigned char guLastCircleLost;          //�ϴζ�ʧԲȦ��ʧ�ű�
extern unsigned char guCirclePass;              //��Ȧͨ����־
extern unsigned char guBeacon_Dark_field;       //�ű갵����־
extern unsigned char guSidePass;                //�����ű��Ժ�Ӳ���ͨ����־
extern unsigned char guBeaconPassflag;          //�ű�ͨ����0->1��־
extern unsigned char gu_Disturb_flag;           //��ɫ���ű�־
extern unsigned char guBeaconCount;             //�����ű����
extern unsigned char guBeaconCountPlus;         //�ű������һ��־

/*
**Getinfor��������
*/
extern void GetImage(void);                     //�γ̱���ɼ�����ͷ��ֵ�������
extern void ExtractChangeNum(void);             //��ȡͼ��������
extern void CurrentLoopValue_Get(void);         //��ȡ������AD
extern void AngleCalculate(void);               //�����˲�����Ƕ�
extern void Seek_Beacon(void);                  //Ѱ���ű꺯��
#endif
