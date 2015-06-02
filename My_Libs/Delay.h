#include "stm32f10x.h"
#define SYSTEMTICK_PERIOD_MS  10
__IO uint32_t LocalTime = 0; /* this variable is used to create a time reference incremented by 10ms */
uint32_t timingdelay;
void Delay();

