/*********************Copyright(C) 2015-2016 TYUT JBD TEAM B.*******************
Copyright (C), 2015-2016, TYUT JBD TEAM B.
File name: keyboard.c 
Author: TEAM B, Alopex
Version: 1.0               
Date: 2015.10.28(E)
      2015.12.31(C)
      2016.03.15(A)
Description: 
            Keyboard Input Display Function(键盘输入显示函数)
Others: Noting
Function List:
            1.keyboardinput 键盘输入参数函数
            2.keyboardadscreen 键盘显示变量函数
            3.keydoBeacon 信标参数键盘扫描函数
            4.gCommandBeacon 信标命令参数输入函数
            5.参数退格键,小数点'.'消除当前错误参数
History:
<author> <time> <version > <desc>
 何雨行 2015/10/28 1.0 修改程序
        2015/12/31 1.1 添加注释
        2016/2/7   1.2 添加注释
        2016/4/1   2.0 添加注释
        2016/4/8   2.1 '.'退格键
        2016/4/9   2.1 添加注释
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
**全局变量定义
*/
unsigned char KeyValue[8];//键盘键值
unsigned char ShowText[8] = {0x3d,0x10,0x50,0x38,0x00,0x00,0x00,0x00};//键盘显示内容
unsigned char InputString[5] = {'0','0','0','0','0'};//输入字符数组
unsigned char KeyCode;//键盘键值编码
unsigned char guPushNum = 0;//字符限制数
unsigned char BackSpace = 0;//退格键
unsigned char BeaconString[10]={'0'};//信标次序数组
unsigned char guComStr[8]={0x00};//当前显示参数字符数字
int ValueTest;//键盘返回值
int ConClick = 0; //连续点击标志
int PointFlag = 0;//小数点标志
int EnterFlag = 0;//回车键按下标志


/**
**Function: KeyPortInit
**Description: I/O端口初始化，M口负责显示输出和键盘输入
**Calls:   无
**Called By: keyboardinput
             keyboardadscreen
**Table Accessed: 无
**Table Updated: 无
**Input:  无
**Output: 无
**Return: 无
**Others:无
**/
void KeyPortInit() 
{ 
    INIT_KB_STB; //初始化STB
    INIT_KB_CLK; //初始化CLK
    INIT_KB_DO;  //初始化DIO,输出
                                      
    KB_CLKH;     //CLK，设置初始高电平
    KB_DOH;      //DIO，设置初始高电平
    KB_STBH;     //STB，设置初始高电平
}

/**
**Function: StrToInt
**Description: 将字符串转化为整数型数据
**Calls:   无
**Called By: 
**Table Accessed: 无
**Table Updated: 无
**Input:  无
**Output: 无
**Return: 无
**Others:无
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
**Description: 将字符串转化成浮点型数据
**Calls:   无
**Called By: 
**Table Accessed: 无
**Table Updated: 无
**Input:  无
**Output: 无
**Return: 无
**Others:无
**/
float StrToFloat(unsigned char *str)    //未测试（不用）
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
        sum = sum * 10 + (str[i] - '0');//整数部分
      }
        
      else
      {
        j = j / 10;  
	frac = frac + (str[i] - '0') * j;//小数部分
       }
    }
    sum = (sum + frac) * sign;
    return sum;
}

/*
**功能函数
*/

/**
**Function: delayms1 delay1
**Description: 延时
**Calls:   无
**Called By: 
**Table Accessed: 无
**Table Updated: 无
**Input:  ms
**Output: 无
**Return: 无
**Others:无
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
**Description: 送数据函数
**Calls:   无
**Called By: 
**Table Accessed: 无
**Table Updated: 无
**Input:  无
**Output: 无
**Return: 无
**Others:无
**/
void SendData(unsigned char data)
{
    int i;
    
    KB_STBL;                            //STB低电平，允许数据操作
    
    for (i = 0; i < 8; i++)
    {
        KB_CLKL;                        //CLK低电平，传输1bit
 
        if (data & 0x01 != 0)           //0,1操作
        {
            KB_DOH;                     //DO高电平
        }  
        
        else
        {
            KB_DOL;                     //DO低电平 
        }
    
        KB_CLKH;                        //CLK高电平,传输完成
        
        data >>= 1;                     //移位
    }
}

/*
**显示函数
*/

/**
**Function: DisplayA
**Description: 显示模式A，地址自动加一模式
**Calls:   无
**Called By: 
**Table Accessed: 无
**Table Updated: 无
**Input:  无
**Output: 无
**Return: 无
**Others:无
**/
void DisplayA(unsigned char text)
{
    int i;
                                        //端口准备
    KB_DOH;                             //DO高电平                             
    KB_CLKH;                            //CLK高电平
    KB_STBH;                            //STB高电平
  
    SendData(WRITE_MODE_A);             //写数据模式
    KB_STBH;                            //STB高电平
    SendData(START_ADD);                //设置起始地址
    
    for (i = 0; i < DATA_COUNT; i++) 
    {                                   //传输显示数据，共16位
        SendData(text);
    }
    
    KB_STBH;
    SendData(LED_CTRL);                 //设置显示亮度
    KB_STBH;
}         

/**
**Function: DisplayB
**Description: 显示模式B，制定地址显示
**Calls:   无
**Called By: 
**Table Accessed: 无
**Table Updated: 无
**Input:  无
**Output: 无
**Return: 无
**Others:无
**/
void DisplayB(unsigned char address,unsigned char text) 
{
                                        //端口准备
    KB_DOH;                             //DO高电平
    KB_CLKH;                            //CLK高电平
    KB_STBH;                            //STB高电平
  
    SendData(WRITE_MODE_B);             //显示模式
    KB_STBH;                            //STB高电平
    SendData(address);                  //传输地址
    SendData(text);                     //传输内容
    KB_STBH;  
}         

/**
**Function: KeyScan KeyDo
**Description: 键盘扫描程序
**Calls:   无
**Called By: 
**Table Accessed: 无
**Table Updated: 无
**Input:  无
**Output: 无
**Return: 无
**Others:无
**/
void KeyScan() 
{
    int i;
    
    KB_STBH;                            //端口STB高
    
    SendData(READ_MODE);                //传送读数据命令
    
    for (i = 0; i < 2; i++)
    {                                   //读数据，共8位，每位代表两行六个按键
      KeyValue[i] = ReadKeyValue(8);    //循环扫描2组2*6键值
    }
    
    KB_STBH;                            //结束扫描，抬高STB电平
}

void KeyDo()
{
    int i, j, k;
    
    for (i = 0; i < 2; i++)             //分别处理上下两排各6个按键的按压情况
    {
        if (i == 1)                     //下排按键编码  
        {
          KeyValue[i] |= 0x80;
        }
        
        if (
            #ifdef VerticalKeyboard
            KeyValue[i] == 0x01
            #elif defined HorizontalKeyboard
            KeyValue[i] == 0xc0
            #endif 
            )                           //如果'#'键按下
        {
          InEnter();                    //输入处理
        }
      
        KeyCode = LedEncode(KeyDecode(KeyValue[i]));//键值编码解码
      
        if ((KeyDecode(KeyValue[i]) == InputString[4] 
             || (
                #ifdef VerticalKeyboard
                (BackSpace == 1 && i == 0)          //竖版键盘'.'键(0x04),退格延时i == 0
                #elif defined HorizontalKeyboard
                (BackSpace == 1 && i == 1)          //横板键盘'.'键(0x90),退格延时i == 1
                #endif
                ) 
                ) && (ConClick == 0))//按键与上一次相同或者'.'退格键按下,连击标志为0
        {
          ConClick = 1;                 //连击标志置位
          BackSpace = 0;                //退格标志清零
          delayms1(PRESS_DELAY);        //按压延时消除抖动
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
              )//键值输入不为' ',且不为'#'
            { 
              if(KeyCode == 0x80 && ShowText[7] != 0x00)//如果输入的是小数点'.'且前一位不为' '
              {                 
                for (i = 4; i < 8; i++) //键盘显示下排参数清零
                {
                  ShowText[i] = 0x00;
                }
                
                for (k = 0; k < 5; k++) //输入字符数组清零
                {
                  InputString[k] = '0';
                }

                BackSpace = 1;          //退格标志置位
                ConClick = 0;           //连击标志清零
              }
              
              else if((ShowText[4] == 0x00) || (ShowText[4] == 0x3f))//键盘显示下排4位未写满
              {         
                for (j = 4; j < 7; j++) //键盘显示
                {
                  ShowText[j] = ShowText[j + 1];  
                }
                    
                ShowText[7] = KeyCode;
                    
                for (k = 0; k < 4; k++) //字符数组记录
                {
                  InputString[k] = InputString[k + 1];
                }
                    
                InputString[4] = KeyDecode(KeyValue[i]);
                ConClick = 0;           //连击标志清零
              }
            }
        }
    }
}

//信标用KeyDo,选择方向
void KeyDo_Beacon(void)
{
  int i, j, k;
  
  for(i = 0;i < 2;i++)//扫描两组按键，0:上面6个按键 1:下面6个按键
  {
    if(i == 1)//将下排按键编码为1
    {
      KeyValue[i] |= 0x80;
    }
    
    if (
        #ifdef VerticalKeyboard
        KeyValue[i] == 0x01
        #elif defined HorizontalKeyboard
        KeyValue[i] == 0xc0
        #endif
        )                   //'#'键输入
    {
      InEnter();            //输入处理
    }
    
    KeyCode = LedEncode(KeyDecode(KeyValue[i]));//键值编码解码
      
    if ((KeyDecode(KeyValue[i]) == InputString[4] 
         || (
            #ifdef VerticalKeyboard
            (BackSpace == 1 && i == 0)          //竖版键盘'.'键(0x04),退格延时i == 0
            #elif defined HorizontalKeyboard
            (BackSpace == 1 && i == 1)          //横板键盘'.'键(0x90),退格延时i == 1
            #endif
            )
            ) && (ConClick == 0))//按键与上一次相同或者'.'退格键按下,连击标志为0
    {
      ConClick = 1;             //连击标志置位
      BackSpace = 0;            //退格标志清零
      delayms1(PRESS_DELAY);    //按压延时消除抖动
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
          )//键值输入不为' ',且不为'#'
      {
        if(KeyCode == 0x3f || KeyCode == 0x06 || KeyCode == 0x80)//只能按键'0','1','.'
        {
          if(KeyCode == 0x80 && ShowText[7] != 0x00)//如果输入的是小数点'.'且前一位不为' '
          {
            guPushNum = 0;//键盘输入参数位数清零
              
            for (i = 0; i < 8; i++)//还原原来的参数显示
            {
              ShowText[i] = guComStr[i];
            }
            
            for (k = 0; k < 5; k++)//输入字符数组清零
            {
              InputString[k] = '0';
            }
            
            BackSpace = 1;      //退格标志置位
            ConClick = 0;       //连击状态清零
          }
          
          else if(guPushNum < 5)//输入字符数目小于5
          {
            guPushNum++;        //输入数目加一
                
            if((ShowText[4] == 0x00))//键盘显示
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
                
            for (k = 0; k < 4; k++)//输入字符数组
            {
              InputString[k] = InputString[k + 1];
            }
                            
            InputString[4] = KeyDecode(KeyValue[i]);
            ConClick = 0;//连击清零
          }
          
       }
        
      }
    }
    
  }
}

/**
**Function: ReadKeyValue
**Description: 读取键值
**Calls:   无
**Called By: 
**Table Accessed: 无
**Table Updated: 无
**Input:  无
**Output: 无
**Return: 无
**Others:无
**/                                                                
unsigned char ReadKeyValue(int bitnum)
{
    unsigned char kt = 0;
    int i = 0;
    
    KB_DOH;                             //DO高电平
    INIT_KB_DI;                         //DIO设为输入口，读数据
                   
    KB_STBL;                            //STB放低，开始读数据
    
    for (i = 0; i < bitnum; i++)
    {                                   //循环读bit
        KB_CLKL;                        //时钟下降沿，开始读一个bit
        
        /*原来是250nop,改完退格键120nop*/
        //这样的形式也可以...ARM汇编伪指令nop做延时用哇...
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
    
        if (KB_DI_Get == 0)             //DI低电平
        {
          kt &= 0x7f;
        } 
        
        else                            //DI高电平
        {
          kt |= 0x80;
        }
        
        KB_CLKH;                        //时钟上升沿，1bit读取完成
    } 
    
    INIT_KB_DO;                         //设置DIO为输出口
    
    return kt;
}

/**
**Function: MyDisplay
**Description: 显示程序
**Calls:   无
**Called By: 
**Table Accessed: 无
**Table Updated: 无
**Input:  ms
**Output: 无
**Return: 无
**Others:无
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
**Description: 按下回车键处理
**Calls:   无
**Called By: 
**Table Accessed: 无
**Table Updated: 无
**Input:  无
**Output: 无
**Return: 无
**Others:无
**/    
void InEnter(void)
{
    int i;

    for (i = 0; i < 4; i++) 
    {
      ShowText[i] = ShowText[i + 4];    //下排参数移动到上排
      ShowText[i + 4] = 0x00;           //下派显示清零
    }

    ValueTest = StrToInt(InputString);  //字符到整形转换

    for (i = 0; i < 5; i++)             //输入字符数组清零
    {
      InputString[i] = '0';
    }
    
    guPushNum = 0;                      
    PointFlag = 0;                      //小数点'.'标志清零
    EnterFlag = 1;                      //输入标志置位  
    delayms1(3000);                     //延时等待
}

/*
**Name:int gCommand(unsigned char ch1,unsigned char ch2,unsigned char ch3)--赋值命令
**Parameter:the name of variable that you want to chage the value(你要改变的变量的名字)
**Output:get the value from the key(通过键盘输入的值)
*/
int gCommand(unsigned char ch1,unsigned char ch2,unsigned char ch3)
{
    unsigned char comStr[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    int value = 0;
    int i;
    
    comStr[0] = LedEncode(ch1); //显示第一个字符
    comStr[1] = LedEncode(ch2); //显示第二个字符
    comStr[2] = LedEncode(ch3); //显示第三个字符
  
    for (i = 0; i < 8; i++)
    {
      ShowText[i] = comStr[i];  //键盘显示
    }
  
    EnterFlag = 0;              //输入标志清零
  
    while (EnterFlag != 1)      //'#'键没有按下
    {
      KeyScan();                //键盘扫描
      KeyDo();                  //键值处理
      MyDisplay();              //显示
      value = ValueTest;        //返回
    }
  
    return value;
}

//信标用gCommand
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
**Description: 键盘、显示编解码程序
**Calls:   无
**Called By: 
**Table Accessed: 无
**Table Updated: 无
**Input:  无
**Output: 无
**Return: 无
**Others:无
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
**主函数
*/

/**
**Function: keyboardadscreen_SD_Start
**Description: SD卡写开始显示
**Calls:   无
**Called By: 
**Table Accessed: 无
**Table Updated: 无
**Input:  无
**Output: 无
**Return: 无
**Others:无
**/   
void keyboardadscreen_SD_Start()
{
  KeyPortInit();
  DisplayA(LedEncode('1'));     

}
/**
**Function: keyboardadscreen_SD_Stop
**Description: SD卡写完显示
**Calls:   无
**Called By: 
**Table Accessed: 无
**Table Updated: 无
**Input: 无
**Output: 无
**Return: 无
**Others:无
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
**Description: 键盘输入函数
**Calls:   KeyPortInit
**Called By: main.c
**Table Accessed: 无
**Table Updated: 无
**Input:  无
**Output: 无
**Return: 无
**Others:无
**/ 
void keyboardinput(void) 
{
    KeyPortInit();
    DisplayA(0x4f); 
  
    gfDesirSpeed_High = gCommand('A', 's', 'h');        //高速状态
    gfDesirSpeed_Mid = gCommand('A', 's', 't');         //中速状态
    gfDesirSpeed_Low = gCommand('A', 's', 'l');         //低速状态
    giSp = gCommand('s', 'p', ' ');                     //速度P
    giSi = gCommand('s', 'i', ' ');                     //速度I
    //giSd = gCommand('s', 'd', ' ');                     //速度D
    giDp = gCommand('d', 'p', '_');                     //方向P//80
    giDi = 0;//gCommand('d', 'i', '_');                     //方向I
    giDd = gCommand('d', 'd', '_');                     //方向D//200
    //giDk = gCommand('d', 'u', '_');                   //转向陀螺仪微分
    //giAddOffset = gCommand('d', 'A', 'd');            //方向上补偿
    SDTIME = gCommand('s', 'd', 't');                   //TF卡记录时间
    giBeaconExcept = gCommand('n', 'u', 'b');           //信标个数
    //giMeterSetValue = gCommand('d', 'i', 's');          //信标路程米数
    giBeaconOrder1 = gCommand_Beacon('O', 'r', 'd','1');//信标转向次序前5个信标
    giBeaconOrder2 = gCommand_Beacon('O', 'r', 'd','2');//信标转向次序后5个信标
    giBeaconOrder3 = gCommand_Beacon('O', 'r', 'd','3');//信标转向次序最后5个信标
    giBeaconSpeed1 = gCommand_Beacon('A', 's', 'l','1');//信标转向次序前5个信标的速度
    giBeaconSpeed2 = gCommand_Beacon('A', 's', 'l','2');//信标转向次序后5个信标的速度
    giBeaconSpeed3 = gCommand_Beacon('A', 's', 'l','3');//信标转向次序最后5个信标的速度
}
  
/**
**Function: keyboardadscreen
**Description: 键盘显示函数
**Calls:   KeyPortInit
**Called By: main.c
**Table Accessed: 无
**Table Updated: 无
**Input:  无
**Output: 无
**Return: 无
**Others:无
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
