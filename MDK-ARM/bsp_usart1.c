#include "bsp_usart1.h"
#include <stdio.h>

/*****************  �����ַ��� **********************/
/*
void Usart_SendString(uint8_t *str)
{
	unsigned int k=0;
  do
  {
      HAL_UART_Transmit(&huart1,(uint8_t *)(str + k) ,1,1000);
      k++;
  } while(*(str + k)!='\0');

}
*/
//�ض���c�⺯��printf������DEBUG_USART���ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
    /* ����һ���ֽ����ݵ�����DEBUG_USART */
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 1000);

    return (ch);
}

//�ض���c�⺯��scanf������DEBUG_USART����д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
    int ch;
    HAL_UART_Receive(&huart1, (uint8_t *)&ch, 1, 1000);
    return (ch);
}

void Usart_SendString(uint8_t *str)
{
    unsigned int k = 0;
    do {
        HAL_UART_Transmit(&huart1,(uint8_t *)(str+k),1,1000);
        k++;
    } while(*(str+k) != '\0');
}

/************************ (C) COPYRIGHT Adma ׯ *****END OF FILE****/
