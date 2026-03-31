#include "aht20.h"

#define AHT20_CMD_INIT       0xBE
#define AHT20_CMD_TRIGGER    0xAC
#define AHT20_CMD_SOFTRESET  0xBA

#define AHT20_STATUS_BUSY    0x80
#define AHT20_STATUS_CALIB   0x08

static HAL_StatusTypeDef AHT20_Write(AHT20_t *dev, uint8_t *data, uint16_t size)
{
    return HAL_I2C_Master_Transmit(dev->hi2c, AHT20_I2C_ADDR << 1, data, size, 100);
}

static HAL_StatusTypeDef AHT20_Read(AHT20_t *dev, uint8_t *data, uint16_t size)
{
    return HAL_I2C_Master_Receive(dev->hi2c, AHT20_I2C_ADDR << 1, data, size, 100);
}

void AHT20_Init(AHT20_t *dev, I2C_HandleTypeDef *hi2c)
{
    dev->hi2c = hi2c;
    dev->temperature = 0.0f;
    dev->humidity = 0.0f;

    HAL_Delay(40);

    uint8_t cmd[3] = {AHT20_CMD_INIT, 0x08, 0x00};
    AHT20_Write(dev, cmd, 3);

    HAL_Delay(10);
}

HAL_StatusTypeDef AHT20_TriggerMeasurement(AHT20_t *dev)
{
    uint8_t cmd[3] = {AHT20_CMD_TRIGGER, 0x33, 0x00};
    return AHT20_Write(dev, cmd, 3);
}

HAL_StatusTypeDef AHT20_ReadData(AHT20_t *dev)
{
    uint8_t data[7];
    HAL_StatusTypeDef status;

    status = AHT20_Read(dev, data, 7);
    if (status != HAL_OK) {
        return status;
    }

    if (data[0] & AHT20_STATUS_BUSY) {
        return HAL_BUSY;
    }

    uint32_t humidity_raw = ((uint32_t)data[1] << 12) | ((uint32_t)data[2] << 4) | (data[3] >> 4);
    uint32_t temp_raw = (((uint32_t)data[3] & 0x0F) << 16) | ((uint32_t)data[4] << 8) | data[5];

    dev->humidity = (float)humidity_raw * 100.0f / 1048576.0f;
    dev->temperature = (float)temp_raw * 200.0f / 1048576.0f - 50.0f;

    return HAL_OK;
}

float AHT20_GetTemperature(AHT20_t *dev)
{
    return dev->temperature;
}

float AHT20_GetHumidity(AHT20_t *dev)
{
    return dev->humidity;
}
