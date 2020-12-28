#include "hw_charge.h" 
#include "hw_config.h"
#include "stm32l4xx_hal.h"
#include "delay.h"
#include "hw_led.h"


uint8_t charge_state = 0;



void hw_charge_init(void)
{    	 	

}

uint8_t hw_charge_detect(void)
{
  //��ȡprechg״̬�� �ͳ���У��ߣ�δ���
	GPIO_PinState state;
	state = HAL_GPIO_ReadPin(PRECHG_GPIO_Port, PRECHG_Pin);
	if(state == GPIO_PIN_RESET)
	{
	  charge_state = 1;
	}
	else{
	
	  charge_state = 0;
	}
	return state;
	
}


//prechg�жϴ���  //˫����
void hw_charge_prechg(void)
{
  
		GPIO_PinState state;
	
	  delay_ms(40);//����5ms
    //taskENTER_CRITICAL();
	  state = HAL_GPIO_ReadPin(PRECHG_GPIO_Port,PRECHG_Pin);
		
	if(state == GPIO_PIN_RESET)//�ͣ����
	{
	
		if(HAL_GPIO_ReadPin(CHARGE_GPIO_Port, CHARGE_Pin) == GPIO_PIN_SET)//����
		{
			hw_led_key_chg_red_set(GPIO_PIN_RESET);
			hw_led_key_chg_green_set(GPIO_PIN_SET);
		}
		else
		{
		  hw_led_key_chg_red_set(GPIO_PIN_SET);
			hw_led_key_chg_green_set(GPIO_PIN_RESET);
		   
		}
		
		}else  //��
		{
		  //�γ�  gao
			hw_led_key_chg_red_set(GPIO_PIN_RESET);
			hw_led_key_chg_green_set(GPIO_PIN_RESET);
			
		}
	//taskEXIT_CRITICAL();

}


void hw_charge_charge(void)
{
			GPIO_PinState state;
		//	taskENTER_CRITICAL();
	 delay_ms(40);//����5ms
	
	 state = HAL_GPIO_ReadPin(CHARGE_GPIO_Port,CHARGE_Pin);
		
	if(state == GPIO_PIN_SET)//��
	{
		//if(HAL_GPIO_ReadPin(CHARGE_GPIO_Port, CHARGE_Pin) == GPIO_PIN_SET)//����
		//{
		if(HAL_GPIO_ReadPin(PRECHG_GPIO_Port, PRECHG_Pin) == GPIO_PIN_RESET)
		{
			hw_led_key_chg_red_set(GPIO_PIN_RESET);
			hw_led_key_chg_green_set(GPIO_PIN_SET);
		}

	}
	//else//�����
	//{
	//    hw_led_key_chg_red_set(GPIO_PIN_SET);
	//		hw_led_key_chg_green_set(GPIO_PIN_RESET);
	//}
//	taskEXIT_CRITICAL();


}


