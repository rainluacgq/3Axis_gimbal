
#ifndef __PID_H
#define __PID_H

#include "stm32f10x.h"
#include "gimbalmath.h"

static float pitchPPara = 90.0;
static float pitchIPara = 0.0;
static float pitchDPara = 10.0;
static float rollPPara = 90.0;
static float rollIPara = 0.0;
static float rollDPara = 10.0;
static float yawPPara = 0.0;
static float yawIPara = 0.0;
static float yawDPara = 0.0;

#define PITCH_P_PARAM		((float)(1.0))
#define PITCH_I_PARAM		((float)(0.0))
#define PITCH_D_PARAM		((float)(0.0))

#define ROLL_P_PARAM		((float)(1.0))
#define ROLL_I_PARAM		((float)(0.0))
#define ROLL_D_PARAM		((float)(0.0))

#define YAW_P_PARAM			((float)(1.0))
#define YAW_I_PARAM			((float)(0.0))
#define YAW_D_PARAM			((float)(0.0))

#define PITCH_P_UPPER		((float)(0.0))
#define PITCH_P_LOWER		((float)(0.0))
#define PITCH_I_UPPER		((float)(100.0))
#define PITCH_I_LOWER		((float)(-100.0))
#define PITCH_RES_UPPER		((float)(0.0))
#define PITCH_RES_LOWER		((float)(0.0))

#define ROLL_P_UPPER		((float)(0.0))
#define ROLL_P_LOWER		((float)(0.0))
#define ROLL_I_UPPER		((float)(100.0))
#define ROLL_I_LOWER		((float)(-100.0))
#define ROLL_RES_UPPER		((float)(0.0))
#define ROLL_RES_LOWER		((float)(0.0))

#define YAW_P_UPPER			((float)(0.0))
#define YAW_P_LOWER			((float)(0.0))
#define YAW_I_UPPER			((float)(100.0))
#define YAW_I_LOWER			((float)(-100.0))
#define YAW_RES_UPPER		((float)(0.0))
#define YAW_RES_LOWER		((float)(0.0))

#define I_CAL_PERIOD		((uint8_t)(10))
#define P_CAL_DEADBAND		((float)(2.0))

#define DEBUG_USART1_PID	0
#define DEBUG_PARA_LEN		9
extern __IO u8 debugPara[9];

#define MEDIAN_FILTER_LEN	31
#define MEDIAN_AVE_SCALE	((float)(0.5))
static float mfData[MEDIAN_FILTER_LEN];
static float mfSort[MEDIAN_FILTER_LEN];

float PID_Motor0(float actAngle, float expAngle);
float PID_Motor1(float actAngle, float expAngle);
float PID_Motor2(float actAngle, float expAngle);
void Usart1StringToFloat(void);
float MedianFilter(float data);

#endif

