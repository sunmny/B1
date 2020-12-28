#ifndef HARDWARE_UART_H
#define HARDWARE_UART_H

#include <stdint.h>
#include "stm32l4xx_hal.h"

#define UART1  0
#define UART2  1


/* USER CODE BEGIN Private defines */
#define RECEIVELEN 1536
#define USART_DMA_SENDING 1//����δ���
#define USART_DMA_SENDOVER 0//�������
typedef struct
{
uint8_t receive_flag:1;//���н��ձ��
uint8_t dmaSend_flag:1;//������ɱ��
uint16_t rx_len;//���ճ���
uint8_t usartDMA_rxBuf[RECEIVELEN];//DMA���ջ���
}USART_RECEIVETYPE;
extern USART_RECEIVETYPE UsartType1;
extern USART_RECEIVETYPE UsartType2;
extern USART_RECEIVETYPE UsartType3;
extern USART_RECEIVETYPE UsartType4;
/* USER CODE END Private defines */







void hw_uart_init(void);
void hw_uart_deinit(void);

uint8_t hw_uart1_send(uint8_t *tx_buf, uint16_t tx_len);
uint8_t hw_uart2_send(uint8_t *tx_buf, uint16_t tx_len);
uint8_t hw_uart3_send(uint8_t *tx_buf, uint16_t tx_len);
uint8_t hw_uart4_send(uint8_t *tx_buf, uint16_t tx_len);


uint8_t hw_uart1_receive(uint8_t *rx_buf,uint16_t *rx_len,uint32_t timeout);
uint8_t hw_uart2_receive(uint8_t *rx_buf,uint16_t *rx_len,uint32_t timeout);
uint8_t hw_uart3_receive(uint8_t *rx_buf,uint16_t *rx_len,uint32_t timeout);
uint8_t hw_uart4_receive(uint8_t *rx_buf,uint16_t *rx_len,uint32_t timeout);

	
void Usart1Receive_IDLE(UART_HandleTypeDef *huart);
void Usart2Receive_IDLE(UART_HandleTypeDef *huart);
void Usart3Receive_IDLE(UART_HandleTypeDef *huart);
void Usart4Receive_IDLE(UART_HandleTypeDef *huart);

#endif
