#include "stm32f10x.h"
#include <stdio.h>
void usart_Init(void);
void USARTsendByte(USART_TypeDef* USARTx, uint16_t Data);
void USARTsendStr(USART_TypeDef* USARTx, char* str);
