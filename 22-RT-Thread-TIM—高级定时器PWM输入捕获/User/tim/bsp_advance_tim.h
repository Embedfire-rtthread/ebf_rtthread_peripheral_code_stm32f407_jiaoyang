#ifndef __ADVANCE_TIM_H
#define	__ADVANCE_TIM_H

#include "stm32f4xx.h"

/* ͨ�ö�ʱ�� */
#define GENERAL_TIM           		    	TIM4
#define GENERAL_TIM_CLK_ENABLE()       		__TIM4_CLK_ENABLE()

/* ͨ�ö�ʱ��PWM��� */
/* PWM������� */
#define GENERAL_OCPWM_PIN            		GPIO_PIN_14             
#define GENERAL_OCPWM_GPIO_PORT      		GPIOD                     
#define GENERAL_OCPWM_GPIO_CLK_ENABLE()		__GPIOD_CLK_ENABLE()
#define GENERAL_OCPWM_AF					GPIO_AF2_TIM3

/* �߼����ƶ�ʱ�� */
#define ADVANCE_TIM           		    	TIM8
#define ADVANCE_TIM_CLK_ENABLE()      		__TIM8_CLK_ENABLE()

/* ����/�Ƚ��ж� */
#define ADVANCE_TIM_IRQn					TIM8_CC_IRQn
#define ADVANCE_TIM_IRQHandler        		TIM8_CC_IRQHandler
/* �߼����ƶ�ʱ��PWM���벶�� */
/* PWM���벶������ */
#define ADVANCE_ICPWM_PIN              		GPIO_PIN_6              
#define ADVANCE_ICPWM_GPIO_PORT        		GPIOC                      
#define ADVANCE_ICPWM_GPIO_CLK_ENABLE()  	__GPIOC_CLK_ENABLE()
#define ADVANCE_ICPWM_AF					GPIO_AF3_TIM8
#define ADVANCE_IC1PWM_CHANNEL        		TIM_CHANNEL_1
#define ADVANCE_IC2PWM_CHANNEL        		TIM_CHANNEL_2

extern TIM_HandleTypeDef  TIM_PWMOUTPUT_Handle;
extern TIM_HandleTypeDef  TIM_PWMINPUT_Handle;

void TIMx_Configuration(void);
#endif /* __ADVANCE_TIM_H */

