#include "infrarouge.h"
#include "spi.h"
#include "gpio.h"

/* * Configuration spécifique P-NUCLEO-WB55 / AlphaBot2
 * D10 correspond à PA4 (Signal CS pour le TLC1543)
 */
#define IR_CS_PORT  GPIOA
#define IR_CS_PIN   GPIO_PIN_4
#define IR_THRESHOLD 600u  // Seuil pour détecter le noir (à ajuster)

/**
 * @brief Initialise les capteurs (force l'état haut du CS)
 */
void IR_Init(void) {
    HAL_GPIO_WritePin(IR_CS_PORT, IR_CS_PIN, GPIO_PIN_SET);
}

/**
 * @brief Lit un canal spécifique du TLC1543
 */
static uint16_t IR_Read_Single_Channel(uint8_t channel) {
    uint8_t txData[2];
    uint8_t rxData[2] = {0, 0};
    uint16_t value = 0;

    // L'adresse du canal doit être sur les 4 bits de poids fort
    txData[0] = (channel << 4);
    txData[1] = 0x00;

    // 1. Activer le CS (D10 / PA4) -> Passage à 0V
    HAL_GPIO_WritePin(IR_CS_PORT, IR_CS_PIN, GPIO_PIN_RESET);

    // Petite latence pour laisser le hardware se stabiliser
    for(volatile int d=0; d<50; d++);

    // 2. Échange SPI (Génère le signal d'horloge SCK sur D13/PA5)
    // On envoie l'adresse et on récupère la valeur de la lecture précédente
    if (HAL_SPI_TransmitReceive(&hspi1, txData, rxData, 2, 10) == HAL_OK) {
        // Le TLC1543 renvoie 10 bits de données
        value = ((rxData[0] & 0x0F) << 6) | (rxData[1] >> 2);
    }

    // 3. Désactiver le CS (D10 / PA4) -> Retour à 3.3V
    HAL_GPIO_WritePin(IR_CS_PORT, IR_CS_PIN, GPIO_PIN_SET);

    // Temps de conversion requis par le TLC1543
    HAL_Delay(1);

    return value;
}

/**
 * @brief Met à jour tous les capteurs
 */
void IR_Read_All(IR_Sensors_t *sensors) {
    // On effectue 6 lectures car le TLC1543 renvoie la valeur du cycle N-1
    // La première lecture (dummy) amorce le convertisseur
    for (uint8_t i = 0; i < 5; i++) {
        sensors->raw[i] = IR_Read_Single_Channel(i);
    }

    // Mise à jour des états logiques (1 = Noir, 0 = Blanc)
    sensors->L2 = (sensors->raw[0] > IR_THRESHOLD);
    sensors->L1 = (sensors->raw[1] > IR_THRESHOLD);
    sensors->M  = (sensors->raw[2] > IR_THRESHOLD);
    sensors->R1 = (sensors->raw[3] > IR_THRESHOLD);
    sensors->R2 = (sensors->raw[4] > IR_THRESHOLD);
}

