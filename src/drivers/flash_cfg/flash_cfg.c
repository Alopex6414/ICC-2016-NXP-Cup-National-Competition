/*
 * File:		swap_demo.c
 * Purpose:		Main process
 *
 */

//#include "common.h"
//#include "uif.h"
#include "stdio.h"
#include "flash_cfg.h"
#include "SSD_FTFx.h"
#include <stdint.h>

static uint32_t  _flash_write_offset;
static uint32_t  _flash_read_offset = 0;


/* Flash driver structure */
 FLASH_SSD_CONFIG ftfl_cfg =
    {
        FTFx_REG_BASE,          /* FTFx control register base */
        PFLASH_BLOCK_BASE,      /* base address of PFlash block */
        PFLASH_SIZE,            /* size of PFlash block */
        DEFLASH_BLOCK_BASE,     /* base address of DFlash block */
        0,                      /* size of DFlash block */
        EERAM_BLOCK_BASE,       /* base address of EERAM block */
        EERAM_BLOCK_SIZE,       /* size of EERAM block */
        0,                      /* size of EEE block */
        DEBUGENABLE,            /* background debug mode enable bit */
        NULL_CALLBACK           /* pointer to callback function */
    };

/*********************************************************************
*
*  Function Name    : main
*  Description      : swap demo
*  Arguments        : 
*  Return Value     : 
*
*********************************************************************/
 
 
uint8_t flash_erase()
{
    UINT32 returnCode;      /* return code variable */
    
    returnCode = FlashEraseSector(&ftfl_cfg, USER_APP_ADDRESS, USER_APP_MAX_SIZE, FlashCommandSequence);
    if (returnCode == FTFx_OK)
    {
        //printf("FlashEraseSector Success!\r\n");
        
    } else
    {
        printf("FlashEraseSector Failed!\r\n");
        return 0;
    }
    
    returnCode == FlashVerifySection(&ftfl_cfg, USER_APP_ADDRESS, USER_APP_MAX_SIZE, 0, FlashCommandSequence);
    if (returnCode == FTFx_OK)
    {
       // printf("FlashVerifySection Success!\r\n");
        
    } else
    {
        printf("FlashVerifySection Failed!\r\n");
        return 0;
    }
    
    _flash_write_offset = 0;
    return 1;
}


/*uint32_t flash_write(char * buff, uint32_t bufflen)
{
    UINT32 returnCode,temp, faddr;
    
    faddr = USER_APP_ADDRESS + _flash_write_offset;
    temp = bufflen % FTFx_PHRASE_SIZE;
    if(temp != 0)
    {
        bufflen -= temp;
    }
    
    returnCode = FlashProgramPhrase(&ftfl_cfg, faddr, bufflen, (uint32_t)buff, FlashCommandSequence);
    if (returnCode != FTFx_OK)
    {
     	return 0;
        
    } 
    
    _flash_write_offset += bufflen;
    
    return bufflen;    
}

uint32_t flash_read(char * buff, uint32_t bufflen)
{
    UINT32 i,temp, faddr;
    
    temp = _flash_write_offset - _flash_read_offset;
    bufflen = bufflen > temp ? temp : bufflen;    
    faddr = USER_APP_ADDRESS + _flash_read_offset;
    for(i=0; i<bufflen; i++)
    {
        buff[i] = (char)(*(volatile char *)(faddr + i));
    }
    _flash_read_offset += bufflen;
    return bufflen;
}*/

void ErrorTrap(UINT32 returnCode)
{
    UINT32 failedReason;
    failedReason = returnCode;
    
        if(failedReason  == FTFx_OK)
          printf("No Error: FTFx_OK \n"); 
        if((failedReason & FTFx_ERR_SIZE) == FTFx_ERR_SIZE)
          printf("Error: FTFx_ERR_SIZE \n");
        if((failedReason & FTFx_ERR_RANGE) == FTFx_ERR_RANGE)
          printf("Error: FTFx_ERR_RANGE \n");
        if((failedReason & FTFx_ERR_ACCERR) == FTFx_ERR_ACCERR)
          printf("Error: FTFx_ERR_ACCERR \n");
        if((failedReason & FTFx_ERR_PVIOL) == FTFx_ERR_PVIOL)
          printf("Error: FTFx_ERR_PVIOL \n");
        if((failedReason & FTFx_ERR_MGSTAT0) == FTFx_ERR_MGSTAT0)
          printf("Error: FTFx_ERR_MGSTAT0 \n");
        if((failedReason & FTFx_ERR_CHANGEPROT) == FTFx_ERR_CHANGEPROT)
          printf("Error: FTFx_ERR_CHANGEPROT \n");
        if((failedReason & FTFx_ERR_EEESIZE) == FTFx_ERR_EEESIZE)
          printf("Error: FTFx_ERR_EEESIZE \n");
        if((failedReason & FTFx_ERR_EFLASHSIZE) == FTFx_ERR_EFLASHSIZE)
          printf("Error: FTFx_ERR_EFLASHSIZE \n");
        if((failedReason & FTFx_ERR_ADDR) == FTFx_ERR_ADDR)
          printf("Error: FTFx_ERR_ADDR \n");
        if((failedReason & FTFx_ERR_NOEEE) == FTFx_ERR_NOEEE)
          printf("Error: FTFx_ERR_NOEEE \n");
        if((failedReason & FTFx_ERR_EFLASHONLY) == FTFx_ERR_EFLASHONLY)
          printf("Error: FTFx_ERR_EFLASHONLY \n");
        if((failedReason & FTFx_ERR_DFLASHONLY) == FTFx_ERR_DFLASHONLY)
          printf("Error: FTFx_ERR_DFLASHONLY \n");
        if((failedReason & FTFx_ERR_RDCOLERR) == FTFx_ERR_RDCOLERR)
          printf("Error: FTFx_ERR_RDCOLERR \n");
        if((failedReason & FTFx_ERR_RAMRDY) == FTFx_ERR_RAMRDY)
          printf("Error: FTFx_ERR_RAMRDY \n");

        
    while (1)
    {
        ;
    }
}

/* end of file */
