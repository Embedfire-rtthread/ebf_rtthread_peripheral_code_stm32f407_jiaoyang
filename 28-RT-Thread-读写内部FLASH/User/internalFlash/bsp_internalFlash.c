/**
  ******************************************************************************
  * @file    bsp_internalFlash.c
  * @author  fire
  * @version V1.0
  * @date    2017-xx-xx
  * @brief   �ڲ�FLASH��д���Է���
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F407 ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "./internalFlash/bsp_internalFlash.h"   

/*׼��д��Ĳ�������*/
#define DATA_32                 ((uint32_t)0x87654321)


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Ҫ�����ڲ�FLASH����ʼ��ַ */
#define FLASH_USER_START_ADDR   ADDR_FLASH_SECTOR_5   
/* Ҫ�����ڲ�FLASH�Ľ�����ַ */
#define FLASH_USER_END_ADDR     ADDR_FLASH_SECTOR_7  


static uint32_t GetSector(uint32_t Address);

/**
  * @brief  InternalFlash_Test,���ڲ�FLASH���ж�д����
  * @param  None
  * @retval None
  */
int InternalFlash_Test(void)
{
	/*Ҫ��������ʼ����(����)����������(������)����8-12����ʾ����8��9��10��11����*/
	uint32_t FirstSector = 0;
	uint32_t NbOfSectors = 0;
	
	uint32_t SECTORError = 0;
	
	uint32_t Address = 0;

	__IO uint32_t Data32 = 0;
	__IO uint32_t MemoryProgramStatus = 0;
	static FLASH_EraseInitTypeDef EraseInitStruct;
	
	/* FLASH ���� ********************************/
	/* ʹ�ܷ���FLASH���ƼĴ��� */
	HAL_FLASH_Unlock();

	FirstSector = GetSector(FLASH_USER_START_ADDR);
	NbOfSectors = GetSector(FLASH_USER_END_ADDR)- FirstSector + 1;
	
	/* �����û����� (�û�����ָ������û��ʹ�õĿռ䣬�����Զ���)**/
	/* Fill EraseInit structure*/
	EraseInitStruct.TypeErase     = FLASH_TYPEERASE_SECTORS;
	EraseInitStruct.VoltageRange  = FLASH_VOLTAGE_RANGE_3;/* �ԡ��֡��Ĵ�С���в��� */ 
	EraseInitStruct.Sector        = FirstSector;
	EraseInitStruct.NbSectors     = NbOfSectors;
	/* ��ʼ�������� */
	if (HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError) != HAL_OK)
	{
		/*�����������أ�ʵ��Ӧ���пɼ��봦�� */
		return -1;
	}

	/* �ԡ��֡��Ĵ�СΪ��λд������ ********************************/
	Address = FLASH_USER_START_ADDR;

	while (Address < FLASH_USER_END_ADDR)
	{
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, DATA_32) == HAL_OK)
		{
		  Address = Address + 4;
		}
		else
		{ 
		  /*д��������أ�ʵ��Ӧ���пɼ��봦�� */
				return -1;
		}
	}


	/* ��FLASH��������ֹ���ݱ��۸�*/
	HAL_FLASH_Lock(); 


	/* ��FLASH�ж�ȡ�����ݽ���У��***************************************/
	/*  MemoryProgramStatus = 0: д���������ȷ
	  MemoryProgramStatus != 0: д������ݴ�����ֵΪ����ĸ��� */
	Address = FLASH_USER_START_ADDR;
	MemoryProgramStatus = 0;

	while (Address < FLASH_USER_END_ADDR)
	{
		Data32 = *(__IO uint32_t*)Address;

		if (Data32 != DATA_32)
		{
		  MemoryProgramStatus++;  
		}

		Address = Address + 4;
	}  
	/* ����У�鲻��ȷ */
	if(MemoryProgramStatus)
	{    
		return -1;
	}
	else /*����У����ȷ*/
	{ 
		return 0;   
	}
}

/**
  * @brief  ��������ĵ�ַ���������ڵ�sector
  *					���磺
						uwStartSector = GetSector(FLASH_USER_START_ADDR);
						uwEndSector = GetSector(FLASH_USER_END_ADDR);	
  * @param  Address����ַ
  * @retval ��ַ���ڵ�sector
  */
static uint32_t GetSector(uint32_t Address)
{
  uint32_t sector = 0;
  
  if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
  {
    sector = FLASH_SECTOR_0;  
  }
  else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
  {
    sector = FLASH_SECTOR_1;  
  }
  else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
  {
    sector = FLASH_SECTOR_2;  
  }
  else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
  {
    sector = FLASH_SECTOR_3;  
  }
  else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
  {
    sector = FLASH_SECTOR_4;  
  }
  else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
  {
    sector = FLASH_SECTOR_5;  
  }
  else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
  {
    sector = FLASH_SECTOR_6;  
  }
  else/*(Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_23))*/
  {
    sector = FLASH_SECTOR_7;  
  }
  return sector;
}



