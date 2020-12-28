
#include "task_gps.h"
#include <stdint.h>
#include <stdio.h>
#include "cmsis_os.h"
#include "task_meerwespe.h"
#include "app_meerwespe.h"

#include "hw_config.h"
#include "stm32l4xx_hal.h"
#include "delay.h"
#include "app_lte.h"

extern  UART_HandleTypeDef huart1;
extern uint8_t hw_uart1_send(uint8_t *tx_buf, uint16_t tx_len);
extern WORK_MODE work_mode;
extern WORK_SEND_MODE  work_send_mode;

extern osMessageQId LteQueueHandle;

extern uint8_t wakeup_init_flag;

extern  WORK_SEND_MODE work_send_mode_huxi ;



osStatus task_lte_message_put(uint32_t info)
{
  	osStatus  status = osOK;
	status = osMessagePut(LteQueueHandle, info, 5000);
	 
	return status;
}

osEvent task_Lte_message_get(void)
{
  return osMessageGet(LteQueueHandle,osWaitForever);
}






/* GpsTask function */
uint8_t lte_buf[10] = {'A','T','+','C','P','I','N','?',0x0d,0x0a};
void LteTask(void const * argument)
{
		hw_lte_init();
	
  for(;;)
  {
		//	printf("LteTask ---3\r\n");
		//osDelay(1000);
		 ulTaskNotifyTake( pdTRUE, portMAX_DELAY);
		 lte_receive_and_handle();
		//printf("LteTask\r\n");
	}

}
