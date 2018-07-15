
#include "mpu6050.h"
#include "pid.h"
uint8_t Mpu6050_ID;		//Identity of device
float Mpu6050_Accel_X;	//Aceeleration x-axis
float Mpu6050_Accel_Y;	//Aceeleration y-axis
float Mpu6050_Accel_Z;	//Aceeleration z-axis
float Mpu6050_Temp;		//Temperature calculated result
float Mpu6050_Gyro_X;	//Angular rate x-axis
float Mpu6050_Gyro_Y;	//Angular rate y-axis
float Mpu6050_Gyro_Z;	//Angular rate z-axis
float Mpu6050_Pitch;	//Pitch angle (degree)
float Mpu6050_Roll;		//Roll angle (degree)
float Mpu6050_Yaw;		//Roll angle (degree)
uint8_t Mpu6050_Init(uint16_t sampleRate, uint8_t flagDLPF)
{
	uint8_t data;
	I2C2_Init(0);//parameter is unused
	delay_ms(10);//must delay
	if(Mpu6050_GetDevID())//mpu6050 WHO_AM_I register
	{
		return 1;
	}
	//DEBUG_PRINT("ID: 0x%x\r\n", Mpu6050_ID);
	if(Mpu6050_ID != MPU6050_ID)//verify device ID
	{
		return 2;
	}
	data = 0x80;//PWR_MGMT_1=0x80 reset device 
	if(I2C2_WriteData(MPU6050_ADDRESS, PWR_MGMT_1, &data, 1))
	{
		return 3;
	}
	delay_ms(10);//must delay
	data = 0x00;//after reset PWR_MGMT_1=0x40 sleep mode; PWR_MGMT_1=0x00 wake up
	if(I2C2_WriteData(MPU6050_ADDRESS, PWR_MGMT_1, &data, 1))
	{
		return 4;
	}
	
	/**
	data = 0x00;//wake-up frequency 1.25Hz and all axes in normal mode
	if(I2C2_WriteData(MPU6050_ADDRESS, PWR_MGMT_2, &data, 1))
	{
		return 5;
	}
	data = 0x00;//INT pin is active high and push-pull
	if(I2C2_WriteData(MPU6050_ADDRESS, INT_PIN_CFG, &data, 1))
	{
		return 6;
	}
	data = 0x00;//disable interrupt
	if(I2C2_WriteData(MPU6050_ADDRESS, INT_ENABLE, &data, 1))
	{
		return 7;
	}
	data = 0x00;//don't load any sensor data into FIFO buffer
	if(I2C2_WriteData(MPU6050_ADDRESS, FIFO_EN, &data, 1))
	{
		return 8;
	}
	data = 0x00;//disable FIFO buffer and I2C master mode
	if(I2C2_WriteData(MPU6050_ADDRESS, USER_CONTROL, &data, 1))
	{
		return 9;
	}
	*/
	data = 0x01;//clock source PLL with X axis gyroscope reference
	if(I2C2_WriteData(MPU6050_ADDRESS, PWR_MGMT_1, &data, 1))
	{
		return 10;
	}
	
	data = 0x03<<3;//gyroscope full scale range ¡À2000¡ã/s
	if(I2C2_WriteData(MPU6050_ADDRESS, GYRO_CONFIG, &data, 1))
	{
		return 11;
	}
	data = 0x00;//accelerometer full scale range ¡À2g
	if(I2C2_WriteData(MPU6050_ADDRESS, ACCEL_CONFIG, &data, 1))
	{
		return 12;
	}
	
	if(Mpu6050_SetSampleRate(sampleRate, flagDLPF))
	{
		return 13;
	}
	if(Mpu6050_SetDLPF(sampleRate/2, flagDLPF))
	{
		return 14;
	}
	return 0;
}

uint8_t Mpu6050_GetDevID(void)
{
	if(I2C2_ReadData(MPU6050_ADDRESS, WHO_AM_I, &Mpu6050_ID, 1))
	{
		return 1;
	}
	//DEBUG_PRINT("0x%x\t", Mpu6050_ID);
	return 0;
}

uint8_t Mpu6050_GetAccelData(void)
{
	uint8_t buff[6];
	if(I2C2_ReadData(MPU6050_ADDRESS, ACCEL_XOUT_HIGH, buff, 6))
	{
		return 1;
	}
	AccelData[0] = (buff[0] & 0x00ff)<<8 | buff[1];
	AccelData[1] = (buff[2] & 0x00ff)<<8 | buff[3];
	AccelData[2] = (buff[4] & 0x00ff)<<8 | buff[5];
	Mpu6050_Accel_X = AccelData[0]/FACTOR_ACCEL_RANGE_2;//¡À2g	
	Mpu6050_Accel_Y = AccelData[1]/FACTOR_ACCEL_RANGE_2;//¡À2g	
	Mpu6050_Accel_Z = AccelData[2]/FACTOR_ACCEL_RANGE_2;//¡À2g
	//DEBUG_PRINT("%-5d %-5d %-5d\r\n", AccelData[0], AccelData[1], AccelData[2]);
	return 0;
}

uint8_t Mpu6050_GetTempData(void)
{
	uint8_t buff[2];
	if(I2C2_ReadData(MPU6050_ADDRESS, TEMP_OUT_HIGH, buff, 2))
	{
		return 1;
	}
	TempData = (buff[0] & 0x00ff)<<8 | buff[1];
	Mpu6050_Temp = TempData/340.0 + 36.53;
	//DEBUG_PRINT("%-8.6f\r\n", Mpu6050_Temp);
	return 0;
}

uint8_t Mpu6050_GetGyroData(void)
{
	uint8_t buff[6];
	if(I2C2_ReadData(MPU6050_ADDRESS, GYRO_XOUT_HIGH, buff, 6))
	{
		return 1;
	}
	GyroData[0] = (buff[0] & 0x00ff)<<8 | buff[1];
	GyroData[1] = (buff[2] & 0x00ff)<<8 | buff[3];
	GyroData[2] = (buff[4] & 0x00ff)<<8 | buff[5];
	Mpu6050_Gyro_X = GyroData[0]/FACTOR_GYRO_RANGE_2000;//¡À2000¡ã/s
	Mpu6050_Gyro_Y = GyroData[1]/FACTOR_GYRO_RANGE_2000;//¡À2000¡ã/s
	Mpu6050_Gyro_Z = GyroData[2]/FACTOR_GYRO_RANGE_2000;//¡À2000¡ã/s
	//DEBUG_PRINT("%-5d %-5d %-5d\r\n", GyroData[0], GyroData[1], GyroData[2]);
	return 0;
}

uint8_t Mpu6050_SetSampleRate(uint16_t sampleRate, uint8_t flagDLPF)
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
	if(I2C2_WriteData(MPU6050_ADDRESS, SMPRT_DIV, &divider, 1))
	{
		return 1;
	}
	return 0;
}

uint8_t Mpu6050_SetDLPF(uint16_t bandWidth, uint8_t flagDLPF)
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
	if(I2C2_WriteData(MPU6050_ADDRESS, CONFIG, &data, 1))
	{
		return 1;
	}
	return 0;
}

void Mpu6050_Calu(float calWeight, float calPeriod,float gypoZBiasMpu)
{
	//static float aP = 0.0, aR = 0.0, gP = 0.0, gR = 0.0;
	//2017-01-27 when pitch is greater than 90¡ãroll is abnormal
	//2017-01-28 in dynamic environment angles are incorrect 
	Mpu6050_GetAccelData();
	Mpu6050_GetGyroData();
	//Mpu6050_Pitch = atan2(Mpu6050_Accel_X, Mpu6050_Accel_Z)*180/MATH_PI;//range -PI~PI
	//Mpu6050_Roll = atan2(Mpu6050_Accel_Y, Mpu6050_Accel_Z)*180/MATH_PI;//range -PI~PI
	//the first-order complementary filter algorithm    data fusion
	Mpu6050_Pitch = calWeight*atan2(Mpu6050_Accel_Y, Mpu6050_Accel_Z)*180/MATH_PI + (1-calWeight)*(Mpu6050_Pitch+Mpu6050_Gyro_X*calPeriod);
	Mpu6050_Roll = calWeight*(-1)*atan2(Mpu6050_Accel_X, Mpu6050_Accel_Z)*180/MATH_PI + (1-calWeight)*(Mpu6050_Roll+Mpu6050_Gyro_Y*calPeriod);
  Mpu6050_Yaw += Mpu6050_Gyro_Z-gypoZBiasMpu/ (1000 / SYSTEM_PERIOD);
	/**aP = atan2(Mpu6050_Accel_Y, Mpu6050_Accel_Z)*180/MATH_PI;
	aR = (-1)*atan2(Mpu6050_Accel_X, Mpu6050_Accel_Z)*180/MATH_PI;
	gP = gP + Mpu6050_Gyro_X*calPeriod;
	gR = gR + Mpu6050_Gyro_Y*calPeriod;
	DEBUG_PRINT("%f,%f,%f,%f,%f,%f\r\n", aP, aR, gP, gR, Mpu6050_Pitch, Mpu6050_Roll);*/
//	DEBUG_PRINT("%f,%f,%f\r\n", Mpu6050_Pitch, Mpu6050_Roll,Mpu6050_Yaw);
}

