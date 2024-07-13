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

//ģ��IIC��ʼ�ź�
static void OLED_IIC_Start(void)
{
	OLED_SDIN_Set();  //�������߸ߵ�ƽ
	OLED_SCLK_Set();  //ʱ�����߸ߵ�ƽ
	delay_u(1);

	OLED_SDIN_CLR();  //����������һ��������
	delay_u(1);
	OLED_SCLK_CLR();
	delay_u(1);
}	

//ģ��IICֹͣ�ź�
static void OLED_IIC_Stop(void)
{
	OLED_SDIN_CLR();   //�������ߵ͵�ƽ
	delay_u(1);
	OLED_SCLK_Set();  //ʱ�����߸ߵ�ƽ
	delay_u(1);
	OLED_SDIN_Set();  //�������߸ߵ�ƽ  ������������ 
	delay_u(1);
	
}

//ģ��IIC��ȡ�ӻ�Ӧ���ź�
static unsigned char IIC_Wait_Ack(void)
{
	unsigned char ack;

	OLED_SCLK_CLR();      //ʱ���ߵ͵�ƽ
	delay_u(1);
	OLED_SDIN_Set();      //�������߸ߵ�ƽ
	delay_u(1);
	OLED_SCLK_Set();      //ʱ���߸ߵ�ƽ
	delay_u(1);
	if(OLED_READ_SDIN()){  //PB1�����иߵ�ƽ
	
		ack = IIC_NO_ACK;
	}else{
	
		ack = IIC_ACK;
	}
	OLED_SCLK_CLR();       //ʱ��������
	delay_u(1);

	return ack;
}

//IIC��ȡһ��Byte
static void Write_IIC_Byte(unsigned char IIC_Byte)
{
	unsigned char i;
	for(i = 0;i < 8;i++){
	
		OLED_SCLK_CLR();   //ʱ���ߵ͵�ƽ
		delay_u(1);
		if(IIC_Byte & 0x80){    //��ȡ���λ
			
			OLED_SDIN_Set();   //���λΪ1
		}else{
		
			OLED_SDIN_CLR();   //���λΪ0
		}
		
		IIC_Byte <<= 1 ;      //��������һλ
		delay_u(1);
		OLED_SCLK_Set();       //ʱ������λ�����������أ���һλ���ݷ��ͳ�ȥ
		delay_u(1);
	}
	OLED_SCLK_CLR();       //ʱ���ߵ͵�ƽ
	delay_u(1);
	
	while(IIC_Wait_Ack());     //�ȴ��ӻ�Ӧ��
}

//IICд����
static void Write_IIC_Command(unsigned char IIC_Command)
{
	OLED_IIC_Start() ;
	Write_IIC_Byte(0x78);  //д��ӻ���ַ
	Write_IIC_Byte(0x00);  //д������
	Write_IIC_Byte(IIC_Command);  //����
	OLED_IIC_Stop();        //����ֹͣ�ź�
}

//IICд����
static void Write_IIC_Data(unsigned char IIC_Data)
{
	OLED_IIC_Start() ;
	Write_IIC_Byte(0x78);  //д��ӻ���ַ
	Write_IIC_Byte(0x40);  //д������
	Write_IIC_Byte(IIC_Data);  //����
	OLED_IIC_Stop();       //����ֹͣ�ź�
}

//��OLEDд��һ���ֽ�
void OLED_WR_Byte(unsigned char dat,unsigned char cmd)
{
	if(cmd){
	
		Write_IIC_Data(dat);  //д������
	}else{
	
		Write_IIC_Command(dat);  //д���ַ
	}
	
}

//��������д�����ʼ�к���
//x : �е���ʼ�͵�ַ���еĸߵ�ַ
//y : ҳ  ��ʼҳ�ĵ�ַ
void OLED_Set_Pos(unsigned char x,unsigned char y)
{
	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte((x&0x0f),OLED_CMD);
	OLED_WR_Byte(((x&0xf0) >> 4) | 0x10,OLED_CMD);
}

//����ʾ
void OLED_Display_on(void)
{
	OLED_WR_Byte(0x8D,OLED_CMD); //����OLED��ɱ�
	OLED_WR_Byte(0x14,OLED_CMD); //ʹ��
	OLED_WR_Byte(0xAF,OLED_CMD); //����ʾ
}

//����ʾ
void OLED_Display_off(void)
{
	OLED_WR_Byte(0x8D,OLED_CMD); //����OLED��ɱ�
	OLED_WR_Byte(0x10,OLED_CMD); //ʧ��
	OLED_WR_Byte(0xAE,OLED_CMD); //����ʾ
}

//����Ļ
void OLED_clear(void)
{
	unsigned char i,n;
	for(i = 0;i < 8;i++){
		OLED_WR_Byte(0xb0 + i,OLED_CMD); //��0 ���� 7ҳ����д��
		OLED_WR_Byte(0x00,OLED_CMD); //�е͵�ַ
		OLED_WR_Byte(0x10,OLED_CMD); //�иߵ�ַ
		for(n = 0;n < 128;n++){
		
			OLED_WR_Byte(0,OLED_DATA); //0 ����Ļ
		}
	}
}

void OLED_ShouChar(unsigned char x,unsigned char y,unsigned char chr)
{
	unsigned char c = 0,i = 0;

	c = chr - ' ';  //��ȡ�ַ�ƫ����
	if(x > Max_Column){
	
		x = 0;    //����г��˷�Χ���ʹ�����ҳ�ĵ�0�п�ʼ
		y += 2;
	}
	if(SIZE == 16){  //�ַ���СΪ16 = 8 * 16
	
		OLED_Set_Pos(x,y); //��x,y��ʼ����
		for(i = 0;i < 8;i++)
		OLED_WR_Byte(F8X16[c*16+i],OLED_DATA); //�ҳ��ַ�c������λ�����Ȱѵ�һ�л���
		
		OLED_Set_Pos(x,y + 1);  //��x,y��ʼ����
		for(i = 0;i < 8;i++)
		OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA); //�ҳ��ַ�c������λ�����Ȱѵ�һ�л���
		
	}
	else{  //6 = 6 * 8
	
		OLED_Set_Pos(x,y); 
		for(i = 0;i < 6;i++)  //��x,y��ʼ����
		OLED_WR_Byte(F6x8[c][i] , OLED_DATA);  //�ҳ��ַ�c������λ�����Ȱѵ�һ�л���
		
		
	}
}

void OLED_Init(void)
{
	
	delay_m(200);	//�ӳ٣����ڵ�Ƭ���ϵ��ʼ����OLED�죬���Ա�������ӳ٣��ȴ�OLED�ϸ�λ���

	GPIO_IicoledInit();	//GPIO�ڳ�ʼ��
	
	OLED_WR_Byte(0xAE,OLED_CMD);	//�ر���ʾ

	OLED_WR_Byte(0x00,OLED_CMD);	//���õ��е�ַ
	OLED_WR_Byte(0x10,OLED_CMD);	//���ø��е�ַ
	OLED_WR_Byte(0x40,OLED_CMD);	//������ʼ�е�ַ
	OLED_WR_Byte(0xB0,OLED_CMD);	//����ҳ��ַ

	OLED_WR_Byte(0x81,OLED_CMD); 	// �Աȶ����ã�����������
	OLED_WR_Byte(0xFF,OLED_CMD);	//  265  

	OLED_WR_Byte(0xA1,OLED_CMD);	//���öΣ�SEG������ʼӳ���ַ��column��127��ַ��SEG0�ĵ�ַ
	OLED_WR_Byte(0xA6,OLED_CMD);	//������ʾ��0xa7����ʾ

	OLED_WR_Byte(0xA8,OLED_CMD);	//��������·����16~64��
	OLED_WR_Byte(0x3F,OLED_CMD);	//64duty
	
	OLED_WR_Byte(0xC8,OLED_CMD);	//��ӳ��ģʽ��COM[N-1]~COM0ɨ��

	OLED_WR_Byte(0xD3,OLED_CMD);	//������ʾƫ��
	OLED_WR_Byte(0x00,OLED_CMD);	//��ƫ��
	
	OLED_WR_Byte(0xD5,OLED_CMD);	//����������Ƶ
	OLED_WR_Byte(0x80,OLED_CMD);	//ʹ��Ĭ��ֵ
	
	OLED_WR_Byte(0xD9,OLED_CMD);	//���� Pre-Charge Period
	OLED_WR_Byte(0xF1,OLED_CMD);	//ʹ�ùٷ��Ƽ�ֵ
	
	OLED_WR_Byte(0xDA,OLED_CMD);	//���� com pin configuartion
	OLED_WR_Byte(0x12,OLED_CMD);	//ʹ��Ĭ��ֵ
	
	OLED_WR_Byte(0xDB,OLED_CMD);	//���� Vcomh���ɵ������ȣ�Ĭ�ϣ�
	OLED_WR_Byte(0x40,OLED_CMD);	////ʹ�ùٷ��Ƽ�ֵ
	
	OLED_WR_Byte(0x8D,OLED_CMD);	//����OLED��ɱ�
	OLED_WR_Byte(0x14,OLED_CMD);	//����ʾ
	
	OLED_WR_Byte(0xAF,OLED_CMD);	//����OLED�����ʾ
	OLED_clear();        //����
	OLED_Set_Pos(0,0); 	 //��������д�����ʼ�С���
}  


//��ʾ�ַ���
void OLED_ShowString(unsigned char x, unsigned char y,unsigned char *chr)
{
   unsigned char j=0;
	 while(chr[j] != '\0') //�ж��ǲ������һ���ַ�
	 {
	    
    OLED_ShouChar(x,y,chr[j]);//��ʾ�ַ�
		 x+=8;  //����+8��һ���ַ�ռ 8
		 if(x>=128)
		 {
		     x= 0 ;
			 y = y + 2;
		    
		 }
		 j++;
	 }
}

//����m^n�η���result һ������n�η�
unsigned int oled_pow(unsigned char m,unsigned char n)
{
	unsigned int result=1;	 
	while(n--)result*=m;    
	return result;
}

void OLED_ShowNum(unsigned char x,unsigned char y,unsigned int num,unsigned char len,unsigned char size)
{
	//size:�ֵļ�޼�� num :��ʾ������       	
	unsigned char t,temp;  //�������
	unsigned char enshow=0;		//�������

	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;//ȡ����������ÿ��λ���ɸߵ���
		if(enshow==0&&t<(len-1)) //enshow���Ƿ�Ϊ��һ������t<(len-1)���ж��Ƿ�Ϊ���һ����
		{
			if(temp==0) //�������Ϊ0 
			{
				OLED_ShouChar(x+(size/2)*t,y,' ');//��ʾ 0 ��x+(size2/2)*t���������Сƫ�Ƶ�������8��
				continue; //����ʣ����䣬�����ظ�ѭ���������ظ���ʾ��
			}else enshow=1; 
		}
	 	OLED_ShouChar(x+(size/2)*t,y,temp+'0'); //��ʾһ��λ��x+(size2/2)*t���������Сƫ�Ƶ�������8��
	}
} 


//��ʾ����
void OLED_ShowCHinese(unsigned char x,unsigned char y,unsigned char no) //no:�ڼ����ַ�
{
    unsigned char t,adder = 0;
	OLED_Set_Pos(x,y);
	for(t=0;t<16;t++)
	{ 
		 OLED_WR_Byte( Hzk[2*no][t],OLED_DATA);//�� no������λ�õĵ�һҳ16�еĵ�
		 //OLED_WR_Byte( LWL[2*no][t],OLED_DATA);//�� no������λ�õĵ�һҳ16�еĵ�
		 adder+=1; //�����ַ+1
	}
		  OLED_Set_Pos(x,y+1);
	for(t=0;t<16;t++)
	{
		OLED_WR_Byte( Hzk[2*no+1][t],OLED_DATA);//�� no������λ�õĵ�2ҳ16�еĵ�
		//OLED_WR_Byte( LWL[2*no+1][t],OLED_DATA);
		adder+=1; //�����ַ+1

	}
}


