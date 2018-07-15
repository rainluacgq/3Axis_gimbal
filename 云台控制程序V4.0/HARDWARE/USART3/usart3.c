#include "usart3.h"
#include "stm32f10x.h"
#include "tim5message.h"
#include "as14ttl.h"
void uart3_init(u32 bound){  
  //GPIO????  
  GPIO_InitTypeDef GPIO_InitStructure;  
    USART_InitTypeDef USART_InitStructure;  
    NVIC_InitTypeDef NVIC_InitStructure;  
       
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE); //remap??|RCC_APB2Periph_AFIO  
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);//???????  
        //??,?????remap,????b11,b10  
    GPIO_PinRemapConfig(GPIO_PartialRemap_USART3,ENABLE);    
  
    //USART3_TX   GPIOC.10  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   //??????  
  GPIO_Init(GPIOC, &GPIO_InitStructure);//???  
     
  //USART3_RX     GPIOC.11???  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//????  
  GPIO_Init(GPIOC, &GPIO_InitStructure);//??   
  
      
  
  //Usart3 NVIC ??  
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//?????3  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;      //????3  
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //IRQ????  
    NVIC_Init(&NVIC_InitStructure); //??????????VIC???  
    
   //USART ?????  
  
    USART_InitStructure.USART_BaudRate = bound;//?????  
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//???8?????  
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//?????  
    USART_InitStructure.USART_Parity = USART_Parity_No;//??????  
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//????????  
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //????  
  
  USART_Init(USART3, &USART_InitStructure); //?????3  
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//????????  
  USART_Cmd(USART3, ENABLE);                    //????3    
}

static u8 index=0;
static u8 USART_RX[20];
void USART3_IRQHandler(void)  
{
	u8 Res;
	u8 i;
	if(Res==0x0a&&USART_RX[index-1]==0x0d)
			{				
				if(index-1==0x06)  //Sum_Check(USART_RX,6,USART_RX[5]
				{
					 if(USART_RX[2]==0x01&&USART_RX[4]==0x11)   //通信检查
								{
									index=0;
									//Send_Check(USART_RX[2],USART_RX[5]);
									for(i=0;i<index+1;i++)  {USART_RX[i]=0;}
								}
							 else if(USART_RX[2]==0x03)     //读取键值
								{
									index=0;
									keyValue= USART_RX[4];  
                //  Send_Check(USART_RX[2],USART_RX[5]);
			          	for(i=0;i<index+1;i++){USART_RX[i]=0;}
		            }
								Usart_SendArray(USART3,USART_RX,6);
				}
				else if(index-1==0x0b)
				{
					index=0;
					//Send_Check(USART_RX[2],USART_RX[10]);
						 ADC_Value[0] = (USART_RX[4] & 0xff)<<8 | USART_RX[5];
						 ADC_Value[1] = (USART_RX[6] & 0xff)<<8 | USART_RX[7];
	           ADC_Value[2] = (USART_RX[8] & 0xff)<<8 | USART_RX[9];
					DEBUG_PRINT("%-5d %-5d %-5d\r\n", ADC_Value[0], ADC_Value[1], ADC_Value[2]);
					for(i=0;i<index+1;i++)  {USART_RX[i]=0;}        //清楚串口缓存 
				}
				else  //错误遥控信息
				{
					index=0;
					for(i=0;i<index+1;i++)  {USART_RX[i]=0;}
				}
			}
			else
			{
				USART_RX[index]=Res;
				index++;
			}
}
