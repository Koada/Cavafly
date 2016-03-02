#include "globals.h"
#include <math.h>
#include "MPU6050.h"
#include "sample.h"
#include "control.h"
#include "AHRS.h"
#include "PID.h"
//----------------------------------------------------------------------------------------------------
// Init function

void Sensors_Init(void)
{
	mpu6050_Config();
	
}

void Clocks_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Clock_Sample_Init();
	Clock_Control_Start();
}
//----------------------------------------------------------------------------------------------------
// 6050 ACC GYRO gain

Triaxial_f ACC, GYRO;

static uint8_t buffer_6050[6];
void ACC_Gain(void)
{
	mpu6050_GetRegister(buffer_6050, 6, MPU6050_RA_ACCEL_XOUT_H);
	ACC.x = (int16_t)(buffer_6050[0]<<8 | buffer_6050[1]);
	ACC.y = (int16_t)(buffer_6050[2]<<8 | buffer_6050[3]);
	ACC.z = (int16_t)(buffer_6050[4]<<8 | buffer_6050[5]);
}

void GYRO_Gain(void)
{
	mpu6050_GetRegister(buffer_6050, 6, MPU6050_RA_GYRO_XOUT_H);
	GYRO.x = (int16_t)(buffer_6050[0]<<8 | buffer_6050[1]);
	GYRO.y = (int16_t)(buffer_6050[2]<<8 | buffer_6050[3]);
	GYRO.z = (int16_t)(buffer_6050[4]<<8 | buffer_6050[5]);
}

//----------------------------------------------------------------------------------------------------
// Eular Angle get

Eular ANGLE;
extern float q0, q1, q2, q3;

void ANGLE_Update(void)					// 360бу = 2ж╨            2000бу = 11.111...ж╨ = 34.906585          32768        
{
	static float t_roll = .0, t_pitch = .0, t_yaw = .0, temp;
	
	GYRO.x = GYRO.x / 32768.0 * 34.906585;
	GYRO.y = GYRO.y / 32768.0 * 34.906585;
	GYRO.z = GYRO.z / 32768.0 * 34.906585;
	
	IMUupdate(GYRO.x, GYRO.y, GYRO.z, ACC.x, ACC.y, ACC.z);

	t_roll = atan2f(2.0f*(q0*q1+q2*q3), 1-2.0f*(q1*q1+q2*q2));
	
	temp = 2.0f*(q0*q2-q1*q3);
	temp = (temp > 1.0)? 1.0: temp;
	temp = (temp < -1.0)? -1.0: temp;
	t_pitch = asinf(temp);
	
	t_yaw = atan2f(2.0f*(q1*q2-q0*q3), 2.0f*(q0*q0+q1*q1)-1);
	
	ANGLE.roll = t_roll * 57.295780;
	ANGLE.pitch = t_pitch * 57.295780;
	ANGLE.yaw = t_yaw * 57.295780;
}

//----------------------------------------------------------------------------------------------------
// Motors Shift

void Motors_Shift(float pwm1, float pwm2, float pwm3, float pwm4)
{
	if(pwm1 > 1800.0) pwm1 = 1800.0;
	if(pwm1 < 1000.0) pwm1 = 1000.0;
	if(pwm2 > 1800.0) pwm2 = 1800.0;
	if(pwm2 < 1000.0) pwm2 = 1000.0;
	if(pwm3 > 1800.0) pwm3 = 1800.0;
	if(pwm3 < 1000.0) pwm3 = 1000.0;
	if(pwm4 > 1800.0) pwm4 = 1800.0;
	if(pwm4 < 1000.0) pwm4 = 1000.0;
	
	TIM3->CCR1 = (uint16_t)pwm1;
	TIM3->CCR2 = (uint16_t)pwm2;
	TIM3->CCR3 = (uint16_t)pwm3;
	TIM3->CCR4 = (uint16_t)pwm4;
}

//----------------------------------------------------------------------------------------------------
// Aircraft State
		// Control
float			 		THRO  =  1000;
uint8_t			  THRO_CHANGE = 0;
		// State
uint8_t CAVA_RUNNING = RUNNING_NOP;

//----------------------------------------------------------------------------------------------------
// PID Globals and Control
PID_Typedef RollRate_PID;
PID_Typedef PitchRate_PID;
PID_Typedef Roll_PID;
PID_Typedef Pitch_PID;


void Control_Init(void)
{
	PID_Init(&RollRate_PID, 1.25, 0, 0.009);										// 		*** INSIDE ROLL PASS ***					1.25  0   0.009       1.5   0   0.014
	PID_Init(&Roll_PID, 1.16, 0, 0.002);												// ***** OUTSIDE ROLL PASS *****				1.16  0   0.002       2.5   0   0.01
																															//																			New
	PID_Init(&PitchRate_PID, 2.0, 0, 0.016);										// 		*** INSIDE PITCH PASS ***					2.0   0   0.016				1.65  0   0.015
	PID_Init(&Pitch_PID, 0.65, 0, 0.0014);										  //	***** OUTSIDE ROLL PASS *****       0.65	0		0.0014			2.2   0   0.008
}
