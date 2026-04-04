#include "ultrasonic.h"
#include "tim.h"
// Prototype d'une fonction d'attente microseconde (à implémenter via un Timer)
extern void delay_us(uint16_t us);

void Ultrasonic_Init(Ultrasonic_t* dev, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
    dev->GPIOx = GPIOx;
    dev->GPIO_Pin = GPIO_Pin;
}

void Set_Pin_Output(Ultrasonic_t* dev) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = dev->GPIO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(dev->GPIOx, &GPIO_InitStruct);
}

void Set_Pin_Input(Ultrasonic_t* dev) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = dev->GPIO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(dev->GPIOx, &GPIO_InitStruct);
}


long Ultrasonic_MeasureInCentimeters(Ultrasonic_t* dev, uint32_t timeout) {

    Set_Pin_Output(dev);
    HAL_GPIO_WritePin(dev->GPIOx, dev->GPIO_Pin, GPIO_PIN_RESET);
    delay_us(2);
    HAL_GPIO_WritePin(dev->GPIOx, dev->GPIO_Pin, GPIO_PIN_SET);
    delay_us(10);
    HAL_GPIO_WritePin(dev->GPIOx, dev->GPIO_Pin, GPIO_PIN_RESET);

    Set_Pin_Input(dev);

    // Enregistre le temps actuel en millisecondes pour éviter un blocage infini
    uint32_t start_wait = HAL_GetTick();

    while (HAL_GPIO_ReadPin(dev->GPIOx, dev->GPIO_Pin) == GPIO_PIN_RESET) {
        // Si le capteur ne répond pas après 10ms, on sort avec une erreur (0)
        if ((HAL_GetTick() - start_wait) > 10) return 0;
    }

    // Remet le compteur du Timer à zéro pour commencer la mesure précise
    __HAL_TIM_SET_COUNTER(&htim1, 0);

    // Boucle tant que la broche est à l'état HAUT (le signal est en cours)
    while (HAL_GPIO_ReadPin(dev->GPIOx, dev->GPIO_Pin) == GPIO_PIN_SET) {
        // Si la durée dépasse la limite définie par l'utilisateur, on annule
        if (__HAL_TIM_GET_COUNTER(&htim1) > timeout) return 0;
    }

    // Récupère la valeur finale du compteur (durée de l'impulsion en microsecondes)
    uint32_t duration = __HAL_TIM_GET_COUNTER(&htim1);

    // Convertit le temps en distance : (Vitesse du son * temps) / 2
    // En microsecondes, diviser par 58 est le raccourci standard pour obtenir des cm
    return  duration *(0.000001*33000) / 2;
}

