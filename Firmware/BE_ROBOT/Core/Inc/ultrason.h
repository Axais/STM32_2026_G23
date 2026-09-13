#ifndef ULTRASON_H
#define ULTRASON_H

#include "stm32wbxx_hal.h"
#include <stdint.h>

void HCSR04_Init(TIM_HandleTypeDef *htim);
uint32_t HCSR04_Read (void);

#endif




