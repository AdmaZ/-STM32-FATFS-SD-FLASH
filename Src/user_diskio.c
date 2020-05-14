/* USER CODE BEGIN Header */
/**
 ******************************************************************************
  * @file    user_diskio.c
  * @brief   This file includes a diskio driver skeleton to be completed by the user.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
#include "../MDK-ARM/bsp_SPI_Flash.h"
 /* USER CODE END Header */

#ifdef USE_OBSOLETE_USER_CODE_SECTION_0
/* 
 * Warning: the user section 0 is no more in use (starting from CubeMx version 4.16.0)
 * To be suppressed in the future. 
 * Kept to ensure backward compatibility with previous CubeMx versions when 
 * migrating projects. 
 * User code previously added there should be copied in the new user sections before 
 * the section contents can be deleted.
 */
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */
#endif

/* USER CODE BEGIN DECL */

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "ff_gen_drv.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SPI_FLASH       0     // �ⲿSPI Flash
#define ATA             1     // Ԥ������User�豸
/* Private variables ---------------------------------------------------------*/
/* Disk status */
static volatile DSTATUS Stat = STA_NOINIT;

/* USER CODE END DECL */

/* Private function prototypes -----------------------------------------------*/
DSTATUS USER_initialize (BYTE pdrv);
DSTATUS USER_status (BYTE pdrv);
DRESULT USER_read (BYTE pdrv, BYTE *buff, DWORD sector, UINT count);
#if _USE_WRITE == 1
  DRESULT USER_write (BYTE pdrv, const BYTE *buff, DWORD sector, UINT count);  
#endif /* _USE_WRITE == 1 */
#if _USE_IOCTL == 1
  DRESULT USER_ioctl (BYTE pdrv, BYTE cmd, void *buff);
#endif /* _USE_IOCTL == 1 */

Diskio_drvTypeDef  USER_Driver =
{
  USER_initialize,
  USER_status,
  USER_read, 
#if  _USE_WRITE
  USER_write,
#endif  /* _USE_WRITE == 1 */  
#if  _USE_IOCTL == 1
  USER_ioctl,
#endif /* _USE_IOCTL == 1 */
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes a Drive
  * @param  pdrv: Physical drive number (0..)
  * @retval DSTATUS: Operation status
  */
DSTATUS USER_initialize (
	BYTE pdrv           /* Physical drive nmuber to identify the drive */
)
{
  /* USER CODE BEGIN INIT */
    uint16_t i;
    Stat = STA_NOINIT;

    switch (pdrv)
    {
    case ATA:            /* ATA */
        break;

    case SPI_FLASH:    /* SPI Flash */
        /* ��ʼ��SPI Flash */
        SPI_FLASH_Init();
        /* ��ʱһС��ʱ�� */
        i = 500;

        while (--i);

        /* ����SPI Flash */
        SPI_Flash_WAKEUP();
        /* ��ȡSPI FlashоƬ״̬ */
        Stat = disk_status(SPI_FLASH);
        break;

    default:
        Stat = STA_NOINIT;
    }

    return Stat;
  /* USER CODE END INIT */
}
 
/**
  * @brief  Gets Disk Status 
  * @param  pdrv: Physical drive number (0..)
  * @retval DSTATUS: Operation status
  */
DSTATUS USER_status (
	BYTE pdrv       /* Physical drive number to identify the drive */
)
{
  /* USER CODE BEGIN STATUS */
    Stat = STA_NOINIT;

    switch (pdrv)
    {
    case ATA:   /* ATA */
        break;

    case SPI_FLASH:

        /* SPI Flash״̬��⣺��ȡSPI Flash �豸ID */
        if (sFLASH_ID == SPI_FLASH_ReadID())
        {
            /* �豸ID��ȡ�����ȷ */
            Stat &= ~STA_NOINIT;
        }
        else
        {
            /* �豸ID��ȡ������� */
            Stat = STA_NOINIT;;
        }

        break;

    default:
        Stat = STA_NOINIT;
    }

    return Stat;
  /* USER CODE END STATUS */
}

/**
  * @brief  Reads Sector(s) 
  * @param  pdrv: Physical drive number (0..)
  * @param  *buff: Data buffer to store read data
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to read (1..128)
  * @retval DRESULT: Operation result
  */
DRESULT USER_read (
	BYTE pdrv,      /* Physical drive nmuber to identify the drive */
	BYTE *buff,     /* Data buffer to store read data */
	DWORD sector,   /* Sector address in LBA */
	UINT count      /* Number of sectors to read */
)
{
  /* USER CODE BEGIN READ */
    volatile    DRESULT status = RES_PARERR;

    switch (pdrv)
    {
    case ATA:   /* ATA */
        break;

    case SPI_FLASH:
        /* ����ƫ��2MB���ⲿFlash�ļ�ϵͳ�ռ����SPI Flash����6MB�ռ� */
        sector += 512;
        SPI_FLASH_BufferRead(buff, sector << 12, count << 12);
        status = RES_OK;
        break;

    default:
        status = RES_PARERR;
    }

    return RES_OK;
  /* USER CODE END READ */
}

/**
  * @brief  Writes Sector(s)  
  * @param  pdrv: Physical drive number (0..)
  * @param  *buff: Data to be written
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to write (1..128)
  * @retval DRESULT: Operation result
  */
#if _USE_WRITE == 1
DRESULT USER_write (
	BYTE pdrv,          /* Physical drive nmuber to identify the drive */
	const BYTE *buff,   /* Data to be written */
	DWORD sector,       /* Sector address in LBA */
	UINT count          /* Number of sectors to write */
)
{ 
  /* USER CODE BEGIN WRITE */
    /* USER CODE HERE */
    uint32_t write_addr;
    volatile    DRESULT status = RES_PARERR;

    if (!count)
    {
        return RES_PARERR;      /* Check parameter */
    }

    switch (pdrv)
    {
    case ATA:   /* ATA */
        break;

    case SPI_FLASH:
        /* ����ƫ��2MB���ⲿFlash�ļ�ϵͳ�ռ����SPI Flash����6MB�ռ� */
        sector += 512;
        write_addr = sector << 12;
        SPI_FLASH_SectorErase(write_addr);
        SPI_FLASH_BufferWrite((uint8_t *)buff, write_addr, count << 12);
        status = RES_OK;
        break;

    default:
        status = RES_PARERR;
    }

    return RES_OK;
  /* USER CODE END WRITE */
}
#endif /* _USE_WRITE == 1 */

/**
  * @brief  I/O control operation  
  * @param  pdrv: Physical drive number (0..)
  * @param  cmd: Control code
  * @param  *buff: Buffer to send/receive control data
  * @retval DRESULT: Operation result
  */
#if _USE_IOCTL == 1
DRESULT USER_ioctl (
	BYTE pdrv,      /* Physical drive nmuber (0..) */
	BYTE cmd,       /* Control code */
	void *buff      /* Buffer to send/receive control data */
)
{
  /* USER CODE BEGIN IOCTL */
    DRESULT res = RES_ERROR;

    switch (pdrv)
    {
    case ATA:   /* ATA */
        break;

    case SPI_FLASH:
        switch (cmd)
        {
        /* ����������1536*4096/1024/1024=6(MB) */
        case GET_SECTOR_COUNT:
            *(DWORD *)buff = 1536;
            break;

        /* ������С  */
        case GET_SECTOR_SIZE :
            *(WORD *)buff = 4096;
            break;

        /* ͬʱ������������ */
        case GET_BLOCK_SIZE :
            *(DWORD *)buff = 1;
            break;
        }

        res = RES_OK;
        break;

    default:
        res = RES_PARERR;
    }

    return res;
  /* USER CODE END IOCTL */
}
#endif /* _USE_IOCTL == 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
