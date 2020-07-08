#ifndef __BSP_ADC_H
#define	__BSP_ADC_H

#include "stm32f4xx.h"

#define RHEOSTAT_NOFCHANEL      3

// PC3 ͨ����ñ�ӵ�λ��
// ADC IO�궨��
#define RHEOSTAT_ADC_GPIO_PORT             GPIOC
#define RHEOSTAT_ADC_GPIO_PIN              GPIO_PIN_2
#define RHEOSTAT_ADC_GPIO_CLK_ENABLE()     __GPIOC_CLK_ENABLE()

// ADC ��ź궨��
#define RHEOSTAT_ADC1                      ADC1
#define RHEOSTAT_ADC2                      ADC2
#define RHEOSTAT_ADC3                      ADC3
#define RHEOSTAT_ADC1_CLK_ENABLE()         __ADC1_CLK_ENABLE()
#define RHEOSTAT_ADC2_CLK_ENABLE()         __ADC2_CLK_ENABLE()
#define RHEOSTAT_ADC3_CLK_ENABLE()         __ADC3_CLK_ENABLE()
#define RHEOSTAT_ADC_CHANNEL               ADC_CHANNEL_12

// ADC DR�Ĵ����궨�壬ADCת���������ֵ����������
#define RHEOSTAT_ADC_DR_ADDR               ((uint32_t)0x40012308)

// ADC DMA ͨ���궨�壬��������ʹ��DMA����
#define RHEOSTAT_ADC_DMA_CLK_ENABLE()       __DMA2_CLK_ENABLE()
#define RHEOSTAT_ADC_DMA_CHANNEL            DMA_CHANNEL_0
#define RHEOSTAT_ADC_DMA_STREAM             DMA2_Stream0



void Rheostat_Init(void);

#endif /* __BSP_ADC_H */



