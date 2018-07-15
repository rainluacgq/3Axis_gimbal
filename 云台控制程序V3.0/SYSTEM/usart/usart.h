#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 
extern u8 keyValue;	

void uart_init(u32 bound);
u8 Sum_Check(u8 *buf,u8 len,u8 checksum);

void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void Usart_SendString( USART_TypeDef * pUSARTx, char *str);
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch);
void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num);
#endif


