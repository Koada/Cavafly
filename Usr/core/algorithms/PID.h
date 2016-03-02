#ifndef __PID_H
#define __PID_H

typedef struct {
	float Kp;
	float Ki;
	float Kd;
	float Desired;
	float Error;
	float PreError;
	float PrePreError;
	float Integ;
	float Deriv;
	float Output;
	float iLimit;
	float iRange;
} PID_Typedef;


void PID_Init(PID_Typedef *PID, const float kp, const float ki, const float kd);
void PID_SetIntegralRangeAndLimit(PID_Typedef *PID, const float range, const float limit);
float PID_Postion_Cal(PID_Typedef *PID, float target, float present, float dt);

#endif
