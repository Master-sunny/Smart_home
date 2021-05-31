#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "dht11.h"
#include "adc.h"
#include "led.h"
#include "oled.h"
#include "timer.h"
#include "stdio.h"

//u8 Res;
int main()
{
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	 uart_init(9600);
	 delay_init();	    //��ʱ������ʼ��	  
	 LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�
	 TIM3Init(499,7199);
	 Adc_Init();
	 OLED_Init();
	 DHT11_Init();
	
	OLED_Clear();
	OLED_ON();
	OLED_Position(12,4);
	OLED_Send(1,1);
	OLED_ShowStr(0,0,"Temp:        C  ",16);
	OLED_ShowStr(0,2,"Humi:        %  ",16);
	OLED_ShowStr(0,4," CO :        P  ",16);
	OLED_ShowStr(0,6," CO2:        P  ",16);
	 while(1)
	 {

	 }
 

}

