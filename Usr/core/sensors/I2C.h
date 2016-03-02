#ifndef __I2C_H
#define __I2C_H
#include "stm32f10x.h"

#define GPIO_Pin_SCL 								GPIO_Pin_6
#define GPIO_Pin_SDA 								GPIO_Pin_7
#define SCL_HIGH() 									GPIO_SetBits(GPIOB, GPIO_Pin_SCL)//GPIOB->BSRR=GPIO_Pin_SCL
#define SCL_LOW()  									GPIO_ResetBits(GPIOB, GPIO_Pin_SCL)//GPIOB->BRR=GPIO_Pin_SCL
#define SDA_HIGH() 									GPIO_SetBits(GPIOB, GPIO_Pin_SDA)//GPIOB->BSRR=GPIO_Pin_SDA
#define SDA_LOW()  									GPIO_ResetBits(GPIOB, GPIO_Pin_SDA)//GPIOB->BRR=GPIO_Pin_SDA
#define ACK_RECEIVED() 							!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_SDA)//!(GPIOB->IDR&GPIO_Pin_SDA)
#define SDA_STATE()									GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_SDA)//GPIOB->IDR&GPIO_Pin_SDA

void I2C_Start(void);
void I2C_Stop(void);
void I2C_SendByte(u8 byte);
u8 	 I2C_ReceiveAck(void);
u8 	 I2C_ReceiveByte(void);
void I2C_SendAck(u8 yes);

void I2C_Create(void);
#endif
