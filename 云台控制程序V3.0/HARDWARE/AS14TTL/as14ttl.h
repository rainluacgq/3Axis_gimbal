#ifndef __AS14TTL_H
#define __AS14TTL_H

#include "stm32f10x.h"
#include "gimbalmath.h"
#include "motor.h"
#include "gimbalcontrol.h"
extern __IO uint16_t Serial_Z,Serial_X, Serial_Y;

extern __IO  uint16_t ADC_Value[3];

static float K_X = 0.0, K_Y = 0.0,K_Z=0.0;
static float B_X = 0.0, B_Y = 0.0,B_Z=0.0;

#define ANALOG_UPPER	(32768)
#define ANALOG_LOWER	(0)
#define ANGLE_Z_UPPER	(150)//yaw
#define ANGLE_Z_LOWER	(-150)
#define ANGLE_Y_UPPER	(60)//pitch
#define ANGLE_Y_LOWER	(-60)
#define ANGLE_X_UPPER	(55)//roll
#define ANGLE_X_LOWER	(-55)
#define ANALOG_BIAS_X	(14)//static X - 2048 
#define ANALOG_BIAS_Y	(6411)//static Y - 2048
#define ANALOG_BIAS_Z  (10)

void RemoteMode(void);
void Serial_CalXYZ(float calWeight);
void SerialPara_Init(void);
void ConvertAngle(float* xAngle, float* yAngle,float *zAngle);
#endif
