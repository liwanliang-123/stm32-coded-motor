#include "stm32f10x.h"
#include "io.h"

void IO_Init(void)
{
 
	GPIO_InitTypeDef  GPIO_InitStructure;
 	//*************循迹端口初始化************
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOF,ENABLE);	 //使能PB\PE\PF端口时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //P.5 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_5;				 //P.5 端口配置
	GPIO_Init(GPIOE, &GPIO_InitStructure);					 //根据设定参数初始化GPIOE.1.2.5
	
	
	//************L298N初始化*************
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//推挽输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;
	GPIO_Init(GPIOF,&GPIO_InitStructure);//初始化GPIO
	GPIO_ResetBits(GPIOF,GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4);//初始化IO设置低电平
	//**********BEEP初始化**************
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
	GPIO_Init(GPIOB,&GPIO_InitStructure);//初始化GPIO
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);//初始化IO设置低电平	
}
