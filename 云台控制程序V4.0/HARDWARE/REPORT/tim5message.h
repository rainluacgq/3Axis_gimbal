#ifndef __TIM5MESSAGE_H
#define __TIM5MESSAGE_H
#include "stm32f10x.h"
#include "usart.h"
#include "mpu6050.h"

void TIM5_Init(u16 arr,u16 psc);
void Send_Remote(short roll,short pitch,short yaw);
void Send_Check(u8 checkhead,u8 checksum);
#endif
