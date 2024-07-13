#include "stm32f10x.h"
#include "GPIO_IICOLED.h"
#include "oledfont.h"

static void delay_u(unsigned char num)
{
	uint8_t i = 10;
	while(num--){
		while(i--);
	}
}

static void delay_m(unsigned char ms)
{
	unsigned int x,y;
	for(x = ms;x > 0;x--){
		for(y = 12000;y > 0;y--);
	}
}

void GPIO_IicoledInit(void)
{
	GPIO_InitTypeDef GPIO_Initstructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	
	GPIO_Initstructure.GPIO_Mode  = GPIO_Mode_Out_OD;
	GPIO_Initstructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_Initstructure);

	OLED_SCLK_Set();
	OLED_SDIN_Set();
	
}

//模拟IIC起始信号
static void OLED_IIC_Start(void)
{
	OLED_SDIN_Set();  //数据总线高电平
	OLED_SCLK_Set();  //时钟总线高电平
	delay_u(1);

	OLED_SDIN_CLR();  //数据总线有一个负跳变
	delay_u(1);
	OLED_SCLK_CLR();
	delay_u(1);
}	

//模拟IIC停止信号
static void OLED_IIC_Stop(void)
{
	OLED_SDIN_CLR();   //数据总线低电平
	delay_u(1);
	OLED_SCLK_Set();  //时钟总线高电平
	delay_u(1);
	OLED_SDIN_Set();  //数据总线高电平  数据线正跳变 
	delay_u(1);
	
}

//模拟IIC读取从机应答信号
static unsigned char IIC_Wait_Ack(void)
{
	unsigned char ack;

	OLED_SCLK_CLR();      //时钟线低电平
	delay_u(1);
	OLED_SDIN_Set();      //数据总线高电平
	delay_u(1);
	OLED_SCLK_Set();      //时钟线高电平
	delay_u(1);
	if(OLED_READ_SDIN()){  //PB1引脚有高电平
	
		ack = IIC_NO_ACK;
	}else{
	
		ack = IIC_ACK;
	}
	OLED_SCLK_CLR();       //时钟线清零
	delay_u(1);

	return ack;
}

//IIC读取一个Byte
static void Write_IIC_Byte(unsigned char IIC_Byte)
{
	unsigned char i;
	for(i = 0;i < 8;i++){
	
		OLED_SCLK_CLR();   //时钟线低电平
		delay_u(1);
		if(IIC_Byte & 0x80){    //读取最高位
			
			OLED_SDIN_Set();   //最高位为1
		}else{
		
			OLED_SDIN_CLR();   //最高位为0
		}
		
		IIC_Byte <<= 1 ;      //数据左移一位
		delay_u(1);
		OLED_SCLK_Set();       //时钟线置位，产生上升沿，把一位数据发送出去
		delay_u(1);
	}
	OLED_SCLK_CLR();       //时钟线低电平
	delay_u(1);
	
	while(IIC_Wait_Ack());     //等待从机应答
}

//IIC写命令
static void Write_IIC_Command(unsigned char IIC_Command)
{
	OLED_IIC_Start() ;
	Write_IIC_Byte(0x78);  //写入从机地址
	Write_IIC_Byte(0x00);  //写入命令
	Write_IIC_Byte(IIC_Command);  //命令
	OLED_IIC_Stop();        //发送停止信号
}

//IIC写数据
static void Write_IIC_Data(unsigned char IIC_Data)
{
	OLED_IIC_Start() ;
	Write_IIC_Byte(0x78);  //写入从机地址
	Write_IIC_Byte(0x40);  //写入数据
	Write_IIC_Byte(IIC_Data);  //数据
	OLED_IIC_Stop();       //发送停止信号
}

//对OLED写入一个字节
void OLED_WR_Byte(unsigned char dat,unsigned char cmd)
{
	if(cmd){
	
		Write_IIC_Data(dat);  //写入数据
	}else{
	
		Write_IIC_Command(dat);  //写入地址
	}
	
}

//设置数据写入的起始行和列
//x : 列的起始低地址与列的高地址
//y : 页  起始页的地址
void OLED_Set_Pos(unsigned char x,unsigned char y)
{
	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte((x&0x0f),OLED_CMD);
	OLED_WR_Byte(((x&0xf0) >> 4) | 0x10,OLED_CMD);
}

//开显示
void OLED_Display_on(void)
{
	OLED_WR_Byte(0x8D,OLED_CMD); //设置OLED电荷泵
	OLED_WR_Byte(0x14,OLED_CMD); //使能
	OLED_WR_Byte(0xAF,OLED_CMD); //开显示
}

//关显示
void OLED_Display_off(void)
{
	OLED_WR_Byte(0x8D,OLED_CMD); //设置OLED电荷泵
	OLED_WR_Byte(0x10,OLED_CMD); //失能
	OLED_WR_Byte(0xAE,OLED_CMD); //关显示
}

//清屏幕
void OLED_clear(void)
{
	unsigned char i,n;
	for(i = 0;i < 8;i++){
		OLED_WR_Byte(0xb0 + i,OLED_CMD); //从0 —— 7页依次写入
		OLED_WR_Byte(0x00,OLED_CMD); //列低地址
		OLED_WR_Byte(0x10,OLED_CMD); //列高地址
		for(n = 0;n < 128;n++){
		
			OLED_WR_Byte(0,OLED_DATA); //0 清屏幕
		}
	}
}

void OLED_ShouChar(unsigned char x,unsigned char y,unsigned char chr)
{
	unsigned char c = 0,i = 0;

	c = chr - ' ';  //获取字符偏移量
	if(x > Max_Column){
	
		x = 0;    //如果列出了范围，就从下两页的第0行开始
		y += 2;
	}
	if(SIZE == 16){  //字符大小为16 = 8 * 16
	
		OLED_Set_Pos(x,y); //从x,y开始画点
		for(i = 0;i < 8;i++)
		OLED_WR_Byte(F8X16[c*16+i],OLED_DATA); //找出字符c的数组位数，先把第一列画完
		
		OLED_Set_Pos(x,y + 1);  //从x,y开始画点
		for(i = 0;i < 8;i++)
		OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA); //找出字符c的数组位数，先把第一列画完
		
	}
	else{  //6 = 6 * 8
	
		OLED_Set_Pos(x,y); 
		for(i = 0;i < 6;i++)  //从x,y开始画点
		OLED_WR_Byte(F6x8[c][i] , OLED_DATA);  //找出字符c的数组位数，先把第一列画完
		
		
	}
}

void OLED_Init(void)
{
	
	delay_m(200);	//延迟，由于单片机上电初始化比OLED快，所以必须加上延迟，等待OLED上复位完成

	GPIO_IicoledInit();	//GPIO口初始化
	
	OLED_WR_Byte(0xAE,OLED_CMD);	//关闭显示

	OLED_WR_Byte(0x00,OLED_CMD);	//设置低列地址
	OLED_WR_Byte(0x10,OLED_CMD);	//设置高列地址
	OLED_WR_Byte(0x40,OLED_CMD);	//设置起始行地址
	OLED_WR_Byte(0xB0,OLED_CMD);	//设置页地址

	OLED_WR_Byte(0x81,OLED_CMD); 	// 对比度设置，可设置亮度
	OLED_WR_Byte(0xFF,OLED_CMD);	//  265  

	OLED_WR_Byte(0xA1,OLED_CMD);	//设置段（SEG）的起始映射地址；column的127地址是SEG0的地址
	OLED_WR_Byte(0xA6,OLED_CMD);	//正常显示；0xa7逆显示

	OLED_WR_Byte(0xA8,OLED_CMD);	//设置驱动路数（16~64）
	OLED_WR_Byte(0x3F,OLED_CMD);	//64duty
	
	OLED_WR_Byte(0xC8,OLED_CMD);	//重映射模式，COM[N-1]~COM0扫描

	OLED_WR_Byte(0xD3,OLED_CMD);	//设置显示偏移
	OLED_WR_Byte(0x00,OLED_CMD);	//无偏移
	
	OLED_WR_Byte(0xD5,OLED_CMD);	//设置震荡器分频
	OLED_WR_Byte(0x80,OLED_CMD);	//使用默认值
	
	OLED_WR_Byte(0xD9,OLED_CMD);	//设置 Pre-Charge Period
	OLED_WR_Byte(0xF1,OLED_CMD);	//使用官方推荐值
	
	OLED_WR_Byte(0xDA,OLED_CMD);	//设置 com pin configuartion
	OLED_WR_Byte(0x12,OLED_CMD);	//使用默认值
	
	OLED_WR_Byte(0xDB,OLED_CMD);	//设置 Vcomh，可调节亮度（默认）
	OLED_WR_Byte(0x40,OLED_CMD);	////使用官方推荐值
	
	OLED_WR_Byte(0x8D,OLED_CMD);	//设置OLED电荷泵
	OLED_WR_Byte(0x14,OLED_CMD);	//开显示
	
	OLED_WR_Byte(0xAF,OLED_CMD);	//开启OLED面板显示
	OLED_clear();        //清屏
	OLED_Set_Pos(0,0); 	 //设置数据写入的起始行、列
}  


//显示字符串
void OLED_ShowString(unsigned char x, unsigned char y,unsigned char *chr)
{
   unsigned char j=0;
	 while(chr[j] != '\0') //判断是不是最后一个字符
	 {
	    
    OLED_ShouChar(x,y,chr[j]);//显示字符
		 x+=8;  //列数+8，一个字符占 8
		 if(x>=128)
		 {
		     x= 0 ;
			 y = y + 2;
		    
		 }
		 j++;
	 }
}

//计算m^n次方，result 一个数的n次方
unsigned int oled_pow(unsigned char m,unsigned char n)
{
	unsigned int result=1;	 
	while(n--)result*=m;    
	return result;
}

void OLED_ShowNum(unsigned char x,unsigned char y,unsigned int num,unsigned char len,unsigned char size)
{
	//size:字的艰巨间距 num :显示的数字       	
	unsigned char t,temp;  //定义变量
	unsigned char enshow=0;		//定义变量

	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;//取出输入数的每个位，由高到低
		if(enshow==0&&t<(len-1)) //enshow：是否为第一个数；t<(len-1)：判断是否为最后一个数
		{
			if(temp==0) //如果该数为0 
			{
				OLED_ShouChar(x+(size/2)*t,y,' ');//显示 0 ；x+(size2/2)*t根据字体大小偏移的列数（8）
				continue; //跳过剩下语句，继续重复循环（避免重复显示）
			}else enshow=1; 
		}
	 	OLED_ShouChar(x+(size/2)*t,y,temp+'0'); //显示一个位；x+(size2/2)*t根据字体大小偏移的列数（8）
	}
} 


//显示文字
void OLED_ShowCHinese(unsigned char x,unsigned char y,unsigned char no) //no:第几个字符
{
    unsigned char t,adder = 0;
	OLED_Set_Pos(x,y);
	for(t=0;t<16;t++)
	{ 
		 OLED_WR_Byte( Hzk[2*no][t],OLED_DATA);//画 no在数组位置的第一页16列的点
		 //OLED_WR_Byte( LWL[2*no][t],OLED_DATA);//画 no在数组位置的第一页16列的点
		 adder+=1; //数组地址+1
	}
		  OLED_Set_Pos(x,y+1);
	for(t=0;t<16;t++)
	{
		OLED_WR_Byte( Hzk[2*no+1][t],OLED_DATA);//画 no在数组位置的第2页16列的点
		//OLED_WR_Byte( LWL[2*no+1][t],OLED_DATA);
		adder+=1; //数组地址+1

	}
}


