#include "motor.h"
#include "tim.h"   // Pour accéder à htim1 et htim2 [cite: 14]
#include "main.h"  // Pour les labels MOT_G_DIR et MOT_D_DIR

/**
 * @brief Initialise et démarre les signaux PWM
 */
void Motor_Init(void) {
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1); // Moteur Gauche (PA15)
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); // Moteur Droit (PA8)
    Motor_Stop();
}

/**
 * @brief Ajuste la vitesse PWM des deux moteurs
 */
void Motor_SetSpeed(uint16_t speedG, uint16_t speedD) {
    if (speedG > MOTOR_MAX_PWM) speedG = MOTOR_MAX_PWM;
    if (speedD > MOTOR_MAX_PWM) speedD = MOTOR_MAX_PWM;

    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, speedG);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, speedD);
}

void Motor_Forward(uint16_t speed) {
    // Moteur Gauche (PC0, PC1)
    HAL_GPIO_WritePin(GPIOC, MOT_G_DIR1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC, MOT_G_DIR2_Pin, GPIO_PIN_SET);
    // Moteur Droit (PA1, PA0)
    HAL_GPIO_WritePin(GPIOA, MOT_D_DIR1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, MOT_D_DIR2_Pin, GPIO_PIN_SET);
    Motor_SetSpeed(speed, speed);
}

void Motor_Backward(uint16_t speed) {
    HAL_GPIO_WritePin(GPIOC, MOT_G_DIR1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOC, MOT_G_DIR2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, MOT_D_DIR1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, MOT_D_DIR2_Pin, GPIO_PIN_RESET);
    Motor_SetSpeed(speed, speed);
}

void Motor_TurnLeft(uint16_t speed) {
    // Rotation sur place : Gauche recule, Droite avance
    HAL_GPIO_WritePin(GPIOC, MOT_G_DIR1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOC, MOT_G_DIR2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, MOT_D_DIR1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, MOT_D_DIR2_Pin, GPIO_PIN_SET);
    Motor_SetSpeed(speed, speed);
}

void Motor_TurnRight(uint16_t speed) {
    // Rotation sur place : Gauche avance, Droite recule
    HAL_GPIO_WritePin(GPIOC, MOT_G_DIR1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC, MOT_G_DIR2_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, MOT_D_DIR1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, MOT_D_DIR2_Pin, GPIO_PIN_RESET);
    Motor_SetSpeed(speed, speed);
}

void Motor_Stop(void) {
    Motor_SetSpeed(0, 0);
}
