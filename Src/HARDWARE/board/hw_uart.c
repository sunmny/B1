#include "hw_uart.h" 
#include "stm32l4xx_hal.h"
#include "cmsis_os.h"
#include "string.h"
#include "delay.h"

#include "hw_config.h"

#include "app_ble.h"

static __IO ITStatus Uart1Ready = RESET;
static __IO ITStatus Uart2Ready = RESET;
static __IO ITStatus Uart3Ready = RESET;
static __IO ITStatus Uart4Ready = RESET;

 UART_HandleTypeDef huart1;
 UART_HandleTypeDef huart2;
 UART_HandleTypeDef huart3;
 UART_HandleTypeDef huart4;
UART_HandleTypeDef huart5;

static uint8_t uart1_send_buf[512];
static uint8_t uart2_send_buf[128];
static uint8_t uart3_send_buf[128];
static uint8_t uart4_send_buf[128];
 
  uint8_t uart2_rec_buf[1532];
  uint8_t uart3_rec_buf[1532];
 extern __IO ITStatus init_flag;
 extern uint16_t bd1_rx_len;
 
 /* USER CODE BEGIN 1 */
#ifdef __GNUC__

  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
 set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else

  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
	
PUTCHAR_PROTOTYPE
{
	HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF);
	return ch;
}
 



// static uint16_t k = 0;
 
 
 /* USER CODE BEGIN 0 */

USART_RECEIVETYPE UsartType1;/*aoa ic for Z1, LTE module for B1*/
USART_RECEIVETYPE UsartType2;/*bd1*/
USART_RECEIVETYPE UsartType3;/*bd2*/
USART_RECEIVETYPE UsartType4;/*ble2640*/

/* USER CODE END 0 */
 
__IO FlagStatus Uart1RxReceived = RESET;
__IO FlagStatus Uart2RxReceived = RESET;
__IO FlagStatus Uart3RxReceived = RESET;
__IO FlagStatus Uart4RxReceived = RESET;


/**
  * @brief  Rx Transfer completed callback
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report end of DMA Rx transfer, and 
  *         you can add your own implementation.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  /* Set transmission flag: trasfer complete*/
  	if(UartHandle == &huart1)
	{
       	Uart1Ready = SET;
	}
	else if(UartHandle == &huart2)
	{
		Uart2Ready = SET;
	}
	else if(UartHandle == &huart3)
	{
		Uart3Ready = SET;
	}
	else if(UartHandle == &huart4)
	{
		Uart4Ready = SET;
	}
}

/**
  * @brief  UART error callbacks
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
{
    //Error_Handler();
}

/* USART1 init function */
static void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
   // Error_Handler();
  }
  
  
  	 /* USER CODE BEGIN 2 */
	HAL_UART_Receive_DMA(&huart1, UsartType1.usartDMA_rxBuf, RECEIVELEN);
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
  /* USER CODE END 2 */

}

/* USART2 init function */
 void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
   // Error_Handler();
  }

  	 /* USER CODE BEGIN 2 */
	HAL_UART_Receive_DMA(&huart2, UsartType2.usartDMA_rxBuf, RECEIVELEN);
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
//	__HAL_UART_DISABLE_IT(&huart2, UART_IT_TC);
	
  /* USER CODE END 2 */
  
}

 void MX_USART3_UART_Init(void)
{

  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
   // Error_Handler();
  }
  
  
  	 /* USER CODE BEGIN 2 */
	HAL_UART_Receive_DMA(&huart3, UsartType3.usartDMA_rxBuf, RECEIVELEN);
	__HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);
  /* USER CODE END 2 */

}


static void MX_USART4_UART_Init(void)
{

  huart4.Instance = UART4;
  huart4.Init.BaudRate = 115200;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  huart4.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart4.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
   // Error_Handler();
  }
  
  
  	 /* USER CODE BEGIN 2 */
	HAL_UART_Receive_DMA(&huart4, UsartType4.usartDMA_rxBuf, RECEIVELEN);
	__HAL_UART_ENABLE_IT(&huart4, UART_IT_IDLE);
  /* USER CODE END 2 */

}

void hw_uart_init(void)
{ 
  // uint8_t aa[4]="aaa";	
   MX_USART1_UART_Init();
   MX_USART2_UART_Init();
   MX_USART3_UART_Init();
   MX_USART4_UART_Init();
	
	//hw_uart_send(UART1,aa,4);
	printf("hw_uart_init \r\n");
}

void hw_uart_deinit(void)
{
	 HAL_UART_DeInit(&huart1);
	 HAL_UART_DeInit(&huart2);
	 HAL_UART_DeInit(&huart3);
	 HAL_UART_DeInit(&huart4);
}

uint8_t hw_uart1_send(uint8_t *tx_buf, uint16_t tx_len)
{
#if 1 //liumaobo for test, two branches are ok.
  while(UsartType1.dmaSend_flag == USART_DMA_SENDING);
	UsartType1.dmaSend_flag = USART_DMA_SENDING;
	memcpy(uart1_send_buf, tx_buf, tx_len);
	if(HAL_UART_Transmit_DMA(&huart1, uart1_send_buf, tx_len) != HAL_OK)
		return 1;
	return 0;
#else
	Uart1Ready=RESET;
	if(HAL_UART_Transmit_DMA(&huart1, (uint8_t*)tx_buf, tx_len)!= HAL_OK)
	{
	}
	while (Uart1Ready != SET)
	{
		//delay_ms(1);
	}
#endif
}
uint8_t hw_uart2_send_nodma(uint8_t *tx_buf, uint16_t tx_len)
{
			HAL_UART_Transmit(&huart2,tx_buf,tx_len, 0xFFFF);
}
uint8_t hw_uart2_send(uint8_t *tx_buf, uint16_t tx_len)
{

  while(UsartType2.dmaSend_flag == USART_DMA_SENDING);
	UsartType2.dmaSend_flag = USART_DMA_SENDING;
	memcpy(uart2_send_buf, tx_buf, tx_len);
	if(HAL_UART_Transmit_DMA(&huart2, uart2_send_buf, tx_len) != HAL_OK)
		return 1;
	return 0;
}

uint8_t hw_uart3_send(uint8_t *tx_buf, uint16_t tx_len)
{
  while(UsartType3.dmaSend_flag == USART_DMA_SENDING);
	UsartType3.dmaSend_flag = USART_DMA_SENDING;
	memcpy(uart3_send_buf, tx_buf, tx_len);
	if(HAL_UART_Transmit_DMA(&huart3, uart3_send_buf, tx_len) != HAL_OK)
		return 1;
	return 0;
}


uint8_t hw_uart4_send(uint8_t *tx_buf, uint16_t tx_len)
{
  while(UsartType4.dmaSend_flag == USART_DMA_SENDING);
	UsartType4.dmaSend_flag = USART_DMA_SENDING;

	memcpy(uart4_send_buf, tx_buf, tx_len);
	if(HAL_UART_Transmit_DMA(&huart4, uart4_send_buf, tx_len) != HAL_OK)
		return 1;
	return 0;
}


/*
//DMA���ͺ���
void Usart1SendData_DMA(uint8_t *pdata, uint16_t Length)
{
	while(UsartType1.dmaSend_flag == USART_DMA_SENDING);
	UsartType1.dmaSend_flag = USART_DMA_SENDING;
	HAL_UART_Transmit_DMA(&huart1, pdata, Length);
}
*/

//DMA��������жϻص�����
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
//void HAL_UART_TxHalfCpltCallback(UART_HandleTypeDef *huart)
{
	  /* Set transmission flag: trasfer complete*/
	if(huart == &huart1)
  	{
       //	__HAL_DMA_DISABLE(huart->hdmatx);
	   //   UsartType1.dmaSend_flag = USART_DMA_SENDOVER;

       	Uart1Ready = SET;
	   UsartType1.dmaSend_flag = USART_DMA_SENDOVER;
	}
	else if(huart == &huart2)
	{
		Uart2Ready = SET;
     //    __HAL_DMA_DISABLE(huart->hdmatx);
	      UsartType2.dmaSend_flag = USART_DMA_SENDOVER;
	}
	else if(huart == &huart3)
	{
		UsartType3.dmaSend_flag = USART_DMA_SENDOVER;
	}
	else if(huart == &huart4)
	{
		UsartType4.dmaSend_flag = USART_DMA_SENDOVER;
	}
}

//���ڽ��տ����ж�
void Usart1Receive_IDLE(UART_HandleTypeDef *huart)
{
	uint32_t temp = 0;
	
	if((__HAL_UART_GET_FLAG(&huart1,UART_FLAG_IDLE) != RESET))
	{ 
		__HAL_UART_CLEAR_IDLEFLAG(&huart1);
		HAL_UART_DMAStop(&huart1);
		temp = huart1.hdmarx->Instance->CNDTR;
		UsartType1.rx_len =  RECEIVELEN - temp; 
		UsartType1.receive_flag=1;
		Uart1RxReceived=SET;
		HAL_UART_Receive_DMA(&huart1,UsartType1.usartDMA_rxBuf,RECEIVELEN);
		HAL_UART_DMAResume(&huart1);
	//	printf("UsartType1.usartDMA_rxBuf %s \r\n",UsartType1.usartDMA_rxBuf);
	}
}

void Usart2Receive_IDLE(UART_HandleTypeDef *huart)
{
	uint32_t temp;
	// BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	if((__HAL_UART_GET_FLAG(&huart2,UART_FLAG_IDLE) != RESET))
	{ 
		__HAL_UART_CLEAR_IDLEFLAG(&huart2);
		HAL_UART_DMAStop(&huart2);
		temp = huart2.hdmarx->Instance->CNDTR;
		UsartType2.rx_len =  RECEIVELEN - temp; 
		UsartType2.receive_flag=1;
		//memset(UsartType2.usartDMA_rxBuf,0x00,RECEIVELEN);
		HAL_UART_Receive_DMA(&huart2,UsartType2.usartDMA_rxBuf,RECEIVELEN);
		memcpy(uart2_rec_buf,UsartType2.usartDMA_rxBuf,UsartType2.rx_len);
		bd1_rx_len = UsartType2.rx_len;
		HAL_UART_DMAResume(&huart2);
		send_notify_bd1_task();
		
	}
	
	
	// vTaskNotifyGiveFromISR(Bd1Handle,&xHigherPriorityTaskWoken );  
 
   // portYIELD_FROM_ISR(xHigherPriorityTaskWoken );  
}

void Usart3Receive_IDLE(UART_HandleTypeDef *huart)
{
	uint32_t temp;

	if((__HAL_UART_GET_FLAG(&huart3,UART_FLAG_IDLE) != RESET))
	{ 
		__HAL_UART_CLEAR_IDLEFLAG(&huart3);
		HAL_UART_DMAStop(&huart3);
		temp = huart3.hdmarx->Instance->CNDTR;
	//	printf("hdmarx->Instance->CNDTR = %d \r\n",huart3.hdmarx->Instance->CNDTR);
		UsartType3.rx_len =  RECEIVELEN - temp; 
		UsartType3.receive_flag=1;
		Uart3RxReceived=SET;
		HAL_UART_Receive_DMA(&huart3,UsartType3.usartDMA_rxBuf,RECEIVELEN);
		HAL_UART_DMAResume(&huart3);
	}
}

void Usart4Receive_IDLE(UART_HandleTypeDef *huart)
{
	uint32_t temp;

	if((__HAL_UART_GET_FLAG(&huart4,UART_FLAG_IDLE) != RESET))
	{ 
		__HAL_UART_CLEAR_IDLEFLAG(&huart4);
		HAL_UART_DMAStop(&huart4);
		temp = huart4.hdmarx->Instance->CNDTR;
		//printf("hdmarx->Instance->CNDTR = %d \r\n",huart4.hdmarx->Instance->CNDTR);
		UsartType4.rx_len =  RECEIVELEN - temp; 
		UsartType4.receive_flag=1;
		Uart4RxReceived=SET;
	
		//printf("uart4 UsartType4.usartDMA_rxBuf %s \r\n",UsartType4.usartDMA_rxBuf);
		HAL_UART_Receive_DMA(&huart4,UsartType4.usartDMA_rxBuf,RECEIVELEN);
		HAL_UART_DMAResume(&huart4);
		//printf("uart4 UsartType4.usartDMA_rxBuf %s \r\n",UsartType4.usartDMA_rxBuf);
	
	}

}

/* USER CODE END 1 */

uint8_t hw_uart1_receive(uint8_t *rx_buf,uint16_t *rx_len,uint32_t timeout)
{
	uint32_t i = 0;
  while (1)
  {
		if(UsartType1.receive_flag)//��������˿����ж�
		{
			//Usart1SendData_DMA(UsartType1.usartDMA_rxBuf,UsartType1.rx_len);//���ڴ�ӡ�յ������ݡ�
			memcpy(rx_buf,UsartType1.usartDMA_rxBuf,UsartType1.rx_len);
			//osDelay(10);
			*rx_len = UsartType1.rx_len;
			UsartType1.receive_flag=0;//������
			return 0;	
		}
		
    osDelay(1);
		
		i++;
		if(i >= timeout/1)
		{
			 // UsartType1.receive_flag = 0;
				return 1;
		}
		
	}
}

uint8_t hw_uart2_receive(uint8_t *rx_buf,uint16_t *rx_len,uint32_t timeout)
{
	uint32_t i = 0;
 // while (1)
  //{
		//if(UsartType2.receive_flag)//��������˿����ж�
		//{
			//Usart1SendData_DMA(UsartType1.usartDMA_rxBuf,UsartType1.rx_len);//���ڴ�ӡ�յ������ݡ�
			printf("UsartType2.usartDMA_rxBuf = %s \r\n",uart2_rec_buf);
			memcpy(rx_buf,uart2_rec_buf,UsartType2.rx_len);
			memset(uart2_rec_buf,0x00,1532);
			//osDelay(10);
			*rx_len = UsartType2.rx_len;
			UsartType2.receive_flag=0;//������
			return 0;	
	//	}
		
    //osDelay(1);
		
	//	i++;
		//if(i >= timeout/1)
		//{
			 // UsartType2.receive_flag = 0;
			//	return 1;
		//}
		
	//}
}

uint8_t hw_uart3_receive(uint8_t *rx_buf,uint16_t *rx_len,uint32_t timeout)
{
	uint32_t i = 0;
  while (1)
  {
		if(UsartType3.receive_flag)//��������˿����ж�
		{
			//Usart1SendData_DMA(UsartType1.usartDMA_rxBuf,UsartType1.rx_len);//���ڴ�ӡ�յ������ݡ�
			memcpy(rx_buf,UsartType3.usartDMA_rxBuf,UsartType3.rx_len);
			//osDelay(10);
			*rx_len = UsartType3.rx_len;
			//printf("UsartType3.rx_len = %d \r\n",UsartType3.rx_len);
			UsartType3.receive_flag=0;//������
			return 0;	
		}
		
    osDelay(1);
		
		i++;
		if(i >= timeout/1)
		{
				return 1;
		}
		
	}
}
/*
{
	uint32_t i = 0;

  	while (1)
  	{
		if(Uart3RxReceived)//��������˿����ж�
		{
			memcpy(rx_buf,UsartType3.usartDMA_rxBuf,UsartType3.rx_len);
			*rx_len = UsartType3.rx_len;
			UsartType3.receive_flag=0;//������
			Uart3RxReceived=RESET;
			return 0;
		}
	}
}*/

uint8_t hw_uart4_receive(uint8_t *rx_buf,uint16_t *rx_len,uint32_t timeout)
{
	uint32_t i = 0;
		
  while (1)
  {
		//printf("no receive return \r\n");
		if(UsartType4.receive_flag)//��������˿����ж�
		{
			//Usart1SendData_DMA(UsartType1.usartDMA_rxBuf,UsartType1.rx_len);//���ڴ�ӡ�յ������ݡ�
			
			//printf("UsartType4.receive_flag = %d UsartType4.rx_len = %d\r\n",UsartType4.receive_flag,UsartType4.rx_len);
			memcpy(rx_buf,UsartType4.usartDMA_rxBuf,UsartType4.rx_len);
			//osDelay(10);
			*rx_len = UsartType4.rx_len;
			//printf("rx len = %d \r\n",*rx_len);
			UsartType4.receive_flag=0;//������
			return 0;	
		}
		//printf("no receive return \r\n");
    osDelay(1);
		
		i++;
		if(i >= timeout/1)
		{
	//	printf("no receive return \r\n");
				return 1;
		}
		
	}
}


