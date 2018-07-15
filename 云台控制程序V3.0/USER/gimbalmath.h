
#ifndef __GIMBALMATH_H
#define __GIMBALMATH_H

#include <stdio.h>
#include <math.h>

#define SYSTEM_PERIOD		((5))	//unit: ms
#define MATH_PI				((3.141592653589793))
#define INTERVAL_CONSTRAINT(x, max, min)	((x)>(max)?(max):((x)<(min)?(min):(x)))

#define DEBUG_P0
#ifdef DEBUG_P0
#define DEBUG_PRINT(format, ...)			printf(format, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(format, ...)
#endif


#define GREEN_BLINK_PERIOD	((uint16_t)(300))	//unit: ms
#define RED_BLINK_PERIOD	((uint16_t)(200))	//unit: ms
#define MPU_SAMPLE_RATE		((uint16_t)(50))	//unit: Hz
#define MPU_DLPF_SWITCH		((uint8_t)(1))		//1:on 0:off
#define MPU_ACCEL_WEIGHT	((float)(0.05))
#define MPU_CAL_PERIOD		((float)(SYSTEM_PERIOD/1000.0))		//unit: s
#define POS_INIT_DIFF		((float)(1.0))		//unit: degree
#define POS_INTI_SPEED		((uint16_t)(120))	//unit: degree per second
#define POS_INIT_JUDGE		((uint16_t)(200))
#define LIPO_CAL_WEIGHT		((float)(0.99))//old data weight
#define LIPO_LOW_VOLTAGE	((float)(7.0))//unit: v 3.5v*S 2S->7v 3S->10.5v 4S->14v
#define CAL_WEIGHT		((float)(0.9))//old data weight
#define MPU_CALI_DELAY		((uint16_t)(1000))	//unit: ms
#define MPU_CALI_TIMES		((uint16_t)(800))//800 times about 8s
#define MPU_GYPO_Z_BOUND	((float)(0.16))
#define YAW_AUTO_OL_FACTOR	((float)(3000))
#define ANGLE_MAX_SPEED		((uint16_t)(1000))	//unit: degree per second
#define YAW_MAX_SPEED		((uint16_t)(800))	//unit: degree per second

#define HANDLE_PITCH_UPPER	((float)(55.0))
#define HANDLE_PITCH_LOWER	((float)(-55.0))
#define HANDLE_ROLL_UPPER	((float)(50.0))
#define HANDLE_ROLL_LOWER	((float)(-50.0))
#define CAMERA_PITCH_UPPER	((float)(60.0))
#define CAMERA_PITCH_LOWER	((float)(-60.0))
#define CAMERA_ROLL_UPPER	((float)(60.0))
#define CAMERA_ROLL_LOWER	((float)(-60.0))
#define CAMERA_YAW_UPPER	((float)(30.0))
#define CAMERA_YAW_LOWER	((float)(-30.0))

#define MPU_YAW_BIAS_MAX	((float)(-1.000))
#define MPU_YAW_BIAS_MIN	((float)(-1.250))
#define BOARD_YAW_BIAS_MAX	((float)(-0.450))
#define BOARD_YAW_BIAS_MIN	((float)(-5.500))




#endif

