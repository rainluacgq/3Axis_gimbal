#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
#include "stm32f10x.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
#define BIT_SET(port, pin)		(port->BSRR = pin)
#define BIT_RESET(port, pin)	(port->BRR = pin)
#define BIT_REVERSE(port, pin)	(port->ODR ^= pin)

#define LED0_ON			BIT_SET(GPIOB, GPIO_Pin_12)
#define LED0_OFF		BIT_RESET(GPIOB, GPIO_Pin_12)
#define LED0_TOGGLE		BIT_REVERSE(GPIOB, GPIO_Pin_12)
#define LED1_ON			BIT_SET(GPIOB, GPIO_Pin_13)
#define LED1_OFF		BIT_RESET(GPIOB, GPIO_Pin_13)
#define LED1_TOGGLE		BIT_REVERSE(GPIOB, GPIO_Pin_13)

void LED_Init(void);//初始化

		 				    
#endif
