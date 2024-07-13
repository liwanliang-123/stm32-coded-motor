#ifndef __IO_H
#define __IO_H

#include "sys.h"

#define XRR GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)//”“”“—≠º£
#define XR GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1)//”“—≠º£

#define XL GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)//◊Û—≠º£
#define XLL GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_5)//◊Û◊Û—≠º£

void IO_Init(void);//≥ı ºªØ

		 				    
#endif
