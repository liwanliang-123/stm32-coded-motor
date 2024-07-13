#include "stm32f10x.h"
#include "io.h"

void IO_Init(void)
{
 
	GPIO_InitTypeDef  GPIO_InitStructure;
 	//*************ѭ���˿ڳ�ʼ��************
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOF,ENABLE);	 //ʹ��PB\PE\PF�˿�ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //P.5 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_5;				 //P.5 �˿�����
	GPIO_Init(GPIOE, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOE.1.2.5
	
	
	//************L298N��ʼ��*************
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//�������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;
	GPIO_Init(GPIOF,&GPIO_InitStructure);//��ʼ��GPIO
	GPIO_ResetBits(GPIOF,GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4);//��ʼ��IO���õ͵�ƽ
	//**********BEEP��ʼ��**************
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
	GPIO_Init(GPIOB,&GPIO_InitStructure);//��ʼ��GPIO
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);//��ʼ��IO���õ͵�ƽ	
}
