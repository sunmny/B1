
#include "task_bd1.h"
#include <stdint.h>
#include <stdio.h>
#include "cmsis_os.h"
#include "task_meerwespe.h"
#include "app_meerwespe.h"
#include "hw_bd1.h"
#include "app_power.h"

#include "hw_config.h"
#include "stm32l4xx_hal.h"
#include "delay.h"
#include "app_bd1.h"

 extern void send_bd1_get_card_num(void);
 
/* Bd1 function */
void Bd1Task(void const * argument)
{

	//	HAL_UART_Transmit(&huart2,rd_buf,16, 0xFFFF);
	//osDelay(2000);

	
  for(;;)
  {
	//printf("Bd1Task---4\r\n");
		//osDelay(1000);
			 ulTaskNotifyTake( pdTRUE, portMAX_DELAY);
		
				rd_receive_and_handle();
				//osDelay(1000);
			//	send_bd1_get_card_num();
				//printf("Bd1Task\r\n");
  }
  
 

}
