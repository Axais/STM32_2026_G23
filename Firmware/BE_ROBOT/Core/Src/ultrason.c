#include "ultrason.h"
#include "gpio.h"

static TIM_HandleTypeDef *htim_ult;

void HCSR04_Init(TIM_HandleTypeDef *htim){
	htim_ult = htim;
	HAL_TIM_Base_Start(htim_ult);
}

uint32_t HCSR04_Read(void) {

    uint32_t valeur = 0;
	uint32_t Distance = 0;

    // 1. Envoi de l'impulsion TRIG (10us)
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
    HAL_Delay(0.01); // Note: HAL_Delay est en ms, pour 10us c'est imprécis mais souvent suffisant
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);

    // 2. Attente du début de l'ECHO
    while (!(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6)));

    // 3. Mesure de la durée
    __HAL_TIM_SET_COUNTER(htim_ult, 0);
    while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6));

    valeur = __HAL_TIM_GET_COUNTER(htim_ult);

    // 4. Calcul de la distance
   return Distance = valeur * 0.034 / 2;
}
