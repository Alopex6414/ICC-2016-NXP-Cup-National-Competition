/*
 * File:		swap_demo.h
 * Purpose:		Main process
 *
 */

#ifndef FLASH_CFG_H
#define FLASH_CFG_H

#include "SSD_Types.h"
#include <stdint.h>
//#include "SSD_Types.h"
//#include "SSD_FTFx.h"
//#include "SSD_FTFx_Internal.h"
     
#define USER_APP_ADDRESS                0x010000        //128KB
#define USER_APP_MAX_SIZE               0x0E0000        //896KB   



#define FLASH_SWAP_INDICATOR_ADDR  0x7F000  //last sector of lower half

#define PFLASH_HALF0_BASE  0x00000000
#define PFLASH_HALF1_BASE  0x00080000
#define PFLASH_BLOCK_SIZE  0x00040000
#define PFLASH_HALF_SIZE   0x00080000

#define READ_NORMAL_MARGIN        0x00
#define READ_USER_MARGIN          0x01
#define READ_FACTORY_MARGIN       0x02

#define EE_ENABLE                 0x00
#define RAM_ENABLE                0xFF
#define DEBUGENABLE               0x00
#define PFLASH_IFR_ADDR           0x00000000
#define DFLASH_IFR_ADDR           0x00800000

#define SECTOR_SIZE               0x00001000 /* 4 KB size */
#define BUFFER_SIZE_BYTE          0x10

/* FTFL module base */
#define FTFx_REG_BASE           0x40020000
#define DFLASH_IFR_BASE         0xFFFFFFFF      /* unused */
#define PFLASH_BLOCK_BASE       0x00000000
#define DEFLASH_BLOCK_BASE      0xFFFFFFFF      /* There is not DFlash */
#define EERAM_BLOCK_BASE        0x14000000

#define PFLASH_SIZE             0x00100000      /* 1 MB size */
#define EERAM_BLOCK_SIZE        0x00001000      /* 4 KB size */

#define pFlashInit                      (FlashInit)
#define pDEFlashPartition               (DEFlashPartition)
#define pSetEEEEnable                   (SetEEEEnable)
#define pPFlashSetProtection            (PFlashSetProtection)
#define pPFlashGetProtection            (PFlashGetProtection)
#define pFlashVerifySection             (FlashVerifySection)
#define pFlashVerifyBlock               (FlashVerifyBlock)
#define pFlashVerifyAllBlock            (FlashVerifyAllBlock)
#define pFlashSetInterruptEnable        (FlashSetInterruptEnable)
#define pFlashSecurityBypass            (FlashSecurityBypass)
#define pFlashReadResource              (FlashReadResource)
#define pFlashReadOnce                  (FlashReadOnce)
#define pFlashProgramSection            (FlashProgramSection)
#define pFlashProgramOnce               (FlashProgramOnce)
/* #define pFlashProgramLongword           (FlashProgramLongword) */
#define pFlashProgramPhrase             (FlashProgramPhrase)
#define pFlashProgramCheck              (FlashProgramCheck)
#define pFlashGetSecurityState          (FlashGetSecurityState)
#define pFlashGetInterruptEnable        (FlashGetInterruptEnable)
#define pFlashEraseSuspend              (FlashEraseSuspend)
#define pFlashEraseBlock                (FlashEraseBlock)
#define pFlashEraseAllBlock             (FlashEraseAllBlock)
#define pFlashCommandSequence           (FlashCommandSequence)
#define pFlashCheckSum                  (FlashCheckSum)
#define pEERAMSetProtection             (EERAMSetProtection)
#define pEERAMGetProtection             (EERAMGetProtection)
#define pEEEWrite                       (EEEWrite)
#define pDFlashSetProtection            (DFlashSetProtection)
#define pDFlashGetProtection            (DFlashGetProtection)
#define pFlashEraseResume               (FlashEraseResume)
#define pFlashEraseSector               (FlashEraseSector)
#define pPFlashGetSwapStatus            (PFlashGetSwapStatus)
#define pPFlashSwap                     (PFlashSwap)

void ErrorTrap(UINT32 returnCode);


extern uint8_t flash_erase();
//extern uint32_t flash_write(char * buff, uint32_t bufflen);
//extern uint32_t flash_read(char * buff, uint32_t bufflen);

#endif /* FLASH_CFG_H */
