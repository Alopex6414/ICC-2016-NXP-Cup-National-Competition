/*********************Copyright(C) 2015-2016 TYUT JBD TEAM B.*******************
Copyright (C), 2015-2016, TYUT JBD TEAM B.
File name: bootloader.h 
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
#ifndef __BOOTLOADER_H__
#define __BOOTLOADER_H__

/*#define BASE_ADDR       ((uint32_t)0x0000000UL) //FLASH起始地址,UL:无符号长整形
#define FLASH_APP_ADDR  0x7800UL                //应用程序起始地址(存放在FLASH),UL:无符号长整形
#define BUFF_SIZE       512                     //数组大小512BIT
#define APP_PY	        15

typedef void (*pFunction)(void);
typedef void (*fun)(void);*/

#define FTFx_PHRASE_SIZE                0x0008     /* 8 bytes */
#define USER_APP_ADDRESS                0x010000        //128KB
#define USER_APP_MAX_SIZE               0x0E0000        //896KB  

#endif