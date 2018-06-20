/*********************Copyright(C) 2015-2016 TYUT JBD TEAM B.*******************
Copyright (C), 2015-2016, TYUT JBD TEAM B.
File name: keyboard.c 
Author: TEAM B, Alopex
Version: 1.0               
Date: 2015.10.28(E)
      2015.12.31(C)
      2016.03.15(A)
Description: 
            Keyboard Input Display Function(����������ʾ����)
Others: Noting
Function List:
            1.keyboardinput ���������������
            2.keyboardadscreen ������ʾ��������
            3.keydoBeacon �ű��������ɨ�躯��
            4.gCommandBeacon �ű�����������뺯��
            5.�����˸��,С����'.'������ǰ�������
History:
<author> <time> <version > <desc>
 ������ 2015/10/28 1.0 �޸ĳ���
        2015/12/31 1.1 ���ע��
        2016/2/7   1.2 ���ע��
        2016/4/1   2.0 ���ע��
        2016/4/8   2.1 '.'�˸��
        2016/4/9   2.1 ���ע��
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
**ȫ�ֱ�������
*/
unsigned char KeyValue[8];//���̼�ֵ
unsigned char ShowText[8] = {0x3d,0x10,0x50,0x38,0x00,0x00,0x00,0x00};//������ʾ����
unsigned char InputString[5] = {'0','0','0','0','0'};//�����ַ�����
unsigned char KeyCode;//���̼�ֵ����
unsigned char guPushNum = 0;//�ַ�������
unsigned char BackSpace = 0;//�˸��
unsigned char BeaconString[10]={'0'};//�ű��������
unsigned char guComStr[8]={0x00};//��ǰ��ʾ�����ַ�����
int ValueTest;//���̷���ֵ
int ConClick = 0; //���������־
int PointFlag = 0;//С�����־
int EnterFlag = 0;//�س������±�־


/**
**Function: KeyPortInit
**Description: I/O�˿ڳ�ʼ����M�ڸ�����ʾ����ͼ�������
**Calls:   ��
**Called By: keyboardinput
             keyboardadscreen
**Table Accessed: ��
**Table Updated: ��
**Input:  ��
**Output: ��
**Return: ��
**Others:��
**/
void KeyPortInit() 
{ 
    INIT_KB_STB; //��ʼ��STB
    INIT_KB_CLK; //��ʼ��CLK
    INIT_KB_DO;  //��ʼ��DIO,���
                                      
    KB_CLKH;     //CLK�����ó�ʼ�ߵ�ƽ
    KB_DOH;      //DIO�����ó�ʼ�ߵ�ƽ
    KB_STBH;     //STB�����ó�ʼ�ߵ�ƽ
}

/**
**Function: StrToInt
**Description: ���ַ���ת��Ϊ����������
**Calls:   ��
**Called By: 
**Table Accessed: ��
**Table Updated: ��
**Input:  ��
**Output: ��
**Return: ��
**Others:��
**/
int StrToInt(unsigned char *str)
{  
    int sum = 0, i = 0, sign = 1;
    
    for (i = 0; i < 5; i++)
    {   
      if (str[i] == '-')   
      {
        sign = -1; 
        continue;
      }
        
      sum = sum * 10 + (str[i] - '0');	
     }
    
     sum = sum * sign;
    
     return sum;
}
/**
**Function: StrToFloat
**Description: ���ַ���ת���ɸ���������
**Calls:   ��
**Called By: 
**Table Accessed: ��
**Table Updated: ��
**Input:  ��
**Output: ��
**Return: ��
**Others:��
**/
float StrToFloat(unsigned char *str)    //δ���ԣ����ã�
{
    float frac = 0, sum = 0, j = 1;
    int i = 0, sign = 1, flag = 0;
    
    for (i = 0; i < 5; i++)
    {   
      if (str[i] == '-')   
      {
        sign = -1;
        continue;
      }
        
      if (str[i] == '.') 
      {
        flag = 1;
        continue;
      }
        
      if (flag == 0)
      {
        sum = sum * 10 + (str[i] - '0');//��������
      }
        
      else
      {
        j = j / 10;  
	frac = frac + (str[i] - '0') * j;//С������
       }
    }
    sum = (sum + frac) * sign;
    return sum;
}

/*
**���ܺ���
*/

/**
**Function: delayms1 delay1
**Description: ��ʱ
**Calls:   ��
**Called By: 
**Table Accessed: ��
**Table Updated: ��
**Input:  ms
**Output: ��
**Return: ��
**Others:��
**/
void delayms1(int ms) 
{
    int ii, jj;
    if (ms < 1)
    {
        ms=1;
    }
    
    for (ii = 0; ii < ms; ii++)
        
    for (jj = 0; jj < 3338; jj++);    //40MHz--1ms      
} 

void delay1(int ms) 
{
    int ii, jj;
    if (ms < 1)
    {
        ms=1;
    }
    
    for (ii = 0; ii < ms; ii++)
        
    for (jj = 0; jj < 2000; jj++);    //40MHz--1ms      
} 

/**
**Function: SendData
**Description: �����ݺ���
**Calls:   ��
**Called By: 
**Table Accessed: ��
**Table Updated: ��
**Input:  ��
**Output: ��
**Return: ��
**Others:��
**/
void SendData(unsigned char data)
{
    int i;
    
    KB_STBL;                            //STB�͵�ƽ���������ݲ���
    
    for (i = 0; i < 8; i++)
    {
        KB_CLKL;                        //CLK�͵�ƽ������1bit
 
        if (data & 0x01 != 0)           //0,1����
        {
            KB_DOH;                     //DO�ߵ�ƽ
        }  
        
        else
        {
            KB_DOL;                     //DO�͵�ƽ 
        }
    
        KB_CLKH;                        //CLK�ߵ�ƽ,�������
        
        data >>= 1;                     //��λ
    }
}

/*
**��ʾ����
*/

/**
**Function: DisplayA
**Description: ��ʾģʽA����ַ�Զ���һģʽ
**Calls:   ��
**Called By: 
**Table Accessed: ��
**Table Updated: ��
**Input:  ��
**Output: ��
**Return: ��
**Others:��
**/
void DisplayA(unsigned char text)
{
    int i;
                                        //�˿�׼��
    KB_DOH;                             //DO�ߵ�ƽ                             
    KB_CLKH;                            //CLK�ߵ�ƽ
    KB_STBH;                            //STB�ߵ�ƽ
  
    SendData(WRITE_MODE_A);             //д����ģʽ
    KB_STBH;                            //STB�ߵ�ƽ
    SendData(START_ADD);                //������ʼ��ַ
    
    for (i = 0; i < DATA_COUNT; i++) 
    {                                   //������ʾ���ݣ���16λ
        SendData(text);
    }
    
    KB_STBH;
    SendData(LED_CTRL);                 //������ʾ����
    KB_STBH;
}         

/**
**Function: DisplayB
**Description: ��ʾģʽB���ƶ���ַ��ʾ
**Calls:   ��
**Called By: 
**Table Accessed: ��
**Table Updated: ��
**Input:  ��
**Output: ��
**Return: ��
**Others:��
**/
void DisplayB(unsigned char address,unsigned char text) 
{
                                        //�˿�׼��
    KB_DOH;                             //DO�ߵ�ƽ
    KB_CLKH;                            //CLK�ߵ�ƽ
    KB_STBH;                            //STB�ߵ�ƽ
  
    SendData(WRITE_MODE_B);             //��ʾģʽ
    KB_STBH;                            //STB�ߵ�ƽ
    SendData(address);                  //�����ַ
    SendData(text);                     //��������
    KB_STBH;  
}         

/**
**Function: KeyScan KeyDo
**Description: ����ɨ�����
**Calls:   ��
**Called By: 
**Table Accessed: ��
**Table Updated: ��
**Input:  ��
**Output: ��
**Return: ��
**Others:��
**/
void KeyScan() 
{
    int i;
    
    KB_STBH;                            //�˿�STB��
    
    SendData(READ_MODE);                //���Ͷ���������
    
    for (i = 0; i < 2; i++)
    {                                   //�����ݣ���8λ��ÿλ����������������
      KeyValue[i] = ReadKeyValue(8);    //ѭ��ɨ��2��2*6��ֵ
    }
    
    KB_STBH;                            //����ɨ�裬̧��STB��ƽ
}

void KeyDo()
{
    int i, j, k;
    
    for (i = 0; i < 2; i++)             //�ֱ����������Ÿ�6�������İ�ѹ���
    {
        if (i == 1)                     //���Ű�������  
        {
          KeyValue[i] |= 0x80;
        }
        
        if (
            #ifdef VerticalKeyboard
            KeyValue[i] == 0x01
            #elif defined HorizontalKeyboard
            KeyValue[i] == 0xc0
            #endif 
            )                           //���'#'������
        {
          InEnter();                    //���봦��
        }
      
        KeyCode = LedEncode(KeyDecode(KeyValue[i]));//��ֵ�������
      
        if ((KeyDecode(KeyValue[i]) == InputString[4] 
             || (
                #ifdef VerticalKeyboard
                (BackSpace == 1 && i == 0)          //�������'.'��(0x04),�˸���ʱi == 0
                #elif defined HorizontalKeyboard
                (BackSpace == 1 && i == 1)          //������'.'��(0x90),�˸���ʱi == 1
                #endif
                ) 
                ) && (ConClick == 0))//��������һ����ͬ����'.'�˸������,������־Ϊ0
        {
          ConClick = 1;                 //������־��λ
          BackSpace = 0;                //�˸��־����
          delayms1(PRESS_DELAY);        //��ѹ��ʱ��������
        }
        
        else
        {
          if ((KeyValue[i] != 0x00) && (KeyValue[i] != 0x80) 
              && (
                  #ifdef VerticalKeyboard
                  KeyValue[i] != 0x01
                  #elif defined HorizontalKeyboard
                  KeyValue[i] != 0xc0
                  #endif 
                  )
              )//��ֵ���벻Ϊ' ',�Ҳ�Ϊ'#'
            { 
              if(KeyCode == 0x80 && ShowText[7] != 0x00)//����������С����'.'��ǰһλ��Ϊ' '
              {                 
                for (i = 4; i < 8; i++) //������ʾ���Ų�������
                {
                  ShowText[i] = 0x00;
                }
                
                for (k = 0; k < 5; k++) //�����ַ���������
                {
                  InputString[k] = '0';
                }

                BackSpace = 1;          //�˸��־��λ
                ConClick = 0;           //������־����
              }
              
              else if((ShowText[4] == 0x00) || (ShowText[4] == 0x3f))//������ʾ����4λδд��
              {         
                for (j = 4; j < 7; j++) //������ʾ
                {
                  ShowText[j] = ShowText[j + 1];  
                }
                    
                ShowText[7] = KeyCode;
                    
                for (k = 0; k < 4; k++) //�ַ������¼
                {
                  InputString[k] = InputString[k + 1];
                }
                    
                InputString[4] = KeyDecode(KeyValue[i]);
                ConClick = 0;           //������־����
              }
            }
        }
    }
}

//�ű���KeyDo,ѡ����
void KeyDo_Beacon(void)
{
  int i, j, k;
  
  for(i = 0;i < 2;i++)//ɨ�����鰴����0:����6������ 1:����6������
  {
    if(i == 1)//�����Ű�������Ϊ1
    {
      KeyValue[i] |= 0x80;
    }
    
    if (
        #ifdef VerticalKeyboard
        KeyValue[i] == 0x01
        #elif defined HorizontalKeyboard
        KeyValue[i] == 0xc0
        #endif
        )                   //'#'������
    {
      InEnter();            //���봦��
    }
    
    KeyCode = LedEncode(KeyDecode(KeyValue[i]));//��ֵ�������
      
    if ((KeyDecode(KeyValue[i]) == InputString[4] 
         || (
            #ifdef VerticalKeyboard
            (BackSpace == 1 && i == 0)          //�������'.'��(0x04),�˸���ʱi == 0
            #elif defined HorizontalKeyboard
            (BackSpace == 1 && i == 1)          //������'.'��(0x90),�˸���ʱi == 1
            #endif
            )
            ) && (ConClick == 0))//��������һ����ͬ����'.'�˸������,������־Ϊ0
    {
      ConClick = 1;             //������־��λ
      BackSpace = 0;            //�˸��־����
      delayms1(PRESS_DELAY);    //��ѹ��ʱ��������
    }
    
    else
    {
      if ((KeyValue[i] != 0x00) && (KeyValue[i] != 0x80) 
          && (
              #ifdef VerticalKeyboard
              KeyValue[i] != 0x01
              #elif defined HorizontalKeyboard
              KeyValue[i] != 0xc0
              #endif 
             )
          )//��ֵ���벻Ϊ' ',�Ҳ�Ϊ'#'
      {
        if(KeyCode == 0x3f || KeyCode == 0x06 || KeyCode == 0x80)//ֻ�ܰ���'0','1','.'
        {
          if(KeyCode == 0x80 && ShowText[7] != 0x00)//����������С����'.'��ǰһλ��Ϊ' '
          {
            guPushNum = 0;//�����������λ������
              
            for (i = 0; i < 8; i++)//��ԭԭ���Ĳ�����ʾ
            {
              ShowText[i] = guComStr[i];
            }
            
            for (k = 0; k < 5; k++)//�����ַ���������
            {
              InputString[k] = '0';
            }
            
            BackSpace = 1;      //�˸��־��λ
            ConClick = 0;       //����״̬����
          }
          
          else if(guPushNum < 5)//�����ַ���ĿС��5
          {
            guPushNum++;        //������Ŀ��һ
                
            if((ShowText[4] == 0x00))//������ʾ
            {
              for (j = 4; j < 7; j++)
              {
                ShowText[j] = ShowText[j + 1];  
              }
            }
                
            else
            {
              for (j = 0; j < 7; j++)
              {
                ShowText[j] = ShowText[j + 1];  
              }
            }
                
            ShowText[7] = KeyCode;
                
            for (k = 0; k < 4; k++)//�����ַ�����
            {
              InputString[k] = InputString[k + 1];
            }
                            
            InputString[4] = KeyDecode(KeyValue[i]);
            ConClick = 0;//��������
          }
          
       }
        
      }
    }
    
  }
}

/**
**Function: ReadKeyValue
**Description: ��ȡ��ֵ
**Calls:   ��
**Called By: 
**Table Accessed: ��
**Table Updated: ��
**Input:  ��
**Output: ��
**Return: ��
**Others:��
**/                                                                
unsigned char ReadKeyValue(int bitnum)
{
    unsigned char kt = 0;
    int i = 0;
    
    KB_DOH;                             //DO�ߵ�ƽ
    INIT_KB_DI;                         //DIO��Ϊ����ڣ�������
                   
    KB_STBL;                            //STB�ŵͣ���ʼ������
    
    for (i = 0; i < bitnum; i++)
    {                                   //ѭ����bit
        KB_CLKL;                        //ʱ���½��أ���ʼ��һ��bit
        
        /*ԭ����250nop,�����˸��120nop*/
        //��������ʽҲ����...ARM���αָ��nop����ʱ����...
        /*__asm("nop\n"
                "nop\n"
                "nop\n"
                "nop\n"
                "nop\n"
                "nop\n"
                "nop\n"
                "nop\n"
                "nop\n"
                "nop");*/ 
        asm ("nop");//1
        asm ("nop");//2
        asm ("nop");//3
        asm ("nop");//4
        asm ("nop");//5
        asm ("nop");//6
        asm ("nop");//7
        asm ("nop");//8
        asm ("nop");//9
        asm ("nop");//10
        
        asm ("nop");//11
        asm ("nop");//12
        asm ("nop");//13
        asm ("nop");//14
        asm ("nop");//15
        asm ("nop");//16
        asm ("nop");//17
        asm ("nop");//18
        asm ("nop");//19
        asm ("nop");//20
        
        asm ("nop");//1
        asm ("nop");//2
        asm ("nop");//3
        asm ("nop");//4
        asm ("nop");//5
        asm ("nop");//6
        asm ("nop");//7
        asm ("nop");//8
        asm ("nop");//9
        asm ("nop");//10
        asm ("nop");//11
        asm ("nop");//12
        asm ("nop");//13
        asm ("nop");//14
        asm ("nop");//15
        asm ("nop");//16
        asm ("nop");//17
        asm ("nop");//18
        asm ("nop");//19
        asm ("nop");//20
        asm ("nop");//21
        asm ("nop");//22
        asm ("nop");//23
        asm ("nop");//24
        asm ("nop");//25
        asm ("nop");//26
        asm ("nop");//27
        asm ("nop");//28
        asm ("nop");//29
        asm ("nop");//30
        asm ("nop");//31
        asm ("nop");//32
        asm ("nop");//33
        asm ("nop");//34
        asm ("nop");//35
        asm ("nop");//36
        asm ("nop");//37
        asm ("nop");//38
        asm ("nop");//39
        asm ("nop");//40
        asm ("nop");//41
        asm ("nop");//42
        asm ("nop");//43
        asm ("nop");//44
        asm ("nop");//45
        asm ("nop");//46
        asm ("nop");//47
        asm ("nop");//48
        asm ("nop");//49
        asm ("nop");//50
        asm ("nop");//51
        asm ("nop");//52
        asm ("nop");//53
        asm ("nop");//54
        asm ("nop");//55
        asm ("nop");//56
        asm ("nop");//57
        asm ("nop");//58
        asm ("nop");//59
        asm ("nop");//60
        asm ("nop");//61
        asm ("nop");//62
        asm ("nop");//63
        asm ("nop");//64
        asm ("nop");//65
        asm ("nop");//66
        asm ("nop");//67
        asm ("nop");//68
        asm ("nop");//69
        asm ("nop");//70
        asm ("nop");//71
        asm ("nop");//72
        asm ("nop");//73
        asm ("nop");//74
        asm ("nop");//75
        asm ("nop");//76
        asm ("nop");//77
        asm ("nop");//78
        asm ("nop");//79
        asm ("nop");//80     
        asm ("nop");//81
        asm ("nop");//82
        asm ("nop");//83
        asm ("nop");//84
        asm ("nop");//85
        asm ("nop");//86
        asm ("nop");//87
        asm ("nop");//88
        asm ("nop");//89
        asm ("nop");//90       
        asm ("nop");//91
        asm ("nop");//92
        asm ("nop");//93
        asm ("nop");//94
        asm ("nop");//95
        asm ("nop");//96
        asm ("nop");//97
        asm ("nop");//98
        asm ("nop");//99
        asm ("nop");//100
    
        kt >>= 1;
    
        if (KB_DI_Get == 0)             //DI�͵�ƽ
        {
          kt &= 0x7f;
        } 
        
        else                            //DI�ߵ�ƽ
        {
          kt |= 0x80;
        }
        
        KB_CLKH;                        //ʱ�������أ�1bit��ȡ���
    } 
    
    INIT_KB_DO;                         //����DIOΪ�����
    
    return kt;
}

/**
**Function: MyDisplay
**Description: ��ʾ����
**Calls:   ��
**Called By: 
**Table Accessed: ��
**Table Updated: ��
**Input:  ms
**Output: ��
**Return: ��
**Others:��
**/    
void MyDisplay()
{
    int i;
    unsigned char add, DisChar;
    
    add = 0xC0;
    
    for (i = 0; i < 8; i++)   
    {
      DisChar = ShowText[i];
      DisplayB(add, DisChar);
      add += 0x02;
    }
}

/**
**Function: InEnter
**Description: ���»س�������
**Calls:   ��
**Called By: 
**Table Accessed: ��
**Table Updated: ��
**Input:  ��
**Output: ��
**Return: ��
**Others:��
**/    
void InEnter(void)
{
    int i;

    for (i = 0; i < 4; i++) 
    {
      ShowText[i] = ShowText[i + 4];    //���Ų����ƶ�������
      ShowText[i + 4] = 0x00;           //������ʾ����
    }

    ValueTest = StrToInt(InputString);  //�ַ�������ת��

    for (i = 0; i < 5; i++)             //�����ַ���������
    {
      InputString[i] = '0';
    }
    
    guPushNum = 0;                      
    PointFlag = 0;                      //С����'.'��־����
    EnterFlag = 1;                      //�����־��λ  
    delayms1(3000);                     //��ʱ�ȴ�
}

/*
**Name:int gCommand(unsigned char ch1,unsigned char ch2,unsigned char ch3)--��ֵ����
**Parameter:the name of variable that you want to chage the value(��Ҫ�ı�ı���������)
**Output:get the value from the key(ͨ�����������ֵ)
*/
int gCommand(unsigned char ch1,unsigned char ch2,unsigned char ch3)
{
    unsigned char comStr[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    int value = 0;
    int i;
    
    comStr[0] = LedEncode(ch1); //��ʾ��һ���ַ�
    comStr[1] = LedEncode(ch2); //��ʾ�ڶ����ַ�
    comStr[2] = LedEncode(ch3); //��ʾ�������ַ�
  
    for (i = 0; i < 8; i++)
    {
      ShowText[i] = comStr[i];  //������ʾ
    }
  
    EnterFlag = 0;              //�����־����
  
    while (EnterFlag != 1)      //'#'��û�а���
    {
      KeyScan();                //����ɨ��
      KeyDo();                  //��ֵ����
      MyDisplay();              //��ʾ
      value = ValueTest;        //����
    }
  
    return value;
}

//�ű���gCommand
int gCommand_Beacon(unsigned char ch1,unsigned char ch2,unsigned char ch3,unsigned char ch4)
{
  int i = 0;
  int value = 0;
  unsigned char comStr[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  
  comStr[0] = LedEncode(ch1);
  comStr[1] = LedEncode(ch2);
  comStr[2] = LedEncode(ch3);
  comStr[3] = LedEncode(ch4);
  
  for(i = 0; i < 8; i++)
  {
    guComStr[i]=0x00;
  }
  
  guComStr[0]=comStr[0];
  guComStr[1]=comStr[1];
  guComStr[2]=comStr[2];
  guComStr[3]=comStr[3];
  
  for (i = 0; i < 8; i++)
  {
    ShowText[i] = comStr[i];
  }
  
  PointFlag = 0;
  EnterFlag = 0; 
  while(EnterFlag != 1) 
  {
    KeyScan();
    KeyDo_Beacon();
    MyDisplay();
    value = ValueTest;
  }
    
  return value;
}


/**
**Function: KeyDecode LedEncode
**Description: ���̡���ʾ��������
**Calls:   ��
**Called By: 
**Table Accessed: ��
**Table Updated: ��
**Input:  ��
**Output: ��
**Return: ��
**Others:��
**/    
unsigned char KeyDecode(unsigned char KeyCode)
{
    switch (KeyCode)
    {
        #ifdef VerticalKeyboard
        case 0x01:return ('#');
                  break;
        case 0x02:return ('0');
                  break;
        case 0x04:return ('.');
                  break;
        case 0x10:return ('9');
                  break;
        case 0x20:return ('8');
                  break;
        case 0x40:return ('7');
                  break;
        case 0x81:return ('6');
                  break;
        case 0x82:return ('5');
                  break;
        case 0x84:return ('4');
                  break;
        case 0x90:return ('3');
                  break;
        case 0xa0:return ('2');
                  break;
        case 0xc0:return ('1');
                  break;
        #elif defined HorizontalKeyboard
        case 0x01:return ('1');
                  break;
        case 0x02:return ('2');
                  break;
        case 0x04:return ('3');
                  break;
        case 0x10:return ('4');
                  break;
        case 0x20:return ('5');
                  break;
        case 0x40:return ('6');
                  break;
        case 0x81:return ('7');
                  break;
        case 0x82:return ('8');
                  break;
        case 0x84:return ('9');
                  break;
        case 0x90:return ('.');
                  break;
        case 0xa0:return ('0');
                  break;
        case 0xc0:return ('#');
                  break;
        #endif
        default  :return ('-');
                  break;
    }

}

unsigned char LedEncode(unsigned char LedCode)
{
    switch (LedCode)
    {
        case '3':return (0x4f);
                 break;
        case '2':return (0x5b);
                 break;
        case '1':return (0x06);
                 break;
        case '6':return (0x7d);
                 break;
        case '5':return (0x6d);
                 break;
        case '4':return (0x66);
                 break;
        case '9':return (0x6f);
                 break;
        case '8':return (0x7f);
                 break;
        case '7':return (0x07);
                 break;
        case '#':return (0x0c);
                 break;
        case '0':return (0x3f);
                 break;
        case '.':return (0x80);
                 break;
        case 'a':return (0x77);
                 break;
        case 'b':return (0x7c);
                 break;
        case 'c':return (0x58);
                 break;
        case 'd':return (0x5e);
                 break;
        case 'e':return (0x79);
                 break;
        case 'f':return (0x71);
                 break;
        case 'g':return (0x6f);
                 break;
        case 'h':return (0x74);
                 break;
        case 'i':return (0x30);
                 break;
        case 'j':return (0x1e);
                 break;
        case 'l':return (0x38);
                 break;
        case 'n':return (0x54);
                 break;
        case 'o':return (0x5c);
                 break;
        case 'p':return (0x73);
                 break;
        case 'q':return (0x67);
                 break;
        case 'r':return (0x50);
                 break;
        case 's':return (0x6d);
                 break;
        case 't':return (0x78);
                 break;
        case 'u':return (0x1c);
                 break;
        case 'y':return (0x6e);
                 break;
        case 'A':return (0x77);
                 break;
        case 'C':return (0x39);
                 break;
        case 'H':return (0x76);
                 break;
        case 'J':return (0x1f);
                 break;
        case 'N':return (0x80);
                 break;
        case 'O':return (0x3f);
                 break;
        case 'U':return (0x3e);
                 break;
        case 'G':return (0x3d);
                 break;
        case ' ':return (0x00);
                 break;
        case '-':return (0x40);
                 break;
        default  :return (0x08);
                 break;
    }
}
/*
**������
*/

/**
**Function: keyboardadscreen_SD_Start
**Description: SD��д��ʼ��ʾ
**Calls:   ��
**Called By: 
**Table Accessed: ��
**Table Updated: ��
**Input:  ��
**Output: ��
**Return: ��
**Others:��
**/   
void keyboardadscreen_SD_Start()
{
  KeyPortInit();
  DisplayA(LedEncode('1'));     

}
/**
**Function: keyboardadscreen_SD_Stop
**Description: SD��д����ʾ
**Calls:   ��
**Called By: 
**Table Accessed: ��
**Table Updated: ��
**Input: ��
**Output: ��
**Return: ��
**Others:��
**/ 
void keyboardadscreen_SD_Stop()
{
   
    int add = 0xC0;
    
    KeyPortInit();
    add += 0x08;

    DisplayB(add, LedEncode(ThousandUp+'0'));
    add += 0x02;
    DisplayB(add, LedEncode(HundredUp+'0'));
    add += 0x02;
    DisplayB(add, LedEncode(TenUp+'0'));
    add += 0x02;
    DisplayB(add, LedEncode(SingleUp+'0'));

}

/**
**Function: keyboardinput
**Description: �������뺯��
**Calls:   KeyPortInit
**Called By: main.c
**Table Accessed: ��
**Table Updated: ��
**Input:  ��
**Output: ��
**Return: ��
**Others:��
**/ 
void keyboardinput(void) 
{
    KeyPortInit();
    DisplayA(0x4f); 
  
    gfDesirSpeed_High = gCommand('A', 's', 'h');        //����״̬
    gfDesirSpeed_Mid = gCommand('A', 's', 't');         //����״̬
    gfDesirSpeed_Low = gCommand('A', 's', 'l');         //����״̬
    giSp = gCommand('s', 'p', ' ');                     //�ٶ�P
    giSi = gCommand('s', 'i', ' ');                     //�ٶ�I
    //giSd = gCommand('s', 'd', ' ');                     //�ٶ�D
    giDp = gCommand('d', 'p', '_');                     //����P//80
    giDi = 0;//gCommand('d', 'i', '_');                     //����I
    giDd = gCommand('d', 'd', '_');                     //����D//200
    //giDk = gCommand('d', 'u', '_');                   //ת��������΢��
    //giAddOffset = gCommand('d', 'A', 'd');            //�����ϲ���
    SDTIME = gCommand('s', 'd', 't');                   //TF����¼ʱ��
    giBeaconExcept = gCommand('n', 'u', 'b');           //�ű����
    //giMeterSetValue = gCommand('d', 'i', 's');          //�ű�·������
    giBeaconOrder1 = gCommand_Beacon('O', 'r', 'd','1');//�ű�ת�����ǰ5���ű�
    giBeaconOrder2 = gCommand_Beacon('O', 'r', 'd','2');//�ű�ת������5���ű�
    giBeaconOrder3 = gCommand_Beacon('O', 'r', 'd','3');//�ű�ת��������5���ű�
    giBeaconSpeed1 = gCommand_Beacon('A', 's', 'l','1');//�ű�ת�����ǰ5���ű���ٶ�
    giBeaconSpeed2 = gCommand_Beacon('A', 's', 'l','2');//�ű�ת������5���ű���ٶ�
    giBeaconSpeed3 = gCommand_Beacon('A', 's', 'l','3');//�ű�ת��������5���ű���ٶ�
}
  
/**
**Function: keyboardadscreen
**Description: ������ʾ����
**Calls:   KeyPortInit
**Called By: main.c
**Table Accessed: ��
**Table Updated: ��
**Input:  ��
**Output: ��
**Return: ��
**Others:��
**/            
void keyboardadscreen(void)
{
    int add = 0xC0;
    KeyPortInit();
    
    DisplayB(add, LedEncode(ThousandUp+'0'));
    add += 0x02;
    DisplayB(add, LedEncode(HundredUp+'0'));
    add += 0x02;
    DisplayB(add, LedEncode(TenUp+'0'));
    add += 0x02;
    DisplayB(add, LedEncode(SingleUp+'0'));
    add += 0x02;
  
    DisplayB(add, LedEncode(ThousandDown+'0'));
    add += 0x02;
    DisplayB(add, LedEncode(HundredDown+'0'));
    add += 0x02;
    DisplayB(add, LedEncode(TenDown+'0'));
    add += 0x02;
    DisplayB(add, LedEncode(SingleDown+'0')); 
}
