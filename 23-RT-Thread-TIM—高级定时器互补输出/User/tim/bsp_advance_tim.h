#ifndef __ADVANCE_TIM_H
#define	__ADVANCE_TIM_H

#include "stm32f4xx.h"

/* ��ʱ�� */
#define ADVANCE_TIM           				TIM8
#define ADVANCE_TIM_CLK_ENABLE()  			__TIM8_CLK_ENABLE()

/* TIM8ͨ��1������� */
#define ADVANCE_OCPWM_PIN           		GPIO_PIN_5             
#define ADVANCE_OCPWM_GPIO_PORT     		GPIOI                    
#define ADVANCE_OCPWM_GPIO_CLK_ENABLE() 	__GPIOI_CLK_ENABLE()
#define ADVANCE_OCPWM_AF					GPIO_AF3_TIM8

/* TIM8ͨ��1����������� */
#define ADVANCE_OCNPWM_PIN            		GPIO_PIN_13              
#define ADVANCE_OCNPWM_GPIO_PORT      		GPIOH                     
#define ADVANCE_OCNPWM_GPIO_CLK_ENABLE()	__GPIOH_CLK_ENABLE()
#define ADVANCE_OCNPWM_AF					GPIO_AF3_TIM8

/* TIM8��·�������� */
#define ADVANCE_BKIN_PIN              		GPIO_PIN_6              
#define ADVANCE_BKIN_GPIO_PORT        		GPIOA                      
#define ADVANCE_BKIN_GPIO_CLK_ENABLE()  	__GPIOA_CLK_ENABLE()
#define ADVANCE_BKIN_AF						GPIO_AF3_TIM8


extern TIM_HandleTypeDef TIM_TimeBaseStructure;

void TIMx_Configuration(void);

#endif /* __ADVANCE_TIM_H */

