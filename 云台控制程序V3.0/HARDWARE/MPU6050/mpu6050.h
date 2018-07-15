
#ifndef __MPU6050_H
#define __MPU6050_H


#include "stm32f10x.h"
#include "mpu6050reg.h"

#include "mpui2c.h"
#include "gimbalmath.h"
#include "delay.h"
#define MPU6050_ADDRESS	0x68	//mpu6050 I2C address AD0=0
#define MPU6050_ID		0x68	//mpu6050 WHO_AM_I=0x68

static int16_t AccelData[3];	//Accelerometer data
static int16_t TempData;		//Temperature data
static int16_t GyroData[3];		//Gyroscope data

extern uint8_t Mpu6050_ID;		//Identity of device
extern float Mpu6050_Accel_X;	//Aceeleration x-axis
extern float Mpu6050_Accel_Y;	//Aceeleration y-axis
extern float Mpu6050_Accel_Z;	//Aceeleration z-axis
extern float Mpu6050_Temp;		//Temperature calculated result
extern float Mpu6050_Gyro_X;	//Angular rate x-axis
extern float Mpu6050_Gyro_Y;	//Angular rate y-axis
extern float Mpu6050_Gyro_Z;	//Angular rate z-axis

extern float Mpu6050_Pitch;		//Pitch angle
extern float Mpu6050_Roll;		//Roll angle
extern float Mpu6050_Yaw;		//Yaw angle
uint8_t Mpu6050_Init(uint16_t sampleRate, uint8_t flagDLPF);
uint8_t Mpu6050_GetDevID(void);
uint8_t Mpu6050_GetAccelData(void);
uint8_t Mpu6050_GetTempData(void);
uint8_t Mpu6050_GetGyroData(void);
uint8_t Mpu6050_SetSampleRate(uint16_t sampleRate, uint8_t flagDLPF);
uint8_t Mpu6050_SetDLPF(uint16_t bandWidth, uint8_t flagDLPF);
void Mpu6050_Calu(float calWeight, float calPeriod,float gypoZBiasMpu);

#endif

