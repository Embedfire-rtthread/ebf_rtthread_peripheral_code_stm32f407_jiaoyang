/**
  ******************************************************************************
  * @file    bsp_usart_dma.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   ����c��printf������usart�˿�,ʹ��DMAģʽ��������
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F407 ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
  
#include "./usart/bsp_debug_usart.h"
/* RT-Thread���ͷ�ļ� */
#include <rthw.h>
#include <rtthread.h>

DMA_HandleTypeDef  DMA_Handle;      //DMA���
UART_HandleTypeDef UartHandle;      //UART���

uint8_t Usart_Rx_Buf[USART_RBUFF_SIZE];

 /**
  * @brief  USART �ж�����
  * @param  ��
  * @retval ��
  */
void USART_NVIC_Config(void)
{
	HAL_NVIC_SetPriority(DMA2_Stream5_IRQn,7,0);
	HAL_NVIC_EnableIRQ(DMA2_Stream5_IRQn);
	
	HAL_NVIC_SetPriority(USART1_IRQn,8,0);
	HAL_NVIC_EnableIRQ(USART1_IRQn);
}

 /**
  * @brief  USART GPIO ����,����ģʽ���á�115200 8-N-1
  * @param  ��
  * @retval ��
  */
void Debug_USART_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
      
  DEBUG_USART_RX_GPIO_CLK_ENABLE();
  DEBUG_USART_TX_GPIO_CLK_ENABLE();
  /* ʹ�� UART ʱ�� */
  DEBUG_USART_CLK_ENABLE();
  
   /* ����Tx����Ϊ���ù���  */
  GPIO_InitStruct.Pin = DEBUG_USART_TX_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = DEBUG_USART_TX_AF;
  HAL_GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStruct);
  
  /* ����Rx����Ϊ���ù��� */
  GPIO_InitStruct.Pin = DEBUG_USART_RX_PIN;
  GPIO_InitStruct.Alternate = DEBUG_USART_RX_AF;
  HAL_GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStruct);
  
  
  UartHandle.Instance          = DEBUG_USART;
  UartHandle.Init.BaudRate     = DEBUG_USART_BAUDRATE;
  UartHandle.Init.WordLength   = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits     = UART_STOPBITS_1;
  UartHandle.Init.Parity       = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode         = UART_MODE_TX_RX;
  
  HAL_UART_Init(&UartHandle); 
	
	__HAL_UART_ENABLE_IT(&UartHandle,UART_IT_IDLE);
}

///�ض���c�⺯��printf������DEBUG_USART���ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
	/* ����һ���ֽ����ݵ�����DEBUG_USART */
	HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1, 1000);	
	
	return (ch);
}

///�ض���c�⺯��scanf������DEBUG_USART����д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
		
	int ch;
	HAL_UART_Receive(&UartHandle, (uint8_t *)&ch, 1, 1000);	
	return (ch);
}

/**
  * @brief  USART1 TX DMA ���ã����赽�ڴ�(USART1->DR)
  * @param  ��
  * @retval ��
  */
void USART_DMA_Config(void)
{
  DEBUG_USART_DMA_CLK_ENABLE();  
   
  //Tx DMA����
  DMA_Handle.Instance=DEBUG_USART_DMA_STREAM;                            //������ѡ��
  DMA_Handle.Init.Channel=DEBUG_USART_DMA_CHANNEL;                                //ͨ��ѡ��
  DMA_Handle.Init.Direction=DMA_PERIPH_TO_MEMORY;             //���赽�洢��
  DMA_Handle.Init.PeriphInc=DMA_PINC_DISABLE;                 //���������ģʽ
  DMA_Handle.Init.MemInc=DMA_MINC_ENABLE;                     //�洢������ģʽ
  DMA_Handle.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;    //�������ݳ���:8λ
  DMA_Handle.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;       //�洢�����ݳ���:8λ
  DMA_Handle.Init.Mode=DMA_CIRCULAR;                            //������ͨģʽ
  DMA_Handle.Init.Priority=DMA_PRIORITY_MEDIUM;               //�е����ȼ�
  DMA_Handle.Init.FIFOMode=DMA_FIFOMODE_DISABLE;              //����FIFO
  DMA_Handle.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;      
  DMA_Handle.Init.MemBurst=DMA_MBURST_SINGLE;                 //�洢��ͻ�����δ���
  DMA_Handle.Init.PeriphBurst=DMA_PBURST_SINGLE;              //����ͻ�����δ���
//	DMA_Handle.XferCpltCallback = callback;
  HAL_DMA_Init(&DMA_Handle);
  /* Associate the DMA handle */
  __HAL_LINKDMA(&UartHandle, hdmarx, DMA_Handle); 
	__HAL_DMA_ENABLE_IT(UartHandle.hdmarx, DMA_IT_TC | DMA_IT_HT | DMA_IT_TE);//�ر�DMA ���� ����һ�� ȫ����� �ж�
	HAL_UART_Receive_DMA(&UartHandle,Usart_Rx_Buf,USART_RBUFF_SIZE);

}

void USART_Init(void)
{
	Debug_USART_Config();
	USART_DMA_Config();
	USART_NVIC_Config();
}

/* �ⲿ�����ź������ƿ� */
extern rt_sem_t test_sem;

void HAL_UART_AbortReceiveCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &UartHandle)
	{
		//������ֵ�ź��� �����ͽ��յ������ݱ�־����ǰ̨�����ѯ
		rt_sem_release(test_sem);  
		//���¿�ʼDMA����
		HAL_UART_Receive_DMA(&UartHandle,Usart_Rx_Buf,USART_RBUFF_SIZE);
	}
}


/*********************************************END OF FILE**********************/
