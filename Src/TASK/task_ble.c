
#include "task_ble.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "cmsis_os.h"
#include "task_meerwespe.h"
#include "app_meerwespe.h"

#include "hw_config.h"
#include "stm32l4xx_hal.h"
#include "delay.h"
#include "app_ble.h"

#include "hw_uart.h" //for test

   #define BIT_0	( 1 << 0 )
   #define BIT_1	( 1 << 1 )
   #define BIT_2	( 1 << 2 )

extern WORK_MODE work_mode;
extern WORK_SEND_MODE  work_send_mode;



extern uint8_t wakeup_init_flag;

extern  WORK_SEND_MODE work_send_mode_huxi ;



osStatus task_ble_message_put(uint32_t info)
{
  	osStatus  status = osOK;
	
	 
	return status;
}

osEvent task_ble_message_get(void)
{
  
}

osStatus task_ble_mail_put(uint8_t *data, uint16_t len)
{
	
}
extern uint8_t link_buf[256];

osEvent task_ble_mail_get(void)
{
//	return osMailGet(BleMailQueueHandle,osWaitForever);//parameter to do, 20181025
}

extern ADC_HandleTypeDef        AdcHandle;

extern uint32_t                 uwConvertedValue;
extern void zdev_read_battery_adc(void);
/* BleTask function */
void BleTask(void const * argument)
{
//printf("BleTask\r\n");
  for(;;)
  {

   ulTaskNotifyTake( pdTRUE, portMAX_DELAY);
		printf("BleTask ---2\r\n");
		//osDelay(1000);
		ble_receive_and_handle();

		}

	  /* USER CODE END 5 */ 
}
