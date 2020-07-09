/**
  ******************************************************************************
  * @file    bsp_485.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   485����
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F407 ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 
#include "./485/bsp_485.h"
#include <stdarg.h>
#include "./led/bsp_led.h" 
#include <rtthread.h>

static void Delay(__IO uint32_t nCount); 

UART_HandleTypeDef Uart4_Handle;

/// ����USART�����ж�
static void NVIC_Configuration(void)
{	
	/*�ж����ã���ռ���ȼ�0�������ȼ�Ϊ0*/
	HAL_NVIC_SetPriority(_485_INT_IRQ, 6 ,0);
	HAL_NVIC_EnableIRQ(_485_INT_IRQ);
}
/*
 * ��������_485_Config
 * ����  ��USART GPIO ����,����ģʽ����
 * ����  ����
 * ���  : ��
 * ����  ���ⲿ����
 */
void _485_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
  
  _485_USART_RX_GPIO_CLK_ENABLE();
  _485_USART_TX_GPIO_CLK_ENABLE();
  _485_RE_GPIO_CLK_ENABLE();
  
  /* ʹ�� UART ʱ�� */
  _485_USART_CLK_ENABLE();

  /**USART2 GPIO Configuration    
  PD5    ------> USART2_TX
  PD6    ------> USART2_RX 
  */
  /* ����Tx����Ϊ���ù���  */
  GPIO_InitStruct.Pin = _485_USART_TX_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = _485_USART_TX_AF;
  HAL_GPIO_Init(_485_USART_TX_GPIO_PORT, &GPIO_InitStruct);
  
  /* ����Rx����Ϊ���ù��� */
  GPIO_InitStruct.Pin = _485_USART_RX_PIN;
  GPIO_InitStruct.Alternate = _485_USART_RX_AF;
  HAL_GPIO_Init(_485_USART_RX_GPIO_PORT, &GPIO_InitStruct); 
  
 /* 485�շ����ƹܽ� */
  GPIO_InitStruct.Pin = _485_RE_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(_485_RE_GPIO_PORT, &GPIO_InitStruct);
  
  /* ���ô�485_USART ģʽ */
  Uart4_Handle.Instance = _485_USART;
  Uart4_Handle.Init.BaudRate = _485_USART_BAUDRATE;
  Uart4_Handle.Init.WordLength = UART_WORDLENGTH_8B;
  Uart4_Handle.Init.StopBits = UART_STOPBITS_1;
  Uart4_Handle.Init.Parity = UART_PARITY_NONE;
  Uart4_Handle.Init.Mode = UART_MODE_TX_RX;
  Uart4_Handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  Uart4_Handle.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&Uart4_Handle);

  /*����1�жϳ�ʼ�� */
  NVIC_Configuration();
  /*���ô��ڽ����ж� */
  __HAL_UART_ENABLE_IT(&Uart4_Handle,UART_IT_RXNE);
  //Ĭ�Ͻ������ģʽ
  HAL_GPIO_WritePin(_485_RE_GPIO_PORT,_485_RE_PIN,GPIO_PIN_SET);
}


/***************** ����һ���ַ�  **********************/
//ʹ�õ��ֽ����ݷ���ǰҪʹ�ܷ������ţ����ͺ�Ҫʹ�ܽ������š�
void _485_SendByte(  uint8_t ch )
{
	/* ����һ���ֽ����ݵ�USART1 */
    HAL_UART_Transmit(&Uart4_Handle, (uint8_t *)&ch, 1, 0xFFFF);	
}
/*****************  ����ָ�����ȵ��ַ��� **********************/
void _485_SendStr_length( uint8_t *str,uint32_t strlen )
{
	unsigned int k=0;

	_485_TX_EN()	;//	ʹ�ܷ�������	
    do 
    {
        _485_SendByte( *(str + k) );
        k++;
    } while(k < strlen);
		
	/*�Ӷ�����ʱ����֤485�����������*/
	Delay(0xFFF);
		
	_485_RX_EN()	;//	ʹ�ܽ�������
}


/*****************  �����ַ��� **********************/
void _485_SendString(  uint8_t *str)
{
	unsigned int k=0;
	
	_485_TX_EN()	;//	ʹ�ܷ�������
	
    do 
    {
        _485_SendByte(  *(str + k) );
        k++;
    } while(*(str + k)!='\0');
	
	/*�Ӷ�����ʱ����֤485�����������*/
	Delay(0xFFF);
		
	_485_RX_EN()	;//	ʹ�ܽ�������
}



//�жϻ��洮������
#define UART_BUFF_SIZE      1024
volatile    uint16_t uart_p = 1;
uint8_t     uart_buff[UART_BUFF_SIZE];

extern rt_sem_t test_sem;

void bsp_485_IRQHandler(void)
{ 
    if(uart_p<UART_BUFF_SIZE)
    {
		if (__HAL_UART_GET_FLAG(&Uart4_Handle,UART_FLAG_RXNE) != RESET)
//        if(Uart4_Handle.Instance->SR & (1<<5) )
        {
            HAL_UART_Receive(&Uart4_Handle, (uint8_t *)(&uart_buff[uart_p]),1 , 1000);
            uart_p++;
						rt_sem_release(	test_sem	);	//�ͷŶ�ֵ�ź���
        }
    }
		else
		{
			clean_rebuff();       
		}
	HAL_UART_IRQHandler(&Uart4_Handle);
}

//��ȡ���յ������ݺͳ���
char *get_rebuff(uint16_t *len) 
{
    *len = uart_p;
    return (char *)&uart_buff;
}

//��ջ�����
void clean_rebuff(void) 
{

    uint16_t i=UART_BUFF_SIZE+1;
    uart_p = 0;
	while(i)
		uart_buff[--i]=0;
}

static void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}
