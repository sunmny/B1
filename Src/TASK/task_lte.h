#ifndef TASK_LTE_H
#define TASK_LTE_H

#include "stdint.h"
#include "cmsis_os.h"

void LteTask(void const * argument);

osStatus task_lte_message_put(uint32_t info);

#endif
