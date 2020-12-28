#ifndef APP_LTE_H
#define APP_LTE_H

#include <stdint.h>
#include "stm32l4xx_hal.h"

#define LTE_DATA_LOOP_SIZE 1024
extern uint8_t lte_uart_data_loop[LTE_DATA_LOOP_SIZE+1];

typedef struct {
    uint8_t *pData;
}lte_at_symbol;


extern void lte_receive_and_handle(void);
extern void lte_send_data(void);
#endif
