#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"
#include "stm32f10x.h"
#include "delay.h"


void OLED_IIC()
{
	GPIO_InitTypeDef GPIO_InitTypestr;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypestr.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitTypestr.GPIO_Pin=GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitTypestr.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitTypestr);
	GPIO_SetBits(GPIOB,GPIO_Pin_15|GPIO_Pin_14);
}
void IIC_Start()
{
	SDA_OUT();
	SCL=1;
	SDAO=1;
	//delay_us(2);
	SDAO=0;
	//delay_us(2);
	SCL=0;
}

void IIC_Stop()
{
	SDA_OUT();
	SCL=0;
	SDAO=0;
	//delay_us(2);
	SCL=1;
	SDAO=1;
	//delay_ms(2);
}

void Wait_Ack(void)
{/*
	u8 ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
	SDAI=1;delay_ms(2);	 //SDAI���ߣ�ֱ��Ӧ����ְ�������  
	SCL=1;delay_ms(2);//SCL�����ͷ����ߵȴ�Ӧ��λ����	 
	while(SDAI)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;//Ӧ��λδ����ʱ�䳬�����趨ֵ�ͷ���1
		}
	}
	SCL=0;//ʱ�����0 	   
	return 0;//����Ӧ��λ�ͷ���0 */
	SCL=1;
	SCL=0;
} 
/*����ACKӦ��
void IIC_Ack(void)
{
	SCL=0;//��SCLΪ��ʱ��SDAΪ��ʱ�Ͳ���Ӧ��λ
	SDA_OUT();
	SDAO=0;
	delay_us(2);
	SCL=1;
	delay_us(2);
	SCL=0;
}
//������ACKӦ��		    
void IIC_NAck(void)
{
	SCL=0;//��SCLΪ��ʱSDAΪ���򲻲���Ӧ��λ
	SDA_OUT();
	SDAO=1;
	delay_us(2);
	SCL=1;
	delay_us(2);
	SCL=0;
}	
*/
//�Ӹ�λ���
void IIC_Send(unsigned char data)
{
	unsigned char m;
	u8 t;
	
	//SDA_OUT();
	SCL=0;//scl������
	for(t=0;t<8;t++)
	{
		m=data;
		m&=0x80;
		if(m==0x80)
			SDAO=1;//׼������
		else
			SDAO=0;
		data<<=1;
		SCL=1;//delay_us(2);//SCL�����ط�������
		SCL=0;//delay_us(2);//�����͵ȴ���һ������
		
	}
}
/*
u8 IIC_Read(u8 ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
  for(i=0;i<8;i++)
	{
    SCL=0; //������
    delay_us(2);
		SCL=1;//�����߶�ȡ����
    receive<<=1;
    if(SDAI)receive++; //��ȡ���ݱ��浽receive�� 
		delay_us(2); 
  }					 
  if (!ack)//�Ƿ���Ӧ��λ
		IIC_NAck();//����nACK
  else
    IIC_Ack(); //����ACK   
  return receive;
}
*/
void IIC_CMD(u8 cmd)
{
	IIC_Start();
	IIC_Send(0x78);
	Wait_Ack();
	IIC_Send(0x00);
	Wait_Ack();
	IIC_Send(cmd);
	Wait_Ack();
	IIC_Stop();
}

void IIC_DATA(u8 data)
{
	IIC_Start();
	IIC_Send(0x78);
	Wait_Ack();
	IIC_Send(0x40);
	Wait_Ack();
	IIC_Send(data);
	Wait_Ack();
	IIC_Stop();
}

void OLED_Send(u8 data,u8 i)
{
	if(i==1)//��������
		IIC_DATA(data);
	else if(i==0)//��������
		IIC_CMD(data);
}

void OLED_Position(unsigned char x,unsigned char y)
{
	OLED_Send(0xb0+y,CMD);
	OLED_Send(((x&0xf0)>>4)|0x10,CMD);
	OLED_Send(x&0x0f,CMD);
}
void OLED_DisplayON()
{
	OLED_Send(0x8D,CMD);
	OLED_Send(0x14,CMD);
	OLED_Send(0xAF,CMD);;
}

void OLED_DisplayOFF()
{
	OLED_Send(0x8D,CMD);
	OLED_Send(0x10,CMD);
	OLED_Send(0xAE,CMD);;
}
void OLED_ON()
{
	u8 i,j;
	for(i=0;i<8;i++)
	{
		OLED_Send(0xb0+i,CMD);
		OLED_Send(0x00,CMD);
		OLED_Send(0x10,CMD);
		for(j=0;j<128;j++)
			OLED_Send(1,DATA );
	}
}
void OLED_Clear()
{
	u8 i,j;
	for(i=0;i<8;i++)
	{
		OLED_Send(0xb0+i,CMD);
		OLED_Send(0x00,CMD);
		OLED_Send(0x10,CMD);
		for(j=0;j<128;j++)
			OLED_Send(0,DATA );
	}
}
//ÿһҳ���У�16*8��Ҫ��ҳ
void OLED_WriteChar(u8 x,u8 y,u8 charter,u8 mode)
{
	unsigned char a=0,i,j;
	a=charter-' ';
	if(x>127){x=0;y+=2;}
	if(mode==16)//16*8 ��һ���ַ�ռ��ҳ
	{
		OLED_Position(x,y);//��һҳд��ǰ����
		for(i=0;i<8;i++)//
			OLED_Send(F8X16[a*16+i],DATA);
		OLED_Position(x,y+1);//�ڶ�ҳд������
		for(j=0;j<8;j++)
			OLED_Send(F8X16[a*16+j+8],DATA);
	}
}

int OLED_Pow(u8 x,u8 n)
{
	int result=1;
	while(n--)
		result*=x;
	return result;
}
void OLED_WriteNUM(u8 x,u8 y,int num,u8 len,u8 mode)
{
	u8 i,temp;
	u8 enshow=0;
	for(i=0;i<len;i++)
	{
		temp=(num/OLED_Pow(10,len-i-1))%10;
		if(enshow==0&&i<(len-1))
		{
			if(temp==0)
			{	
				OLED_WriteChar(x+(mode/2)*i,y,' ',mode);
				continue;
			}
			else
				enshow=1;
		}
		OLED_WriteChar(x+(mode/2)*i,y,temp+'0',mode);
		
	}
	
}
void OLED_ShowStr(u8 x,u8 y,char *chr,u8 mode)
{
	unsigned char j=0;
	while (*chr!='\0')
	{		OLED_WriteChar(x,y,chr[j],mode);
			x+=8;
		if(x>120){x=0;y+=2;}
			chr++;
	}
}

void OLED_WriteChinese(u8 x,u8 y,u8 z,u8 mode)
{
	unsigned char i,j;
	if(x>127){x=0;y+=2;}
	if(mode==16)//16*8 ��һ���ַ�ռ��ҳ
	{
		OLED_Position(x,y);//��һҳд��ǰ����
		for(i=0;i<8;i++)//
			OLED_Send(Hzk[z*16+i],DATA);
		OLED_Position(x,y+1);//�ڶ�ҳд������
		for(j=0;j<8;j++)
			OLED_Send(Hzk[z*16+j+8],DATA);
	}
}

void OLED_Init()
{
	OLED_IIC();
	delay_ms(800);

	OLED_Send(0xAE,CMD);//--display off
	OLED_Send(0x00,CMD);//---set low column address
	OLED_Send(0x10,CMD);//---set high column address
	OLED_Send(0x40,CMD);//--set start line address  
	OLED_Send(0xB0,CMD);//--set page address
	OLED_Send(0x81,CMD); // contract control
	OLED_Send(0xFF,CMD);//--128   
	OLED_Send(0xA1,CMD);//set segment remap 
	OLED_Send(0xA6,CMD);//--normal / reverse
	OLED_Send(0xA8,CMD);//--set multiplex ratio(1 to 64)
	OLED_Send(0x3F,CMD);//--1/32 duty
	OLED_Send(0xC8,CMD);//Com scan direction
	OLED_Send(0xD3,CMD);//-set display offset
	OLED_Send(0x00,CMD);//
	
	OLED_Send(0xD5,CMD);//set osc division
	OLED_Send(0x80,CMD);//
	
	OLED_Send(0xD8,CMD);//set area color mode off
	OLED_Send(0x05,CMD);//
	
	OLED_Send(0xD9,CMD);//Set Pre-Charge Period
	OLED_Send(0xF1,CMD);//
	
	OLED_Send(0xDA,CMD);//set com pin configuartion
	OLED_Send(0x12,CMD);//
	
	OLED_Send(0xDB,CMD);//set Vcomh
	OLED_Send(0x30,CMD);//
	
	OLED_Send(0x8D,CMD);//set charge pump enable
	OLED_Send(0x14,CMD);//
	
	OLED_Send(0xAF,CMD);//--turn on oled panel
}