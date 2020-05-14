#ifndef __BSP_LED_H_
#define __BSP_LED_H_
/*
 *������ɫ������߼��÷�ʹ�� PWM �ɻ��ȫ����ɫ,��Ч������
 */
#define			LED_RED		\
							digitalLo(GPIOB,LED_R_Pin)\
							digitalHi(GPIOB,LED_G_Pin)\
							digitalHi(GPIOB,LED_B_Pin)
#define			LED_GREEN		\
							digitalHi(GPIOB,LED_R_Pin)\
							digitalLo(GPIOB,LED_G_Pin)\
							digitalHi(GPIOB,LED_B_Pin)
#define			LED_BLUE		\
							digitalHi(GPIOB,LED_R_Pin)\
							digitalHi(GPIOB,LED_G_Pin)\
							digitalLo(GPIOB,LED_B_Pin)

/** ���� LED ������ĺ꣬
* LED �͵�ƽ�������� ON=0��OFF=1
* �� LED �ߵ�ƽ�����Ѻ����ó� ON=1 ��OFF=0 ����
*/
#define digitalHi(p,i) {p->BSRR=i;} //����Ϊ�ߵ�ƽ
#define digitalLo(p,i) {p->BSRR=(uint32_t)i << 16;}

#endif

/************************ (C) COPYRIGHT Adma ׯ *****END OF FILE****/
