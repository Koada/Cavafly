#include "PID.h"
#include <math.h>
#include "globals.h"

#define DEFAULT_PID_INTEGRATION_LIMIT 1000

void PID_Init(PID_Typedef *PID, const float kp, const float ki, const float kd)
{
	PID->Kp = kp;
	PID->Ki = ki;
	PID->Kd = kd;
	PID->Desired = 0;
	PID->Error = 0;
	PID->PreError = 0;
	PID->PrePreError = 0;
	PID->Integ = 0;
	PID->iRange = 0;
	PID->Deriv = 0;
	PID->Output = 0;
	PID->iLimit = DEFAULT_PID_INTEGRATION_LIMIT;
}

void PID_SetIntegralRangeAndLimit(PID_Typedef *PID, const float range, const float limit)
{
	PID->iRange = range;
	PID->iLimit = limit;
}



float PID_Postion_Cal(PID_Typedef* PID, float target, float present, float dt)
{
	float termI = 0, T;

	PID->Error= target - present;

	PID->Deriv= (PID->Error-PID->PreError)/dt;
	
	T = PID->Kp*PID->Error;

	PID->Output=PID->Kp*PID->Error + PID->Ki*PID->Integ + PID->Kd*PID->Deriv;
	
	PID->PreError=PID->Error;
	//????????????
	
			if(fabs(PID->Output) < THRO)		//?????????
			{
				termI = PID->Integ + PID->Error * dt;
				if(termI > - PID->iLimit && termI < PID->iLimit && PID->Output > - PID->iLimit && PID->Output < PID->iLimit)
						PID->Integ = termI;
			}

	return T;
}


