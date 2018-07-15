#ifndef __GIMBALCONTROL_H
#define __GIMBALCONTROL_H
#include"stm32f10x.h"
#include "stm32f10x.h"
#include "usart.h"
#include "mpu6050.h"
#include "boardmpu.h"
#include "motor.h"
#include "pid.h"
#include "gimbalmath.h"
#include "led.h"
#include "delay.h"



#define KEY_DOWN_PERIOD	(10)

extern float pitchSpeed ;
extern float rollSpeed ;
extern float yawSpeed ;
extern float yawDiff;
void GimbalControl(void);
void Serial_CalXYZ(float calWeight);
void ConvertAngle(float* xAngle, float* yAngle,float *zAngle);
void SerialMode(void);
void AutoMode(void);
void BiasInit(void);
void SerialPara_Init(void);
void positionInit(void);
#endif
