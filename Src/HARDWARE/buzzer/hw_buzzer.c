#include "hw_buzzer.h" 
#include "hw_config.h"
#include "stm32l4xx_hal.h"


uint8_t hw_buzzer_get_status(void)
{
	GPIO_PinState state;
	state = HAL_GPIO_ReadPin(BUZZER_ENABLE_GPIO_Port, BUZZER_ENABLE_Pin);
	if(state == GPIO_PIN_RESET)
	{
	  return  0;
	}
	else{
	  return 1;
	}
	
}

void hw_buzzer_enable(uint8_t enable)
{
	if(enable)
		HAL_GPIO_WritePin(BUZZER_ENABLE_GPIO_Port, BUZZER_ENABLE_Pin, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(BUZZER_ENABLE_GPIO_Port, BUZZER_ENABLE_Pin, GPIO_PIN_RESET);
}


void hw_buzzer_init(void)
{    	 	
    //hw_gps_open();
}
