#ifndef _BSP_I2C_GPIO_H
#define _BSP_I2C_GPIO_H

#include "stm32f4xx.h"
#include <inttypes.h>


#define EEPROM_I2C_WR	0		/* д����bit */
#define EEPROM_I2C_RD	1		/* ������bit */


/* ����I2C�������ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����4�д��뼴������ı�SCL��SDA������ */
#define EEPROM_I2C_GPIO_PORT				GPIOH			/* GPIO�˿� */
#define EEPROM_I2C_GPIO_CLK_ENABLE()    	__GPIOH_CLK_ENABLE()
#define EEPROM_I2C_SCL_PIN					GPIO_PIN_4			/* ���ӵ�SCLʱ���ߵ�GPIO */
#define EEPROM_I2C_SDA_PIN					GPIO_PIN_5			/* ���ӵ�SDA�����ߵ�GPIO */


/* �����дSCL��SDA�ĺ꣬�����Ӵ���Ŀ���ֲ�ԺͿ��Ķ��� */
#if 1	/* �������룺 1 ѡ��GPIO�Ŀ⺯��ʵ��IO��д */
	#define EEPROM_I2C_SCL_1()  HAL_GPIO_WritePin(EEPROM_I2C_GPIO_PORT, EEPROM_I2C_SCL_PIN,GPIO_PIN_SET)		/* SCL = 1 */
	#define EEPROM_I2C_SCL_0()  HAL_GPIO_WritePin(EEPROM_I2C_GPIO_PORT, EEPROM_I2C_SCL_PIN,GPIO_PIN_RESET)		/* SCL = 0 */
	
	#define EEPROM_I2C_SDA_1()  HAL_GPIO_WritePin(EEPROM_I2C_GPIO_PORT, EEPROM_I2C_SDA_PIN,GPIO_PIN_SET)		/* SDA = 1 */
	#define EEPROM_I2C_SDA_0()  HAL_GPIO_WritePin(EEPROM_I2C_GPIO_PORT, EEPROM_I2C_SDA_PIN,GPIO_PIN_RESET)		/* SDA = 0 */
	
	#define EEPROM_I2C_SDA_READ()  HAL_GPIO_ReadPin(EEPROM_I2C_GPIO_PORT, EEPROM_I2C_SDA_PIN)	/* ��SDA����״̬ */
#else	/* �����֧ѡ��ֱ�ӼĴ�������ʵ��IO��д */
    /*��ע�⣺����д������IAR��߼����Ż�ʱ���ᱻ�����������Ż� */
	#define EEPROM_I2C_SCL_1()  EEPROM_I2C_GPIO_PORT->BSRRL = EEPROM_I2C_SCL_PIN				/* SCL = 1 */
	#define EEPROM_I2C_SCL_0()  EEPROM_I2C_GPIO_PORT->BSRRH = EEPROM_I2C_SCL_PIN				/* SCL = 0 */
	
	#define EEPROM_I2C_SDA_1()  EEPROM_I2C_GPIO_PORT->BSRRL = EEPROM_I2C_SDA_PIN				/* SDA = 1 */
	#define EEPROM_I2C_SDA_0()  EEPROM_I2C_GPIO_PORT->BSRRH = EEPROM_I2C_SDA_PIN				/* SDA = 0 */
	
	#define EEPROM_I2C_SDA_READ()  ((EEPROM_I2C_GPIO_PORT->IDR & EEPROM_I2C_SDA_PIN) != 0)	/* ��SDA����״̬ */
#endif


void i2c_Start(void);
void i2c_Stop(void);
void i2c_SendByte(uint8_t _ucByte);
uint8_t i2c_ReadByte(void);
uint8_t i2c_WaitAck(void);
void i2c_Ack(void);
void i2c_NAck(void);
uint8_t i2c_CheckDevice(uint8_t _Address);


#endif

