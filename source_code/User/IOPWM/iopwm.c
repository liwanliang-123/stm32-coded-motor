#include "stm32f10x.h"
#include "iopwm.h"

void TIM3_PWM_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	     //使能GPIOA外设
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);        //使能定时器3时钟

	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;                 //输出
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_7|GPIO_Pin_6;            // GPIOA.7;GPIOA.6
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;               //TIM3_CH2  TIM3_CH1 
	GPIO_Init(GPIOA,&GPIO_InitStruct);                         //初始化GPIO
	
	//定时器TIM3初始化
	TIM_TimeBaseInitStruct.TIM_Period=arr;                     //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc;                  //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseInitStruct.TIM_ClockDivision=0;                //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up; //TIM向上计数模式
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);            //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	//初始化TIM3 Channel 2 PWM模式	
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM2;               //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;   //比较输出使能
	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_Low;        //输出极性:TIM输出比较极性

	TIM_OC2Init(TIM3,&TIM_OCInitStruct);                       //根据T指定的参数初始化外设TIM3 OC2
	TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);           //使能TIM3在CCR2上的预装载寄存器

	TIM_OC1Init(TIM3,&TIM_OCInitStruct);                       //根据T指定的参数初始化外设TIM3 OC1
	TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);           //使能TIM3在CCR1上的预装载寄存器

	TIM_Cmd(TIM3,ENABLE);                                      //使能定时器
}
