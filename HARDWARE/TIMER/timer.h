#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//WKS STM32F407VET6核心板
//定时器中断 驱动代码	   
//版本：V1.0									  
////////////////////////////////////////////////////////////////////////////////// 	
extern TIM_HandleTypeDef TIM3_Handler;      //定时器句柄 

void TIM3_Init(u16 arr,u16 psc);
void TIM4_PWM_Init(u16 arr,u16 psc);
void TIM_SetTIM4Compare1(u32 compare);

#endif

