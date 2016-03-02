#include "ano_debug.h"
#include "globals.h"

void ANO_USART_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	
	RCC_APB2PeriphClockCmd(
		RCC_APB2Periph_GPIOA|
		RCC_APB2Periph_USART1,
	ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_Mode = USART_Mode_Tx;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART1, &USART_InitStruct);
	USART_Cmd(USART1, ENABLE);
}



ANO_STATUS_Typedef ANO_Status_Structure;
ANO_SENSOR_Typedef ANO_Sensor_Structure;

static void delay_us(uint16_t t)
{
	uint16_t k;
	while(t--) {
		k = 0x00ff;
		while(k--) continue;
	}
}


static  void USART1_SendBuffer(uint8_t *buf, uint16_t n)
{
	uint16_t index;
	for(index = 0; index < n; index++) {
		USART_SendData(USART1, buf[index]);
		delay_us(5);
	}
}


#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))

extern Eular DEBUG_FILTERED;
uint8_t ano_usart_send_buffer[256];
static void ANO_USART_SendStatus(void)
{
	uint8_t sum = 0;
	uint16_t cnt = 0, index;
	int16_t temp;
	int32_t temp2;
	
	ano_usart_send_buffer[cnt++] = 0xAA;
	ano_usart_send_buffer[cnt++] = 0xAA;
	ano_usart_send_buffer[cnt++] = 0x01;
//	ano_usart_send_buffer[cnt++] = sizeof(ANO_STATUS_Typedef);
	ano_usart_send_buffer[cnt++] = 0;
	temp = ((int16_t)DEBUG_FILTERED.roll * 100);
	ano_usart_send_buffer[cnt++] = BYTE1(temp);
	ano_usart_send_buffer[cnt++] = BYTE0(temp);
	temp = ((int16_t)DEBUG_FILTERED.pitch * 100);
	ano_usart_send_buffer[cnt++] = BYTE1(temp);
	ano_usart_send_buffer[cnt++] = BYTE0(temp);
	temp = ((int16_t)DEBUG_FILTERED.yaw * 100);
	ano_usart_send_buffer[cnt++] = BYTE1(temp);
	ano_usart_send_buffer[cnt++] = BYTE0(temp);	
	
	temp2 = 0x0000;
	ano_usart_send_buffer[cnt++] = BYTE3(temp2);
	ano_usart_send_buffer[cnt++] = BYTE2(temp2);
	ano_usart_send_buffer[cnt++] = BYTE1(temp2);
	ano_usart_send_buffer[cnt++] = BYTE0(temp2);
	
//	ano_usart_send_buffer[cnt++] = 0x00;
	ano_usart_send_buffer[cnt++] = 0xA0;
	ano_usart_send_buffer[3] = cnt - 4;
	
	for(index = 0; index < cnt; index++)
		sum += ano_usart_send_buffer[index];
	ano_usart_send_buffer[cnt++] = sum;	
	
	USART1_SendBuffer(ano_usart_send_buffer, cnt);
}

static void ANO_USART_SendSensor(void)
{
	
}

void ANO_USART_DataUpdate(void)
{
	ANO_USART_SendSensor();
	ANO_USART_SendStatus();
}

