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

/******************************* 全局变量声明 ************************************/
/*
 * 当我们在写应用程序的时候，可能需要用到一些全局变量。
 */
char USBDISKPath[4];
char scan_path[255] = "0:";
DIR dir; 
FIL fnew;													/* 文件对象 */
UINT fnum;            			  /* 文件成功读写数量 */
BYTE ReadBuffer[1024]={0};        /* 读缓冲区 */
BYTE WriteBuffer[] =              /* 写缓冲区*/
"欢迎使用野火STM32 F407骄阳开发板 今天是个好日子，新建文件系统测试文件\r\n";  
FATFS fs;

FRESULT res_sd;                /* 文件操作结果 */

/*
*************************************************************************
*                             函数声明
*************************************************************************
*/
static void led1_thread_entry(void* parameter);
static void key_thread_entry(void* parameter);
static FRESULT scan_files (char* path) ;

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
	rt_kprintf("这是一个[野火]-STM32-RTT-USB主机读取U盘实验！\n");

	 /* 创建一个信号量 */
	test_sem = rt_sem_create("test_sem",/* 信号量名字 */
                     1,     /* 信号量初始值，默认有一个信号量 */
                     RT_IPC_FLAG_FIFO); /* 信号量模式 FIFO(0x00)*/
  if (test_sem != RT_NULL)
    rt_kprintf("信号量创建成功！\n\n");
	
	led1_thread =                          /* 线程控制块指针 */
    rt_thread_create( "led1",              /* 线程名字 */
                      led1_thread_entry,   /* 线程入口函数 */
                      RT_NULL,             /* 线程入口函数参数 */
                      10*1024,                 /* 线程栈大小 */
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
                      5*1024,                 /* 线程栈大小 */
                      3,                   /* 线程的优先级 */
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
	MX_USB_HOST_Init();
	LED1_ON;
	while (1)
	{
		rt_thread_delay(1);   /* 延时500个tick */
		MX_USB_HOST_Process();
	}
}

static void key_thread_entry(void* parameter)
{
	rt_kprintf("请插入U盘，当提示USB就绪后，按下Key1以扫描U盘内文件\n");
	static char Already_Mount=0,Link_State=0;
	if( FATFS_LinkDriver(&USBH_Driver, USBDISKPath) == 0)
		Link_State = 1;

	//获取USB就绪通知 ,没获取到则一直等待
	rt_sem_take(test_sem,	/* 获取信号量 */
							RT_WAITING_FOREVER); 	/* 等待时间：一直等 */
  while (1)
  {
		if( Key_Scan(KEY1_GPIO_PORT,KEY1_PIN) == KEY_ON)  
		{
			if ( (1 == Link_State) && (0 == Already_Mount))
			{
					res_sd = f_mount(&fs,"0:",1);	/* Mount a logical drive */
				
					if(res_sd != FR_OK)
					{
						rt_kprintf("\r\n文件系统挂载失败！\r\n");
						continue; 
					}
					Already_Mount = 1;
					scan_files(scan_path);
					res_sd = f_open(&fnew, "0:FatFs读写测试文件.txt",FA_CREATE_ALWAYS | FA_WRITE );
					if ( res_sd == FR_OK )
					{
						rt_kprintf("》打开/创建FatFs读写测试文件.txt文件成功，向文件写入数据。\r\n");
						/* 将指定存储区内容写入到文件内 */
						res_sd=f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);
						if(res_sd==FR_OK)
						{
							rt_kprintf("》文件写入成功，写入字节数据：%d\n",fnum);
							rt_kprintf("》向文件写入的数据为：\r\n%s\r\n",WriteBuffer);
						}
						else
						{
							rt_kprintf("！！文件写入失败：(%d)\n",res_sd);
						}    
						/* 不再读写，关闭文件 */
						f_close(&fnew);
					}
			}else if (Already_Mount == 1)
			{
				scan_files(scan_path);
			}else
			{
				rt_kprintf("\r\n文件系统链接失败挂载失败！\r\n");
			}
		}
    rt_thread_delay(20);/* 延时20个tick */
  }
}

/**
  * @brief  scan_files 递归扫描FatFs内的文件
  * @param  path:初始扫描路径
  * @retval result:文件系统的返回值
  */
static FRESULT scan_files (char* path) 
{ 
  FRESULT res; 		//部分在递归过程被修改的变量，不用全局变量	
  FILINFO fno; 
  DIR dir; 
  int i;            
  char *fn;        // 文件名	
	
#if _USE_LFN 
  /* 长文件名支持 */
  /* 简体中文需要2个字节保存一个“字”*/
  static char lfn[_MAX_LFN*2 + 1]; 	
  fno.lfname = lfn; 
  fno.lfsize = sizeof(lfn); 
#endif 
  //打开目录
  res = f_opendir(&dir, path); 
  if (res == FR_OK) 
	{ 
    i = strlen(path); 
    for (;;) 
		{ 
      //读取目录下的内容，再读会自动读下一个文件
      res = f_readdir(&dir, &fno); 								
      //为空时表示所有项目读取完毕，跳出
      if (res != FR_OK || fno.fname[0] == 0) break; 	
#if _USE_LFN 
      fn = *fno.lfname ? fno.lfname : fno.fname; 
#else 
      fn = fno.fname; 
#endif 
      //点表示当前目录，跳过			
      if (*fn == '.') continue; 	
      //目录，递归读取      
      if (fno.fattrib & AM_DIR)         
			{ 			
        //合成完整目录名        
        sprintf(&path[i], "/%s", fn); 		
        //递归遍历         
        res = scan_files(path);	
        path[i] = 0;         
        //打开失败，跳出循环        
        if (res != FR_OK) 
					break; 
      } 
			else 
			{ 
				printf("%s/%s\r\n", path, fn);								//输出文件名	
        /* 可以在这里提取特定格式的文件路径 */        
      }//else
    } //for
  } 
  return res; 
}
/********************************END OF FILE****************************/
