#ifndef  _OLED_
#define _OLED_

#include "sys.h"

#define CMD 0
#define DATA 1
#define SDA_IN()  {GPIOB->CRH&=0XF0FFFFFF;GPIOB->CRH|=8<<24;}
#define SDA_OUT() {GPIOB->CRH&=0XF0FFFFFF;GPIOB->CRH|=3<<24;}
#define SCL PBout(15)
#define SDAO PBout(14)
#define SDAI PBin(14)

void OLED_Init(void);
void IIC_Start(void);

#endif