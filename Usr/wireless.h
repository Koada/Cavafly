#ifndef __WIRELESS_H
#define __WIRELESS_H

#include "stm32f10x.h"

void   	Wireless_Test(void);
void 		Wireless_Init(void);
void 		Wireless_RxMode(void);
uint8_t	Wireless_ReceiveOrders(uint8_t *rxbuf);

#endif /* __SPI_NRF_H */ 
