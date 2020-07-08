#ifndef __BEEP_H
#define	__BEEP_H


#include "stm32f4xx.h"
#include "./led/bsp_led.h"   

/* ������������ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����Ĵ��뼴�ɸı���Ƶķ��������� */
#define BEEP_GPIO_PORT    	    GPIOG		              /* GPIO�˿� */
#define BEEP_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOG_CLK_ENABLE()  /* GPIO�˿�ʱ�� */
#define BEEP_GPIO_PIN		  	GPIO_PIN_7			          /* ���ӵ���������GPIO */



/* ���κ꣬��������������һ��ʹ�� */
#define BEEP(a)	HAL_GPIO_WritePin(BEEP_GPIO_PORT,BEEP_GPIO_PIN,a)
				


/* �������IO�ĺ� */
#define BEEP_TOGGLE		digitalToggle(BEEP_GPIO_PORT,BEEP_GPIO_PIN)
#define BEEP_ON	        digitalHi(BEEP_GPIO_PORT,BEEP_GPIO_PIN)
#define BEEP_OFF		digitalLo(BEEP_GPIO_PORT,BEEP_GPIO_PIN)

void BEEP_GPIO_Config(void);
					
#endif /* __BEEP_H */
