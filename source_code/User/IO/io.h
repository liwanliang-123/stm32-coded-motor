#ifndef __IO_H
#define __IO_H

#include "sys.h"

#define XRR GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)//����ѭ��
#define XR GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1)//��ѭ��

#define XL GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)//��ѭ��
#define XLL GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_5)//����ѭ��

void IO_Init(void);//��ʼ��

		 				    
#endif
