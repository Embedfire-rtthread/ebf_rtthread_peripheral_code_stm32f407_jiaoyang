#ifndef __BOARD_INIT_H
#define __BOARD_INIT_H

/*****************************************************/
/*
 * 此处用以包含各种外设的头文件，在main函数中只包含本文件即可
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

/* RT-Thread相关头文件 */
#include <rthw.h>
#include <rtthread.h>

void SystemClock_Config(void);

void rt_hw_board_init(void);
void SysTick_Handler(void);


#endif
