#ifndef __BOARD_INIT_H
#define __BOARD_INIT_H

/*****************************************************/
/*
 * �˴����԰������������ͷ�ļ�����main������ֻ�������ļ�����
 */
#include "bsp_led.h"
#include ".\key\bsp_key.h" 
#include "./usart/bsp_debug_usart.h"
#include "./dac/bsp_dac.h"
#include "./tim/bsp_general_tim.h"

/* RT-Thread���ͷ�ļ� */
#include <rthw.h>
#include <rtthread.h>

void SystemClock_Config(void);

void rt_hw_board_init(void);
void SysTick_Handler(void);

void Error_Handler(void);
#endif
