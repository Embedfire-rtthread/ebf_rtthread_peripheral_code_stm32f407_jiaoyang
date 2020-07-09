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
	rt_kprintf("����һ��[Ұ��]-STM32-RTT-RS485ͨ�Ų���ʵ�飡\n");

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
	char *pbuf;
	uint16_t len;
  /*��ʼ��485ʹ�õĴ��ڣ�ʹ���ж�ģʽ����*/
	_485_Config();
	printf("\r\n ��ӭʹ��Ұ��  STM32 F407 �����塣\r\n");
	printf("\r\n Ұ��F407 RS485ͨѶʵ������\r\n");

	printf("\r\n ʵ�鲽�裺\r\n");

	printf("\r\n 1.ʹ�õ������Ӻ�����485ͨѶ�豸\r\n");
	printf("\r\n 2.��ʹ������Ұ�𿪷������ʵ�飬�����������嶼���ر����򼴿ɡ�\r\n");
	printf("\r\n 3.׼���ú󣬰�������һ���������KEY1������ʹ��485���ⷢ��0-255������ \r\n");
	printf("\r\n 4.���������485���յ�256���ֽ����ݣ����������16������ʽ��ӡ������ \r\n");
	while (1)
	{
		rt_sem_take(test_sem,	/* ��ȡ�ź��� */
                RT_WAITING_FOREVER); 	/* �ȴ�ʱ�䣺һֱ�� */
		
		pbuf = get_rebuff(&len);
		if(len>=255)
		{
			LED_ALLON;
			printf("\r\n���յ�����Ϊ%d������\r\n",len);	
			_485_DEBUG_ARRAY((uint8_t*)pbuf,len);
			clean_rebuff();
		}
	}
}

static void key_thread_entry(void* parameter)
{
	while (1)
	{
			if( Key_Scan(KEY1_GPIO_PORT, KEY1_PIN) != KEY_OFF)
			{
				uint16_t i;
				
				LED1_ON;
				
				_485_TX_EN();
				
				for(i=0;i<=0xff;i++)
				{
					_485_SendByte(i);	 //��������
				}
				
				/*�Ӷ�����ʱ����֤485�����������*/
				rt_thread_delay(200);
				_485_RX_EN();
				
				LED2_ON;
				
				printf("\r\n�������ݳɹ���\r\n"); //ʹ�õ��Դ��ڴ�ӡ������Ϣ���ն�
			}
			rt_thread_delay(10);   /* ��ʱ10��tick */		 	
	}
}

/********************************END OF FILE****************************/
