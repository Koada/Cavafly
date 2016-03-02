#include "I2C.h"

static void I2C_Delay(void)
{
	u8 i;
	for(i = 0; i < 5; i++);
}


void I2C_Start(void)
{
	SDA_HIGH();
	I2C_Delay();
	SCL_HIGH();
	I2C_Delay();
	SDA_LOW();
	I2C_Delay();
	SCL_LOW();
	I2C_Delay();
}


void I2C_Stop(void)
{
	SDA_LOW();
	I2C_Delay();
	SCL_HIGH();
	I2C_Delay();
	SDA_HIGH();
	I2C_Delay();
}



void I2C_Create(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_SDA|GPIO_Pin_SCL;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	I2C_Stop();
}


void I2C_SendByte(u8 byte)
{
	u8 i = 8, mask = 0x80;
	while(i--){
		if(byte & mask)
			SDA_HIGH();
		else
			SDA_LOW();
		I2C_Delay();
		
		
		SCL_HIGH();
		I2C_Delay();
		SCL_LOW();
		I2C_Delay();
		
		
		byte <<= 1;
	}
}

u8 I2C_ReceiveAck(void)	
{
	u8 ok;
	
	SDA_HIGH();
	I2C_Delay();
	
	SCL_HIGH();
	I2C_Delay();
	
	if(ACK_RECEIVED())
		ok = 1;
	else
		ok = 0;

	SCL_LOW();
	I2C_Delay();
	
	return ok;
}


u8 I2C_ReceiveByte(void)
{
	u8 i = 8, byte = 0x00;
	
	SDA_HIGH();	
	I2C_Delay();
	
	while(i--){
		byte <<= 1;
		
		SCL_HIGH();
		I2C_Delay();
		
		if(SDA_STATE())
			byte++;
		
		SCL_LOW();
		I2C_Delay();
	}
	
	return byte;
}


void I2C_SendAck(u8 yes)
{
	if(yes)
		SDA_LOW();
	else
		SDA_HIGH();
	I2C_Delay();
	
	SCL_HIGH();
	I2C_Delay();
	SCL_LOW();
	I2C_Delay();
	
	if(yes)
		SDA_HIGH();
	else
		SDA_LOW();	
}

