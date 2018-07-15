
#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f10x.h"
#include "gimbalmath.h"

#define TIM_PERIOD			((uint16_t)(50-1))	//50us
#define TIM_PRESCALER		((uint16_t)(72-1))		//72MHz/72=1MHz 1us
//#define TIM_PERIOD		((uint16_t)(256-1))		//7.96875ms
//#define TIM_PRESCALER		((uint16_t)(2250-1))	//72MHz/2250=32kHz 31.25us
#define TIM_DEADTIME		((uint8_t)(0x08))
#define MOTOR_BASIC_SPEED	((uint16_t)(60))	//unit: °/s
#define MOTOR_RUN_LIMIT		((uint16_t)(5))		//unit: °/s

typedef enum {mdir_anticlk = 0, mdir_clkwise} mdir_t;
const static int8_t mdir_cal_factor[] = {-1, 1};

//const static uint8_t pwmSin[] = {128, 147, 166, 185, 203, 221, 238, 243, 248, 251, 253, 255, 255, 255, 253, 251, 248, 243, 238, 243, 248, 251, 253, 255, 255, 255, 253, 251, 248, 243, 238, 221, 203, 185, 166, 147, 128, 109, 90, 71, 53, 35, 18, 13, 8, 5, 3, 1, 1, 1, 3, 5, 8, 13, 18, 13, 8, 5, 3, 1, 1, 1, 3, 5, 8, 13, 18, 35, 53, 71, 90, 109};
//const static uint8_t pwmSin[] = {128, 132, 136, 140, 143, 147, 151, 155, 159, 162, 166, 170, 174, 178, 181, 185, 189, 192, 196, 200, 203, 207, 211, 214, 218, 221, 225, 228, 232, 235, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 248, 249, 250, 250, 251, 252, 252, 253, 253, 253, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 254, 254, 254, 253, 253, 253, 252, 252, 251, 250, 250, 249, 248, 248, 247, 246, 245, 244, 243, 242, 241, 240, 239, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 248, 249, 250, 250, 251, 252, 252, 253, 253, 253, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 254, 254, 254, 253, 253, 253, 252, 252, 251, 250, 250, 249, 248, 248, 247, 246, 245, 244, 243, 242, 241, 240, 239, 238, 235, 232, 228, 225, 221, 218, 214, 211, 207, 203, 200, 196, 192, 189, 185, 181, 178, 174, 170, 166, 162, 159, 155, 151, 147, 143, 140, 136, 132, 128, 124, 120, 116, 113, 109, 105, 101, 97, 94, 90, 86, 82, 78, 75, 71, 67, 64, 60, 56, 53, 49, 45, 42, 38, 35, 31, 28, 24, 21, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 8, 7, 6, 6, 5, 4, 4, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 5, 6, 6, 7, 8, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 8, 7, 6, 6, 5, 4, 4, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 5, 6, 6, 7, 8, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 21, 24, 28, 31, 35, 38, 42, 45, 49, 53, 56, 60, 64, 67, 71, 75, 78, 82, 86, 90, 94, 97, 101, 105, 109, 113, 116, 120, 124};
//const static uint8_t pwmSin[] = {120, 140, 158, 174, 186, 195, 199, 199, 195, 186, 174, 158, 140, 120, 100, 79, 59, 41, 25, 13, 4, 0, 0, 4, 13, 25, 41, 59, 79, 100};
//const static uint8_t pwmSin[] = {191, 238, 255, 238, 191, 127, 64, 17, 0, 17, 64, 127};//12n14p sine
//const static uint8_t pwmSin[] = {238, 238, 127, 17, 17, 127};
#define SINE_ARRAY_MAX_LEN 2048
static uint16_t pwmSin[SINE_ARRAY_MAX_LEN];

static uint16_t sineArraySize;
static uint16_t phaseShift;

typedef struct
{
	uint16_t stepA;
	uint16_t stepB;
	uint16_t stepC;
}mphase_t;
static mphase_t mPitch, mRoll, mYaw;

void Motor_Init(void);
void SineArray_Init(void);
void MotorPWM_Init(void);
void MotorPos_Init(void);
void Motor0_Run(mdir_t mdir, uint16_t speed);
/**
void Motor0_Disable(void);
void Motor0_Enable(void);
*/
void Motor1_Run(mdir_t mdir, uint16_t speed);
void Motor2_Run(mdir_t mdir, uint16_t speed);
void Motor0_SetPWM(void);
void Motor1_SetPWM(void);
void Motor2_SetPWM(void);
void Motor0_Shutdown(void);
void Motor1_Shutdown(void);
void Motor2_Shutdown(void);

#endif

