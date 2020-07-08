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
	rt_kprintf("����һ��[Ұ��]-STM32-RTT-��д�ڲ�FLASHʵ�飡\n");

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
                      4,                   /* �̵߳����ȼ� */
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
		if(InternalFlash_Test()==0)
		{
			LED_ALLON;
			printf("��д�ڲ�FLASH���Գɹ�\r\n");
		}
		else
		{
			printf("��д�ڲ�FLASH����ʧ��\r\n");
			LED2_ON;
		}
    while (1)
    {
        LED1_ON;
        rt_thread_delay(500);   /* ��ʱ500��tick */
        
        LED1_OFF;     
        rt_thread_delay(500);   /* ��ʱ500��tick */		 		
    }
}

static void key_thread_entry(void* parameter)
{
	while (1)
	{
			if( Key_Scan(KEY1_GPIO_PORT, KEY1_PIN) != KEY_OFF)
			{
				LED2_TOGGLE;
				rt_kprintf("LED״̬��ת\n");
			}
			rt_thread_delay(10);   /* ��ʱ10��tick */		 	
	}
}

/********************************END OF FILE****************************/
