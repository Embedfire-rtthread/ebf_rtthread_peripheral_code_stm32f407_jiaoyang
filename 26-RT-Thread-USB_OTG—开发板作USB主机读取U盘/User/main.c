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

/******************************* ȫ�ֱ������� ************************************/
/*
 * ��������дӦ�ó����ʱ�򣬿�����Ҫ�õ�һЩȫ�ֱ�����
 */
char USBDISKPath[4];
char scan_path[255] = "0:";
DIR dir; 
FIL fnew;													/* �ļ����� */
UINT fnum;            			  /* �ļ��ɹ���д���� */
BYTE ReadBuffer[1024]={0};        /* �������� */
BYTE WriteBuffer[] =              /* д������*/
"��ӭʹ��Ұ��STM32 F407���������� �����Ǹ������ӣ��½��ļ�ϵͳ�����ļ�\r\n";  
FATFS fs;

FRESULT res_sd;                /* �ļ�������� */

/*
*************************************************************************
*                             ��������
*************************************************************************
*/
static void led1_thread_entry(void* parameter);
static void key_thread_entry(void* parameter);
static FRESULT scan_files (char* path) ;

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
	rt_kprintf("����һ��[Ұ��]-STM32-RTT-USB������ȡU��ʵ�飡\n");

	 /* ����һ���ź��� */
	test_sem = rt_sem_create("test_sem",/* �ź������� */
                     1,     /* �ź�����ʼֵ��Ĭ����һ���ź��� */
                     RT_IPC_FLAG_FIFO); /* �ź���ģʽ FIFO(0x00)*/
  if (test_sem != RT_NULL)
    rt_kprintf("�ź��������ɹ���\n\n");
	
	led1_thread =                          /* �߳̿��ƿ�ָ�� */
    rt_thread_create( "led1",              /* �߳����� */
                      led1_thread_entry,   /* �߳���ں��� */
                      RT_NULL,             /* �߳���ں������� */
                      10*1024,                 /* �߳�ջ��С */
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
                      5*1024,                 /* �߳�ջ��С */
                      3,                   /* �̵߳����ȼ� */
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
	MX_USB_HOST_Init();
	LED1_ON;
	while (1)
	{
		rt_thread_delay(1);   /* ��ʱ500��tick */
		MX_USB_HOST_Process();
	}
}

static void key_thread_entry(void* parameter)
{
	rt_kprintf("�����U�̣�����ʾUSB�����󣬰���Key1��ɨ��U�����ļ�\n");
	static char Already_Mount=0,Link_State=0;
	if( FATFS_LinkDriver(&USBH_Driver, USBDISKPath) == 0)
		Link_State = 1;

	//��ȡUSB����֪ͨ ,û��ȡ����һֱ�ȴ�
	rt_sem_take(test_sem,	/* ��ȡ�ź��� */
							RT_WAITING_FOREVER); 	/* �ȴ�ʱ�䣺һֱ�� */
  while (1)
  {
		if( Key_Scan(KEY1_GPIO_PORT,KEY1_PIN) == KEY_ON)  
		{
			if ( (1 == Link_State) && (0 == Already_Mount))
			{
					res_sd = f_mount(&fs,"0:",1);	/* Mount a logical drive */
				
					if(res_sd != FR_OK)
					{
						rt_kprintf("\r\n�ļ�ϵͳ����ʧ�ܣ�\r\n");
						continue; 
					}
					Already_Mount = 1;
					scan_files(scan_path);
					res_sd = f_open(&fnew, "0:FatFs��д�����ļ�.txt",FA_CREATE_ALWAYS | FA_WRITE );
					if ( res_sd == FR_OK )
					{
						rt_kprintf("����/����FatFs��д�����ļ�.txt�ļ��ɹ������ļ�д�����ݡ�\r\n");
						/* ��ָ���洢������д�뵽�ļ��� */
						res_sd=f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);
						if(res_sd==FR_OK)
						{
							rt_kprintf("���ļ�д��ɹ���д���ֽ����ݣ�%d\n",fnum);
							rt_kprintf("�����ļ�д�������Ϊ��\r\n%s\r\n",WriteBuffer);
						}
						else
						{
							rt_kprintf("�����ļ�д��ʧ�ܣ�(%d)\n",res_sd);
						}    
						/* ���ٶ�д���ر��ļ� */
						f_close(&fnew);
					}
			}else if (Already_Mount == 1)
			{
				scan_files(scan_path);
			}else
			{
				rt_kprintf("\r\n�ļ�ϵͳ����ʧ�ܹ���ʧ�ܣ�\r\n");
			}
		}
    rt_thread_delay(20);/* ��ʱ20��tick */
  }
}

/**
  * @brief  scan_files �ݹ�ɨ��FatFs�ڵ��ļ�
  * @param  path:��ʼɨ��·��
  * @retval result:�ļ�ϵͳ�ķ���ֵ
  */
static FRESULT scan_files (char* path) 
{ 
  FRESULT res; 		//�����ڵݹ���̱��޸ĵı���������ȫ�ֱ���	
  FILINFO fno; 
  DIR dir; 
  int i;            
  char *fn;        // �ļ���	
	
#if _USE_LFN 
  /* ���ļ���֧�� */
  /* ����������Ҫ2���ֽڱ���һ�����֡�*/
  static char lfn[_MAX_LFN*2 + 1]; 	
  fno.lfname = lfn; 
  fno.lfsize = sizeof(lfn); 
#endif 
  //��Ŀ¼
  res = f_opendir(&dir, path); 
  if (res == FR_OK) 
	{ 
    i = strlen(path); 
    for (;;) 
		{ 
      //��ȡĿ¼�µ����ݣ��ٶ����Զ�����һ���ļ�
      res = f_readdir(&dir, &fno); 								
      //Ϊ��ʱ��ʾ������Ŀ��ȡ��ϣ�����
      if (res != FR_OK || fno.fname[0] == 0) break; 	
#if _USE_LFN 
      fn = *fno.lfname ? fno.lfname : fno.fname; 
#else 
      fn = fno.fname; 
#endif 
      //���ʾ��ǰĿ¼������			
      if (*fn == '.') continue; 	
      //Ŀ¼���ݹ��ȡ      
      if (fno.fattrib & AM_DIR)         
			{ 			
        //�ϳ�����Ŀ¼��        
        sprintf(&path[i], "/%s", fn); 		
        //�ݹ����         
        res = scan_files(path);	
        path[i] = 0;         
        //��ʧ�ܣ�����ѭ��        
        if (res != FR_OK) 
					break; 
      } 
			else 
			{ 
				printf("%s/%s\r\n", path, fn);								//����ļ���	
        /* ������������ȡ�ض���ʽ���ļ�·�� */        
      }//else
    } //for
  } 
  return res; 
}
/********************************END OF FILE****************************/
