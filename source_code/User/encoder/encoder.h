#ifndef _ENCODER_H_
#define _ENCODER_H_

#include "stm32f10x.h"

#define ENCODER_TIM_PSC  0          /*计数器分频*/
#define ENCODER_TIM_PERIOD  65535   /*计数器最大值*/
#define CNT_INIT 0                  /*计数器初值*/

#define ENCODER_RESOLUTION  11      //104   /*编码器一圈的物理脉冲数*/
#define ENCODER_MULTIPLE 4          /*编码器倍频，通过定时器的编码器模式设置*/
#define MOTOR_REDUCTION_RATIO  4.4  //30 /*电机的减速比*/
/*电机转一圈总的脉冲数(定时器能读到的脉冲数) = 编码器物理脉冲数*编码器倍频*电机减速比 */
#define TOTAL_RESOLUTION ( ENCODER_RESOLUTION*ENCODER_MULTIPLE*MOTOR_REDUCTION_RATIO ) 
int read_encoder(void);
void TIM4_ENCODER_Init(void);
void calc_motor_rotate_speed(void);


#endif



