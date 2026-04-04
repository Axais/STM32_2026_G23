/*
 * sht31.c
 *
 * Created on: Feb 5, 2026
 * Author: jaloz
 * Description: Pilote pour le capteur d'humidité et de température SHT31 via I2C.
 */

#include "sht31.h"



HAL_StatusTypeDef SHT31_Init(I2C_HandleTypeDef *hi2c)
{
    // Commande de réinitialisation logicielle (Soft Reset)
    uint8_t cmd[2] = {0x30, 0xA2};

    // Transmission de la commande au capteur
    return HAL_I2C_Master_Transmit(hi2c, SHT31_ADDR, cmd, 2, HAL_MAX_DELAY);
}


HAL_StatusTypeDef SHT31_Read(I2C_HandleTypeDef *hi2c, float *temperature, float *humidity)
{
    // Commande : Mesure "Single Shot" avec répétabilité élevée (Clock Stretching désactivé)
    uint8_t cmd[2]  = {0x24, 0x00};
    uint8_t data[6]; // Buffer : [T_MSB, T_LSB, T_CRC, H_MSB, H_LSB, H_CRC]

    // 1. Envoyer la commande de déclenchement de mesure
    if (HAL_I2C_Master_Transmit(hi2c, SHT31_ADDR, cmd, 2, HAL_MAX_DELAY) != HAL_OK)
    {
        return HAL_ERROR;
    }

    // 2. Attendre la fin de la conversion (le mode haute précision prend ~15ms)
    HAL_Delay(15);

    // 3. Lire les 6 octets de données (Données + CRC)
    if (HAL_I2C_Master_Receive(hi2c, SHT31_ADDR, data, 6, HAL_MAX_DELAY) != HAL_OK)
    {
        return HAL_ERROR;
    }

    // 4. Recomposition des valeurs brutes (Assemblage de deux octets de 8 bits en un 16 bits)
    uint16_t rawT = (data[0] << 8) | data[1];
    uint16_t rawH = (data[3] << 8) | data[4];

    // 5. Conversion des valeurs brutes en unités physiques selon la datasheet
    // Formule T : -45 + 175 * (S_raw / (2^16 - 1))
    *temperature = -45.0f + 175.0f * ((float)rawT / 65535.0f);

    // Formule H : 100 * (S_raw / (2^16 - 1))
    *humidity    = 100.0f * ((float)rawH / 65535.0f);

    return HAL_OK;
}
