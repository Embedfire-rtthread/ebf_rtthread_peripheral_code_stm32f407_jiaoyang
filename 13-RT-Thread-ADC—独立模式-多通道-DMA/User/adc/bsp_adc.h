#ifndef __BSP_ADC_H
#define	__BSP_ADC_H

#include "stm32f4xx.h"

#define RHEOSTAT_NOFCHANEL      3

/*=====================ͨ��1 IO======================*/
// PB0 ͨ����ñ�ӵ�λ��
// ADC IO�궨��
#define RHEOSTAT_ADC_GPIO_PORT1             GPIOB
#define RHEOSTAT_ADC_GPIO_PIN1              GPIO_PIN_0
#define RHEOSTAT_ADC_GPIO_CLK1_ENABLE()     __GPIOB_CLK_ENABLE()
#define RHEOSTAT_ADC_CHANNEL1               ADC_CHANNEL_8
/*=====================ͨ��2 IO ======================*/
// PB1 ͨ����ñ�ӹ�������
// ADC IO�궨��
#define RHEOSTAT_ADC_GPIO_PORT2             GPIOB
#define RHEOSTAT_ADC_GPIO_PIN2              GPIO_PIN_1
#define RHEOSTAT_ADC_GPIO_CLK2_ENABLE()     __GPIOB_CLK_ENABLE()
#define RHEOSTAT_ADC_CHANNEL2               ADC_CHANNEL_9
/*=====================ͨ��3 IO ======================*/
// PA6 ���գ����öŰ��߽�3V3����GND��ʵ��
// ADC IO�궨��
#define RHEOSTAT_ADC_GPIO_PORT3             GPIOA
#define RHEOSTAT_ADC_GPIO_PIN3              GPIO_PIN_6
#define RHEOSTAT_ADC_GPIO_CLK3_ENABLE()     __GPIOA_CLK_ENABLE()
#define RHEOSTAT_ADC_CHANNEL3               ADC_CHANNEL_6
   
// ADC ��ź궨��
#define RHEOSTAT_ADC                        ADC1
#define RHEOSTAT_ADC_CLK_ENABLE()           __ADC1_CLK_ENABLE()

// ADC DR�Ĵ����궨�壬ADCת���������ֵ����������
#define RHEOSTAT_ADC_DR_ADDR                ((uint32_t)ADC1+0x4c)

// ADC DMA ͨ���궨�壬��������ʹ��DMA����
#define RHEOSTAT_ADC_DMA_CLK_ENABLE()       __DMA2_CLK_ENABLE()
#define RHEOSTAT_ADC_DMA_CHANNEL            DMA_CHANNEL_0
#define RHEOSTAT_ADC_DMA_STREAM             DMA2_Stream0

extern ADC_HandleTypeDef ADC_Handle;

void Rheostat_Init(void);

#endif /* __BSP_ADC_H */



