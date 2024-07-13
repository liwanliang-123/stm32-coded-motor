#include "usart.h"
#include "stm32f10x.h"

void usart_Init(void)
{
	GPIO_InitTypeDef gpioartInit;
	USART_InitTypeDef Usart_Init;
	NVIC_InitTypeDef NVIC_Initstr;
	
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_2); //�ж����ȼ��� miss.h 196

	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //GPIO�ڵ�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);  //���Ÿ���ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//����ʱ��
	
	//����PA9Ϊ TX ���
	gpioartInit.GPIO_Mode  = GPIO_Mode_AF_PP; //������� 
	gpioartInit.GPIO_Pin   = GPIO_Pin_9;      //����
	gpioartInit.GPIO_Speed = GPIO_Speed_2MHz; //�ٶ�

	GPIO_Init(GPIOA,&gpioartInit);
	//����PA10Ϊ RX ����
	gpioartInit.GPIO_Mode  = GPIO_Mode_IN_FLOATING; //��������
	gpioartInit.GPIO_Pin   = GPIO_Pin_10;
	gpioartInit.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOA,&gpioartInit);
	
	//USART_InitTypeDef Usart_Init;

	Usart_Init.USART_BaudRate = 115200;                      //������
	Usart_Init.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //Ӳ����
	Usart_Init.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;        //ģʽ
	Usart_Init.USART_Parity = USART_Parity_No;         //У��λ
	Usart_Init.USART_StopBits = USART_StopBits_1;      //ֹͣλ
	Usart_Init.USART_WordLength = USART_WordLength_8b; //�ֽڳ���
	
	USART_Init(USART1, &Usart_Init);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  //�����ж�����
	
	USART_Cmd(USART1,  ENABLE); //�򿪴���

	//�����ж�		
	NVIC_Initstr.NVIC_IRQChannel = USART1_IRQn; //ͨ�� stm32f10x.h 215��216
	NVIC_Initstr.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Initstr.NVIC_IRQChannelPreemptionPriority = 1; 
	NVIC_Initstr.NVIC_IRQChannelSubPriority = 1;

	NVIC_Init(&NVIC_Initstr);

}


//�����ַ�
void USARTsendByte(USART_TypeDef* USARTx, uint16_t Data)
{
	//void USART_SendData(USART_TypeDef* USARTx, uint16_t Data);
	USART_SendData(USARTx, Data);
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);

}

//�����ַ���
void USARTsendStr(USART_TypeDef* USARTx, char* str)
{
	uint16_t i = 0;
	do{
		USARTsendByte(USARTx, *(str + i)); //��ַȡ����
		i++;
		
	}while(*(str + i) != '\0');         //�ж��Ƿ����һλ
	
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET); //������ɱ�ʾ��
}

int fputc(int ch,FILE *f)  //�ض���printf����
{
	USART_SendData(USART1, (uint8_t)ch );
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); //�����ַ���

	return ch;
}

int fgetc(FILE *f)
{
	
	while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET); //����

	return (int)USART_ReceiveData(USART1);


}




