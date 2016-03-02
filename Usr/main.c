/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "sample.h"
#include "globals.h"
#include "wireless.h"
/* Debug Includes ------------------------------------------------------------*/
#include "show_char.h"
/* Private functions ---------------------------------------------------------*/
void main_delay(uint16_t t)
{
	uint16_t k;
	while(t-- ) {
		k = 0x00ff;
		while(k--);
	}
}
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */

static uint8_t remote_buf[4], THRO_GEAR;
static int8_t ROLL_GEAR, PITCH_GEAR, YAW_GEAR;

int main(void)
{

	/* Debug Functions --------------------------------------------------------*/
	show_char_init();
	
	/* Global functions -------------------------------------------------------*/
	Sensors_Init();
	Clock_Sample_Init();
	Control_Init();
	Clock_Sample_Start();
	Motors_PWMOutput_Init();
	
	/* Wireless functions -----------------------------------------------------*/
	Wireless_Init();
	Wireless_Test();
	
  /* Infinite loop */
  while (1)
  {
		main_delay(750);
		Wireless_RxMode();
		Wireless_ReceiveOrders(remote_buf);
			
			if(CAVA_RUNNING){
				// get control orders
				if(remote_buf[2] + remote_buf[3] < 0x02) { // push start or adjust in wrong time
					// do nothing
					printf("	CAVA IS RUNNING YET\r\n");
				} else {
					if(remote_buf[2] == 0x01 && remote_buf[3] == 0x01) { // hover
						printf("	HOVER MODE ON\r\n");
					} else if(remote_buf[2] == 0xff && remote_buf[3] == 0xff) {
						printf("	DEBUG: STOP\r\n");
						
						Clock_Sample_Stop();
						Motors_Shift(1000, 1000, 1000, 1000);
						
					} else {
							THRO_GEAR  = remote_buf[1];
							YAW_GEAR   = ((int8_t)(remote_buf[0] - 128));
							ROLL_GEAR  = ((int8_t)(remote_buf[3] - 128));
							PITCH_GEAR = ((int8_t)(remote_buf[2] - 128));

//							THRO = THRO_BASE + THRO_GEAR * 0.8;   				// [1200, 1404]
//							printf("	THRO CHANGE: %.3f\r\n", THRO);	
							THRO = THRO_BASE_DEBUG + THRO_GEAR * 1.6;			// [1000, 1408]
							printf("	THRO CHANGE: %.3f\r\n", THRO);
					}
					
				}
				
			} else {
				// test whether to start or to adjust
				if(remote_buf[2] == 0x00 && remote_buf[3] == 0x01) { // start
					Motors_PulseOn();
					CAVA_RUNNING = RUNNING_YEP;
					printf("	*** MOTORS PULSE ON\r\n");
				}
				if(remote_buf[2] == 0x01 && remote_buf[3] == 0x00) {
					
				}
			}
	
	}
	//---------------------------------------------------------- while(1) ends
}
//--------------------------------------------------------- main() ends

/**
  * @}
  */


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
