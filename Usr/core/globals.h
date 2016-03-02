#ifndef __GLOBALS_H
#define __GLOBALS_H
#include "stm32f10x.h"
#include "PID.h"					// temp

typedef struct Triaxial1 {
	float x;
	float y;
	float z;
} Triaxial_f;

typedef struct Triaxial2 {
	int16_t x;
	int16_t y;
	int16_t z;
} Triaxial_i;

typedef struct Eular_ {
	float roll;
	float pitch;
	float yaw;
} Eular;

// debug

extern Triaxial_f ACC, GYRO;
extern Eular ANGLE;

void ACC_Gain(void);
void GYRO_Gain(void);
void ANGLE_Update(void);

void Sensors_Init(void);
void Clocks_Init(void);

void Motors_PWMOutput_Init(void);
void Motors_PulseOn(void);
void Motors_Calibrate(void);
void Motors_Shift(float pwm1, float pwm2, float pwm3, float pwm4);

#define 					 THRO_BASE					 1200
#define 					 THRO_BASE_DEBUG		 1000
extern float			 THRO;
extern uint8_t		 THRO_CHANGE;

#define THRO_MIN                       0


extern uint8_t CAVA_RUNNING;
#define RUNNING_YEP 								0x01
#define RUNNING_NOP									0x00

extern PID_Typedef RollRate_PID;
extern PID_Typedef PitchRate_PID;
extern PID_Typedef Roll_PID;
extern PID_Typedef Pitch_PID;
void Control_Init(void);

#endif
