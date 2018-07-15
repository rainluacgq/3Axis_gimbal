#include "usart3.h"
#include "stm32f10x.h"

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
