
#include "boardmpui2c.h"
#include"delay.h"

void I2C1_Init(uint32_t clkSpeed)
{
	/**
	GPIO_InitTypeDef i2c1GPIOInitStruct;
	I2C_InitTypeDef i2c1InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	
	i2c1GPIOInitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	i2c1GPIOInitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
	i2c1GPIOInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &i2c1GPIOInitStruct);

	I2C_DeInit(I2C1);
	i2c1InitStruct.I2C_ClockSpeed = clkSpeed;
	i2c1InitStruct.I2C_OwnAddress1 = 0x88;	//Ö÷»úµØÖ·
	i2c1InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	i2c1InitStruct.I2C_Ack = I2C_Ack_Enable;
	i2c1InitStruct.I2C_Mode = I2C_Mode_I2C;
	i2c1InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_Init(I2C1, &i2c1InitStruct);
	I2C_Cmd(I2C1, ENABLE);
	*/
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	i2c1GPIOInitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	i2c1GPIOInitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	i2c1GPIOInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &i2c1GPIOInitStruct);
	GPIO_SetBits(GPIOB, GPIO_Pin_6 | GPIO_Pin_7);
}

void I2C1_Start(void)
{
	I2C1_SDA_HIGH;
	I2C1_SCL_HIGH;
	delay_us(I2C1_DELAY);
	I2C1_SDA_LOW;
	delay_us(I2C1_DELAY);
	I2C1_SCL_LOW;
	delay_us(I2C1_DELAY);
}

void I2C1_Stop(void)
{
	I2C1_SCL_LOW;
	I2C1_SDA_LOW;
	delay_us(I2C1_DELAY);
	I2C1_SCL_HIGH;
	delay_us(I2C1_DELAY);
	I2C1_SDA_HIGH;
	delay_us(I2C1_DELAY);
}

void I2C1_SendACK(uint8_t ack)
{
	I2C1_SCL_LOW;
	delay_us(I2C1_DELAY);
	I2C1_SDA(ack);
	delay_us(I2C1_DELAY);
	I2C1_SCL_HIGH;
	delay_us(I2C1_DELAY);
	I2C1_SCL_LOW;
	delay_us(I2C1_DELAY);
}

uint8_t I2C1_WaitACK(void)
{
	uint8_t ackSignal;
	I2C1_SCL_LOW;
	delay_us(I2C1_DELAY);
	I2C1_SDA_HIGH;
	delay_us(I2C1_DELAY);
	I2C1_SCL_HIGH;
	delay_us(I2C1_DELAY);
	I2C1_SDAin;
	delay_us(I2C1_DELAY);
	ackSignal = I2C1_SDA_READ;
	delay_us(I2C1_DELAY);
	I2C1_SDAout;
	delay_us(I2C1_DELAY);
	I2C1_SCL_LOW;
	delay_us(I2C1_DELAY);
	return ackSignal;
}

void I2C1_SendByte(uint8_t data)
{
	uint8_t i;
	for (i=0; i<8; i++)
	{
		I2C1_SCL_LOW;
		delay_us(I2C1_DELAY);
		//I2C1_SDA(((data<<i) & 0x80)>>7);
		I2C1_SDA(data >> (7-i) & 0x01);
		delay_us(I2C1_DELAY);
		I2C1_SCL_HIGH;
		delay_us(I2C1_DELAY);
	}
	I2C1_SCL_LOW;
	delay_us(I2C1_DELAY);
}

uint8_t I2C1_RecvByte(void)
{
	uint8_t i;
	uint8_t data = 0;
	I2C1_SDA_HIGH; 
	delay_us(I2C1_DELAY);
	I2C1_SDAin;
	delay_us(I2C1_DELAY);
	for (i=0; i<8; i++)
	{
		data <<= 1;
		I2C1_SCL_LOW;
		delay_us(I2C1_DELAY);
		I2C1_SCL_HIGH;
		delay_us(I2C1_DELAY);
		data |= I2C1_SDA_READ;
	}
	I2C1_SCL_LOW;
	delay_us(I2C1_DELAY);
	I2C1_SDAout;
	delay_us(I2C1_DELAY);
	return data;
}

uint8_t I2C1_WriteData(uint8_t devAddress, uint8_t regAddress, uint8_t *data, uint8_t length)
{
	uint8_t i;
	I2C1_Start();
	I2C1_SendByte(devAddress << 1 | 0);
	if(I2C1_WaitACK() == NACK_SIGNAL)
	{
		I2C1_Stop();
		return 1;
	}
	I2C1_SendByte(regAddress);
	if(I2C1_WaitACK() == NACK_SIGNAL)
	{
		I2C1_Stop();
		return 1;
	}
	for(i=0; i<length; i++)
	{
		I2C1_SendByte(data[i]);
		if(I2C1_WaitACK() == NACK_SIGNAL)
		{
			I2C1_Stop();
			return 1;
		}
	}
	I2C1_Stop();
	return 0;
}

uint8_t I2C1_ReadData(uint8_t devAddress, uint8_t regAddress, uint8_t *data, uint8_t length)
{
	uint8_t i;
	I2C1_Start();
	I2C1_SendByte(devAddress << 1 | 0);
	if(I2C1_WaitACK() == NACK_SIGNAL)
	{
		I2C1_Stop();
		return 1;
	}
	I2C1_SendByte(regAddress);
	if(I2C1_WaitACK() == NACK_SIGNAL)
	{
		I2C1_Stop();
		return 1;
	}
	I2C1_Start();
	I2C1_SendByte(devAddress << 1 | 1);
	if(I2C1_WaitACK() == NACK_SIGNAL)
	{
		I2C1_Stop();
		return 1;
	}
	for(i=0; i<length; i++)
	{
		data[i] = I2C1_RecvByte();
		if(i == length-1)
		{

			I2C1_SendACK(NACK_SIGNAL);
		}
		else
		{
			I2C1_SendACK(ACK_SIGNAL);
		}
	}
	I2C1_Stop();
	return 0;
}

