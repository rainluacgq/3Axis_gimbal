
#ifndef __MAGI2C_H_
#define __MAGI2C_H_

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "delay.h"
#include "sys.h"

//IO方向设置
#define MAG_SDA_IN()	{GPIOA->CRL &= 0xFFFFFFF0; GPIOB->CRL |= 0x08;}
#define MAG_SDA_OUT()	{GPIOA->CRL &= 0xFFFFFFF0; GPIOB->CRL |= 0x03;}

//IO操作宏
#define MAG_IIC_SCL		PBout(14)
#define MAG_IIC_SDA		PBout(15)
#define MAG_READ_SDA	PBin(15)//SDA输入

//定义函数宏
#define MAG_IIC_DELAY()	{delay_us(1);}
#define MAG3110_IIC_ADDRESS       0x1C
void MAG_IIC_Init(void);
void MAG_IIC_Start(void);
void MAG_IIC_Stop(void);
unsigned char MAG_IIC_Wait_Ack(void);
void MAG_IIC_Ack(void);
void MAG_IIC_NAck(void);
void MAG_IIC_Send_Byte(unsigned char txd);
unsigned char MAG_IIC_Read_Byte(unsigned char ack);
unsigned char MAG_Read_Byte(unsigned char reg);
unsigned char MAG_Write_Byte(unsigned char reg, unsigned char data);
#endif
