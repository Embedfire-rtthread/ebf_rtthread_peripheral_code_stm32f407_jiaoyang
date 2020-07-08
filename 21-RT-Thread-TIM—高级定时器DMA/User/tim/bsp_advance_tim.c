/**
  ******************************************************************************
  * @file    bsp_advance_tim.c
  * @author  STMicroelectronics
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   �߼����ƶ�ʱ����ʱ����
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F407 ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "./tim/bsp_advance_tim.h"
TIM_HandleTypeDef TIM_TimeBaseStructure;

DMA_HandleTypeDef TIM_DMA_HANDLE;

DMA_InitTypeDef	TIM_DMA_InitTypeDef;

uint16_t aSRC_Buffer[TIM_DMA_BUFSIZE] ={383, 767, 127};

static void TIM_DMA_Config(void);
/**
  * @brief  ����TIM�������PWMʱ�õ���I/O
  * @param  ��
  * @retval ��
  */
static void TIMx_GPIO_Config(void) 
{
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*������ʱ����ص�GPIO����ʱ��*/
	ADVANCE_OCPWM_GPIO_CLK_ENABLE(); 

	/* ��ʱ���������ų�ʼ�� */
	/* �߼���ʱ�����벶������ */
	GPIO_InitStructure.Pin = ADVANCE_OCPWM_PIN;	
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;    
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH; 	
	GPIO_InitStructure.Alternate = ADVANCE_OCPWM_AF;
	HAL_GPIO_Init(ADVANCE_OCPWM_GPIO_PORT, &GPIO_InitStructure);

}

 /**
  * @brief  �߼����ƶ�ʱ�� TIMx,x[1,8]�ж����ȼ�����
  * @param  ��
  * @retval ��
  */
static void TIMx_NVIC_Configuration(void)
{	
	//������ռ���ȼ��������ȼ�
	HAL_NVIC_SetPriority(ADVANCE_TIM_IRQn, 6, 0);
	// �����ж���Դ
	HAL_NVIC_EnableIRQ(ADVANCE_TIM_IRQn);
	//������ռ���ȼ��������ȼ�
	HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 5, 0);
	// �����ж���Դ
	HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);
}

/*
 * ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIM6��TIM7�ļĴ�������ֻ��
 * TIM_Prescaler��TIM_Period������ʹ��TIM6��TIM7��ʱ��ֻ���ʼ����������Ա���ɣ�
 * ����������Ա��ͨ�ö�ʱ���͸߼���ʱ������.
 *-----------------------------------------------------------------------------
 * TIM_Prescaler         ����
 * TIM_CounterMode			 TIMx,x[6,7]û�У��������У�������ʱ����
 * TIM_Period            ����
 * TIM_ClockDivision     TIMx,x[6,7]û�У���������(������ʱ��)
 * TIM_RepetitionCounter TIMx,x[1,8]����(�߼���ʱ��)
 *-----------------------------------------------------------------------------
 */
static void TIM_Mode_Config(void)
{

	// ����TIMx_CLK,x[1,8] 
	ADVANCE_TIM_CLK_ENABLE(); 
	
	TIM_TimeBaseStructure.Instance = ADVANCE_TIM;
	/* �ۼ� TIM_Period�������һ�����»����ж�*/		
	//����ʱ����0������4999����Ϊ5000�Σ�Ϊһ����ʱ����
	TIM_TimeBaseStructure.Init.Period =5000-1;       
	// �߼����ƶ�ʱ��ʱ��ԴTIMxCLK = HCLK=216MHz 
	// �趨��ʱ��Ƶ��Ϊ=TIMxCLK/(TIM_Prescaler+1)=10000Hz
	TIM_TimeBaseStructure.Init.Prescaler = 16800-1;	
	// ����ʱ�ӷ�Ƶ
	TIM_TimeBaseStructure.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
	// ������ʽ
	TIM_TimeBaseStructure.Init.CounterMode=TIM_COUNTERMODE_UP;
	// �ظ�������:�ظ�1�Σ����������β�����һ���ж�
	TIM_TimeBaseStructure.Init.RepetitionCounter=3;
	
	TIM_TimeBaseStructure.hdma[TIM_DMA_ID_CC1] = &TIM_DMA_HANDLE;
	
	// ��ʼ����ʱ��TIMx, x[1,8]
	HAL_TIM_PWM_Init(&TIM_TimeBaseStructure);

	TIM_DMA_Config();

	TIM_OC_InitTypeDef TIM_OC_Init;
	/*PWMģʽ����*/
	//����ΪPWMģʽ1
	TIM_OC_Init.OCMode = TIM_OCMODE_PWM1;
	TIM_OC_Init.Pulse = 383;
	TIM_OC_Init.OCPolarity = TIM_OCPOLARITY_HIGH;
	TIM_OC_Init.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	TIM_OC_Init.OCIdleState = TIM_OCIDLESTATE_SET;
	TIM_OC_Init.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	
	//��ʼ��ͨ��1���PWM 
	HAL_TIM_PWM_ConfigChannel(&TIM_TimeBaseStructure,&TIM_OC_Init,TIM_CHANNEL_1);

}

static void TIM_DMA_Config(void)
{
	TIM_DMA_CLK;
	
	TIM_DMA_HANDLE.Instance 									= TIM_DMA_STREAM;
	TIM_DMA_HANDLE.Init.Channel								= TIM_DMA_CHANNEL;
	TIM_DMA_HANDLE.Init.Direction							= DMA_MEMORY_TO_PERIPH;
	TIM_DMA_HANDLE.Init.FIFOMode							= DMA_FIFOMODE_DISABLE;
	TIM_DMA_HANDLE.Init.MemDataAlignment			= DMA_MDATAALIGN_HALFWORD;
	TIM_DMA_HANDLE.Init.MemInc								= DMA_MINC_ENABLE;
	TIM_DMA_HANDLE.Init.Mode									= DMA_CIRCULAR;
	TIM_DMA_HANDLE.Init.PeriphDataAlignment		= DMA_PDATAALIGN_HALFWORD;
	TIM_DMA_HANDLE.Init.PeriphInc							= DMA_PINC_DISABLE;
	TIM_DMA_HANDLE.Init.Priority							= DMA_PRIORITY_HIGH;
	
	HAL_DMA_Init(TIM_TimeBaseStructure.hdma[TIM_DMA_ID_CC1]);
	
	__HAL_LINKDMA(&TIM_TimeBaseStructure,hdma[TIM_DMA_ID_CC1],TIM_DMA_HANDLE);

}
/**
  * @brief  ��ʼ���߼����ƶ�ʱ����ʱ��1s����һ���ж�
  * @param  ��
  * @retval ��
  */
void TIMx_Configuration(void)
{
	TIMx_GPIO_Config();
	TIMx_NVIC_Configuration();
	TIM_Mode_Config();	
	HAL_TIM_PWM_Start_DMA(&TIM_TimeBaseStructure,TIM_CHANNEL_1,(uint32_t *)aSRC_Buffer,3);
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	//����DMA�������ݣ������PWM����ص�
}
/*********************************************END OF FILE**********************/
