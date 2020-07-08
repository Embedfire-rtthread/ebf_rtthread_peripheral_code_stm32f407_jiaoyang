#ifndef __BSP_ADC_H
#define	__BSP_ADC_H

#include "stm32f4xx.h"

// PC3 ͨ����ñ�ӵ�λ��
// ADC IO�궨��
#define RHEOSTAT_ADC_GPIO_PORT             GPIOB
#define RHEOSTAT_ADC_GPIO_PIN              GPIO_PIN_0
#define RHEOSTAT_ADC_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE()
#define RHEOSTAT_ADC_CHANNEL               ADC_CHANNEL_8

// PA4 ͨ����ñ�ӹ�������
// ADC IO�궨��
#define RL_ADC_GPIO_PORT                   GPIOB
#define RL_ADC_GPIO_PIN                    GPIO_PIN_1
#define RL_ADC_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOB_CLK_ENABLE()
#define RL_ADC_CHANNEL                     ADC_CHANNEL_9

// ADC ��ź궨��
#define RHEOSTAT_ADC1                       ADC1
#define RL_ADC2                             ADC2
#define RHEOSTAT_ADC1_CLK_ENABLE()          __HAL_RCC_ADC1_CLK_ENABLE()
#define RL_ADC2_CLK_ENABLE()                __HAL_RCC_ADC2_CLK_ENABLE()


// ADC DR�Ĵ����궨�壬ADCת���������ֵ����������
#define RHEOSTAT_ADC_DR_ADDR                ((uint32_t)0x40012308)

// ADC DMA ͨ���궨�壬��������ʹ��DMA����
#define RHEOSTAT_ADC_DMA_CLK_ENABLE()       __HAL_RCC_DMA2_CLK_ENABLE()
#define RHEOSTAT_ADC_DMA_CHANNEL            DMA_CHANNEL_0
#define RHEOSTAT_ADC_DMA_STREAM             DMA2_Stream0



void Rheostat_Init(void);

#endif /* __BSP_ADC_H */



