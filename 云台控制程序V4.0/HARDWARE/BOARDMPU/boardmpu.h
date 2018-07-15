
#ifndef __BOARDMPU_H
#define __BOARDMPU_H

#include "stm32f10x.h"
#include "gimbalmath.h"
#include "mpui2c.h"
#include "mpu6050reg.h"
#include "boardmpui2c.h"
#include "delay.h"
#include "pid.h"

#define BOARDMPU_ADDRESS	0x69	//mpu6050 I2C address AD0=1
#define BOARDMPU_ID			0x68	//mpu6050 WHO_AM_I=0x68

static int16_t AccelData[3];	//Accelerometer data
static int16_t TempData;		//Temperature data
static int16_t GyroData[3];		//Gyroscope data

extern uint8_t BoardMpu_ID;		//Identity of device
extern float BoardMpu_Accel_X;	//Aceeleration x-axis
extern float BoardMpu_Accel_Y;	//Aceeleration y-axis
extern float BoardMpu_Accel_Z;	//Aceeleration z-axis
extern float BoardMpu_Temp;		//Temperature calculated result
extern float BoardMpu_Gyro_X;	//Angular rate x-axis
extern float BoardMpu_Gyro_Y;	//Angular rate y-axis
extern float BoardMpu_Gyro_Z;	//Angular rate z-axis

extern float BoardMpu_Pitch;	//Pitch angle
extern float BoardMpu_Roll;		//Roll angle
extern float BoardMpu_Yaw;		//Yaw angle
uint8_t BoardMpu_Init(uint16_t sampleRate, uint8_t flagDLPF);
uint8_t BoardMpu_GetDevID(void);
uint8_t BoardMpu_GetAccelData(void);
uint8_t BoardMpu_GetTempData(void);
uint8_t BoardMpu_GetGyroData(void);
uint8_t BoardMpu_SetSampleRate(uint16_t sampleRate, uint8_t flagDLPF);
uint8_t BoardMpu_SetDLPF(uint16_t bandWidth, uint8_t flagDLPF);
void BoardMpu_Calu(float calWeight, float calPeriod,float BoardMpu_Roll);

#endif

