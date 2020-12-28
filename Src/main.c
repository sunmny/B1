/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32l4xx_hal.h"
#include "cmsis_os.h"
#include "stm32l4xx_hal_tim.h"
#include "hw_meerwespe.h"
#include "hw_config.h"
#include "stm32l4xx_hal_sd.h"
#include "gui_message.h"
#include "app_meerwespe.h"
#include "task_meerwespe.h"
#include "app_power.h"
#include "ff.h"

#include "bd_tf.h"
#define LOGE printf

#define func __func__


__IO ITStatus init_flag=SET;


TIM_HandleTypeDef    TimHandle;
uint32_t uwPrescalerValue = 0;
extern HAL_StatusTypeDef HAL_TIM_Base_Init(TIM_HandleTypeDef *htim);
#define TIMER_COUNT_NUMBER  25

#define TIMx                           TIM3
#define TIMx_CLK_ENABLE()              __HAL_RCC_TIM3_CLK_ENABLE()

/* Definition for TIMx's NVIC */
#define TIMx_IRQn                      TIM3_IRQn


SD_HandleTypeDef hsd1;
DMA_HandleTypeDef hdma_sdmmc1;


FATFS fs;													
FIL file;													
FRESULT f_res;                    
UINT fnum;            					  
BYTE ReadBuffer[1024]={0};        
int ret = 0;
int ret1 = 0;
int ret2 = 0;
int ret3 = 0;
int ret4 = 0;
int j = 0;
BYTE WriteBuffer[]= "这是对sd卡进行的读写测试 \n";
extern char SDPath[4];
extern uint8_t retSD; 

extern UART_HandleTypeDef huart1;
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

HAL_StatusTypeDef sd_status;    
TestStatus test_status;           
// WWDG_HandleTypeDef   WwdgHandle;

IWDG_HandleTypeDef   IwdgHandle;

static void MX_SDMMC1_SD_Init(void);
HAL_SD_CardStateTypedef State;

HAL_SD_CardCIDTypedef SD_CardCID;


/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);
osEvent event;
                           

uint8_t sd_flag =0;
uint8_t dis_data[2];
extern  UART_HandleTypeDef huart1;
void tim_init(void)
{
			 uwPrescalerValue = (uint32_t)(SystemCoreClock / 10000) - 1;

			TIMx_CLK_ENABLE();
			TimHandle.Instance = TIM3;
			TimHandle.Init.Period            = 10000 - 1;
			TimHandle.Init.Prescaler         = 1000;
			TimHandle.Init.ClockDivision     = 0;
			TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
			TimHandle.Init.RepetitionCounter = 0;

			if (HAL_TIM_Base_Init(&TimHandle) != HAL_OK)
			{
				/* Initialization Error */
				Error_Handler();
			}
	
}

uint8_t power_count =0;
uint8_t power_flag =0;
void TIMEx_DMACommutationCplt(DMA_HandleTypeDef *hdma)
{

}
 void HAL_TIMEx_BreakCallback(TIM_HandleTypeDef *htim)
 {

 }
 void HAL_TIMEx_CommutationCallback(TIM_HandleTypeDef *htim)
 {

 }
 void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
 {
 
			 GPIO_PinState status;
			status=HAL_GPIO_ReadPin(SYSTEM_POWERON_IND_GPIO_Port, SYSTEM_POWERON_IND_Pin);
			// printf("lock pin status is %d \r\n",status);
			 if(!status && (power_count<5)){
			//	 HAL_TIM_Base_Stop_IT(htim);
				power_count=0;		 
			 }
			 
			 if(power_count ==1){
					HAL_GPIO_WritePin(CHARGE_KEY_GPIO_Port, CHARGE_KEY_Pin, GPIO_PIN_SET);
			 }
			 if(power_count ==2){
				HAL_GPIO_WritePin(CHARGE_KEY_GPIO_Port, CHARGE_KEY_Pin, GPIO_PIN_RESET);
				}
			 if(power_count >TIMER_COUNT_NUMBER)
							power_count =0;
			 
			// printf("power_count = %d \r\n ",power_count);
			 if(power_count ==5)
			 {
				 HAL_UART_Transmit(&huart1, (uint8_t *)"AT+CPOF\r\n", 11, 0xFFFF);
				hw_buzzer_enable(1);
				 //uint8_t led[6] = {0x29,0x2d,0x2c,0x2b,0x26,0x2a};
							aw9523_set_light(0x29,0x30);
							aw9523_set_light(0x2d,0x30);
							aw9523_set_light(0x2c,0x30);
							aw9523_set_light(0x2b,0x30);
							aw9523_set_light(0x26,0x30);
							aw9523_set_light(0x2a,0x30);
			 }else if (power_count ==6){
			 
			 hw_buzzer_enable(0);
			 }else if(power_count ==7){
			 hw_buzzer_enable(1);
			 
			 }else if(power_count ==8){
					hw_buzzer_enable(0);
			 }
			 else if((power_count ==TIMER_COUNT_NUMBER) &&(power_flag ==1)){
						 HAL_TIM_Base_Stop_IT(htim);
						 aw9523_set_light(0x29,0x30);
							aw9523_set_light(0x2d,0x30);
							aw9523_set_light(0x2c,0x30);
							aw9523_set_light(0x2b,0x30);
							aw9523_set_light(0x26,0x30);
							aw9523_set_light(0x2a,0x30);
						power_flag =0;
						power_count =0;
					 
				 
					 printf("power_off\r\n");
					app_power_close();	
				}
			 power_count ++;	
 }
 HAL_StatusTypeDef SD_DMAConfigRx(SD_HandleTypeDef *hsd)
{
			HAL_StatusTypeDef status = HAL_ERROR;
			
			/* Configure DMA Rx parameters */
			hdma_sdmmc1.Instance = DMA2_Channel4;
			hdma_sdmmc1.Init.Request = DMA_REQUEST_7;
			hdma_sdmmc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
			hdma_sdmmc1.Init.PeriphInc = DMA_PINC_DISABLE;
			hdma_sdmmc1.Init.MemInc = DMA_MINC_ENABLE;
			hdma_sdmmc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
			hdma_sdmmc1.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
			hdma_sdmmc1.Init.Mode = DMA_NORMAL;
			hdma_sdmmc1.Init.Priority = DMA_PRIORITY_LOW;
		 
			/* Associate the DMA handle */
			__HAL_LINKDMA(hsd,hdmarx,hdma_sdmmc1);
		 
			/* Stop any ongoing transfer and reset the state*/
			HAL_DMA_Abort(&hdma_sdmmc1);
			
			/* Deinitialize the Channel for new transfer */
			HAL_DMA_DeInit(&hdma_sdmmc1);
		 
			/* Configure the DMA Channel */
			status = HAL_DMA_Init(&hdma_sdmmc1);
				
			return (status);
}
 
/**
  * @brief Configure the DMA to transmit data to the SD card
  * @retval
  *  HAL_ERROR or HAL_OK
  */
HAL_StatusTypeDef SD_DMAConfigTx(SD_HandleTypeDef *hsd)
{
			HAL_StatusTypeDef status;
			
			/* SDMMC1_TX Init */
			hdma_sdmmc1.Instance = DMA2_Channel4;
			hdma_sdmmc1.Init.Request = DMA_REQUEST_7;
			hdma_sdmmc1.Init.Direction = DMA_MEMORY_TO_PERIPH;
			hdma_sdmmc1.Init.PeriphInc = DMA_PINC_DISABLE;
			hdma_sdmmc1.Init.MemInc = DMA_MINC_ENABLE;
			hdma_sdmmc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
			hdma_sdmmc1.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
			hdma_sdmmc1.Init.Mode = DMA_CIRCULAR;
			hdma_sdmmc1.Init.Priority = DMA_PRIORITY_LOW;
		 
			/* Associate the DMA handle */
			__HAL_LINKDMA(hsd, hdmatx, hdma_sdmmc1);
			
			/* Stop any ongoing transfer and reset the state*/
			HAL_DMA_Abort(&hdma_sdmmc1);
			
			/* Deinitialize the Channel for new transfer */
			HAL_DMA_DeInit(&hdma_sdmmc1);  
			
			/* Configure the DMA Channel */
			status = HAL_DMA_Init(&hdma_sdmmc1); 
		 
			return (status);
}


static void MX_DMA_Init(void) 
{
  /* DMA controller clock enable */
			__HAL_RCC_DMA2_CLK_ENABLE();


			HAL_NVIC_SetPriority(DMA2_Channel4_IRQn, 4, 0);
			HAL_NVIC_EnableIRQ(DMA2_Channel4_IRQn);


}

static void MX_SDMMC1_SD_Init(void)
{

			hsd1.Instance = SDMMC1;
			//hsd1->State =HAL_SD_STATE_RESET;
			hsd1.Init.ClockEdge = SDMMC_CLOCK_EDGE_RISING;
			hsd1.Init.ClockBypass = SDMMC_CLOCK_BYPASS_DISABLE;
			hsd1.Init.ClockPowerSave = SDMMC_CLOCK_POWER_SAVE_DISABLE;
			hsd1.Init.BusWide = SDMMC_BUS_WIDE_1B;
			hsd1.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_ENABLE;
			hsd1.Init.ClockDiv = 10;
			
			printf("hal sd init \r\n");
			 if (HAL_SD_Init(&hsd1) != HAL_OK)
			{
				 Error_Handler();
			}
			printf("hal sd bus config \r\n");
			HAL_SD_ConfigWideBusOperation(&hsd1, SDMMC_BUS_WIDE_4B);


}



int main(void)
{
			uint8_t ret;
			
			/* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	#if 0
	int testNum = 10;
	int i = 0;
	unsigned char pucID[32] = {0};
	unsigned int IDLen = 32;
	unsigned int uiSn = 100;
	char pucData[16] = {0x26,0x11,0x0D,0x21,0x2E,0x25,0x00,0x00,0x79,0x27,0x48,0x63,0x29,0x5A,0x00,0x00};
	unsigned int uiDataLen = 16;
	unsigned char pucCData[100] = {0};
	unsigned int puicDataLen = uiDataLen+14;

	unsigned int puiSn = 0;
	unsigned char plainData[100] = {0};
	unsigned int plainDataLen = puicDataLen-14;
	char uID[32] = {0};
	unsigned int uIDLen = 6;
	unsigned int status = 0;
	#endif
  HAL_Init();

			/* Configure the system clock */
			SystemClock_Config();
			HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

			ret = app_init();
			if(ret ==1){
					return 1;
				}

			task_init();
			MX_DMA_Init();
			MX_SDMMC1_SD_Init();	
			MX_FATFS_Init();
			aw9523_init_led();	
			tmd2772_init();
			power_flag =1;
			init_flag = RESET;
				set_ble_work(0);
			State = HAL_SD_GetCardState(&hsd1);

			if(State == HAL_SD_CARD_TRANSFER)
			{
				HAL_SD_GetCardCID(&hsd1,&SD_CardCID);
				printf("\r\n Initialize SD card successfully!\r\n\r\n");
				printf(" SD card information! \r\n");
				printf(" CardCapacity  : %llu \r\n",((unsigned long long)hsd1.SdCard.BlockSize*hsd1.SdCard.BlockNbr));
				printf(" CardBlockSize : %d \r\n",hsd1.SdCard.BlockSize);
				printf(" RCA           : %d \r\n",hsd1.SdCard.RelCardAdd);
				printf(" CardType      : %d \r\n",hsd1.SdCard.CardType);
				printf(" ManufacturerID: %d \r\n",SD_CardCID.ManufacturerID);
					
			}
			else
			{
					printf("SD卡初始化失败\n" );

			}
			if(retSD == 0)
			{

			f_res = f_mount(&fs,SDPath,1);
			


				if(f_res == FR_NO_FILESYSTEM)
				{
			

					f_res=f_mkfs((TCHAR const*)SDPath,0,0,0,0);							
					
					if(f_res == FR_OK)
					{
			 

						f_res = f_mount(NULL,(TCHAR const*)SDPath,1);			
						
						f_res = f_mount(&fs,(TCHAR const*)SDPath,1);
					}
					else
					{
						 printf("《《格式化失败。》》\r\n");
					 
					}
				}
				else if(f_res!=FR_OK)
				{
					printf("！！SD卡挂载文件系统失败。(%d)\r\n",f_res);
			 
				
				}
				else
				{
					printf("mount fs ok\r\n");
				}
		printf("！！加解密测试开始\r\n");
		
		/*
		ret1 = SKF_ConnectDev("stc08.txt",&hDev);
		if(ret1){
			LOGE("==%s-%d===[ SKF_ConnectDev err: %x.]===\n", func, __LINE__, ret1);
			return ret1;
		}
		*/
		#if 0
		//for(j=0;j<testNum;j++){
			memset(plainData,0,sizeof(plainData));
			memset(pucCData,0,sizeof(pucCData));
			memset(pucID,0,sizeof(pucID));
			memset(uID,0,sizeof(uID));
			
			ret2 = TF_GetDeviceID(uID,&uIDLen);
			if(ret2){
				LOGE("==%s-%d===[ TF_GetDeviceID err: %x.]===\n", func, __LINE__, ret2);
				return ret2;
			}
			
			ret3 = TF_EncData((unsigned char *)pucData,uiDataLen,pucCData,&puicDataLen);
			if(ret3){
				printf("==%s-%d===[ TF_EncData TF_EncData: %x.]===\n", func, __LINE__, ret3);
				return ret3;
			}
			
			printf("puicDataLen:%d\n",puicDataLen);
			
 			ret4 = TF_DecData(pucCData,puicDataLen,plainData,&plainDataLen);
			if(ret4){
				LOGE("==%s-%d===[ TF_DecData TF_EncData: %x.]===\n", func, __LINE__, ret4);
				return ret4;
			}
			for(i = 0;i<30;i++){
			    printf("plainData[%d] : 0x%x \r\n",i,plainData[i]);
			}
				printf("plainDataLen:%d\n",plainDataLen);
		//}
		
		printf("！！加解密测试结束\r\n");
		#endif
    
    

   // f_res = f_mount(NULL,(TCHAR const*)SDPath,1);	
  

   // FATFS_UnLinkDriver(SDPath);
	}
	
			
			
	    // pn512_init();

			set_ble_work(1);
	
	#if 0
		//for(j=0;j<testNum;j++){
			memset(plainData,0,sizeof(plainData));
			memset(pucCData,0,sizeof(pucCData));
			memset(pucID,0,sizeof(pucID));
			memset(uID,0,sizeof(uID));
			
			ret2 = TF_GetDeviceID(uID,&uIDLen);
			if(ret2){
				LOGE("==%s-%d===[ TF_GetDeviceID err: %x.]===\n", func, __LINE__, ret2);
				return ret2;
			}
			
			ret3 = TF_EncData((unsigned char *)pucData,uiDataLen,pucCData,&puicDataLen);
			if(ret3){
				printf("==%s-%d===[ TF_EncData TF_EncData: %x.]===\n", func, __LINE__, ret3);
				return ret3;
			}
			
			printf("puicDataLen:%d\n",puicDataLen);
			
 			ret4 = TF_DecData(pucCData,puicDataLen,plainData,&plainDataLen);
			if(ret4){
				LOGE("==%s-%d===[ TF_DecData TF_EncData: %x.]===\n", func, __LINE__, ret4);
				return ret4;
			}
			for(i = 0;i<30;i++){
			    printf("plainData[%d] : 0x%x \r\n",i,plainData[i]);
			}
				printf("plainDataLen:%d\n",plainDataLen);
		//}
		
		printf("！！加解密测试结束\r\n");
		#endif
			 osKernelStart();


			while (1)
			{
				
				
			}
			/* USER CODE END 3 */

}
void SystemClock_Config(void)
{

				RCC_OscInitTypeDef RCC_OscInitStruct;
				RCC_ClkInitTypeDef RCC_ClkInitStruct;
				RCC_PeriphCLKInitTypeDef PeriphClkInit;

					
				RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
				RCC_OscInitStruct.HSEState = RCC_HSE_ON;
				RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
				 RCC_OscInitStruct.MSIState = RCC_MSI_ON;
				RCC_OscInitStruct.MSICalibrationValue = 0;
				RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
				
				if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
				{
					Error_Handler();
				}

					/**Initializes the CPU, AHB and APB busses clocks 
					*/
				RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
																		|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
				RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
				RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
				RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
				RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

				if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
				{
					 Error_Handler();
				}
				
				

				/** Configure the main internal regulator output voltage 
				*/
				if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
				{
					Error_Handler();
				}

				PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_USART2|
																							RCC_PERIPHCLK_USART3|RCC_PERIPHCLK_I2C1|RCC_PERIPHCLK_I2C2
																							|RCC_PERIPHCLK_UART4|RCC_PERIPHCLK_SDMMC1;
				PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
				 PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
				PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
				PeriphClkInit.Uart4ClockSelection = RCC_UART4CLKSOURCE_PCLK1;
				PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
				PeriphClkInit.I2c2ClockSelection = RCC_I2C2CLKSOURCE_PCLK1;
				PeriphClkInit.Sdmmc1ClockSelection = RCC_SDMMC1CLKSOURCE_PLLSAI1;
				PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_MSI;
				PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
				PeriphClkInit.PLLSAI1.PLLSAI1N = 16;
				PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
				PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
				PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
				PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_48M2CLK;
				if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
				{
					 Error_Handler();
				}

					/**Configure the main internal regulator output voltage 
					*/
				if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
				{
					Error_Handler();
				}

					/**Configure the Systick interrupt time 
					*/
				HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

					/**Configure the Systick 
					*/
				HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

				/* SysTick_IRQn interrupt configuration */
				HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}




/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  //while(1) 
  {
  }
  /* USER CODE END Error_Handler */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

/** NVIC Configuration
*/

