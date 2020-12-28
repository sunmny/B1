#include "app_power.h"
#include "hw_rtc.h"
#include "delay.h"
#include "hw_config.h"
#include "app_meerwespe.h"
#include "cmsis_os.h"
#include "hw_adc.h"
#include "hw_gps.h"
#include "hw_rtc.h"
#include "task_timer.h"
#include "stm32l4xx_hal_def.h"
#include "hw_uart.h"
#include "hw_i2c.h"
#include "hw_gpio.h"
#include "task_gps.h"
#include "main.h"

extern osTimerId PowerTimerHandle;
extern osTimerId LedTimerHandle;


static uint8_t PowerTimerStatus=0;


void app_power_open(void)
{
  HAL_GPIO_WritePin(SYSTEM_POWER_LOCK_GPIO_Port, SYSTEM_POWER_LOCK_Pin, GPIO_PIN_SET);
}

void app_power_close(void)
{
  HAL_GPIO_WritePin(SYSTEM_POWER_LOCK_GPIO_Port, SYSTEM_POWER_LOCK_Pin, GPIO_PIN_RESET);
}

void power_timer_en(uint8_t en)
{
	if (PowerTimerStatus==1)
	{
		if(en == 1)
		{
			osTimerStop (PowerTimerHandle);
			osTimerStart (PowerTimerHandle, 3000);
		}else{
			osTimerStop (PowerTimerHandle);
		}
	}
	else if (PowerTimerStatus==0)
	{
		if(en == 1)
		{
			osTimerStart (PowerTimerHandle, 3000);
		}else{
		}
	}
}

uint8_t bd1_count;
extern  UART_HandleTypeDef huart1;
extern  uint8_t report_data[128];
extern uint8_t tcp_buf[39];
void PowerTimerCallback(void const * argument)
{

	lte_tcp_connect_network();
		lte_tcp_send_data();
}
extern uint16_t battery;
extern void aw9523_init(void);
static uint8_t led[6] = {0x29,0x2d,0x2c,0x2b,0x26,0x2a};
extern void aw9523_set_light(uint8_t count,uint8_t val);
uint8_t lte_connect_flag = 0;



void led_timer_en(uint8_t en)
{
	HAL_GPIO_WritePin(CHARGE_KEY_GPIO_Port, CHARGE_KEY_Pin, GPIO_PIN_RESET);
//	osTimerStart (LedTimerHandle, 500);
}


