#include "gimbalcontrol.h"
#include "report.h"
#include "as14ttl.h"
#define ANGLE_MAX_SPEED		((uint16_t)(1000))	//unit: degree per second
#define YAW_MAX_SPEED		((uint16_t)(800))	//unit: degree per second


uint32_t cntGreen = 0, cntRed = 0, cntPosInit = 0, cntCalibrate = 0, cntCaliDelay = 0;
float pitchSpeed = 0.0, rollSpeed = 0.0, yawSpeed = 0.0;
float  gypoZBiasMpu = 0.0,gypoZBiasBoard=0.0;
float gypoMedianBoard=0.0;
float yawDiff = 0.0;
u8 LED0_flag=0;

//云台核心控制程序

void GimbalControl(void)
{
	Mpu6050_Calu(MPU_ACCEL_WEIGHT, MPU_CAL_PERIOD,gypoZBiasMpu);//#1 pitch roll
//	DEBUG_PRINT("[#1]%f,%f ", Mpu6050_Pitch, Mpu6050_Roll);
	BoardMpu_Calu(MPU_ACCEL_WEIGHT, MPU_CAL_PERIOD,gypoZBiasBoard);//#2 pitch roll
//	DEBUG_PRINT("[#2]%f,%f ", BoardMpu_Pitch, BoardMpu_Roll);
  if(fabs(Mpu6050_Pitch) < 0.25)
  {
    Mpu6050_Pitch = 0.0;
  }
  if(fabs(Mpu6050_Roll) < 0.25)
  {
		Mpu6050_Roll = 0.0;
	}

  yawDiff =Mpu6050_Yaw - BoardMpu_Yaw;//#1 yaw - #2 yaw
	//DEBUG_PRINT("[#1]-[#2]%f ", yawDiff);
	gypoMedianBoard = MedianFilter(BoardMpu_Gyro_Z-gypoZBiasBoard);//#2 angular rate
				//DEBUG_PRINT("%f,%f,%f,%f,%f\r\n", gypoZBiasMpu, gypoZBiasBoard, yawMpu, yawBoard, gypoMedianBoard);
				//DEBUG_PRINT("%f,%f,%f ", yawMpu, yawBoard, gypoMedianBoard);
 
				//#2 pitch&roll angle clipping 
	if(BoardMpu_Pitch > HANDLE_PITCH_UPPER || BoardMpu_Pitch < HANDLE_PITCH_LOWER
			|| BoardMpu_Roll > HANDLE_ROLL_UPPER || BoardMpu_Roll < HANDLE_ROLL_LOWER)
	{
			pitchSpeed = rollSpeed = yawSpeed = 0.0;
	}
	
	if(keyValue==0x01)//自动模式
	{
   AutoMode();
	}	
  else if(keyValue==0x02)//遥控模式
	{
		
		LED0_flag++;
		if(LED0_flag==0x20) {LED0_TOGGLE;LED0_flag=0;}
		if(FirstFlag) 	RemoteMode();
	}		
	//usart1_report_imu((int)(Mpu6050_Yaw*100),(int)(BoardMpu_Yaw*100),(int)(yawDiff*100),Mpu6050_Yaw,BoardMpu_Yaw,yawDiff,Mpu6050_Yaw,BoardMpu_Yaw,yawDiff);	
	  rollSpeed = INTERVAL_CONSTRAINT(rollSpeed, ANGLE_MAX_SPEED, ANGLE_MAX_SPEED*(-1));
		pitchSpeed = INTERVAL_CONSTRAINT(pitchSpeed, ANGLE_MAX_SPEED, ANGLE_MAX_SPEED*(-1));//限速
	 yawSpeed = INTERVAL_CONSTRAINT(yawSpeed, YAW_MAX_SPEED, YAW_MAX_SPEED*(-1));
		Motor0_Run((mdir_t)(pitchSpeed > 0), (uint16_t)(fabs(pitchSpeed)));//pitch angle greather than zero, motor run clockwise
		Motor1_Run((mdir_t)(rollSpeed <0), (uint16_t)(fabs(rollSpeed)));//roll angle greather than zero, motor run anticlockwise
  	Motor2_Run((mdir_t)(yawSpeed > 0), (uint16_t)(fabs(yawSpeed)));//yaw  angle difference greather than zero, motor run clockwise
	}


//完成两个mpu bias初始化
void BiasInit(void)
{
        // cntCaliDelay += 1;		
					while(cntCalibrate < MPU_CALI_TIMES)
						{
						Mpu6050_GetGyroData();
						BoardMpu_GetGyroData();
						gypoZBiasMpu +=  Mpu6050_Gyro_Z;
					  gypoZBiasBoard += BoardMpu_Gyro_Z;
						cntCalibrate += 1;
					  }		
           cntCalibrate=0;						
						gypoZBiasMpu /= MPU_CALI_TIMES;
						gypoZBiasBoard /= MPU_CALI_TIMES;
						//Bias clipping 
						//DEBUG_PRINT("%f,%f\r\n", gypoZBiasMpu, gypoZBiasBoard);					
}



//自动模式
void AutoMode(void)
{
	          pitchSpeed = PID_Motor0(Mpu6050_Pitch, 0.0);//#1 pitch
          	if(fabs(gypoMedianBoard) > MPU_GYPO_Z_BOUND)
						{
							//negative direction
							//yawSpeed = gypoMedianBoard;//#2 angular rate
							yawSpeed = gypoMedianBoard/(1000/SYSTEM_PERIOD)*YAW_AUTO_OL_FACTOR;
						}
						else
						{
							yawSpeed = PID_Motor2(yawDiff, 0.0);//#1 yaw - #2 yaw
						}
						
						rollSpeed = PID_Motor1(Mpu6050_Roll, 0.0);//#1 roll

}


void positionInit()
{
     	Mpu6050_Calu(MPU_ACCEL_WEIGHT, MPU_CAL_PERIOD,gypoZBiasMpu);//get pitch and roll angle
			//	DEBUG_PRINT("[#1]pitch: %f roll: %f\r\n", Mpu6050_Pitch, Mpu6050_Roll);
				if(fabs(Mpu6050_Roll) > POS_INIT_DIFF)
				{
					Motor1_Run((mdir_t)(Mpu6050_Roll < 0), POS_INTI_SPEED);//roll angle greather than zero, motor run anticlockwise
				}
				else if(fabs(Mpu6050_Pitch) > POS_INIT_DIFF)
				{
					Motor0_Run((mdir_t)(Mpu6050_Pitch > 0), POS_INTI_SPEED);//pitch angle greather than zero, motor run clockwise
				}
				/**if(fabs(Mpu6050_Roll) > POS_INIT_DIFF || fabs(Mpu6050_Pitch) > POS_INIT_DIFF)
				{
					Motor1_Run((mdir_t)(Mpu6050_Roll < 0), POS_INTI_SPEED);//roll angle greather than zero, motor run anticlockwise
					Motor0_Run((mdir_t)(Mpu6050_Pitch > 0), POS_INTI_SPEED);//pitch angle greather than zero, motor run clockwise
				}*/
				else
				{
					cntPosInit += 1;
					if(cntPosInit > POS_INIT_JUDGE)
					{
						Motor0_Run((mdir_t)(0), 0);
						Motor1_Run((mdir_t)(0), 0);
					}
				}
}
