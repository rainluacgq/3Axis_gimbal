
#include "boardmpu.h"

uint8_t BoardMpu_ID;		//Identity of device
float BoardMpu_Accel_X;		//Aceeleration x-axis
float BoardMpu_Accel_Y;		//Aceeleration y-axis
float BoardMpu_Accel_Z;		//Aceeleration z-axis
float BoardMpu_Temp;		//Temperature calculated result
float BoardMpu_Gyro_X;		//Angular rate x-axis
float BoardMpu_Gyro_Y;		//Angular rate y-axis
float BoardMpu_Gyro_Z;		//Angular rate z-axis
float BoardMpu_Pitch;		//Pitch angle (degree)
float BoardMpu_Roll;		//Roll angle (degree)
float BoardMpu_Yaw;			//Yaw angle (degree)
uint8_t BoardMpu_Init(uint16_t sampleRate, uint8_t flagDLPF)
{
	uint8_t data;
	I2C1_Init(0);//parameter is unused
	delay_ms(10);//must delay
	if(BoardMpu_GetDevID())//mpu6050 WHO_AM_I register
	{
		//DEBUG_PRINT("0x%x\r\n", BoardMpu_ID);
		return 1;
	}
	if(BoardMpu_ID != BOARDMPU_ID)//verify device ID
	{
		return 2;
	}
	data = 0x80;//PWR_MGMT_1=0x80 reset device 
	if(I2C1_WriteData(BOARDMPU_ADDRESS, PWR_MGMT_1, &data, 1))
	{
		return 3;
	}
	delay_ms(10);//must delay
	data = 0x00;//after reset PWR_MGMT_1=0x40 sleep mode; PWR_MGMT_1=0x00 wake up
	if(I2C1_WriteData(BOARDMPU_ADDRESS, PWR_MGMT_1, &data, 1))
	{
		return 4;
	}
	
	/**
	data = 0x00;//wake-up frequency 1.25Hz and all axes in normal mode
	if(I2C1_WriteData(BOARDMPU_ADDRESS, PWR_MGMT_2, &data, 1))
	{
		return 5;
	}
	data = 0x00;//INT pin is active high and push-pull
	if(I2C1_WriteData(BOARDMPU_ADDRESS, INT_PIN_CFG, &data, 1))
	{
		return 6;
	}
	data = 0x00;//disable interrupt
	if(I2C1_WriteData(BOARDMPU_ADDRESS, INT_ENABLE, &data, 1))
	{
		return 7;
	}
	data = 0x00;//don't load any sensor data into FIFO buffer
	if(I2C1_WriteData(BOARDMPU_ADDRESS, FIFO_EN, &data, 1))
	{
		return 8;
	}
	data = 0x00;//disable FIFO buffer and I2C master mode
	if(I2C1_WriteData(BOARDMPU_ADDRESS, USER_CONTROL, &data, 1))
	{
		return 9;
	}
	*/
	data = 0x01;//clock source PLL with X axis gyroscope reference
	if(I2C1_WriteData(BOARDMPU_ADDRESS, PWR_MGMT_1, &data, 1))
	{
		return 10;
	}
	
	data = 0x03<<3;//gyroscope full scale range ¡À2000¡ã/s
	if(I2C1_WriteData(BOARDMPU_ADDRESS, GYRO_CONFIG, &data, 1))
	{
		return 11;
	}
	data = 0x00;//accelerometer full scale range ¡À2g
	if(I2C1_WriteData(BOARDMPU_ADDRESS, ACCEL_CONFIG, &data, 1))
	{
		return 12;
	}
	
	if(BoardMpu_SetSampleRate(sampleRate, flagDLPF))
	{
		return 13;
	}
	if(BoardMpu_SetDLPF(sampleRate/2, flagDLPF))
	{
		return 14;
	}
	return 0;
}

uint8_t BoardMpu_GetDevID(void)
{
	if(I2C1_ReadData(BOARDMPU_ADDRESS, WHO_AM_I, &BoardMpu_ID, 1))
	{
		return 1;
	}
	return 0;
}

uint8_t BoardMpu_GetAccelData(void)
{
	uint8_t buff[6];
	if(I2C1_ReadData(BOARDMPU_ADDRESS, ACCEL_XOUT_HIGH, buff, 6))
	{
		return 1;
	}
	AccelData[0] = (buff[0] & 0x00ff)<<8 | buff[1];
	AccelData[1] = (buff[2] & 0x00ff)<<8 | buff[3];
	AccelData[2] = (buff[4] & 0x00ff)<<8 | buff[5];
	BoardMpu_Accel_X = AccelData[0]/FACTOR_ACCEL_RANGE_2;//¡À2g	
	BoardMpu_Accel_Y = AccelData[1]/FACTOR_ACCEL_RANGE_2;//¡À2g	
	BoardMpu_Accel_Z = AccelData[2]/FACTOR_ACCEL_RANGE_2;//¡À2g
	//DEBUG_PRINT("%-5d %-5d %-5d\r\n", AccelData[0], AccelData[1], AccelData[2]);
	return 0;
}

uint8_t BoardMpu_GetTempData(void)
{
	uint8_t buff[2];
	if(I2C1_ReadData(BOARDMPU_ADDRESS, TEMP_OUT_HIGH, buff, 2))
	{
		return 1;
	}
	TempData = (buff[0] & 0x00ff)<<8 | buff[1];
	BoardMpu_Temp = TempData/340.0 + 36.53;
	//DEBUG_PRINT("%-8.6f\r\n", BoardMpu_Temp);
	return 0;
}

uint8_t BoardMpu_GetGyroData(void)
{
	uint8_t buff[6];
	if(I2C1_ReadData(BOARDMPU_ADDRESS, GYRO_XOUT_HIGH, buff, 6))
	{
		return 1;
	}
	GyroData[0] = (buff[0] & 0x00ff)<<8 | buff[1];
	GyroData[1] = (buff[2] & 0x00ff)<<8 | buff[3];
	GyroData[2] = (buff[4] & 0x00ff)<<8 | buff[5];
	BoardMpu_Gyro_X = GyroData[0]/FACTOR_GYRO_RANGE_2000;//¡À2000¡ã/s
	BoardMpu_Gyro_Y = GyroData[1]/FACTOR_GYRO_RANGE_2000;//¡À2000¡ã/s
	BoardMpu_Gyro_Z = GyroData[2]/FACTOR_GYRO_RANGE_2000;//¡À2000¡ã/s
	//DEBUG_PRINT("%-5d %-5d %-5d\r\n", GyroData[0], GyroData[1], GyroData[2]);
//	DEBUG_PRINT("[#2]%f\r\n", BoardMpu_Gyro_Z);
	return 0;
}

uint8_t BoardMpu_SetSampleRate(uint16_t sampleRate, uint8_t flagDLPF)
{
	uint8_t divider;
	uint16_t gyroOutputRate;
	if(flagDLPF)//enabled DLPF
	{
		gyroOutputRate = 1000;
		sampleRate = INTERVAL_CONSTRAINT(sampleRate, 1000, 4);
	}
	else
	{
		gyroOutputRate = 8000;
		sampleRate = INTERVAL_CONSTRAINT(sampleRate, 8000, 32);
	}
	divider = gyroOutputRate / sampleRate - 1;
	//DEBUG_PRINT("divider: %d \r\n", divider);
	if(I2C1_WriteData(BOARDMPU_ADDRESS, SMPRT_DIV, &divider, 1))
	{
		return 1;
	}
	return 0;
}

uint8_t BoardMpu_SetDLPF(uint16_t bandWidth, uint8_t flagDLPF)
{
	uint8_t data;
	if(flagDLPF)
	{
		if(bandWidth > 188)
		{
			data = 1;
		}
		else if(bandWidth > 98)
		{
			data = 2;
		}
		else if(bandWidth > 42)
		{
			data = 3;
		}
		else if(bandWidth > 20)
		{
			data = 4;
		}
		else if(bandWidth > 10)
		{
			data = 5;
		}
		else
		{
			data = 6;
		}
	}
	else 
	{
		data = 0;
	}
	//DEBUG_PRINT("bandWidth: %d %d \r\n", bandWidth, data);
	if(I2C1_WriteData(BOARDMPU_ADDRESS, CONFIG, &data, 1))
	{
		return 1;
	}
	return 0;
}

void BoardMpu_Calu(float calWeight, float calPeriod,float gypoZBiasBoard)
{
	//2017-01-27 when pitch is greater than 90¡ãroll is abnormal
	//2017-01-28 in dynamic environment angles are incorrect 
	BoardMpu_GetAccelData();
	BoardMpu_GetGyroData();
	//BoardMpu_Pitch = atan2(BoardMpu_Accel_X, BoardMpu_Accel_Z)*180/MATH_PI;//range -PI~PI
	//BoardMpu_Roll = atan2(BoardMpu_Accel_Y, BoardMpu_Accel_Z)*180/MATH_PI;//range -PI~PI
	//the first-order complementary filter algorithm    data fusion
	BoardMpu_Pitch = calWeight*atan2(BoardMpu_Accel_Y, BoardMpu_Accel_Z)*180/MATH_PI + (1-calWeight)*(BoardMpu_Pitch+BoardMpu_Gyro_X*calPeriod);
	BoardMpu_Roll = calWeight*atan2(BoardMpu_Accel_X, BoardMpu_Accel_Z)*180/MATH_PI + (1-calWeight)*(BoardMpu_Roll+BoardMpu_Gyro_Y*calPeriod);
	BoardMpu_Yaw += (BoardMpu_Gyro_Z-gypoZBiasBoard) / (1000 / SYSTEM_PERIOD);//#2 yaw
	//DEBUG_PRINT("%f,%f\r\n", BoardMpu_Pitch, BoardMpu_Roll);
}

