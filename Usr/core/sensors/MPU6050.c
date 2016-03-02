#include "MPU6050.h"
#include "I2C.h"
//#include "show_char.h"

static void mpu6050_Delay(void)
{
	uint16_t i = 1000, j;
	while(i--){
		j = 1000;
		while(j--);
	}
}


void mpu6050_SetRegister(uint8_t RegAddr, uint8_t RegValue)
{
	I2C_Start();
	I2C_SendByte(MPU6050_DEFAULT_ADDRESS);
	I2C_ReceiveAck();
//	if(!I2C_ReceiveAck())
//		printf("	1");
	
	I2C_SendByte(RegAddr);
	I2C_ReceiveAck();
//	if(!I2C_ReceiveAck())
//		printf("	2");
	
	I2C_SendByte(RegValue);
	I2C_ReceiveAck();
//	if(!I2C_ReceiveAck())
//		printf("	3");
	
	I2C_Stop();
}

void mpu6050_GetRegister(uint8_t* DataBuffer, uint8_t DataNum, uint8_t RegAddr)
{
	uint8_t i;
	
	I2C_Start();
	I2C_SendByte(MPU6050_DEFAULT_ADDRESS);
	I2C_ReceiveAck();
//	if(!I2C_ReceiveAck())
//		printf("	1");
	
	I2C_SendByte(RegAddr);
	I2C_ReceiveAck();
//	if(!I2C_ReceiveAck())
//		printf("	2");
	
	I2C_Start();
	I2C_SendByte(MPU6050_DEFAULT_ADDRESS | 0x01);
	I2C_ReceiveAck();
//	if(!I2C_ReceiveAck())
//		printf("	3");
	
	for(i = 0; i < DataNum; i++){
		
		DataBuffer[i] = I2C_ReceiveByte();
		
		if(i + 1 < DataNum)
			I2C_SendAck(1);
		else
			I2C_SendAck(0);
	}
	
	I2C_Stop();
}


void mpu6050_Config(void)
{
	I2C_Create();
	mpu6050_Delay();
	
	mpu6050_SetRegister(MPU6050_RA_PWR_MGMT_1, 0x01);
	mpu6050_SetRegister(MPU6050_RA_I2C_MST_CTRL, 0x13);
	mpu6050_SetRegister(MPU6050_RA_SMPLRT_DIV, 0x07);
	mpu6050_SetRegister(MPU6050_RA_CONFIG, 0x03);
	
	mpu6050_SetRegister(MPU6050_RA_GYRO_CONFIG, 0x18);						// * * * 1 1 * * *			+= 2000бу/s
	mpu6050_SetRegister(MPU6050_RA_ACCEL_CONFIG, 0x08);						// * * * 0 1 * * *			+= 4g
}

