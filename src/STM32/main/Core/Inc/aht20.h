#ifndef __AHT20_H__
#define __AHT20_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include <stdint.h>

#define AHT20_I2C_ADDR      0x38

typedef struct {
    I2C_HandleTypeDef *hi2c;
    float temperature;
    float humidity;
} AHT20_t;

void AHT20_Init(AHT20_t *dev, I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef AHT20_TriggerMeasurement(AHT20_t *dev);
HAL_StatusTypeDef AHT20_ReadData(AHT20_t *dev);
float AHT20_GetTemperature(AHT20_t *dev);
float AHT20_GetHumidity(AHT20_t *dev);

#ifdef __cplusplus
}
#endif

#endif /* __AHT20_H__ */
