#include "hw_bd1.h" 
#include "hw_config.h"
#include "stm32l4xx_hal.h"
#include "delay.h"
#include "cmsis_os.h"
#include "hw_uart.h"

static uint8_t bd1_power_status=0;

void hw_bd1_power(GPIO_PinState state)
{	
	if(state == GPIO_PIN_SET)
	{
		HAL_GPIO_WritePin(BD1_POWER_GPIO_Port, BD1_POWER_Pin, state);
			delay_ms(50);
		HAL_GPIO_WritePin(BD1_POWER_1_GPIO_Port, BD1_POWER_1_Pin, state);
		delay_ms(50);
	 	HAL_GPIO_WritePin(BD1_ENABLE_GPIO_Port, BD1_ENABLE_Pin, state);
		bd1_power_status=1;
	}
	else if (state == GPIO_PIN_RESET)
	{
	 	HAL_GPIO_WritePin(BD1_ENABLE_GPIO_Port, BD1_ENABLE_Pin, state);	
		HAL_GPIO_WritePin(BD1_POWER_GPIO_Port, BD1_POWER_Pin, state);
		HAL_GPIO_WritePin(BD1_POWER_1_GPIO_Port, BD1_POWER_1_Pin, state);
		bd1_power_status=0;
	}
}

static void delay_ms(uint32_t ms)
{	
		uint32_t i ,j;
		
		for(i =0;i<ms ;i++){
					for(j =0;j<10000;j++);
		
		}


}
extern DMA_HandleTypeDef hdma_usart3_rx;
extern  UART_HandleTypeDef huart3;
extern  UART_HandleTypeDef huart2;
extern USART_RECEIVETYPE UsartType3;/*bd2*/
void close_uart2(void)
{
	
	  GPIO_InitTypeDef GPIO_InitStruct;
	    GPIO_InitStruct.Pin = BD1_UART_RX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
   // GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	HAL_NVIC_DisableIRQ(USART2_IRQn);
	HAL_NVIC_DisableIRQ(DMA1_Channel7_IRQn);
		//__HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);
}
void close_uart3(void)
{
	
	  GPIO_InitTypeDef GPIO_InitStruct;
	    GPIO_InitStruct.Pin = BD2_UART_RX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
   // GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	HAL_NVIC_DisableIRQ(USART3_IRQn);
	HAL_NVIC_DisableIRQ(DMA1_Channel3_IRQn);
		//__HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);
}


void open_uart2(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = BD1_UART_RX_Pin ;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    //GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 0, 1);
	HAL_NVIC_SetPriority(USART2_IRQn, 4, 0);
	HAL_NVIC_EnableIRQ(USART2_IRQn);
	HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);
		HAL_UART_Receive_DMA(&huart2, UsartType2.usartDMA_rxBuf, RECEIVELEN);
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);

}
void open_uart3(void)
{
	
	  GPIO_InitTypeDef GPIO_InitStruct;
	    GPIO_InitStruct.Pin = BD2_UART_RX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    //GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
	
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	 HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 0, 1);
	HAL_NVIC_SetPriority(USART3_IRQn, 3, 0);
	HAL_NVIC_EnableIRQ(USART3_IRQn);
	HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);
		HAL_UART_Receive_DMA(&huart3, UsartType3.usartDMA_rxBuf, RECEIVELEN);
	__HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);
		//__HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);
}
void hw_bd1_open(void)
{
   	
		hw_bd1_power(GPIO_PIN_SET);
	
}

void hw_bd1_close(void)
{
	
//	hw_bd1_power(GPIO_PIN_RESET);

	
}

void hw_bd1_init(void)
{    	 	
    hw_bd1_open();
}

uint8_t hw_bd1_get_power_status(void)
{
	return bd1_power_status;
}
