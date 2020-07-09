/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2020-xx-xx
  * @brief   GPIO输出--使用固件库点亮LED灯
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F407 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx.h"
#include "board_init.h"


/*
*************************************************************************
*                               变量
*************************************************************************
*/
/* 定义线程控制块 */
static rt_thread_t led1_thread = RT_NULL;
static rt_thread_t key_thread = RT_NULL;

/* 定义信号量控制块 */
rt_sem_t test_sem = RT_NULL;
/*
*************************************************************************
*                             函数声明
*************************************************************************
*/
static void led1_thread_entry(void* parameter);
static void key_thread_entry(void* parameter);

/*
*************************************************************************
*                             main 函数
*************************************************************************
*/
/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
    /* 
	 * 开发板硬件初始化，RTT系统初始化已经在main函数之前完成，
	 * 即在component.c文件中的rtthread_startup()函数中完成了。
	 * 所以在main函数中，只需要创建线程和启动线程即可。
	 */
	rt_kprintf("这是一个[野火]-STM32-RTT-RS485通信测试实验！\n");

	   /* 创建一个信号量 */
	test_sem = rt_sem_create("test_sem",/* 信号量名字 */
															 0,     /* 信号量初始值，默认有一个信号量 */
									 RT_IPC_FLAG_FIFO); /* 信号量模式 FIFO(0x00)*/
	
  if (test_sem != RT_NULL)
    rt_kprintf("信号量创建成功！\n\n");
	
	led1_thread =                          /* 线程控制块指针 */
    rt_thread_create( "led1",              /* 线程名字 */
                      led1_thread_entry,   /* 线程入口函数 */
                      RT_NULL,             /* 线程入口函数参数 */
                      512,                 /* 线程栈大小 */
                      3,                   /* 线程的优先级 */
                      20);                 /* 线程时间片 */
                   
    /* 启动线程，开启调度 */
   if (led1_thread != RT_NULL)
        rt_thread_startup(led1_thread);
    else
        return -1;

		key_thread =                          /* 线程控制块指针 */
    rt_thread_create( "key",              /* 线程名字 */
                      key_thread_entry,   /* 线程入口函数 */
                      RT_NULL,             /* 线程入口函数参数 */
                      512,                 /* 线程栈大小 */
                      2,                   /* 线程的优先级 */
                      20);                 /* 线程时间片 */
                   
    /* 启动线程，开启调度 */
   if (key_thread != RT_NULL)
        rt_thread_startup(key_thread);
    else
        return -1;
}

/*
*************************************************************************
*                             线程定义
*************************************************************************
*/

static void led1_thread_entry(void* parameter)
{
	char *pbuf;
	uint16_t len;
  /*初始化485使用的串口，使用中断模式接收*/
	_485_Config();
	printf("\r\n 欢迎使用野火  STM32 F407 开发板。\r\n");
	printf("\r\n 野火F407 RS485通讯实验例程\r\n");

	printf("\r\n 实验步骤：\r\n");

	printf("\r\n 1.使用导线连接好两个485通讯设备\r\n");
	printf("\r\n 2.若使用两个野火开发板进行实验，给两个开发板都下载本程序即可。\r\n");
	printf("\r\n 3.准备好后，按下其中一个开发板的KEY1键，会使用485向外发送0-255的数字 \r\n");
	printf("\r\n 4.若开发板的485接收到256个字节数据，会把数据以16进制形式打印出来。 \r\n");
	while (1)
	{
		rt_sem_take(test_sem,	/* 获取信号量 */
                RT_WAITING_FOREVER); 	/* 等待时间：一直等 */
		
		pbuf = get_rebuff(&len);
		if(len>=255)
		{
			LED_ALLON;
			printf("\r\n接收到长度为%d的数据\r\n",len);	
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
					_485_SendByte(i);	 //发送数据
				}
				
				/*加短暂延时，保证485发送数据完毕*/
				rt_thread_delay(200);
				_485_RX_EN();
				
				LED2_ON;
				
				printf("\r\n发送数据成功！\r\n"); //使用调试串口打印调试信息到终端
			}
			rt_thread_delay(10);   /* 延时10个tick */		 	
	}
}

/********************************END OF FILE****************************/
