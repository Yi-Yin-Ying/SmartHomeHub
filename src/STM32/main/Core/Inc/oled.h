#ifndef __OLED_H__
#define __OLED_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include <stdint.h>

#define OLED_WIDTH            128
#define OLED_HEIGHT           64

typedef struct {
    uint8_t GRAM[OLED_WIDTH][8];
    I2C_HandleTypeDef *hi2c;
} OLED_t;

void OLED_Init(OLED_t *dev, I2C_HandleTypeDef *hi2c);
void OLED_Clear(OLED_t *dev);
void OLED_Refresh(OLED_t *dev);
void OLED_ShowChar(OLED_t *dev, uint8_t x, uint8_t y, uint8_t chr);
void OLED_ShowString(OLED_t *dev, uint8_t x, uint8_t y, char *str);

#ifdef __cplusplus
}
#endif

#endif /* __OLED_H__ */
