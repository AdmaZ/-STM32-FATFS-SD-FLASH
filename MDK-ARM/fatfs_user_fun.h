#ifndef __FATFS_USER_FUN_
#define __FATFS_USER_FUN_

/* ----------------- includes ---------------------------------------*/
#include "main.h"
#include "fatfs.h"
#include "sdio.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

#include "string.h"
#include "../MDK-ARM/bsp_led.h"
#include "../MDK-ARM/bsp_usart1.h"
#include "../MDK-ARM/bsp_SPI_Flash.h"

/* ----------------- Prototypes ---------------------------------------*/

FRESULT FlashfMount(void);//flash�����ļ�ϵͳ
FRESULT SDfMount(void);//SD �� �����ļ�ϵͳ
FRESULT FlashDeviceInfoGet(void);//flash ��Ϣ��ȡ
FRESULT SDDeviceInfoGet(void);//SD�� ��Ϣ��ȡ
FRESULT scan_files(char *path);//�����ļ�
void FlashTraverFiles(void);//����flash�ļ�
void SDTraverFiles(void);//����SD���ļ�
void FlashWriteFile(char *string, int num, char WriteBuffer[num]);//�� Flash ��д�ļ�
void SDWriteFile(char *string, int num, char WriteBuffer[num]);//�� SD ����д�ļ�
void FlashReadFile(char *string);
void SDReadFile(char *string);

#endif
