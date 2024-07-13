#ifndef _OLED_
#define _OLED_

#include "stm32f10x.h"

#define OLED_SCLK_Set()  GPIO_SetBits(GPIOB, GPIO_Pin_0)  // PB0(SCL)����ߵ�ƽ
#define OLED_SCLK_CLR()  GPIO_ResetBits(GPIOB, GPIO_Pin_0) //PB0(SCL)����͵�ƽ
#define OLED_SDIN_Set()  GPIO_SetBits(GPIOB, GPIO_Pin_1)  //PB1(SDA)����ߵ�ƽ
#define OLED_SDIN_CLR()  GPIO_ResetBits(GPIOB, GPIO_Pin_1)  //PB1(SDA)����͵�ƽ
#define OLED_READ_SDIN() GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) //��ȡPB1(SDA)��ƽ
#define OLED_CMD 0  //д����
#define OLED_DATA 1  //д����
#define SIZE     16   //��ʾ�ַ���С
#define Max_Column  128  //�������
#define IIC_ACK    0   //Ӧ��
#define IIC_NO_ACK 1   //��Ӧ��


static void delay_u(unsigned char num);
static void delay_m(unsigned char ms);
static void OLED_IIC_Start(void);
static void OLED_IIC_Stop(void);
static unsigned char IIC_Wait_Ack(void);
static void Write_IIC_Byte(unsigned char IIC_Byte);
static void Write_IIC_Command(unsigned char IIC_Command);
static void Write_IIC_Data(unsigned char IIC_Data);
void OLED_WR_Byte(unsigned char dat,unsigned char cmd);
void OLED_Set_Pos(unsigned char x,unsigned char y);
void OLED_Display_on(void);
void OLED_Display_off(void);
void OLED_clear(void);
void OLED_ShouChar(unsigned char x,unsigned char y,unsigned char chr);
void OLED_Init(void);
void OLED_ShowString(unsigned char x, unsigned char y,unsigned char *chr);
unsigned int oled_pow(unsigned char m,unsigned char n);
void OLED_ShowNum(unsigned char x,unsigned char y,unsigned int num,unsigned char len,unsigned char size);
void OLED_ShowCHinese(unsigned char x,unsigned char y,unsigned char no);


#endif


