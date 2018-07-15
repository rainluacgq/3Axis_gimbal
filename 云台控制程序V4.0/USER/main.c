#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "mpu6050.h"
#include "boardmpu.h"
#include "motor.h"
#include "gimbalmath.h" 
#include <stdio.h>
#include "gimbalcontrol.h" 
#include "tim5message.h"
#include "report.h"
#include "usart3.h"
#include "mag3110.h"
/************************************************
 ALIENTEK战舰STM32开发板实验32
 MPU6050六轴传感器 实验
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 作者：武汉伟创科技有限公司
************************************************/


int main(void)
{	
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(9600);	 	//串口初始化为9600
	//uart3_init(9600);
	delay_init();	//延时初始化 
	LED_Init();		  			//初始化与LED连接的硬件接口
//	TIM5_Init(4999,7199);//500ms进入一次中断
	while(BoardMpu_Init(MPU_SAMPLE_RATE, MPU_DLPF_SWITCH))
	{
		delay_ms(200);
	}		 //50Hz、使能低通滤波  
	while(Mpu6050_Init(MPU_SAMPLE_RATE, MPU_DLPF_SWITCH))
	{
		delay_ms(200);
	}
	/*
	while(MAG3110_Init())
	{
		delay_ms(10);
	}
	*/
	Motor_Init();
  BiasInit();
	positionInit();
  while(1)
	{	
	   GimbalControl();
	}
}

 


