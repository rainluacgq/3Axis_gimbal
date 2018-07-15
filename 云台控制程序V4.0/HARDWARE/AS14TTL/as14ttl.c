/*****************2.4G无线通信模块*****************/

#include "as14ttl.h"
#include "report.h"
//合成串口采样
__IO  uint16_t Serial_Z = 0,Serial_X = 0, Serial_Y = 0;
__IO  uint16_t ADC_Value[3]={0,0,0};
float ExpPitch = 0.0, ExpYaw = 0.0,ExpRoll=0.0;

//遥控模式
void RemoteMode(void)
{
	         SerialPara_Init();
	         Serial_CalXYZ(CAL_WEIGHT);       //CAL_WEIGHT为合成权重
				   ConvertAngle(&ExpRoll,&ExpPitch, &ExpYaw);
	       //  usart1_report_imu((int)(Mpu6050_Yaw*100),(int)(ExpPitch*100),(int)(ExpRoll*100),Mpu6050_Yaw,BoardMpu_Yaw,yawDiff,Mpu6050_Yaw,BoardMpu_Yaw,yawDiff);	
						//#1 expect pitch, #1-#2 yaw angle clipping 
          	ExpRoll=INTERVAL_CONSTRAINT(ExpRoll, CAMERA_ROLL_UPPER, CAMERA_ROLL_LOWER);
						ExpPitch = INTERVAL_CONSTRAINT(ExpPitch, CAMERA_PITCH_UPPER, CAMERA_PITCH_LOWER);
						ExpYaw = INTERVAL_CONSTRAINT(ExpYaw, CAMERA_YAW_UPPER, CAMERA_YAW_LOWER);
	
						DEBUG_PRINT("%f,%f,%f\r\n",ExpRoll, ExpPitch, ExpYaw);
	          
		        pitchSpeed = PID_Motor0(Mpu6050_Pitch, ExpPitch);//#1 pitch
	          rollSpeed=PID_Motor1(Mpu6050_Roll, ExpRoll);//#1 roll
						yawSpeed = PID_Motor2(yawDiff, ExpYaw);//#1 yaw - #2 yaw
}

void Serial_CalXYZ(float calWeight)
{
	Serial_X = (uint16_t)(calWeight*Serial_X + (1-calWeight)*ADC_Value[0]);
	Serial_Y = (uint16_t)(calWeight*Serial_Y + (1-calWeight)*ADC_Value[1]);
	Serial_Z=  (uint16_t)(calWeight*Serial_Z + (1-calWeight)*ADC_Value[2]);
	//DEBUG_PRINT("%d,%d,%d", Serial_X, Serial_Y,Serial_Z);
}


void SerialPara_Init(void)
{

	/**
	votage:0~5v  ad:0~32768
		Z 	-> yaw		32768~0	-> +60~-60 degree
		Y   -> pitch	32768~0	-> +60~-60 degree
		X	  -> roll	  32768~0	-> +55~-55 degree
	*/
	K_X = (ANGLE_X_LOWER - ANGLE_X_UPPER) * 1.0 / (ANALOG_LOWER - ANALOG_UPPER);
	B_X = ANGLE_X_LOWER - K_X * ANALOG_LOWER;
	K_Y = (ANGLE_Y_LOWER - ANGLE_Y_UPPER) * 1.0 / (ANALOG_LOWER - ANALOG_UPPER);
	B_Y = ANGLE_Y_LOWER - K_Y * ANALOG_LOWER;
	K_Z = (ANGLE_Z_LOWER - ANGLE_Z_UPPER) * 1.0 / (ANALOG_LOWER - ANALOG_UPPER);
	B_Z = ANGLE_Z_LOWER - K_Z * ANALOG_LOWER;
	//DEBUG_PRINT("%f,%f,%f,%f ,%f,%f",K_X, K_Y, B_X, B_Y,K_Z,B_Z);
}

//convert analog values into expect angles
void ConvertAngle(float* xAngle, float* yAngle,float *zAngle)
{
	uint16_t xCal, yCal,zCal;
	xCal = INTERVAL_CONSTRAINT(Serial_X - ANALOG_BIAS_X, ANALOG_UPPER, ANALOG_LOWER);
	yCal = INTERVAL_CONSTRAINT(Serial_Y - ANALOG_BIAS_Y, ANALOG_UPPER, ANALOG_LOWER);
	zCal = INTERVAL_CONSTRAINT(Serial_Z - ANALOG_BIAS_Z, ANALOG_UPPER, ANALOG_LOWER);
	//DEBUG_PRINT("%d,%d,%d ", xCal, yCal,zCal);
	*xAngle = K_X * xCal + B_X;
	*yAngle = K_Y * yCal + B_Y;
	*zAngle = K_Z	* zCal + B_Z;
}
