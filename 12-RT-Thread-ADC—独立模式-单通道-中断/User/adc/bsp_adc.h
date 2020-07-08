#ifndef __BSP_ADC_H
#define	__BSP_ADC_H

#include "stm32f4xx.h"

// ADC GPIO �궨��
#define RHEOSTAT_ADC_GPIO_PORT              GPIOB
#define RHEOSTAT_ADC_GPIO_PIN               GPIO_PIN_0
#define RHEOSTAT_ADC_GPIO_CLK_ENABLE()      __GPIOB_CLK_ENABLE()
    
// ADC ��ź궨��
#define RHEOSTAT_ADC                        ADC1
#define RHEOSTAT_ADC_CLK_ENABLE()           __ADC1_CLK_ENABLE()
#define RHEOSTAT_ADC_CHANNEL                ADC_CHANNEL_8

// ADC �жϺ궨��
#define Rheostat_ADC_IRQ                    ADC_IRQn
#define Rheostat_ADC_INT_FUNCTION           ADC_IRQHandler

extern ADC_HandleTypeDef ADC_Handle;

void Rheostat_Init(void);

#endif /* __BSP_ADC_H */



