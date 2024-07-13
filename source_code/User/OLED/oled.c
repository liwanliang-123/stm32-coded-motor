#include "stm32f10x.h"
#include "oled.h"
#include "SysTick.h"

void I2C_configuration(void)
{
	I2C_InitTypeDef  I2C_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

	//PB6 - SCL PB7 -- SDA
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_OD;  //复用输出
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6 | GPIO_Pin_7 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	I2C_DeInit(I2C1);
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 400000;  //400k bps/s
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_OwnAddress1 = 0x30;   //主地址
	I2C_Init(I2C1, &I2C_InitStructure);
	I2C_Cmd(I2C1, ENABLE);

}

void I2C_WriteByte(uint8_t addr,uint8_t data)
{
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));  // 检测I2C总线是否繁忙
	
	I2C_GenerateSTART(I2C1, ENABLE);               //开启I2C总线
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)); //EV5，主模式

	I2C_Send7bitAddress(I2C1, OLED_ADDRESS, I2C_Direction_Transmitter);  //发送器件地址
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	
	I2C_SendData(I2C1, addr);   //发送寄存器地址
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING));

	I2C_SendData(I2C1, data);   //发送数据
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING));

	I2C_GenerateSTOP(I2C1, ENABLE);   //关闭I2C总线

}

//写命令
void WriteCmd(unsigned char I2C_Command)
{

	I2C_WriteByte(0x00,I2C_Command);
	
}

//写数据
void WriteDat(unsigned char I2C_Data)
{

	I2C_WriteByte(0x40,I2C_Data);
	
}

