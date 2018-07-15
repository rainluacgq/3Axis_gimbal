#ifndef __REPORT_H
#define __REPORT_H
#include "stm32f10x.h"
#include "usart.h"
void TIM5_Init(u16 arr,u16 psc);
void Send_Remote(short roll,short pitch,short yaw);
void Send_Check(u8 check_head);
#endif
