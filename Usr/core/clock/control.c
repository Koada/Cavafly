#include "control.h"

void Clock_Control_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;
	TIM_TimeBaseInitStructure.TIM_Period = 10000 - 1;		  // 72 000 000      100Hz    1000 000 
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
 	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);
	TIM_ARRPreloadConfig(TIM4, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE); 
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	TIM_Cmd(TIM4, DISABLE);	
}

void Clock_Control_Start(void)
{
	TIM_Cmd(TIM4, ENABLE);
}

void TIM4_IRQHandler(void){		// 100Hz
	
	
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
}

