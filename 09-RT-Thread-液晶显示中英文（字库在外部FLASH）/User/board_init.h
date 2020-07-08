#ifndef __BOARD_INIT_H
#define __BOARD_INIT_H

/*****************************************************/
/*
 * �˴����԰������������ͷ�ļ�����main������ֻ�������ļ�����
 */
#include "bsp_led.h"
#include ".\key\bsp_key.h" 
#include "./usart/bsp_debug_usart.h"
#include "./flash/bsp_spi_flash.h"
#include "ff.h"
#include "./lcd/bsp_ili9806g_lcd.h"
#include "./touch/gt5xx.h"
#include "./touch/palette.h"
#include "./touch/bsp_i2c_touch.h"

/* RT-Thread���ͷ�ļ� */
#include <rthw.h>
#include <rtthread.h>

void SystemClock_Config(void);

void rt_hw_board_init(void);
void SysTick_Handler(void);


#endif
