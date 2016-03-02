#ifndef __ANO_DEBUG
#define __ANO_DEBUG
#include "stm32f10x.h"

typedef struct ANO_STATUS_Typedef_ {
	int16_t ROLx100;
	int16_t PITx100;
	int16_t YAWx100;
	int32_t ALT_USE;
	uint8_t FLY_MOD;
	uint8_t ARMED;
} ANO_STATUS_Typedef;

typedef struct ANO_SENSOR_Typedef_ {
	int16_t ACC_X;
	int16_t ACC_Y;
	int16_t ACC_Z;
	int16_t GYRO_X;
	int16_t GYRO_Y;
	int16_t GYRO_Z;
	int16_t MAG_X;
	int16_t MAG_Y;
	int16_t MAG_Z;
} ANO_SENSOR_Typedef;

void ANO_USART_init(void);
void ANO_USART_DataUpdate(void);
#endif
