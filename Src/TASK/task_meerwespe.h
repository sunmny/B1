#ifndef TASK_MEERWESPE_H
#define TASK_MEERWESPE_H
#include "cmsis_os.h"
extern osTimerId LteTimerHandle;
extern osTimerId BuzzerTimerHandle;
extern osTimerId PowerTimerHandle;
extern osTimerId LedTimerHandle;
extern osMutexId lcdMutexHandle;
extern osMutexId loraMutexHandle;
void task_init(void);







#endif
