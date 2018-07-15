
#ifndef __BOARDMPUI2C_H
#define __BOARDMPUI2C_H

#include "stm32f10x.h"
#include "sys.h"

#define I2C1_SCL(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_6, (BitAction)(x))
#define I2C1_SDA(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_7, (BitAction)(x))

#define I2C1_SCL_LOW	GPIO_ResetBits(GPIOB, GPIO_Pin_6)
#define I2C1_SCL_HIGH	GPIO_SetBits(GPIOB, GPIO_Pin_6)
#define I2C1_SDA_LOW	GPIO_ResetBits(GPIOB, GPIO_Pin_7)
#define I2C1_SDA_HIGH	GPIO_SetBits(GPIOB, GPIO_Pin_7)

#define I2C1_SDA_READ	(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7) & 0x01)
#define I2C1_SDAin											\
{															\
	i2c1GPIOInitStruct.GPIO_Pin = GPIO_Pin_7;				\
	i2c1GPIOInitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;	\
	GPIO_Init(GPIOB, &i2c1GPIOInitStruct);					\
}
#define I2C1_SDAout											\
{															\
	i2c1GPIOInitStruct.GPIO_Pin = GPIO_Pin_7;				\
	i2c1GPIOInitStruct.GPIO_Mode = GPIO_Mode_Out_OD;		\
	GPIO_Init(GPIOB, &i2c1GPIOInitStruct);					\
}

#define I2C1_DELAY		3
#define ACK_SIGNAL		0
#define NACK_SIGNAL		1

static GPIO_InitTypeDef i2c1GPIOInitStruct;

void I2C1_Init(uint32_t clkSpeed);
void I2C1_Start(void);
void I2C1_Stop(void);
void I2C1_SendACK(uint8_t ack);
uint8_t I2C1_WaitACK(void);
void I2C1_SendByte(uint8_t data);
uint8_t I2C1_RecvByte(void);
uint8_t I2C1_WriteData(uint8_t devAddress, uint8_t regAddress, uint8_t *data, uint8_t length);
uint8_t I2C1_ReadData(uint8_t devAddress, uint8_t regAddress, uint8_t *data, uint8_t length);

#endif

