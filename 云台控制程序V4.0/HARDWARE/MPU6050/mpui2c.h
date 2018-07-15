
#ifndef __I2C_2_H
#define __I2C_2_H

#include "stm32f10x.h"
#include "sys.h"

#define I2C2_SCL(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_10, (BitAction)(x))
#define I2C2_SDA(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_11, (BitAction)(x))

#define I2C2_SCL_LOW	GPIO_ResetBits(GPIOB, GPIO_Pin_10)
#define I2C2_SCL_HIGH	GPIO_SetBits(GPIOB, GPIO_Pin_10)
#define I2C2_SDA_LOW	GPIO_ResetBits(GPIOB, GPIO_Pin_11)
#define I2C2_SDA_HIGH	GPIO_SetBits(GPIOB, GPIO_Pin_11)

#define I2C2_SDA_READ	(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) & 0x01)
#define I2C2_SDAin											\
{															\
	i2c2GPIOInitStruct.GPIO_Pin = GPIO_Pin_11;				\
	i2c2GPIOInitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;	\
	GPIO_Init(GPIOB, &i2c2GPIOInitStruct);					\
}
#define I2C2_SDAout											\
{															\
	i2c2GPIOInitStruct.GPIO_Pin = GPIO_Pin_11;				\
	i2c2GPIOInitStruct.GPIO_Mode = GPIO_Mode_Out_OD;		\
	GPIO_Init(GPIOB, &i2c2GPIOInitStruct);					\
}

#define I2C2_DELAY		3
#define ACK_SIGNAL		0
#define NACK_SIGNAL		1

static GPIO_InitTypeDef i2c2GPIOInitStruct;

void I2C2_Init(uint32_t clkSpeed);
void I2C2_Start(void);
void I2C2_Stop(void);
void I2C2_SendACK(uint8_t ack);
uint8_t I2C2_WaitACK(void);
void I2C2_SendByte(uint8_t data);
uint8_t I2C2_RecvByte(void);
uint8_t I2C2_WriteData(uint8_t devAddress, uint8_t regAddress, uint8_t *data, uint8_t length);
uint8_t I2C2_ReadData(uint8_t devAddress, uint8_t regAddress, uint8_t *data, uint8_t length);

#endif

