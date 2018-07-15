
#include "magi2c.h"

/**
  **********************************
  * @brief  MPU6050初始化
  * @param  None
  * @retval None
  **********************************
*/
void MAG_IIC_Init(void)
{
	/*
	PB14 SCL ,PB15 SDA
	*/
	GPIO_InitTypeDef GPIO_InitStructure;//定义结构体
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_14 | GPIO_Pin_15);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_15);
	
}

/**
  **********************************
  * @brief  产生IIC起始信号
  * @param  None
  * @retval None
  **********************************
*/
void MAG_IIC_Start(void)
{
	MAG_SDA_OUT();//sda线输出
	MAG_IIC_SDA = 1;
	MAG_IIC_SCL = 1;
	MAG_IIC_DELAY();
	MAG_IIC_SDA = 0;//START:when CLK is high,DATA change form high to low 
	MAG_IIC_DELAY();
	MAG_IIC_SCL = 0;//钳住I2C总线，准备发送或接收数据
}

/**
  **********************************
  * @brief  产生IIC停止信号
  * @param  None
  * @retval None
  **********************************
*/
void MAG_IIC_Stop(void)
{
	MAG_SDA_OUT();//sda线输出
	MAG_IIC_SCL = 0;
	MAG_IIC_SDA = 0;//STOP:when CLK is high DATA change form low to high
	MAG_IIC_DELAY();
	MAG_IIC_SCL = 1;
	MAG_IIC_SDA = 1;//发送I2C总线结束信号
	MAG_IIC_DELAY();					   	
}

/**
  **********************************
  * @brief  等待应答信号
  * @param  None
  * @retval 1：接收应答失败	0：接收应答成功
  **********************************
*/
unsigned char MAG_IIC_Wait_Ack(void)
{
	unsigned char ucErrTime = 0;
	MAG_SDA_IN();      //SDA设置为输入  
	MAG_IIC_SDA = 1;
	MAG_IIC_DELAY();
	MAG_IIC_SCL = 1;
	MAG_IIC_DELAY();
	while (MAG_READ_SDA)
	{
		ucErrTime++;
		if (ucErrTime>250)
		{
			MAG_IIC_Stop();
			return 1;
		}
	}
	MAG_IIC_SCL = 0;//时钟输出0
	return 0;
} 

/**
  **********************************
  * @brief  产生应答信号
  * @param  None
  * @retval None
  **********************************
*/
void MAG_IIC_Ack(void)
{
	MAG_IIC_SCL = 0;
	MAG_SDA_OUT();
	MAG_IIC_SDA = 0;
	MAG_IIC_DELAY();
	MAG_IIC_SCL = 1;
	MAG_IIC_DELAY();
	MAG_IIC_SCL = 0;
}

/**
  **********************************
  * @brief  不产生应答信号
  * @param  None
  * @retval None
  **********************************
*/
void MAG_IIC_NAck(void)
{
	MAG_IIC_SCL = 0;
	MAG_SDA_OUT();
	MAG_IIC_SDA = 1;
	MAG_IIC_DELAY();
	MAG_IIC_SCL = 1;
	MAG_IIC_DELAY();
	MAG_IIC_SCL = 0;
}					 				     

/**
  **********************************
  * @brief  发送一个字节
  * @param  None
  * @retval None
  **********************************
*/
void MAG_IIC_Send_Byte(unsigned char txd)
{                        
	unsigned char t;
	MAG_SDA_OUT();
	MAG_IIC_SCL = 0;//拉低时钟开始数据传输
	for (t = 0;t<8;t++)
	{
		MAG_IIC_SDA = (txd & 0x80) >> 7;
		txd <<= 1;
		MAG_IIC_SCL = 1;
		MAG_IIC_DELAY();
		MAG_IIC_SCL = 0;
		MAG_IIC_DELAY();
	}
} 	    

/**
  **********************************
  * @brief  读一个字节
  * @param  None
  * @retval None
  **********************************
*/
unsigned char MAG_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i, receive = 0;
	MAG_SDA_IN();//SDA设置为输入
	for (i = 0;i<8;i++)
	{
		MAG_IIC_SCL = 0;
		MAG_IIC_DELAY();
		MAG_IIC_SCL = 1;
		receive <<= 1;
		if (MAG_READ_SDA)receive++;
		MAG_IIC_DELAY();
	}
	if (!ack)
		MAG_IIC_NAck();//发送nACK
	else
		MAG_IIC_Ack(); //发送ACK   
	return receive;
}
/**
  **********************************
  * @brief  MPU6050写一个字节
	* @param  reg:寄存器地址 data:数据
  * @retval None
  **********************************
*/
unsigned char MAG_Write_Byte(unsigned char reg, unsigned char data)
{
	MAG_IIC_Start();
	MAG_IIC_Send_Byte((MAG3110_IIC_ADDRESS << 1) | 0);//发送器件地址+写命令
	if (MAG_IIC_Wait_Ack())	//等待应答
	{
		MAG_IIC_Stop();
		return 1;
	}
	MAG_IIC_Send_Byte(reg);	//写寄存器地址
	MAG_IIC_Wait_Ack();		//等待应答 
	MAG_IIC_Send_Byte(data);//发送数据
	if (MAG_IIC_Wait_Ack())	//等待ACK
	{
		MAG_IIC_Stop();
		return 1;
	}
	MAG_IIC_Stop();
	return 0;
}

/**
  **********************************
  * @brief  MPU6050读一个字节
	* @param  reg:寄存器地址
  * @retval None
  **********************************
*/
unsigned char MAG_Read_Byte(unsigned char reg)
{
	unsigned char res;
	MAG_IIC_Start();
	MAG_IIC_Send_Byte((MAG3110_IIC_ADDRESS << 1) | 0);//发送器件地址+写命令	
	MAG_IIC_Wait_Ack();		//等待应答 
	MAG_IIC_Send_Byte(reg);	//写寄存器地址
	MAG_IIC_Wait_Ack();		//等待应答
	MAG_IIC_Start();
	MAG_IIC_Send_Byte((MAG3110_IIC_ADDRESS << 1) | 1);//发送器件地址+读命令	
	MAG_IIC_Wait_Ack();		//等待应答 
	res = MAG_IIC_Read_Byte(0);//读取数据,发送nACK 
	MAG_IIC_Stop();			//产生一个停止条件 
	return res;
}

