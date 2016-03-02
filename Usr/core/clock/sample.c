#include "sample.h"
#include "globals.h"
#include "PID.h"
// debug
//#include "show_char.h"
void Clock_Sample_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;
	TIM_TimeBaseInitStructure.TIM_Period = 2000 - 1;		  // 72 000 000      500Hz    1000 000 
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
 	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	TIM_ARRPreloadConfig(TIM2, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); 
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	TIM_Cmd(TIM2, DISABLE);
}

static Eular FILTERED;
static Triaxial_f GYRO2;
void Clock_Sample_Start(void)
{
	FILTERED.pitch = FILTERED.roll = FILTERED.yaw = .0;
	GYRO2.x = GYRO2.y = GYRO2.z = .0;
	TIM_Cmd(TIM2, ENABLE);
}


void TIM2_IRQHandler(void){		// 500Hz
	
	static uint8_t inside_cnt = 0;
	static float ROLL = 0.0, PITCH = 0.0, YAW;
	static float MOTO1_PWM = 0.0, MOTO2_PWM = 0.0, MOTO3_PWM = 0.0, MOTO4_PWM = 0.0;
	
	/*** Angles update 500Hz ***********************************/
	ACC_Gain();
	GYRO_Gain();
			GYRO2.x = GYRO.x * (2000.0 / 32768);
			GYRO2.y = GYRO.y * (2000.0 / 32768);
			GYRO2.z = GYRO.z * (2000.0 / 32768);
	ANGLE_Update();
	/*** Angles update 500Hz ***********************************/
	
	
	
	
	/*** PID Call 100Hz ****************************************/
	if(inside_cnt < 5) {
			
			FILTERED.pitch += ANGLE.roll;
			FILTERED.roll += ANGLE.pitch;
			FILTERED.yaw += ANGLE.yaw;
			
			inside_cnt++;
			
	} else {
	
			FILTERED.roll /= 5;
			FILTERED.pitch /= 5;
			FILTERED.yaw /= 5;
		
			if(CAVA_RUNNING) {
				
				PID_Postion_Cal(&Roll_PID, 0.0, FILTERED.roll, 0.01);
				PID_Postion_Cal(&RollRate_PID, Roll_PID.Output, GYRO2.y, 0.01);
				
				PID_Postion_Cal(&Pitch_PID, 0.0, FILTERED.pitch, 0.01);
				PID_Postion_Cal(&PitchRate_PID, Pitch_PID.Output, GYRO2.x , 0.01);
				
//				PID_Postion_Cal(&RollRate_PID, .0, GYRO2.y, 0.01);
//				PID_Postion_Cal(&PitchRate_PID, .0, GYRO2.x , 0.01);

				
				ROLL = RollRate_PID.Output;
				PITCH = PitchRate_PID.Output;
				
				MOTO1_PWM = THRO + PITCH + ROLL;
				MOTO2_PWM = THRO - PITCH + ROLL;
				MOTO3_PWM = THRO - PITCH - ROLL;
				MOTO4_PWM = THRO + PITCH - ROLL;
				
				Motors_Shift(MOTO1_PWM, MOTO2_PWM, MOTO3_PWM, MOTO4_PWM);
			}
			
			
			FILTERED.roll = FILTERED.pitch = FILTERED.yaw = .0;
			inside_cnt = 0;
	}
	/*** PID Call 100Hz ****************************************/

	
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
}

void Clock_Sample_Stop(void)
{
	TIM_Cmd(TIM2, DISABLE);
}
