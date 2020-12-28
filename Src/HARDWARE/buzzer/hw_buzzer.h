#ifndef HARDWARE_BUZZER_H
#define HARDWARE_BUZZER_H

#include "stm32l4xx_hal.h"



extern uint8_t hw_buzzer_get_status(void);
extern void hw_buzzer_enable(uint8_t enable);
#endif
