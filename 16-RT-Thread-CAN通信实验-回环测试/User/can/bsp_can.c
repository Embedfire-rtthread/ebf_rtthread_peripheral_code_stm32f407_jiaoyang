/**
  ******************************************************************************
  * @file    bsp_can.c
  * @author  fire
  * @version V1.0
  * @date    2016-xx-xx
  * @brief   can�������ػ�ģʽ��
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F407 ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 

#include "./can/bsp_can.h"
#include <rtthread.h>

CAN_HandleTypeDef Can_Handle;
CAN_TxHeaderTypeDef TxHeader;		//����ͷ
CAN_RxHeaderTypeDef RxHeader;		//����ͷ

/* ��װ���� */
Tx_Packge my_Packge = 
{
	.TxHeader = &TxHeader,
};

uint8_t Rx_Buff[8];

/*
 * ��������CAN_GPIO_Config
 * ����  ��CAN��GPIO ����
 * ����  ����
 * ���  : ��
 * ����  ���ڲ�����
 */
static void CAN_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* ʹ������ʱ�� */
    CAN_TX_GPIO_CLK_ENABLE();
    CAN_RX_GPIO_CLK_ENABLE();	

    /* ����CAN�������� */
    GPIO_InitStructure.Pin = CAN_TX_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
    GPIO_InitStructure.Pull  = GPIO_PULLUP;
    GPIO_InitStructure.Alternate =  GPIO_AF9_CAN1;
    HAL_GPIO_Init(CAN_TX_GPIO_PORT, &GPIO_InitStructure);

    /* ����CAN�������� */
    GPIO_InitStructure.Pin = CAN_RX_PIN ;
    HAL_GPIO_Init(CAN_RX_GPIO_PORT, &GPIO_InitStructure);
}

/*
 * ��������CAN_NVIC_Config
 * ����  ��CAN��NVIC ����,��1���ȼ��飬0��0���ȼ�
 * ����  ����
 * ���  : ��
 * ����  ���ڲ�����
 */
static void CAN_NVIC_Config(void)
{
	/*�ж����ã���ռ���ȼ�0�������ȼ�Ϊ0*/
	HAL_NVIC_SetPriority(CAN_RX_IRQ, 8 ,0);
	HAL_NVIC_EnableIRQ(CAN_RX_IRQ);
	HAL_NVIC_SetPriority(CAN1_TX_IRQn, 8 ,0);
	HAL_NVIC_EnableIRQ(CAN1_TX_IRQn);
}

/*
 * ��������CAN_Mode_Config
 * ����  ��CAN��ģʽ ����
 * ����  ����
 * ���  : ��
 * ����  ���ڲ�����
 */
static void CAN_Mode_Config(void)
{
	/********************CANͨ�Ų�������******************/
	/* ʹ��CANʱ�� */
	CAN_CLK_ENABLE();
	Can_Handle.Instance = CANx;
	/* CAN��Ԫ��ʼ�� */
	//MCR-TTCM  �ر�ʱ�䴥��ͨ��ģʽʹ��
	Can_Handle.Init.TimeTriggeredMode=DISABLE;			   
	 //MCR-ABOM  �Զ����߹���
	Can_Handle.Init.AutoBusOff=ENABLE;			
	 //MCR-AWUM  ʹ���Զ�����ģʽ
	Can_Handle.Init.AutoWakeUp=ENABLE;			 
	//MCR-NART  ��ֹ�����Զ��ش�	  DISABLE-�Զ��ش�
	Can_Handle.Init.AutoRetransmission=DISABLE;			 
   //MCR-RFLM  ����FIFO ����ģʽ  DISABLE-���ʱ�±��ĻḲ��ԭ�б���  
	Can_Handle.Init.ReceiveFifoLocked=DISABLE;			  
	 //MCR-TXFP  ����FIFO���ȼ� DISABLE-���ȼ�ȡ���ڱ��ı�ʾ�� 
	Can_Handle.Init.TransmitFifoPriority=DISABLE;			  
	//�ػ�ģʽ
	Can_Handle.Init.Mode = CAN_MODE_LOOPBACK;  
	//BTR-SJW ����ͬ����Ծ��� 2��ʱ�䵥Ԫ
	Can_Handle.Init.SyncJumpWidth=CAN_SJW_1TQ;		   
	 
	/* ss=1 bs1=5 bs2=3 λʱ����Ϊ(1+5+3) �����ʼ�Ϊʱ������tq*(1+3+6)  */
	 //BTR-TS1 ʱ���1 ռ����6��ʱ�䵥Ԫ
	Can_Handle.Init.TimeSeg1=CAN_BS1_5TQ;		  
	//BTR-TS1 ʱ���2 ռ����3��ʱ�䵥Ԫ	
	Can_Handle.Init.TimeSeg2=CAN_BS2_3TQ;		   
	
	/* CAN Baudrate = 1 MBps (1MBps��Ϊstm32��CAN�������) (CAN ʱ��Ƶ��Ϊ APB 1 = 54 MHz) */
	Can_Handle.Init.Prescaler =6;	//BTR-BRP �����ʷ�Ƶ��  ������ʱ�䵥Ԫ��ʱ�䳤�� 54/(1+5+3)/5=1 Mbps
	HAL_CAN_Init(&Can_Handle);
}

/*
 * ��������CAN_Filter_Config
 * ����  ��CAN�Ĺ����� ����
 * ����  ����
 * ���  : ��
 * ����  ���ڲ�����
 */
static void CAN_Filter_Config(void)
{
	CAN_FilterTypeDef  CAN_FilterInitStructure;

	/*CANɸѡ����ʼ��*/
	CAN_FilterInitStructure.FilterBank=14;	//ɸѡ����0
	//����������ģʽ
	CAN_FilterInitStructure.FilterMode=CAN_FILTERMODE_IDMASK;	
	//ɸѡ��λ��Ϊ����32λ��
	CAN_FilterInitStructure.FilterScale=CAN_FILTERSCALE_32BIT;	
	/* ʹ��ɸѡ�������ձ�־�����ݽ��бȶ�ɸѡ����չID�������µľ����������ǵĻ��������FIFO0�� */
	//Ҫɸѡ��ID��λ
	CAN_FilterInitStructure.FilterIdHigh= ((((uint32_t)0x1314<<3)|CAN_ID_EXT|CAN_RTR_DATA)&0xFFFF0000)>>16;	
	//Ҫɸѡ��ID��λ 
	CAN_FilterInitStructure.FilterIdLow= (((uint32_t)0x1314<<3)|CAN_ID_EXT|CAN_RTR_DATA)&0xFFFF; 
	//ɸѡ����16λÿλ����ƥ��
	CAN_FilterInitStructure.FilterMaskIdHigh= 0xFFFF;			
	//ɸѡ����16λÿλ����ƥ��
	CAN_FilterInitStructure.FilterMaskIdLow= 0xFFFF;			
	//ɸѡ����������FIFO0
	CAN_FilterInitStructure.FilterFIFOAssignment=CAN_FILTER_FIFO0 ;	
	//ʹ��ɸѡ��
	CAN_FilterInitStructure.FilterActivation=ENABLE;			
	HAL_CAN_ConfigFilter(&Can_Handle,&CAN_FilterInitStructure);
}


/*
 * ��������CAN_Config
 * ����  ����������CAN�Ĺ���
 * ����  ����
 * ���  : ��
 * ����  ���ⲿ����
 */
void CAN_Config(void)
{
  CAN_GPIO_Config();
  CAN_NVIC_Config();
  CAN_Mode_Config();
  CAN_Filter_Config();
  Init_RxMes(); 
  HAL_CAN_ActivateNotification(&Can_Handle, CAN_IT_RX_FIFO0_MSG_PENDING); 	
  HAL_CAN_Start(&Can_Handle);
}


/**
  * @brief  ��ʼ�� Rx Message���ݽṹ��
  * @param  RxMessage: ָ��Ҫ��ʼ�������ݽṹ��
  * @retval None
  */
void Init_RxMes(void)
{

  /*�ѽ��սṹ������*/
  RxHeader.StdId = 0x00;
  RxHeader.ExtId = 0x00;
  RxHeader.IDE = CAN_ID_STD;
  RxHeader.DLC = 0;
  RxHeader.FilterMatchIndex = 0;

}


/*
 * ��������CAN_SetMsg
 * ����  ��CANͨ�ű�����������,����һ����������Ϊ0-7�����ݰ�
 * ����  �����ͱ��Ľṹ��
 * ���  : ��
 * ����  ���ⲿ����
 */	 
void CAN_SetMsg(void)
{	  
  my_Packge.TxHeader->StdId=0x00;						 
  my_Packge.TxHeader->ExtId=0x1314;					 //ʹ�õ���չID
  my_Packge.TxHeader->IDE=CAN_ID_EXT;				  //��չģʽ
  my_Packge.TxHeader->RTR=CAN_RTR_DATA;				 //���͵�������
  my_Packge.TxHeader->DLC=8;							 //���ݳ���Ϊ8�ֽ�
  for(uint8_t i = 0;i<8;i++)
	{
		my_Packge.Payload[i] = i;
	}
  HAL_CAN_ActivateNotification(&Can_Handle, CAN_IT_TX_MAILBOX_EMPTY);
}

/**
  * @brief  CAN����ص�����
  * @param  hcan: CAN���ָ��
  * @retval ��
  */
void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
{
	printf("\r\nCAN����\r\n");
}

extern rt_sem_t test_sem;

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	rt_enter_critical();
	/* �Ƚ�ID�Ƿ�Ϊ0x1314 */ 
 	HAL_CAN_GetRxMessage(hcan,CAN_RX_FIFO0,&RxHeader,Rx_Buff);
	if((RxHeader.ExtId==0x1314) && (RxHeader.IDE==CAN_ID_EXT) && (RxHeader.DLC==8) )
	{
		rt_sem_release(	test_sem	);	//�ͷŶ�ֵ�ź���
	}

	/* ׼���жϽ��� */
	HAL_CAN_ActivateNotification(&Can_Handle, CAN_IT_RX_FIFO0_MSG_PENDING);
	/* �˳��ٽ�� */
  rt_exit_critical();
}
/**************************END OF FILE************************************/
