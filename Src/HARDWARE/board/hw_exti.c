#include "hw_exti.h" 
#include "hw_config.h"
#include "stm32l4xx_hal.h"
#include "hw_gsensor.h"
#include "hw_heartrate.h"
#include "hw_key.h"
#include "hw_led.h"
#include "delay.h"
#include "FreeRTOS.h"
#include "task.h"
#include "hw_charge.h"
#include "stm32l4xx_hal_tim.h"
#include "app_ble.h"
#include "cmsis_os.h"
extern TIM_HandleTypeDef    TimHandle;
/*

KEY_CALL_Pin

KEY_NEXT_Pin

PRECHG_Pin

CHARGE_Pin

KEY_SOS_Pin

GSENSOR_INT_Pin

HEARTRATE_SENSOR_INT_Pin;

SYSTEM_POWERON_IND_Pin;

*/
	  GPIO_PinState state;
		

void Hall_IRQHandler_Config(void)
{
  GPIO_InitTypeDef   GPIO_InitStructure;

  /* Enable GPIOC clock */
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /* Configure PC.13 pin as input floating */
  GPIO_InitStructure.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Pin = HALL_EINT_GPIO_Pin;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

  /* Enable and set EXTI line 13 Interrupt to the lowest priority */
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}


void Charger_IRQHandler_Config(void)
{
  GPIO_InitTypeDef   GPIO_InitStructure;

  /* Enable GPIOC clock */
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /* Configure PC.13 pin as input floating */
  GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Pin = GPIO_PIN_9;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);

  /* Enable and set EXTI line 13 Interrupt to the lowest priority */
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}
void Key_IRQHandler_Config(void)
{
  GPIO_InitTypeDef   GPIO_InitStructure;

  /* Enable GPIOC clock */
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /* Configure PC.13 pin as input floating */
  GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStructure.Pull = GPIO_PULLUP;
  GPIO_InitStructure.Pin = KEY_EINT_GPIO_Pin;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* Enable and set EXTI line 13 Interrupt to the lowest priority */
  HAL_NVIC_SetPriority(EXTI4_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);
}
void hw_exti_init(void)
{    	 

}


/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
uint8_t cstatus;
extern TaskHandle_t bd1task ;
extern TaskHandle_t gpstask ;
extern TaskHandle_t bletask ;
extern TaskHandle_t ltetask ;
void send_notify_bd1_task(void)
{
	 BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		vTaskNotifyGiveFromISR(bd1task,&xHigherPriorityTaskWoken );  
 
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken );  
}

void send_notify_gps_task(void)
{
	 BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		vTaskNotifyGiveFromISR(gpstask,&xHigherPriorityTaskWoken );  
 
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken );  
}

void send_notify_ble_task(void)
{
	 BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		vTaskNotifyGiveFromISR(bletask,&xHigherPriorityTaskWoken );  
 
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken );  
}

void send_notify_lte_task(void)
{
	 BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		vTaskNotifyGiveFromISR(ltetask,&xHigherPriorityTaskWoken );  
 
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken );  
}
uint8_t data_ps[2];
extern osTimerId rdTimerHandle;
extern uint8_t open_lock_flag ;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	
		printf(" exti gpio pin = %x \r\n",GPIO_Pin);
  if (GPIO_Pin == SYSTEM_POWERON_IND_Pin)//power in
  {
			hw_key_power_in_irq();
			HAL_TIM_Base_Start_IT(&TimHandle);
			ft_dev.pwkey_flags = 1;
		
  }else if(GPIO_Pin == GPIO_PIN_4 ){
		ft_dev.lock_flags = 1;
			// set_moto_work(1);
			//osTimerStart (rdTimerHandle, 1000);
	}else if(GPIO_Pin == GPIO_PIN_7 ){
					ft_dev.hall_flags = 1;
					read_ps_data(data_ps);
					if(data_ps[0] <5 && data_ps[1]<5){
								 set_moto_work(1);
								osTimerStart (rdTimerHandle, 1000);
						open_lock_flag = 0;
					}
		
	}else if(GPIO_Pin == GPIO_PIN_9 ){
			cstatus = HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_9);
			if(cstatus ==1){
			ble_send_response("ftcin##",7);
			}else {
			ble_send_response("ftcout##",16);
			}
		printf("status = %d \r\n",cstatus);
	}
}

void exti_init(void)
{

	Hall_IRQHandler_Config();
	Key_IRQHandler_Config();
	Charger_IRQHandler_Config();
}
