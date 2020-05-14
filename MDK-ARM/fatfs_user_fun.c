/* ----------------- includes ---------------------------------------*/

#include "fatfs_user_fun.h"

/* ----------------- Private variables ---------------------------------------*/

char buff[6];

/* ----------------- function ---------------------------------------*/

FRESULT FlashfMount()
{
    FRESULT retUSER;
    retUSER = f_mount(&USERFatFS, "1:/", 1);//Ĭ�Ͻ�flash���䵽���� 1:/
    if (retUSER == FR_NO_FILESYSTEM)
    {
        printf("��SPI FLASH ��û���ļ�ϵͳ���������и�ʽ��...\n\n");
        /* ��ʽ�� */
        retUSER = f_mkfs("1:/", 0, 0);
    }
    else if (retUSER != FR_OK)
    {
        printf("����SPI FLASH �����ļ�ϵͳʧ�ܡ�(%d)\n\n", retUSER);
        printf("��������ԭ��SPI FLASH ��ʼ�����ɹ���\n\n");
        while (1);
    }
    else
    {
        printf("��SPI FLASH �ļ�ϵͳ���سɹ������Խ��ж�д����\n\n");
    }
    return retUSER;
}
FRESULT SDfMount()
{
    FRESULT retSD;
    retSD = f_mount(&SDFatFS, "0:/", 1); //Ĭ�Ͻ�SD�����䵽���� 0:/
    if (retSD == FR_NO_FILESYSTEM)
    {
        printf("��SD�� ��û���ļ�ϵͳ���������и�ʽ��...\n\n");
        /* ��ʽ�� */
        retSD = f_mkfs("0:/", 0, 0);
    }
    else if (retSD != FR_OK)
    {
        printf("����SD �������ļ�ϵͳʧ�ܡ�(%d)\n\n", retSD);
        printf("��������ԭ��SD�� ��ʼ�����ɹ���\n\n");
        while (1);
    }
    else
    {
        printf("��SD�� �ļ�ϵͳ���سɹ������Խ��ж�д����\n\n");
    }
    return retSD;
}

FRESULT FlashDeviceInfoGet()
{
    FRESULT retUSER;
    printf("******** �豸��Ϣ��ȡ ********\n\n");
    FATFS *fs;
    DWORD fre_clust, fre_sect, tot_sect;

    /*��ȡ������1�ľ���Ϣ�Ϳ���Ⱥ��*/
    retUSER = f_getfree("1:/", &fre_clust, &fs);//Ĭ��������Ϊ1

    /*��ȡ���������Ϳ���������*/
    tot_sect = (fs-> n_fatent - 2) * fs-> csize;
    fre_sect = fre_clust * fs-> csize;

    /* ��ӡ���ÿռ䣨����ÿ����4096�ֽڣ� */
    printf("%10lu Kb ���������ռ�.\n%10lu KB ����.\n\n", tot_sect * 4, fre_sect * 4);

    return retUSER;
}

FRESULT SDDeviceInfoGet()
{
    FRESULT retSD;

    printf("******** �豸��Ϣ��ȡ ********\n\n");
    FATFS *fs;
    DWORD fre_clust, fre_sect, tot_sect;


    /*��ȡ������0�ľ���Ϣ�Ϳ���Ⱥ��*/
    retSD = f_getfree("0:/", &fre_clust, &fs);//Ĭ��������Ϊ0

    /*��ȡ���������Ϳ���������*/
    tot_sect = (fs-> n_fatent - 2) * fs-> csize;
    fre_sect = fre_clust * fs-> csize;

    /* ��ӡ���ÿռ䣨����ÿ����512�ֽڣ� */
    printf("%10lu MB ���������ռ�.\n%10lu MB ����.\n\n", tot_sect / 2 / 1024, fre_sect / 2 / 1024);

    return retSD;
}


FRESULT scan_files(char *path)
{
    FRESULT res;      //�����ڵݹ���̱��޸ĵı���������ȫ�ֱ���
    FILINFO fno;
    DIR dir;
    int i;
    char *fn;        // �ļ���

#if _USE_LFN
    /* ���ļ���֧�� */
    /* ����������Ҫ2���ֽڱ���һ�����֡�*/
    static char lfn[_MAX_LFN * 2 + 1];
    fno.lfname = lfn;
    fno.lfsize = sizeof(lfn);
#endif
    res = f_opendir(&dir, path); //��Ŀ¼
    if (res == FR_OK)
    {
        i = strlen(path);
        for (;;)
        {
            //��ȡĿ¼�µ����ݣ��ٶ����Զ�����һ���ļ�
            res = f_readdir(&dir, &fno);
            //Ϊ��ʱ��ʾ������Ŀ��ȡ��ϣ�����
            if (res != FR_OK || fno.fname[0] == 0) break;
#if _USE_LFN
            fn = *fno.lfname ? fno.lfname : fno.fname;
#else
            fn = fno.fname;
#endif
            //���ʾ��ǰĿ¼������
            if (*fn == '.') continue;
            //Ŀ¼���ݹ��ȡ
            if (fno.fattrib & AM_DIR)
            {
                //�ϳ�����Ŀ¼��
                sprintf(&path[i], "/%s", fn);
                //�ݹ����
                res = scan_files(path);
                path[i] = 0;
                //��ʧ�ܣ�����ѭ��
                if (res != FR_OK)
                    break;
            }
            else
            {
                printf("%s/%s\r\n", path, fn);//����ļ���
                /* ������������ȡ�ض���ʽ���ļ�·�� */
            }
        }
    }
    res = f_closedir(&dir);//�ر�Ŀ¼
    return res;
}

void SDTraverFiles()
{
    printf("******** ���� SD�� �������ļ� ********\n\n");
    strcpy(buff, "0:/");
    retSD = scan_files(buff);
    printf("\n");
}

void FlashTraverFiles()
{
    printf("******** ���� SPI FLASH	�������ļ� ********\n\n");
    strcpy(buff, "1:/");
    retUSER = scan_files(buff);
    printf("\n");
}

void FlashWriteFile(char *string, int num, char WriteBuffer[num] )
{
    UINT fnumfalsh;
    /* ���ļ�������ļ��������򴴽��� */
    printf("******** ���������ļ�д�����... ********\n\n");
    retUSER = f_open(&USERFile, string,
                     FA_CREATE_ALWAYS | FA_WRITE);

    if (retUSER == FR_OK)
    {
        printf("����/���� �ļ��ɹ������ļ�д�����ݡ�\r\n");
        /* ��ָ���洢������д�뵽�ļ��� */
        retUSER = f_write(&USERFile, (void *)WriteBuffer, strlen((void *)WriteBuffer), &fnumfalsh);
        if (retUSER == FR_OK)
        {
            printf("���ļ�д��ɹ���д���ֽ����ݣ�%d\n", fnumfalsh);
            printf("�����ļ�д�������Ϊ��\n\n%s \n\r\n", WriteBuffer);
        }
        else
        {
            printf("�����ļ�д��ʧ�ܣ�(%d)\n", retUSER);
        }

        /* ���ٶ�д���ر��ļ� */
        f_close(&USERFile);
    }
    else
    {
        LED_RED;
        printf("������/�����ļ�ʧ�ܡ�\n\n");
    }
}

void FlashReadFile(char *string)
{
    char ReadBuffer[255];
    UINT fnum;
    printf("******** ���������ļ���ȡ����... ********\n\n");
    retUSER = f_open(&USERFile, string, FA_OPEN_EXISTING | FA_READ);
    if (retUSER == FR_OK)
    {
        LED_GREEN;
        printf("�����ļ��ɹ���\r\n");
        retUSER = f_read(&USERFile, ReadBuffer, sizeof(ReadBuffer), &fnum);
        if (retUSER == FR_OK)
        {
            printf("���ļ���ȡ�ɹ�,�����ֽ����ݣ�%d\r\n", fnum);
            printf("����ȡ�õ��ļ�����Ϊ��\n\n%s \n\r\n", ReadBuffer);
        }
        else
        {
            printf("�����ļ���ȡʧ�ܣ�(%d)\n", retUSER);
        }
    }
    else
    {
        LED_RED;
        printf("�������ļ�ʧ�ܡ�\r\n");
    }
    /* ���ٶ�д���ر��ļ� */
    f_close(&USERFile);
}
void SDWriteFile(char *string, int num, char WriteBuffer[num] )
{

    UINT fnum;
    /* ���ļ�������ļ��������򴴽��� */
    printf("******** ���������ļ�д�����... ********\n\n");
    retSD = f_open(&SDFile, string,
                     FA_CREATE_ALWAYS | FA_WRITE);

    if (retSD == FR_OK)
    {
        printf("����/���� �ļ��ɹ������ļ�д�����ݡ�\r\n");
        /* ��ָ���洢������д�뵽�ļ��� */
        retSD = f_write(&SDFile, (void *)WriteBuffer, strlen((void *)WriteBuffer), &fnum);
				if (retSD == FR_OK)
        {
            printf("���ļ�д��ɹ���д���ֽ����ݣ�%d\n", fnum);
            printf("�����ļ�д�������Ϊ��\n\n%s \n\r\n", WriteBuffer);
        }
        else
        {
            printf("�����ļ�д��ʧ�ܣ�(%d)\n", retSD);
        }

        /* ���ٶ�д���ر��ļ� */
        f_close(&SDFile);
    }
    else
    {
        LED_RED;
        printf("������/�����ļ�ʧ�ܡ�\n\n");
    }
}
void SDReadFile(char *string)
{
    char ReadBuffer[255];
    UINT fnum;
		uint8_t res;
    printf("******** ���������ļ���ȡ����... ********\n\n");
    res = f_open(&SDFile, string, FA_OPEN_EXISTING | FA_READ);
    if (res == FR_OK)
    {
        LED_GREEN;
        printf("�����ļ��ɹ���\r\n");
        res = f_read(&SDFile, ReadBuffer, sizeof(ReadBuffer), &fnum);
        if (res == FR_OK)
        {
            printf("���ļ���ȡ�ɹ�,�����ֽ����ݣ�%d\r\n", fnum);
            printf("����ȡ�õ��ļ�����Ϊ��\n\n%s \n\r\n", ReadBuffer);
        }
        else
        {
            printf("�����ļ���ȡʧ�ܣ�(%d)\n", res);
        }
    }
    else
    {
        LED_RED;
        printf("�������ļ�ʧ�ܡ�\r\n");
    }
    /* ���ٶ�д���ر��ļ� */
    f_close(&SDFile);
}
/************************ (C) COPYRIGHT Adma Z *****END OF FILE****/

#if 0
    /*----------------------- FLASH �ļ�ϵͳ -----------------*/
    //��ʼ������������������
    //f_mount()->find_volume()->disk_initialize->SPI_FLASH_Init()
    //��SD�������ļ�ϵͳ���ļ�ϵͳ����ʱ���SD����ʼ��
    retSD = f_mount(&SDFatFS, "0:/", 1);
    //���ⲿSPI Flash�����ļ�ϵͳ���ļ�ϵͳ����ʱ���SPI�豸��ʼ��
    retUSER = f_mount(&USERFatFS, "1:/", 1);

    /*----------------------- ��ʽ������ -----------------*/
    printf("******** SPI FLASH �ļ�ϵͳ ��ʼ���� ********\n\n");
    /* ���û���ļ�ϵͳ�͸�ʽ�����������ļ�ϵͳ */
    if (retUSER == FR_NO_FILESYSTEM)
    {
        printf("��FLASH��û���ļ�ϵͳ���������и�ʽ��...\n\n");
        /* ��ʽ�� */
        retUSER = f_mkfs("1:/", 0, 0);

        if (retUSER == FR_OK)
        {
            printf("��FLASH�ѳɹ���ʽ���ļ�ϵͳ��\n\n");
            /* ��ʽ������ȡ������ */
            retUSER = f_mount(NULL, "1:/", 1);
            /* ���¹���   */
            retUSER = f_mount(&USERFatFS, "1:/", 1);
        }
        else
        {
            LED_RED;
            printf("������ʽ��ʧ�ܡ�����\n\n");
            while (1);
        }
    }
    else if (retUSER != FR_OK)
    {
        printf("�����ⲿFlash�����ļ�ϵͳʧ�ܡ�(%d)\n\n", retUSER);
        printf("��������ԭ��SPI Flash��ʼ�����ɹ���\n\n");
        while (1);
    }
    else
    {
        printf("���ļ�ϵͳ���سɹ������Խ��ж�д����\r\n");
    }


    /*----------------------- �洢�ռ��ȡ ---------------------------*/
    printf("******** �豸��Ϣ��ȡ ********\r\n");
    FATFS *fsflash;
    DWORD fre_clustflash, fre_sectflash, tot_sectflash;

    /*��ȡ������1�ľ���Ϣ�Ϳ���Ⱥ��*/
    retUSER = f_getfree("1:/", &fre_clustflash, &fsflash);

    /*��ȡ���������Ϳ���������*/
    tot_sectflash = (fsflash-> n_fatent - 2) * fsflash-> csize;
    fre_sectflash = fre_clustflash * fsflash-> csize;

    /* ��ӡ���ÿռ䣨����ÿ����4096�ֽڣ� */
    printf("%10lu Kb ���������ռ�.\n%10lu Kb ����.\n\n", tot_sectflash * 4, fre_sectflash * 4);



    /*----------------------- �ļ�ϵͳ���ԣ�д���� -----------------------------*/
    /* ���ļ�������ļ��������򴴽��� */
    printf("******** ���������ļ�д�����... ********\n\n");
    retUSER = f_open(&USERFile, "1:/FLASHFatFs��д�����ļ�.txt",
                     FA_CREATE_ALWAYS | FA_WRITE);

    if (retUSER == FR_OK)
    {
        printf("����/����FLASHFatFs��д�����ļ�.txt�ļ��ɹ������ļ�д�����ݡ�\r\n");
        /* ��ָ���洢������д�뵽�ļ��� */
        retUSER = f_write(&USERFile, WriteBuffer, sizeof(WriteBuffer), &fnumfalsh);

        if (retUSER == FR_OK)
        {
            printf("���ļ�д��ɹ���д���ֽ����ݣ�%d\n", fnumfalsh);
            printf("�����ļ�д�������Ϊ��\r\n%s\r\n", WriteBuffer);
        }
        else
        {
            printf("�����ļ�д��ʧ�ܣ�(%d)\n", retUSER);
        }

        /* ���ٶ�д���ر��ļ� */
        f_close(&USERFile);
    }
    else
    {
        LED_RED;
        printf("������/�����ļ�ʧ�ܡ�\n\n");
    }
    /*------------------- �ļ�ϵͳ���ԣ������� --------------------------*/
    printf("******** ���������ļ���ȡ����... ********\n\n");
    retUSER = f_open(&USERFile, "1:/FLASHFatFs��д�����ļ�.txt", FA_OPEN_EXISTING | FA_READ);
    if (retUSER == FR_OK)
    {
        LED_GREEN;
        printf("�����ļ��ɹ���\r\n");
        retUSER = f_read(&USERFile, ReadBuffer, sizeof(ReadBuffer), &fnum);
        if (retUSER == FR_OK)
        {
            printf("���ļ���ȡ�ɹ�,�����ֽ����ݣ�%d\r\n", fnum);
            printf("����ȡ�õ��ļ�����Ϊ��\r\n%s \r\n", ReadBuffer);
        }
        else
        {
            printf("�����ļ���ȡʧ�ܣ�(%d)\n", retUSER);
        }
    }
    else
    {
        LED_RED;
        printf("�������ļ�ʧ�ܡ�\r\n");
    }
    /* ���ٶ�д���ر��ļ� */
    f_close(&USERFile);

    /* -------------------ɨ��Flash����ļ�-------------------- */

    printf("******** ����FLASH���������ļ� ********\n\n");
    printf("buff:%s\n", buff);
    if (retUSER == FR_OK)
    {
        strcpy(buff, "1:/");
        retUSER = scan_files(buff);
    }

    f_mount(NULL, "1:/", 1);

    printf("\r\n****** ����һ��SD�� �ļ�ϵͳʵ�� ******\n\n");

    printf("��SD��(%d)\r\n", retSD);
    printf("��SDPathΪ��%s\r\n", SDPath);

    /*----------------------- ��ʽ������ ---------------------------*/
    /* ���û���ļ�ϵͳ�͸�ʽ�����������ļ�ϵͳ */
    if (retSD == FR_NO_FILESYSTEM)
    {
        printf("��SD����û���ļ�ϵͳ���������и�ʽ��...\r\n");
        /* ��ʽ�� */
        retSD = f_mkfs((TCHAR const *)SDPath, 0, 0);

        if (retSD == FR_OK)
        {
            printf("��SD���ѳɹ���ʽ���ļ�ϵͳ��\r\n");
            /* ��ʽ������ȡ������ */
            retSD = f_mount(NULL, (TCHAR const *)SDPath, 1);
            /* ���¹��� */
            retSD = f_mount(&SDFatFS, (TCHAR const *)SDPath, 1);
        }
        else
        {
            printf("������ʽ��ʧ�ܡ�����\r\n");

            while (1);
        }
    }
    else if (retSD != FR_OK)
    {
        printf("����SD�������ļ�ϵͳʧ�ܡ�(%d)\r\n", retSD);
        printf("(%d)\r\n", retSD);

        while (1);
    }
    else
    {
        printf("���ļ�ϵͳ���سɹ������Խ��ж�д����\n\n");
    }

    /*----------------------- �洢�ռ��ȡ ---------------------------*/
    printf("******** �豸��Ϣ��ȡ ********\n\n");
    FATFS *fs;
    DWORD fre_clust, fre_sect, tot_sect;


    /*��ȡ������0�ľ���Ϣ�Ϳ���Ⱥ��*/
    retSD = f_getfree("0:/", &fre_clust, &fs);

    /*��ȡ���������Ϳ���������*/
    tot_sect = (fs-> n_fatent - 2) * fs-> csize;
    fre_sect = fre_clust * fs-> csize;

    /* ��ӡ���ÿռ䣨����ÿ����512�ֽڣ� */
    printf("%10lu MB ���������ռ�.\n%10lu MB ����.\n\n", tot_sect / 2 / 1024, fre_sect / 2 / 1024);



    /*----------------------- �ļ�ϵͳ���ԣ�д���� -----------------------------*/
    /* ���ļ�������ļ��������򴴽��� */
    printf("******** ���������ļ�д�����... ********\n\n");
    retSD = f_open(&SDFile, "0:/SD��FatFs��д�����ļ�.txt",
                   FA_CREATE_ALWAYS | FA_WRITE);

    if (retSD == FR_OK)
    {
        printf("����/����SD��FatFs��д�����ļ�.txt�ļ��ɹ������ļ�д�����ݡ�\r\n");
        /* ��ָ���洢������д�뵽�ļ��� */
        retSD = f_write(&SDFile, WriteBuffer, sizeof(WriteBuffer), &fnum);

        if (retSD == FR_OK)
        {
            printf("���ļ�д��ɹ���д���ֽ����ݣ�%d\r\n", fnum);
            printf("�����ļ�д�������Ϊ��\r\n%s\r\n", WriteBuffer);
        }
        else
        {
            printf("�����ļ�д��ʧ�ܣ�(%d)\r\n", retSD);
        }

        /* ���ٶ�д���ر��ļ� */
        f_close(&SDFile);
    }
    else
    {
        printf("������/�����ļ�ʧ�ܡ�\r\n");
    }

    /*------------------- �ļ�ϵͳ���ԣ������� ------------------------------------*/
    printf("******** ���������ļ���ȡ����... ********\n\n");
    retSD = f_open(&SDFile, "0:/SD��FatFs��д�����ļ�.txt",
                   FA_OPEN_EXISTING | FA_READ);

    if (retSD == FR_OK)
    {
        printf("�����ļ��ɹ���\r\n");
        retSD = f_read(&SDFile, ReadBuffer, sizeof(ReadBuffer), &fnum);

        if (retSD == FR_OK)
        {
            printf("���ļ���ȡ�ɹ�,�����ֽ����ݣ�%d\r\n", fnum);
            printf("����ȡ�õ��ļ�����Ϊ��\r\n%s \r\n", ReadBuffer);
        }
        else
        {
            printf("�����ļ���ȡʧ�ܣ�(%d)\r\n", retSD);
        }
    }
    else
    {
        printf("�������ļ�ʧ�ܡ�\r\n");
    }

    /* ���ٶ�д���ر��ļ� */
    f_close(&SDFile);

    /* -------------------ɨ��SD����ļ�-------------------- */

    printf("\n******** ����SD���������ļ� ********\r\n");

    if (retSD == FR_OK)
    {
        strcpy(buff, "0:/");
        retSD = scan_files(buff);
    }


    printf("\n******** SD��������� ********\r\n\n");
    /* --------------ע��һ��SD��������---------------------*/
    f_mount(NULL, (TCHAR const *)SDPath, 1);


    /* --------------------------������ɣ�ͣ��---------------------------- */
#endif
