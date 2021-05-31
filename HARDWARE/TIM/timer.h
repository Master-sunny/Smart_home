#ifndef TIMER_H
#define TIMER_H

#include "sys.h"

#define TRIG PBout(1)
#define ECHO PBout(0)

void TIM3Init(u32 arr,u32 psc);
float getDistance(void);

#endif
