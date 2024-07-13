#include "stm32f10x.h"
#include "encoder.h"
#include "sys.h"
#include "oled.h"
#include "SysTick.h"
#include "GPIO_IICOLED.h"
#include "encoder.h"
#include "PIDEncoder.h"
#include <stdio.h>

int encoderNum = 0;
float rotateSpeed = 0;
float Speed = 0;
float Speed2 = 0;
int para_L,para_R;                                    //����
int Encoder_Left,Encoder_Right;
static int SetPoint=150;                              //����Ŀ���ٶ�
int Moto_Left = 0,Moto_Right; 
//static int number = 0;

/**
* @brief TIM4 ͨ��1ͨ��2 ����������
*/
void TIM4_ENCODER_Init(void)                      
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStruct;
	TIM_ICInitTypeDef TIM_ICInitStruct;   	                  //���벶��

	                                                          //PB6 ch1  A,PB7 ch2 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);      //ʹ��TIM4ʱ��	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);     //ʹ��GPIOAʱ��
	
	GPIO_StructInit(&GPIO_InitStructure);                     //��GPIO_InitStruct�еĲ�����ȱʡֵ����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;     //PA6 PA7��������	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);    
	

																														/*ʱ����ʼ��*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);      /*ʹ�ܶ�ʱ��ʱ�� APB1*/
	TIM_DeInit(TIM4);                          
	//TIM_TimeBaseStructInit(&TIM_TimeBaseStruct);    
	TIM_TimeBaseStruct.TIM_Prescaler = ENCODER_TIM_PSC;       /*Ԥ��Ƶ 0*/        
	TIM_TimeBaseStruct.TIM_Period = ENCODER_TIM_PERIOD;       /*����(��װ��ֵ)65535*/
	TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;      
	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;  /*�������ϼ���ģʽ*/  
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStruct); 

	/*������ģʽ���ã�ͬʱ����ͨ��1��ͨ��2(��4��Ƶ)�����Ծ�ΪRising*/
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12,TIM_ICPolarity_Rising, TIM_ICPolarity_Rising); 
	TIM_ICStructInit(&TIM_ICInitStruct);        
	TIM_ICInitStruct.TIM_ICFilter = 0;                       /*����ͨ�����˲�����*/
	TIM_ICInit(TIM4, &TIM_ICInitStruct);                     /*����ͨ����ʼ��*/
	TIM_SetCounter(TIM4, CNT_INIT);                          /*CNT���ֵ  0 */
	TIM_ClearFlag(TIM4,TIM_IT_Update);                       /*�жϱ�־��0*/
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);               /*�ж�ʹ��*/
	TIM_Cmd(TIM4,ENABLE);                                    /*ʹ��CR�Ĵ���*/
}


// ��ȡ��ʱ������ֵ
int read_encoder(void)
{
	int encoderNum = 0;
	encoderNum = (int)((int16_t)(TIM4->CNT));                /*CNTΪuint32, תΪint16*/
	TIM_SetCounter(TIM4, CNT_INIT);                          /*CNT���ֵ*/

	return encoderNum;
}

//������ת�٣�����һ����ʱ��100ms����1�Σ�
void calc_motor_rotate_speed()
{
	
	/*��ȡ��������ֵ������������ת����*/
	encoderNum = read_encoder();

	/* ת��(1����ת����Ȧ)=��λʱ���ڵļ���ֵ/�ֱܷ���*ʱ��ϵ�� */
	rotateSpeed = (int)encoderNum/TOTAL_RESOLUTION*10;
	
//	Speed = (rotateSpeed * 6.0 * 3.14)/1.0;       //�ٶ�   1����ת��Ȧ�� X һȦ�ľ���(�����ܳ�)/1s  

	Speed = ((encoderNum/(48.4*4)) * 18.84)/0.1;    //(���ܵ�������/���һȦ������*������*4��Ƶ)*�����ܳ�)/100ms 
	
	para_L = PID_Calc(Speed,SetPoint);	            //�������õ�����ʽPID��������ֵ 
	
	if((para_L < -3) || (para_L > 3)) 						  // ���� PID ��������������СʱƵ�����������𵴡�
	{                                               //����10�ŵ���
		Moto_Left += para_L;  
	}   
	
	if(Moto_Left > 19000)Moto_Left = 19000;         //�޷�
	
	TIM_SetCompare1(TIM3, Moto_Left);
	TIM_SetCompare2(TIM3, 0);
	
//OLED_ShowNum(0,0,para_L,8,16); ��ʾ���˶���ռ�ձȣ�pid���ڳ�����ֵ
	
//OLED_ShowNum(0,0,rotateSpeed,8,16);
//OLED_ShowString(70,0,"r/s");
	
//OLED_ShowString(5,3,"Speed: ");
	OLED_ShowString(0,1,"hope");
	OLED_ShowNum(32,1,SetPoint,8,16);
	OLED_ShowString(97,1,"cm/s");
	
	OLED_ShowString(0,4,"now:");
	OLED_ShowNum(32,4,Speed,8,16);
	OLED_ShowString(97,4,"cm/s");

}

//��ʱ��4�жϷ�����
void TIM4_IRQHandler(void)
{ 		    		  			    
	if(TIM4->SR&0X0001)                                   //����ж�
	{    				   				     	    	
	}				   
	TIM4->SR&=~(1<<0);                                    //����жϱ�־λ 	    
}

