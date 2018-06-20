/*********************Copyright(C) 2015-2016 TYUT JBD TEAM B.*******************
Copyright (C), 2015-2016, TYUT JBD TEAM B.
File name: bootloader.c 
Author: TEAM B, Alopex
Version: 1.0               
Date: 2015.10.28(E)
      2015.12.31(C)
      2016.03.15(A)
Description: 
            Updata_App Function(�����ļ�App����)
            JumpToUserApplication(��ת���û�����)
Others: Noting
Function List:
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
    __set_MSP(*(u32*)app_start_addr); //ջ��ַ
    jump_to_application();
  }
}

void ReadSDFile(void)
{
  FATFS fs;//�ļ�ϵͳ
  FRESULT res;//�ļ�Ӧ��
  FILINFO finfo;//�ļ���Ϣ
  DIR dirs;//����
  int result;
  
  i_name=0;
  res=f_mount(0, &fs);//���ļ�
  
  if(f_opendir(&dirs,"") == FR_OK)//�򿪸�Ŀ¼
  {
    while(f_readdir(&dirs, &finfo) == FR_OK)//���ζ�����
    {
      if(finfo.fattrib & AM_ARC)//�ж��Ƿ�Ϊ�浵���ļ�
      {
        if(!finfo.fname[0])
        {
          break;
        }
        
        result=strstr(finfo.fname,".BIN");//�ж��Ƿ�Ϊ".bin"�ļ�
        
        if(result!=NULL)
        {
          stringcopy(buff_filename[i_name], (char*)finfo.fname);//��bin���ļ���������buff_filename����
          filesize[i_name]=finfo.fsize;////��bin���ļ���С���浽filesize����
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
  FIL fdst;//�ļ�
  FATFS fs;//�ļ�ϵͳ
  FRESULT res;//�ļ�״̬
  u32 a=0,b=0;
  uint32 br;
  u16 pag=0;
  u16 ByteOffest;
  
  f_mount(0, &fs);//�ļ�ϵͳ
  res = f_open(&fdst,(char const*)buff_filename[xuan], FA_OPEN_EXISTING  | FA_READ);//�򿪵�xuan��bin�ļ�   xuan�Ǹո�ȷ���õ�
 
  a = fdst.fsize / 250;
  a &= 0xfffffffe;
  b = 0;
  
  while(1)//��bin�ļ����ݴ�TF����ȡ,Ȼ��д��flash...��д��ɹ����������ѭ��
  {
    while(flash_erase_sector(APP_PY+pag)!=0);//������APP_PY+pagҳ,1ҳ��2048���ֽ�...
    
    //�����2048�ֽڷֳ�4��д��flash������Ϊ����fatfs�ļ�ϵͳÿ�ζ�ȡ2048���ֽڻ����������Էֳ�4����ȥ��
    res = f_read(&fdst, buff1, BUFF_SIZE, &br);//��ȡbin��BUFF_SIZE(512)�ֽڸ��Ƶ�buff1����
    
    //��һ��������
    if (res || br == 0)//��bin�ļ�����ȫ����ȡ��Ϻ�����ѭ��
    {
      break;
    }
    
    for(ByteOffest=0;ByteOffest<512;ByteOffest+=4)
    {
      while(flash_write(APP_PY+pag,ByteOffest,*(u32*)(buff1 + ByteOffest))!=0);//��buff1����������д��flash      
    }
    res = f_read(&fdst, buff1, BUFF_SIZE, &br);
    
    //�ڶ���������
    if (res || br == 0)//��bin�ļ�����ȫ����ȡ��Ϻ�����ѭ��
    {
      break;
    }
    
    for(ByteOffest=0;ByteOffest<512;ByteOffest+=4)
    {
      while(flash_write(APP_PY+pag,ByteOffest+512,*(u32*)(buff1 + ByteOffest))!=0);//��buff1����������д��flash
    }
    res = f_read(&fdst, buff1, BUFF_SIZE, &br);
      
    //������������
    if (res || br == 0)//��bin�ļ�����ȫ����ȡ��Ϻ�����ѭ��
    {
      break;
    }
    
    for(ByteOffest=0;ByteOffest<512;ByteOffest+=4)
    {
      while(flash_write(APP_PY+pag,ByteOffest+1024,*(u32*)(buff1 + ByteOffest))!=0);//��buff1����������д��flash
    }
    res = f_read(&fdst, buff1, BUFF_SIZE, &br);
    
    //���Ĳ�������
    if (res || br == 0)//��bin�ļ�����ȫ����ȡ��Ϻ�����ѭ��
    {
      break;
    }
    
    for(ByteOffest=0;ByteOffest<512;ByteOffest+=4)
    {
      while(flash_write(APP_PY+pag,ByteOffest+1536,*(u32*)(buff1 + ByteOffest))!=0);//��buff1����������д��flash
    }
    res = f_read(&fdst, buff1, BUFF_SIZE, &br);
    
    pag++;//ҳ����һ
  }
  
  f_close(&fdst);//�ر��ļ�ϵͳ
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