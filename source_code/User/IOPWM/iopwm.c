#include "stm32f10x.h"
#include "iopwm.h"

void TIM3_PWM_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	     //ʹ��GPIOA����
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);        //ʹ�ܶ�ʱ��3ʱ��

	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;                 //���
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_7|GPIO_Pin_6;            // GPIOA.7;GPIOA.6
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;               //TIM3_CH2  TIM3_CH1 
	GPIO_Init(GPIOA,&GPIO_InitStruct);                         //��ʼ��GPIO
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseInitStruct.TIM_Period=arr;                     //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc;                  //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseInitStruct.TIM_ClockDivision=0;                //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up; //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);            //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	//��ʼ��TIM3 Channel 2 PWMģʽ	
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM2;               //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;   //�Ƚ����ʹ��
	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_Low;        //�������:TIM����Ƚϼ���

	TIM_OC2Init(TIM3,&TIM_OCInitStruct);                       //����Tָ���Ĳ�����ʼ������TIM3 OC2
	TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);           //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���

	TIM_OC1Init(TIM3,&TIM_OCInitStruct);                       //����Tָ���Ĳ�����ʼ������TIM3 OC1
	TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);           //ʹ��TIM3��CCR1�ϵ�Ԥװ�ؼĴ���

	TIM_Cmd(TIM3,ENABLE);                                      //ʹ�ܶ�ʱ��
}
