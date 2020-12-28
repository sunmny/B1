#include "hw_bd2.h" 
#include "hw_config.h"
#include "stm32l4xx_hal.h"
#include "delay.h"
#include "cmsis_os.h"



void hw_bd2_power(GPIO_PinState state)
{	
	if(state == GPIO_PIN_SET || state == GPIO_PIN_RESET)
	{
	 	HAL_GPIO_WritePin(BD2_POWER_GPIO_Port, BD2_POWER_Pin, state);	
	}
}

void hw_bd2_reset(GPIO_PinState state)
{
	HAL_GPIO_WritePin(BD2_RESET_GPIO_Port, BD2_RESET_Pin, state);
}


void hw_bd2_open(void)
{
	hw_bd2_reset(GPIO_PIN_RESET);
   	hw_bd2_power(GPIO_PIN_SET);
	osDelay(50);
	hw_bd2_reset(GPIO_PIN_SET);
	set_rn_led_blue();
}

void hw_bd2_close(void)
{
	hw_bd2_reset(GPIO_PIN_RESET);	
	hw_bd2_power(GPIO_PIN_RESET);
	set_rn_led_blue_close();
}

void hw_bd2_init(void)
{    	 	
    hw_bd2_open();
}
