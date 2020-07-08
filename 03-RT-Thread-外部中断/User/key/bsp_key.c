/**
  ******************************************************************************
  * @file    bsp_exti.c
  * @author  fire
  * @version V1.0
  * @date    2020-xx-xx
  * @brief   I/O���ж�Ӧ��bsp
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F407 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "./bsp_key.h"


 /**
  * @brief  ���� PA0 Ϊ���жϿڣ��������ж����ȼ�
  * @param  ��
  * @retval ��
  */
void EXTI_Key_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; 

    /*��������GPIO�ڵ�ʱ��*/
    KEY1_INT_GPIO_CLK_ENABLE();
    KEY2_INT_GPIO_CLK_ENABLE();
    KEY3_INT_GPIO_CLK_ENABLE();
    KEY4_INT_GPIO_CLK_ENABLE();
    KEY5_INT_GPIO_CLK_ENABLE();

    /* ѡ�񰴼�1������ */ 
    GPIO_InitStructure.Pin = KEY1_INT_GPIO_PIN;
    /* ��������Ϊ����ģʽ */ 
    GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;	    		
    /* �������Ų�����Ҳ������ */
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    /* ʹ������Ľṹ���ʼ������ */
    HAL_GPIO_Init(KEY1_INT_GPIO_PORT, &GPIO_InitStructure); 
    /* ���� EXTI �ж�Դ ��key1 ���š������ж����ȼ�*/
    HAL_NVIC_SetPriority(KEY1_INT_EXTI_IRQ, 6, 0);
    /* ʹ���ж� */
    HAL_NVIC_EnableIRQ(KEY1_INT_EXTI_IRQ);

    /* ѡ�񰴼�2������ */ 
    GPIO_InitStructure.Pin = KEY2_INT_GPIO_PIN;  
    /* ����������������ͬ */
    HAL_GPIO_Init(KEY2_INT_GPIO_PORT, &GPIO_InitStructure);      
    /* ���� EXTI �ж�Դ ��key2 ���š������ж����ȼ�*/
    HAL_NVIC_SetPriority(KEY2_INT_EXTI_IRQ, 6, 0);
    /* ʹ���ж� */
    HAL_NVIC_EnableIRQ(KEY2_INT_EXTI_IRQ);
	
    /* ѡ�񰴼�3������ */ 
    GPIO_InitStructure.Pin = KEY3_INT_GPIO_PIN;  
    /* ����������������ͬ */
    HAL_GPIO_Init(KEY3_INT_GPIO_PORT, &GPIO_InitStructure);      
    /* ���� EXTI �ж�Դ ��key3 ���š������ж����ȼ�*/
    HAL_NVIC_SetPriority(KEY3_INT_EXTI_IRQ, 6, 0);
    /* ʹ���ж� */
    HAL_NVIC_EnableIRQ(KEY3_INT_EXTI_IRQ);
	
    /* ѡ�񰴼�4������ */ 
    GPIO_InitStructure.Pin = KEY4_INT_GPIO_PIN;  
    /* ����������������ͬ */
    HAL_GPIO_Init(KEY4_INT_GPIO_PORT, &GPIO_InitStructure);      
    /* ���� EXTI �ж�Դ ��key4 ���š������ж����ȼ�*/
    HAL_NVIC_SetPriority(KEY4_INT_EXTI_IRQ, 6, 0);
    /* ʹ���ж� */
    HAL_NVIC_EnableIRQ(KEY4_INT_EXTI_IRQ);
	
    /* ѡ�񰴼�5������ */ 
    GPIO_InitStructure.Pin = KEY5_INT_GPIO_PIN;  
    /* ����������������ͬ */
    HAL_GPIO_Init(KEY5_INT_GPIO_PORT, &GPIO_InitStructure);      
    /* ���� EXTI �ж�Դ ��key5 ���š������ж����ȼ�*/
    HAL_NVIC_SetPriority(KEY5_INT_EXTI_IRQ, 6, 0);
    /* ʹ���ж� */
    HAL_NVIC_EnableIRQ(KEY5_INT_EXTI_IRQ);
}
/*********************************************END OF FILE**********************/
