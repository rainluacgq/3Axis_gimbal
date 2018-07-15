/*****************用定时器向遥控器返回云台信息*****************/

#include "tim5message.h"
u8 fun; //功能字
u8 len;  //数据帧长度
static u8 time_flag=0;  //发送数据时间标志，2s发送一次
 void TIM5_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //时钟使能
	
	//定时器TIM5初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE ); //使能指定的TIM5中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM5中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


	TIM_Cmd(TIM5, ENABLE);  //使能TIMx	
}

//定时器5中断服务程序  
void TIM5_IRQHandler(void)   //TIM5中断
{
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)  //检查TIM5更新中断发生与否
		{
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update  );  //清除TIMx更新中断标志 
			time_flag++;
			if(time_flag==0x04)
			{
				time_flag=0;
				Send_Remote((Mpu6050_Roll*100),(Mpu6050_Pitch*100),(Mpu6050_Yaw*100));
			}
		}
}


//发送应答到遥控器
void Send_Check(u8 check_head,u8 check_sum)
{
	u8 send_buf[9];
	u8 i;
	fun=0x02;
	len=0x02;
   for (i = 0;i<9;i++)send_buf[i] = 0;//清0
	send_buf[0]=0xaa;
	send_buf[1]=0xaa;
	send_buf[2]=fun;
	send_buf[3]=len;
	send_buf[4]=check_head;   //
	send_buf[5]=check_sum;
	for(i=0;i<6;i++) send_buf[6]+=send_buf[i]; //计算和校验
	send_buf[7]=0x0d;
	send_buf[8]=0x0a;
	Usart_SendArray(USART1,send_buf,9);
}

/**
  **********************************
  * @brief  通过串口1上报结算后的姿态数据给电脑
  * @param  
						roll:横滚角.单位0.01度。 -18000 -> 18000 对应 -180.00  ->  180.00度
						pitch:俯仰角.单位 0.01度。-9000 - 9000 对应 -90.00 -> 90.00 度
						yaw:航向角.单位为0.01度 0 -> 3600  对应 0 -> 360.0度
  * @retval None
  **********************************
*/
//发送云台状态信系到遥控器
void Send_Remote(short roll,short pitch,short yaw)
{
	u8 send_buf[13];
	u8 i;
	fun=0x01;
	len=0x06;
	for (i = 0;i<13;i++)send_buf[i] = 0;//清0
	send_buf[0]=0xaa;
	send_buf[1]=0xaa;
	send_buf[2]=fun;
	send_buf[3]=len;
	send_buf[4]=(roll>>8)&0xFF;
	send_buf[5]=roll&0xFF;
	send_buf[6]=(pitch>>8)&0xFF;
	send_buf[7]=pitch&0xFF;
	send_buf[8]=(yaw>>8)&0xFF;
	send_buf[9]=yaw&0xFF;
	for(i=0;i<10;i++) send_buf[10]+=send_buf[i];
	send_buf[11]=0x0d;
	send_buf[12]=0x0a;
	Usart_SendArray(USART1,send_buf,13);
} 


