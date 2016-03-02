#ifndef __6050_H
#define __6050_H
#include "stm32f10x.h"

#define MPU6050_ADDRESS_AD0_LOW     0x68 // address pin low (GND), default for InvenSense evaluation board
#define MPU6050_ADDRESS_AD0_HIGH    0x69 // address pin high (VCC)
#define MPU6050_DEFAULT_ADDRESS     (MPU6050_ADDRESS_AD0_LOW<<1)


#define MPU6050_RA_SMPLRT_DIV       0x19				// yes
#define MPU6050_RA_CONFIG           0x1A				// yes
#define MPU6050_RA_GYRO_CONFIG      0x1B				// yes
#define MPU6050_RA_ACCEL_CONFIG     0x1C				// yes
#define MPU6050_RA_I2C_MST_CTRL     0x24				// yes


#define MPU6050_RA_ACCEL_XOUT_H     0x3B
#define MPU6050_RA_ACCEL_XOUT_L     0x3C
#define MPU6050_RA_ACCEL_YOUT_H     0x3D
#define MPU6050_RA_ACCEL_YOUT_L     0x3E
#define MPU6050_RA_ACCEL_ZOUT_H     0x3F
#define MPU6050_RA_ACCEL_ZOUT_L     0x40
#define MPU6050_RA_TEMP_OUT_H       0x41
#define MPU6050_RA_TEMP_OUT_L       0x42
#define MPU6050_RA_GYRO_XOUT_H      0x43
#define MPU6050_RA_GYRO_XOUT_L      0x44
#define MPU6050_RA_GYRO_YOUT_H      0x45
#define MPU6050_RA_GYRO_YOUT_L      0x46
#define MPU6050_RA_GYRO_ZOUT_H      0x47
#define MPU6050_RA_GYRO_ZOUT_L      0x48


//#define MPU6050_RA_USER_CTRL        0x6A				//yes
#define MPU6050_RA_PWR_MGMT_1       0x6B				// yes
#define MPU6050_RA_WHO_AM_I         0x75

void mpu6050_Config(void);
void mpu6050_SetRegister(uint8_t RegAddr, uint8_t RegValue);
void mpu6050_GetRegister(uint8_t* DataBuffer, uint8_t DataNum, uint8_t RegAddr);

#endif
