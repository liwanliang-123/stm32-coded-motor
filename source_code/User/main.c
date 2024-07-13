#include "stm32f10x.h"
#include "oled.h"
#include "SysTick.h"
#include "encoderTim.h"
#include "encoder.h"
#include "GPIO_IICOLED.h"
#include "iopwm.h"
#include "PIDEncoder.h"

unsigned int i = 0;
extern int  encoderNum;
extern float rotateSpeed;
   
void delay(uint16_t time)
{
	uint16_t i = 0;
	while(time--){
		i = 12000;
		while(i--);
	}
}

int main(void)
{	
		PID_Init();
		I2C_configuration();
		OLED_Init();
		MS_delay(1000);
		OLED_Init();
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);      /*设置系统中断优先级分组2*/

		TIM4_ENCODER_Init();                                 /*捕获正交编码器的脉冲*/
		TIM2_Int_Init(1000-1,7200-1);
		TIM3_PWM_Init(19999,71);                             /*20ms*/ 

		TIM_SetCompare1(TIM3,0);
		TIM_SetCompare2(TIM3,0);
		
		OLED_clear(); 																				/*清屏*/
	
		while(1)
		{
	
		}
}	
	
void TIM2_IRQHandler(void)
{
	
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET){   /*检测是否发送中断*/
			calc_motor_rotate_speed();
	}
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);          /*清除中断*/
}

