#ifndef INC_INFRAROUGE_H_
#define INC_INFRAROUGE_H_

#include "stm32wbxx_hal.h"

/**
 * @brief Structure stockant l'état des 5 capteurs infrarouges (ITR20001/T)
 * Les membres L2 à R2 sont des états logiques (1: Noir, 0: Blanc)
 */
typedef struct {
    uint8_t L2, L1, M, R1, R2;
    uint16_t raw[5];           // Valeurs brutes lues via SPI (0-1023)
} IR_Sensors_t;

/* Prototypes des fonctions exportées */
void IR_Init(void);
void IR_Read_All(IR_Sensors_t *sensors);

#endif /* INC_INFRAROUGE_H_ */
