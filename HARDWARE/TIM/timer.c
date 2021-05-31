#include "timer.h"
#include "stm32f10x_it.h"
#include "delay.h"
#include "sys.h"
#include "led.h"
#include "dht11.h"
#include "adc.h"
#include "oled.h"
extern u8 temp;
extern u8 humi;
int a;
u8 temperature,humidity;
float  co,co2,pm25;
 int CO,CO2,PM25,hongwai;
char dht[]="0001:01,02,000,000,000,0,1\n";
int count;	 
void TIM3Init(u32 arr,u32 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitTypestr;
	NVIC_InitTypeDef NVIC_InitTypestr;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	TIM_TimeBaseInitTypestr.TIM_Period=arr;
	TIM_TimeBaseInitTypestr.TIM_Prescaler=psc;
	TIM_TimeBaseInitTypestr.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitTypestr.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitTypestr.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitTypestr);
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);
	
	NVIC_InitTypestr.NVIC_IRQChannel=TIM3_IRQn;
	NVIC_InitTypestr.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitTypestr.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitTypestr.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitTypestr);
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM3,ENABLE);
}

void TIM3_IRQHandler()
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=0)
	{
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
					
		DHT11_Read_Data(&temperature,&humidity);
		OLED_WriteNUM(64,0,temperature,2,16);
		OLED_WriteNUM(64,2,humidity,2,16);
		co=Get_Adc(ADC_Channel_0);
		CO=100*(co*(3.3/4096));
		OLED_WriteNUM(64,4,CO,3,16);
		co2=Get_Adc(ADC_Channel_2);
		CO2=100*(co2*(3.3/4096));
		OLED_WriteNUM(64,6,CO2,3,16);
		if(CO2>100||CO>100)
			LED1=0;
		else
			LED1=1;

	}
}

