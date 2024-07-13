#include "usart.h"
#include "stm32f10x.h"

void usart_Init(void)
{
	GPIO_InitTypeDef gpioartInit;
	USART_InitTypeDef Usart_Init;
	NVIC_InitTypeDef NVIC_Initstr;
	
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_2); //中断优先级组 miss.h 196

	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //GPIO口的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);  //引脚复用时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//串口时钟
	
	//配置PA9为 TX 输出
	gpioartInit.GPIO_Mode  = GPIO_Mode_AF_PP; //推挽输出 
	gpioartInit.GPIO_Pin   = GPIO_Pin_9;      //引脚
	gpioartInit.GPIO_Speed = GPIO_Speed_2MHz; //速度

	GPIO_Init(GPIOA,&gpioartInit);
	//配置PA10为 RX 输入
	gpioartInit.GPIO_Mode  = GPIO_Mode_IN_FLOATING; //浮空输入
	gpioartInit.GPIO_Pin   = GPIO_Pin_10;
	gpioartInit.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOA,&gpioartInit);
	
	//USART_InitTypeDef Usart_Init;

	Usart_Init.USART_BaudRate = 115200;                      //波特率
	Usart_Init.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //硬件流
	Usart_Init.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;        //模式
	Usart_Init.USART_Parity = USART_Parity_No;         //校验位
	Usart_Init.USART_StopBits = USART_StopBits_1;      //停止位
	Usart_Init.USART_WordLength = USART_WordLength_8b; //字节长度
	
	USART_Init(USART1, &Usart_Init);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  //串口中断配置
	
	USART_Cmd(USART1,  ENABLE); //打开串口

	//配置中断		
	NVIC_Initstr.NVIC_IRQChannel = USART1_IRQn; //通道 stm32f10x.h 215和216
	NVIC_Initstr.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Initstr.NVIC_IRQChannelPreemptionPriority = 1; 
	NVIC_Initstr.NVIC_IRQChannelSubPriority = 1;

	NVIC_Init(&NVIC_Initstr);

}


//发送字符
void USARTsendByte(USART_TypeDef* USARTx, uint16_t Data)
{
	//void USART_SendData(USART_TypeDef* USARTx, uint16_t Data);
	USART_SendData(USARTx, Data);
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);

}

//发送字符串
void USARTsendStr(USART_TypeDef* USARTx, char* str)
{
	uint16_t i = 0;
	do{
		USARTsendByte(USARTx, *(str + i)); //地址取内容
		i++;
		
	}while(*(str + i) != '\0');         //判断是否到最后一位
	
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET); //发送完成表示符
}

int fputc(int ch,FILE *f)  //重定义printf函数
{
	USART_SendData(USART1, (uint8_t)ch );
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); //发送字符串

	return ch;
}

int fgetc(FILE *f)
{
	
	while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET); //接收

	return (int)USART_ReceiveData(USART1);


}




