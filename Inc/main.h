/**
  ******************************************************************************
  * File Name          : main.h
  * Description        : This file contains the common defines of the application
  ******************************************************************************
  *
  * Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H
  /* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

void SystemClock_Config(void);


#define ADCx                            ADC1
#define ADCx_CLK_ENABLE()               __HAL_RCC_ADC_CLK_ENABLE()
#define ADCx_CHANNEL_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOB_CLK_ENABLE()

#define ADCx_FORCE_RESET()              __HAL_RCC_ADC_FORCE_RESET()
#define ADCx_RELEASE_RESET()            __HAL_RCC_ADC_RELEASE_RESET()

/* Definition for ADCx Channel Pin */
#define ADCx_CHANNEL_PIN                GPIO_PIN_0
#define ADCx_CHANNEL_GPIO_PORT          GPIOB

/* Definition for ADCx's Channel */
#define ADCx_CHANNEL                    ADC_CHANNEL_16
#define SAMPLINGTIME                    ADC_SAMPLETIME_6CYCLES_5

/* Definition for ADCx's Oversampling parameters */
#define OVERSAMPLING_RATIO        ADC_OVERSAMPLING_RATIO_128         /* 128-oversampling */                                      
#define RIGHTBITSHIFT             ADC_RIGHTBITSHIFT_3                /* 3-bit right shift of the oversampled summation */          
#define TRIGGEREDMODE             ADC_TRIGGEREDMODE_SINGLE_TRIGGER   /* A single trigger for all channel oversampled conversions */
#define OVERSAMPLINGSTOPRESET     ADC_REGOVERSAMPLING_CONTINUED_MODE /* Oversampling buffer maintained during injection sequence */
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/
/*
#define KEY_CALL_Pin GPIO_PIN_9
#define KEY_CALL_GPIO_Port GPIOG
#define KEY_NEXT_Pin GPIO_PIN_14
#define KEY_NEXT_GPIO_Port GPIOG
#define LED_CHG_GREEN_Pin GPIO_PIN_3
#define LED_CHG_GREEN_GPIO_Port GPIOB
#define SENSOR_SDA_Pin GPIO_PIN_7
#define SENSOR_SDA_GPIO_Port GPIOB
#define GPS_VDD_EN_Pin GPIO_PIN_12
#define GPS_VDD_EN_GPIO_Port GPIOC
#define SENSOR_SCL_Pin GPIO_PIN_6
#define SENSOR_SCL_GPIO_Port GPIOB
#define GSENSOR_INT_Pin GPIO_PIN_13
#define GSENSOR_INT_GPIO_Port GPIOC
#define LED_KEY_R1_Pin GPIO_PIN_12
#define LED_KEY_R1_GPIO_Port GPIOA
#define PRECHG_Pin GPIO_PIN_5
#define PRECHG_GPIO_Port GPIOB
#define LORA_RESET_Pin GPIO_PIN_11
#define LORA_RESET_GPIO_Port GPIOA
#define GPS_UART_RX_Pin GPIO_PIN_10
#define GPS_UART_RX_GPIO_Port GPIOA
#define LED_KEY_R2_Pin GPIO_PIN_9
#define LED_KEY_R2_GPIO_Port GPIOC
#define LCD_RESET_Pin GPIO_PIN_8
#define LCD_RESET_GPIO_Port GPIOA
#define GPS_UART_TX_Pin GPIO_PIN_9
#define GPS_UART_TX_GPIO_Port GPIOA
#define MOTO_PWM_Pin GPIO_PIN_9
#define MOTO_PWM_GPIO_Port GPIOB
#define LED_CHG_RED_Pin GPIO_PIN_7
#define LED_CHG_RED_GPIO_Port GPIOC
#define KEY_SOS_Pin GPIO_PIN_8
#define KEY_SOS_GPIO_Port GPIOC
#define GPS_RESET_Pin GPIO_PIN_2
#define GPS_RESET_GPIO_Port GPIOC
#define HEARTRATE_SENSOR_INT_Pin GPIO_PIN_1
#define HEARTRATE_SENSOR_INT_GPIO_Port GPIOC
#define GPS_POWER_ON_Pin GPIO_PIN_0
#define GPS_POWER_ON_GPIO_Port GPIOC
#define LORA_POWER_ON_Pin GPIO_PIN_15
#define LORA_POWER_ON_GPIO_Port GPIOB
#define LCD_I2C_SDA_Pin GPIO_PIN_14
#define LCD_I2C_SDA_GPIO_Port GPIOB
#define LORA_UART_TX_Pin GPIO_PIN_2
#define LORA_UART_TX_GPIO_Port GPIOA
#define CHARGE_Pin GPIO_PIN_12
#define CHARGE_GPIO_Port GPIOB
#define LCD_I2C_SCL_Pin GPIO_PIN_13
#define LCD_I2C_SCL_GPIO_Port GPIOB
#define LCD_SA0_Pin GPIO_PIN_7
#define LCD_SA0_GPIO_Port GPIOA
#define LORA_UART_RX_Pin GPIO_PIN_3
#define LORA_UART_RX_GPIO_Port GPIOA
#define SYSTEM_POWERON_IND_Pin GPIO_PIN_0
#define SYSTEM_POWERON_IND_GPIO_Port GPIOA
#define SYSTEM_POWERON_Pin GPIO_PIN_2
#define SYSTEM_POWERON_GPIO_Port GPIOB
#define HEARTRATE_SENSOR_RESET_Pin GPIO_PIN_1
#define HEARTRATE_SENSOR_RESET_GPIO_Port GPIOB
#define BATTERY_ADC_Pin GPIO_PIN_0
#define BATTERY_ADC_GPIO_Port GPIOB
#define SENSOR_VCC_EN_Pin GPIO_PIN_5
#define SENSOR_VCC_EN_GPIO_Port GPIOC
#define LCD_VCC_EN_Pin GPIO_PIN_4
#define LCD_VCC_EN_GPIO_Port GPIOC
*/
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MAIN_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
