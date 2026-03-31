#include "oled.h"
#include "Font.h"
#include <string.h>

#define OLED_I2C_ADDR   0x78

static void OLED_WR_Byte(OLED_t *dev, uint8_t dat, uint8_t cmd)
{
    uint8_t buf[2];
    buf[0] = cmd ? 0x40 : 0x00;
    buf[1] = dat;
    HAL_I2C_Master_Transmit(dev->hi2c, OLED_I2C_ADDR, buf, 2, 100);
}

void OLED_Init(OLED_t *dev, I2C_HandleTypeDef *hi2c)
{
    dev->hi2c = hi2c;
    memset(dev->GRAM, 0, sizeof(dev->GRAM));

    HAL_Delay(200);

    OLED_WR_Byte(dev, 0xAE, 0);
    OLED_WR_Byte(dev, 0x00, 0);
    OLED_WR_Byte(dev, 0x10, 0);
    OLED_WR_Byte(dev, 0x40, 0);
    OLED_WR_Byte(dev, 0x81, 0);
    OLED_WR_Byte(dev, 0xCF, 0);
    OLED_WR_Byte(dev, 0xA1, 0);
    OLED_WR_Byte(dev, 0xC8, 0);
    OLED_WR_Byte(dev, 0xA6, 0);
    OLED_WR_Byte(dev, 0xA8, 0);
    OLED_WR_Byte(dev, 0x3f, 0);
    OLED_WR_Byte(dev, 0xD3, 0);
    OLED_WR_Byte(dev, 0x00, 0);
    OLED_WR_Byte(dev, 0xd5, 0);
    OLED_WR_Byte(dev, 0x80, 0);
    OLED_WR_Byte(dev, 0xD9, 0);
    OLED_WR_Byte(dev, 0xF1, 0);
    OLED_WR_Byte(dev, 0xDA, 0);
    OLED_WR_Byte(dev, 0x12, 0);
    OLED_WR_Byte(dev, 0xDB, 0);
    OLED_WR_Byte(dev, 0x30, 0);
    OLED_WR_Byte(dev, 0x20, 0);
    OLED_WR_Byte(dev, 0x02, 0);
    OLED_WR_Byte(dev, 0x8D, 0);
    OLED_WR_Byte(dev, 0x14, 0);
    OLED_WR_Byte(dev, 0xAF, 0);
}

void OLED_Clear(OLED_t *dev)
{
    memset(dev->GRAM, 0, sizeof(dev->GRAM));
}

void OLED_Refresh(OLED_t *dev)
{
    uint8_t i, n;
    for (i = 0; i < 8; i++) {
        OLED_WR_Byte(dev, 0xb0 + i, 0);
        OLED_WR_Byte(dev, 0x00, 0);
        OLED_WR_Byte(dev, 0x10, 0);
        for (n = 0; n < 128; n++) {
            OLED_WR_Byte(dev, dev->GRAM[n][i], 1);
        }
    }
}

void OLED_DrawPoint(OLED_t *dev, uint8_t x, uint8_t y, uint8_t t)
{
    uint8_t i, m, n;
    i = y / 8;
    m = y % 8;
    n = 1 << m;
    if (t) {
        dev->GRAM[x][i] |= n;
    } else {
        dev->GRAM[x][i] = ~dev->GRAM[x][i];
        dev->GRAM[x][i] |= n;
        dev->GRAM[x][i] = ~dev->GRAM[x][i];
    }
}

void OLED_ShowChar(OLED_t *dev, uint8_t x, uint8_t y, uint8_t chr)
{
    uint8_t i, m, temp;
    uint8_t x0 = x, y0 = y;
    
    chr = chr - ' ';
    
    for (i = 0; i < 8; i++) {
        temp = F8X16[chr][i];
        for (m = 0; m < 8; m++) {
            if (temp & 0x01) OLED_DrawPoint(dev, x, y, 1);
            else OLED_DrawPoint(dev, x, y, 0);
            temp >>= 1;
            y++;
        }
        x++;
        y = y0;
    }
    
    y0 += 8;
    y = y0;
    for (i = 8; i < 16; i++) {
        temp = F8X16[chr][i];
        for (m = 0; m < 8; m++) {
            if (temp & 0x01) OLED_DrawPoint(dev, x0 + i - 8, y, 1);
            else OLED_DrawPoint(dev, x0 + i - 8, y, 0);
            temp >>= 1;
            y++;
        }
        y = y0;
    }
}

void OLED_ShowString(OLED_t *dev, uint8_t x, uint8_t y, char *str)
{
    while ((*str >= ' ') && (*str <= '~')) {
        OLED_ShowChar(dev, x, y, *str);
        x += 8;
        str++;
    }
}
