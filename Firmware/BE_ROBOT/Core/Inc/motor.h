#ifndef MOTOR_H_
#define MOTOR_H_

#include "stm32wbxx_hal.h"

// Définitions des vitesses
#define MOTOR_SPEED_DEFAULT 500
#define MOTOR_SPEED_STOP    0
#define MOTOR_MAX_PWM       999  // Basé sur votre Period (ARR) de 999

// Prototypes des fonctions
void Motor_Init(void);
void Motor_Forward(uint16_t speed);
void Motor_Backward(uint16_t speed);
void Motor_TurnLeft(uint16_t speed);
void Motor_TurnRight(uint16_t speed);
void Motor_Stop(void);
void Motor_SetSpeed(uint16_t speedG, uint16_t speedD);

#endif /* MOTOR_H_ */
