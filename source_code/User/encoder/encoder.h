#ifndef _ENCODER_H_
#define _ENCODER_H_

#include "stm32f10x.h"

#define ENCODER_TIM_PSC  0          /*��������Ƶ*/
#define ENCODER_TIM_PERIOD  65535   /*���������ֵ*/
#define CNT_INIT 0                  /*��������ֵ*/

#define ENCODER_RESOLUTION  11      //104   /*������һȦ������������*/
#define ENCODER_MULTIPLE 4          /*��������Ƶ��ͨ����ʱ���ı�����ģʽ����*/
#define MOTOR_REDUCTION_RATIO  4.4  //30 /*����ļ��ٱ�*/
/*���תһȦ�ܵ�������(��ʱ���ܶ�����������) = ����������������*��������Ƶ*������ٱ� */
#define TOTAL_RESOLUTION ( ENCODER_RESOLUTION*ENCODER_MULTIPLE*MOTOR_REDUCTION_RATIO ) 
int read_encoder(void);
void TIM4_ENCODER_Init(void);
void calc_motor_rotate_speed(void);


#endif



