/*********************Copyright(C) 2015-2016 TYUT JBD TEAM B.*******************
Copyright (C), 2015-2016, TYUT JBD TEAM B.
File name: bootloader.c 
Author: TEAM B, Alopex
Version: 1.0               
Date: 2015.10.28(E)
      2015.12.31(C)
      2016.03.15(A)
Description: 
            Updata_App Function(更新文件App函数)
            JumpToUserApplication(跳转到用户函数)
Others: Noting
Function List:
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

/*char buff_filename[15][20];	
unsigned long filesize[15];
u8 buff1[BUFF_SIZE];
u16 i_name=0;
s16 xuan=0;
fun AppStart;

u8 stringcopy(char *buff_to,char *buff_from)
{
  u8 i=0;
  
  for(i=0;i<13;i++)
  {
    *buff_to++ = *buff_from++;
  }
  
  return 1;
}

void UserApplication(u32 app_start_addr)
{
  pFunction jump_to_application;
  u32 jump_addr;
  jump_addr = *(u32*)(app_start_addr + 4);
  
  if((app_start_addr != 0xfffffffful) && (jump_addr > 0x7800 ))
  {
    jump_to_application = (pFunction)jump_addr;
    __set_MSP(*(u32*)app_start_addr); //栈地址
    jump_to_application();
  }
}

void ReadSDFile(void)
{
  FATFS fs;//文件系统
  FRESULT res;//文件应答
  FILINFO finfo;//文件信息
  DIR dirs;//扇区
  int result;
  
  i_name=0;
  res=f_mount(0, &fs);//打开文件
  
  if(f_opendir(&dirs,"") == FR_OK)//打开根目录
  {
    while(f_readdir(&dirs, &finfo) == FR_OK)//依次读扇区
    {
      if(finfo.fattrib & AM_ARC)//判断是否为存档型文件
      {
        if(!finfo.fname[0])
        {
          break;
        }
        
        result=strstr(finfo.fname,".BIN");//判断是否为".bin"文件
        
        if(result!=NULL)
        {
          stringcopy(buff_filename[i_name], (char*)finfo.fname);//把bin的文件名拷贝到buff_filename数组
          filesize[i_name]=finfo.fsize;////把bin的文件大小保存到filesize数组
	  i_name++;
        }
      }
    }
  }
  
  f_mount(0,NULL);

  if(i_name >0)
  {
    ;
  }
  
  else
  {
    UserApplication(FLASH_APP_ADDR);
  }
  
}

void SD_UPdata(void)
{
  FIL fdst;//文件
  FATFS fs;//文件系统
  FRESULT res;//文件状态
  u32 a=0,b=0;
  uint32 br;
  u16 pag=0;
  u16 ByteOffest;
  
  f_mount(0, &fs);//文件系统
  res = f_open(&fdst,(char const*)buff_filename[xuan], FA_OPEN_EXISTING  | FA_READ);//打开第xuan个bin文件   xuan是刚刚确定好的
 
  a = fdst.fsize / 250;
  a &= 0xfffffffe;
  b = 0;
  
  while(1)//把bin文件内容从TF卡读取,然后写入flash...当写入成功后会跳出此循环
  {
    while(flash_erase_sector(APP_PY+pag)!=0);//擦除第APP_PY+pag页,1页有2048个字节...
    
    //下面把2048字节分成4分写入flash。。因为我这fatfs文件系统每次读取2048个字节会死机，所以分成4份来去读
    res = f_read(&fdst, buff1, BUFF_SIZE, &br);//读取bin的BUFF_SIZE(512)字节复制到buff1数组
    
    //第一部分扇区
    if (res || br == 0)//当bin文件内容全部读取完毕后跳出循环
    {
      break;
    }
    
    for(ByteOffest=0;ByteOffest<512;ByteOffest+=4)
    {
      while(flash_write(APP_PY+pag,ByteOffest,*(u32*)(buff1 + ByteOffest))!=0);//把buff1数组内容逐渐写到flash      
    }
    res = f_read(&fdst, buff1, BUFF_SIZE, &br);
    
    //第二部分扇区
    if (res || br == 0)//当bin文件内容全部读取完毕后跳出循环
    {
      break;
    }
    
    for(ByteOffest=0;ByteOffest<512;ByteOffest+=4)
    {
      while(flash_write(APP_PY+pag,ByteOffest+512,*(u32*)(buff1 + ByteOffest))!=0);//把buff1数组内容逐渐写到flash
    }
    res = f_read(&fdst, buff1, BUFF_SIZE, &br);
      
    //第三部分扇区
    if (res || br == 0)//当bin文件内容全部读取完毕后跳出循环
    {
      break;
    }
    
    for(ByteOffest=0;ByteOffest<512;ByteOffest+=4)
    {
      while(flash_write(APP_PY+pag,ByteOffest+1024,*(u32*)(buff1 + ByteOffest))!=0);//把buff1数组内容逐渐写到flash
    }
    res = f_read(&fdst, buff1, BUFF_SIZE, &br);
    
    //第四部分扇区
    if (res || br == 0)//当bin文件内容全部读取完毕后跳出循环
    {
      break;
    }
    
    for(ByteOffest=0;ByteOffest<512;ByteOffest+=4)
    {
      while(flash_write(APP_PY+pag,ByteOffest+1536,*(u32*)(buff1 + ByteOffest))!=0);//把buff1数组内容逐渐写到flash
    }
    res = f_read(&fdst, buff1, BUFF_SIZE, &br);
    
    pag++;//页数加一
  }
  
  f_close(&fdst);//关闭文件系统
  f_mount(0, 0);
  UserApplication(FLASH_APP_ADDR);
  
}*/

void JumpToUserApplication(uint32_t userSP, uint32_t userStartup)
{
  __asm("msr msp, r0");
  __asm("msr psp, r0");
  __asm("mov pc,  r1");
}

void Updata_App(void)
{
    char filebuff[512]={0};
    uint32 filebuffoffset;
    uint32_t i, len, temp;
    FATFS fs;
    FRESULT res;
    FIL faddata, fileRead;

    res = f_mount(0,&fs);
    res = f_open(&faddata,"MYBOY.bin",FA_OPEN_EXISTING | FA_READ);
    
    flash_erase();
    
    while(1)
    {
        filebuffoffset = 0;
        res = f_read(&faddata, filebuff, 512, &filebuffoffset);//read to RAM
        if(FR_OK != res)
        {
            break;
        }

        if(filebuffoffset == 512)
        {
            flash_write(filebuff, filebuffoffset);
        }
        else if(filebuffoffset < 512)
        {
            temp = filebuffoffset % FTFx_PHRASE_SIZE;
            if(temp != 0)
            {
                temp = FTFx_PHRASE_SIZE - temp;
                for(i=0;i<temp; i++)
                {
                    filebuff[filebuffoffset + i] = 0;
                }
            }

            flash_write(filebuff, filebuffoffset + temp); // flash it to flash memory

            break;
        }
    }
    res = f_close(&faddata);

    JumpToUserApplication(*((unsigned long*)USER_APP_ADDRESS), *((unsigned long*)(USER_APP_ADDRESS+4))); //jumpt to the APP
}