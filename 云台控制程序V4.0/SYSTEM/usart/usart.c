#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "gimbalcontrol.h"
#include "tim5message.h"
#include "as14ttl.h"
#include "pid.h"
#include "report.h"
u8 keyValue=0x01;  //默认自动模式

////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_UCOS
#include "includes.h"					//ucos 使用	  
#endif


//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 


 
void uart_init(u32 bound)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;	  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟以及复用功能时钟
   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);  
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);  

  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART1, &USART_InitStructure); //初始化串口
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断
	USART_Cmd(USART1, ENABLE);                    //使能串口 
	
}

u8 FirstFlag=0;
static u8 index=0;
static u8 USART_RX[20];
 void USART1_IRQHandler(void)  
{
	u8 Res;
	u8 i;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
			Res =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
			if(Res==0x0a&&USART_RX[index-1]==0x0d)
			{				
				if(index-1==0x06)  //Sum_Check(USART_RX,6,USART_RX[5]
				{
					 if(USART_RX[2]==0x01&&USART_RX[4]==0x11)   //通信检查
								{
									index=0;
									Send_Check(USART_RX[2],USART_RX[5]);
									for(i=0;i<index+1;i++)  {USART_RX[i]=0;}
								}
							 else if(USART_RX[2]==0x03)     //读取键值
								{
									index=0;
									keyValue= USART_RX[4];  
                  Send_Check(USART_RX[2],USART_RX[5]);
			          	for(i=0;i<index+1;i++){USART_RX[i]=0;}
		            }
				}
				else if(index-1==0x0b)
				{
					FirstFlag=1;
					index=0;
					Send_Check(USART_RX[2],USART_RX[10]);
						 ADC_Value[0] = (USART_RX[4] & 0xff)<<8 | USART_RX[5];
						 ADC_Value[1] = (USART_RX[6] & 0xff)<<8 | USART_RX[7];
	           ADC_Value[2] = (USART_RX[8] & 0xff)<<8 | USART_RX[9];
				//	DEBUG_PRINT("%-5d %-5d %-5d\r\n", ADC_Value[0], ADC_Value[1], ADC_Value[2]);
					for(i=0;i<index+1;i++)  {USART_RX[i]=0;}        //清楚串口缓存 
				}
				else  //错误遥控信息
				{
					index=0;
					for(i=0;i<index+1;i++)  {USART_RX[i]=0;}
				}
					
			//	for(;i<index;i++){USART_RX[i]=0;}
			//	index=0;
			}
			else
			{
				USART_RX[index]=Res;
				index++;
			}
		}
}


//和校验
//成功返回1，失败返回0

u8 Sum_Check(u8 *buf,u8 len,u8 checksum)
{
	u8 i,sum=0;
  for(i=0;i<len;i++)  sum+=buf[i];
	if(checksum==sum) return 1;
	else return 0;
}

/*****************  发送一个字节 **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* 发送一个字节数据到USART */
	USART_SendData(pUSARTx,ch);
		
	/* 等待发送数据寄存器为空 */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/****************** 发送8位的数组 ************************/
void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num)
{
  uint8_t i;
	
	for(i=0; i<num; i++)
  {
	    /* 发送一个字节数据到USART */
	    Usart_SendByte(pUSARTx,array[i]);	
  
  }
	/* 等待发送完成 */
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
}

/*****************  发送字符串 **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* 等待发送完成 */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}

/*****************  发送一个16位数 **********************/
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;
	
	/* 取出高八位 */
	temp_h = (ch&0XFF00)>>8;
	/* 取出低八位 */
	temp_l = ch&0XFF;
	
	/* 发送高八位 */
	USART_SendData(pUSARTx,temp_h);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
	
	/* 发送低八位 */
	USART_SendData(pUSARTx,temp_l);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}



//pid串口参数调试函数
/*
static u8 index=0;
u8 USART_RX[20];
void USART1_IRQHandler(void)  
{
	u8 Res;
	u8 i;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
			Res =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
			if(Res==0x0a&&USART_RX[index-1]==0x0d)
			{
				//Usart_SendArray( USART1,USART_RX, index);
				if(index-1==0x09)
				{
					for(i=0;i<index-1;i++){ debugPara[i]=USART_RX[i];}
			    for(i=0;i<index+1;i++){USART_RX[i]=0;}		            
				}
				else  //错误遥控信息
				{
					index=0;
					for(i=0;i<index+1;i++)  {USART_RX[i]=0;}
				}
					
			//	for(;i<index;i++){USART_RX[i]=0;}
			//	index=0;
			}
			else
			{
				USART_RX[index]=Res;
				index++;
			}
		}
}
*/



