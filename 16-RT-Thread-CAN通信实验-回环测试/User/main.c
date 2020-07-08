/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2020-xx-xx
  * @brief   GPIO���--ʹ�ù̼������LED��
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F407 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx.h"
#include "board_init.h"


/*
*************************************************************************
*                               ����
*************************************************************************
*/
/* �����߳̿��ƿ� */
static rt_thread_t led1_thread = RT_NULL;
static rt_thread_t key_thread = RT_NULL;

/* �����ź������ƿ� */
rt_sem_t test_sem = RT_NULL;
/*
*************************************************************************
*                             ��������
*************************************************************************
*/
static void led1_thread_entry(void* parameter);
static void key_thread_entry(void* parameter);

/*
*************************************************************************
*                             main ����
*************************************************************************
*/
/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
    /* 
	 * ������Ӳ����ʼ����RTTϵͳ��ʼ���Ѿ���main����֮ǰ��ɣ�
	 * ����component.c�ļ��е�rtthread_startup()����������ˡ�
	 * ������main�����У�ֻ��Ҫ�����̺߳������̼߳��ɡ�
	 */
	rt_kprintf("����һ��[Ұ��]-STM32-RTT-CANͨ�Żػ�����ʵ�飡\n");

	   /* ����һ���ź��� */
	test_sem = rt_sem_create("test_sem",/* �ź������� */
															 0,     /* �ź�����ʼֵ��Ĭ����һ���ź��� */
									 RT_IPC_FLAG_FIFO); /* �ź���ģʽ FIFO(0x00)*/
	
  if (test_sem != RT_NULL)
    rt_kprintf("�ź��������ɹ���\n\n");
	
	led1_thread =                          /* �߳̿��ƿ�ָ�� */
    rt_thread_create( "led1",              /* �߳����� */
                      led1_thread_entry,   /* �߳���ں��� */
                      RT_NULL,             /* �߳���ں������� */
                      512,                 /* �߳�ջ��С */
                      3,                   /* �̵߳����ȼ� */
                      20);                 /* �߳�ʱ��Ƭ */
                   
    /* �����̣߳��������� */
   if (led1_thread != RT_NULL)
        rt_thread_startup(led1_thread);
    else
        return -1;

		key_thread =                          /* �߳̿��ƿ�ָ�� */
    rt_thread_create( "key",              /* �߳����� */
                      key_thread_entry,   /* �߳���ں��� */
                      RT_NULL,             /* �߳���ں������� */
                      512,                 /* �߳�ջ��С */
                      2,                   /* �̵߳����ȼ� */
                      20);                 /* �߳�ʱ��Ƭ */
                   
    /* �����̣߳��������� */
   if (key_thread != RT_NULL)
        rt_thread_startup(key_thread);
    else
        return -1;
}

/*
*************************************************************************
*                             �̶߳���
*************************************************************************
*/

static void led1_thread_entry(void* parameter)
{	
	/* ��ʼ��can,���жϽ���CAN���ݰ� */
	CAN_Config();
	printf("\r\n ��ӭʹ��Ұ��  STM32 F407 �����塣\r\n");
	printf("\r\n Ұ��F407 CANͨѶʵ������\r\n");

	printf("\r\n ʵ�鲽�裺\r\n");

	printf("\r\n 1.���¿������KEY1������ʹ��CAN���ⷢ��0-7�����ݰ���������չIDΪ0x1314 \r\n");
	printf("\r\n 2.���������CAN���յ���չIDΪ0x1314�����ݰ�����������Դ�ӡ�����ڡ� \r\n");
	printf("\r\n 3.�����е�can������Ϊ1MBps��Ϊstm32��can������ʡ� \r\n");
	while (1)
	{
		rt_sem_take(test_sem,	/* ��ȡ�ź��� */
                RT_WAITING_FOREVER); 	/* �ȴ�ʱ�䣺һֱ�� */
		
		printf("\r\nCAN���յ����ݣ�\r\n");	
		CAN_DEBUG_ARRAY(Rx_Buff,8); 			
	}
}

static void key_thread_entry(void* parameter)
{
	while (1)
	{
			if( Key_Scan(KEY1_GPIO_PORT, KEY1_PIN) != KEY_OFF)
			{
				uint32_t TxMailbox=0;
				LED1_ON;			
				/* װ��һ֡���� */
				CAN_SetMsg();
				/* ��ʼ�������� */
				HAL_CAN_AddTxMessage(&Can_Handle,my_Packge.TxHeader,my_Packge.Payload,&TxMailbox);
				rt_thread_delay(100);
				LED3_ON;		
			}
			rt_thread_delay(10);   /* ��ʱ10��tick */		 	
	}
}

/********************************END OF FILE****************************/
