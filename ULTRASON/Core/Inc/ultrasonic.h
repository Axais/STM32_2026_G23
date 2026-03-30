#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include "stm32l4xx_hal.h"
#include <stdint.h>

typedef struct {
    GPIO_TypeDef* GPIOx;
    uint16_t GPIO_Pin;
} Ultrasonic_t;

/* Initialisation */
void Ultrasonic_Init(Ultrasonic_t* dev, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void Set_Pin_Output(Ultrasonic_t* dev);
void Set_Pin_Input(Ultrasonic_t* dev);

/* Mesures */
long Ultrasonic_MeasureInCentimeters(Ultrasonic_t* dev, uint32_t timeout);

#endif




