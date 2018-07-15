
#include "mpui2c.h"
#include "delay.h"
void I2C2_Init(uint32_t clkSpeed)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	i2c2GPIOInitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	i2c2GPIOInitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	i2c2GPIOInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &i2c2GPIOInitStruct);
	GPIO_SetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11);
}

void I2C2_Start(void)
{
	I2C2_SDA_HIGH;
	I2C2_SCL_HIGH;
	delay_us(I2C2_DELAY);
	I2C2_SDA_LOW;
	delay_us(I2C2_DELAY);
	I2C2_SCL_LOW;
	delay_us(I2C2_DELAY);
}

void I2C2_Stop(void)
{
	I2C2_SCL_LOW;
	I2C2_SDA_LOW;
	delay_us(I2C2_DELAY);
	I2C2_SCL_HIGH;
	delay_us(I2C2_DELAY);
	I2C2_SDA_HIGH;
	delay_us(I2C2_DELAY);
}

void I2C2_SendACK(uint8_t ack)
{
	I2C2_SCL_LOW;
	delay_us(I2C2_DELAY);
	I2C2_SDA(ack);
	delay_us(I2C2_DELAY);
	I2C2_SCL_HIGH;
	delay_us(I2C2_DELAY);
	I2C2_SCL_LOW;
	delay_us(I2C2_DELAY);
}

uint8_t I2C2_WaitACK(void)
{
	uint8_t ackSignal;
	I2C2_SCL_LOW;
	delay_us(I2C2_DELAY);
	I2C2_SDA_HIGH;
	delay_us(I2C2_DELAY);
	I2C2_SCL_HIGH;
	delay_us(I2C2_DELAY);
	I2C2_SDAin;
	delay_us(I2C2_DELAY);
	ackSignal = I2C2_SDA_READ;
	delay_us(I2C2_DELAY);
	I2C2_SDAout;
	delay_us(I2C2_DELAY);
	I2C2_SCL_LOW;
	delay_us(I2C2_DELAY);
	return ackSignal;
}

void I2C2_SendByte(uint8_t data)
{
	uint8_t i;
	for (i=0; i<8; i++)
	{
		I2C2_SCL_LOW;
		delay_us(I2C2_DELAY);
		//I2C2_SDA(((data<<i) & 0x80)>>7);
		I2C2_SDA(data >> (7-i) & 0x01);
		delay_us(I2C2_DELAY);
		I2C2_SCL_HIGH;
		delay_us(I2C2_DELAY);
	}
	I2C2_SCL_LOW;
	delay_us(I2C2_DELAY);
}

uint8_t I2C2_RecvByte(void)
{
	uint8_t i;
	uint8_t data = 0;
	I2C2_SDA_HIGH; 
	delay_us(I2C2_DELAY);
	I2C2_SDAin;
	delay_us(I2C2_DELAY);
	for (i=0; i<8; i++)
	{
		data <<= 1;
		I2C2_SCL_LOW;
		delay_us(I2C2_DELAY);
		I2C2_SCL_HIGH;
		delay_us(I2C2_DELAY);
		data |= I2C2_SDA_READ;
	}
	I2C2_SCL_LOW;
	delay_us(I2C2_DELAY);
	I2C2_SDAout;
	delay_us(I2C2_DELAY);
	return data;
}

uint8_t I2C2_WriteData(uint8_t devAddress, uint8_t regAddress, uint8_t *data, uint8_t length)
{
	uint8_t i;
	I2C2_Start();
	I2C2_SendByte(devAddress << 1 | 0);
	if(I2C2_WaitACK() == NACK_SIGNAL)
	{
		I2C2_Stop();
		return 1;
	}
	I2C2_SendByte(regAddress);
	if(I2C2_WaitACK() == NACK_SIGNAL)
	{
		I2C2_Stop();
		return 1;
	}
	for(i=0; i<length; i++)
	{
		I2C2_SendByte(data[i]);
		if(I2C2_WaitACK() == NACK_SIGNAL)
		{
			I2C2_Stop();
			return 1;
		}
	}
	I2C2_Stop();
	return 0;
}

uint8_t I2C2_ReadData(uint8_t devAddress, uint8_t regAddress, uint8_t *data, uint8_t length)
{
	uint8_t i;
	I2C2_Start();
	I2C2_SendByte(devAddress << 1 | 0);
	if(I2C2_WaitACK() == NACK_SIGNAL)
	{
		I2C2_Stop();
		return 1;
	}
	I2C2_SendByte(regAddress);
	if(I2C2_WaitACK() == NACK_SIGNAL)
	{
		I2C2_Stop();
		return 1;
	}
	I2C2_Start();
	I2C2_SendByte(devAddress << 1 | 1);
	if(I2C2_WaitACK() == NACK_SIGNAL)
	{
		I2C2_Stop();
		return 1;
	}
	for(i=0; i<length; i++)
	{
		data[i] = I2C2_RecvByte();
		if(i == length-1)
		{

			I2C2_SendACK(NACK_SIGNAL);
		}
		else
		{
			I2C2_SendACK(ACK_SIGNAL);
		}
	}
	I2C2_Stop();
	return 0;
}

