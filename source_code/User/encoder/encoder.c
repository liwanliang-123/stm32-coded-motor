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
int para_L,para_R;                                    //增量
int Encoder_Left,Encoder_Right;
static int SetPoint=150;                              //设置目标速度
int Moto_Left = 0,Moto_Right; 
//static int number = 0;

/**
* @brief TIM4 通道1通道2 正交编码器
*/
void TIM4_ENCODER_Init(void)                      
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStruct;
	TIM_ICInitTypeDef TIM_ICInitStruct;   	                  //输入捕获

	                                                          //PB6 ch1  A,PB7 ch2 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);      //使能TIM4时钟	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);     //使能GPIOA时钟
	
	GPIO_StructInit(&GPIO_InitStructure);                     //将GPIO_InitStruct中的参数按缺省值输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;     //PA6 PA7浮空输入	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);    
	

																														/*时基初始化*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);      /*使能定时器时钟 APB1*/
	TIM_DeInit(TIM4);                          
	//TIM_TimeBaseStructInit(&TIM_TimeBaseStruct);    
	TIM_TimeBaseStruct.TIM_Prescaler = ENCODER_TIM_PSC;       /*预分频 0*/        
	TIM_TimeBaseStruct.TIM_Period = ENCODER_TIM_PERIOD;       /*周期(重装载值)65535*/
	TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;      
	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;  /*连续向上计数模式*/  
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStruct); 

	/*编码器模式配置：同时捕获通道1与通道2(即4倍频)，极性均为Rising*/
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12,TIM_ICPolarity_Rising, TIM_ICPolarity_Rising); 
	TIM_ICStructInit(&TIM_ICInitStruct);        
	TIM_ICInitStruct.TIM_ICFilter = 0;                       /*输入通道的滤波参数*/
	TIM_ICInit(TIM4, &TIM_ICInitStruct);                     /*输入通道初始化*/
	TIM_SetCounter(TIM4, CNT_INIT);                          /*CNT设初值  0 */
	TIM_ClearFlag(TIM4,TIM_IT_Update);                       /*中断标志清0*/
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);               /*中断使能*/
	TIM_Cmd(TIM4,ENABLE);                                    /*使能CR寄存器*/
}


// 读取定时器计数值
int read_encoder(void)
{
	int encoderNum = 0;
	encoderNum = (int)((int16_t)(TIM4->CNT));                /*CNT为uint32, 转为int16*/
	TIM_SetCounter(TIM4, CNT_INIT);                          /*CNT设初值*/

	return encoderNum;
}

//计算电机转速（被另一个定时器100ms调用1次）
void calc_motor_rotate_speed()
{
	
	/*读取编码器的值，正负代表旋转方向*/
	encoderNum = read_encoder();

	/* 转速(1秒钟转多少圈)=单位时间内的计数值/总分辨率*时间系数 */
	rotateSpeed = (int)encoderNum/TOTAL_RESOLUTION*10;
	
//	Speed = (rotateSpeed * 6.0 * 3.14)/1.0;       //速度   1秒钟转的圈数 X 一圈的距离(轮子周长)/1s  

	Speed = ((encoderNum/(48.4*4)) * 18.84)/0.1;    //(（总的脉冲数/电机一圈的脉冲*减数比*4倍频)*轮子周长)/100ms 
	
	para_L = PID_Calc(Speed,SetPoint);	            //，计数得到增量式PID的增量数值 
	
	if((para_L < -3) || (para_L > 3)) 						  // 不做 PID 调整，避免误差较小时频繁调节引起震荡。
	{                                               //大于10才调整
		Moto_Left += para_L;  
	}   
	
	if(Moto_Left > 19000)Moto_Left = 19000;         //限幅
	
	TIM_SetCompare1(TIM3, Moto_Left);
	TIM_SetCompare2(TIM3, 0);
	
//OLED_ShowNum(0,0,para_L,8,16); 显示加了多少占空比，pid调节出来的值
	
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

//定时器4中断服务函数
void TIM4_IRQHandler(void)
{ 		    		  			    
	if(TIM4->SR&0X0001)                                   //溢出中断
	{    				   				     	    	
	}				   
	TIM4->SR&=~(1<<0);                                    //清除中断标志位 	    
}

