#include "lcd_i2c.h"
#include <stdio.h>
#include <stdint.h>

/* I2C handle from main.c */
extern I2C_HandleTypeDef hi2c1;

/* PCF8574 pin mapping */
#define LCD_RS  0x01
#define LCD_RW  0x02
#define LCD_EN  0x04
#define LCD_BL  0x08

/* ---------- LOW LEVEL ---------- */

static void PCF8574_Write(uint8_t data)
{
    HAL_I2C_Master_Transmit(&hi2c1, LCD_I2C_ADDR, &data, 1, 100);
}

static void LCD_Enable(uint8_t data)
{
    PCF8574_Write(data | LCD_EN);
    HAL_Delay(1);
    PCF8574_Write(data & ~LCD_EN);
    HAL_Delay(1);
}

static void LCD_Send(uint8_t data, uint8_t rs)
{
    uint8_t high = data & 0xF0;
    uint8_t low  = (data << 4) & 0xF0;

    PCF8574_Write(high | rs | LCD_BL);
    LCD_Enable(high | rs | LCD_BL);

    PCF8574_Write(low | rs | LCD_BL);
    LCD_Enable(low | rs | LCD_BL);
}

/* ---------- CORE LCD ---------- */

void LCD_Init(void)
{
    HAL_Delay(50);

    LCD_Send(0x33, 0);
    LCD_Send(0x32, 0);
    LCD_Send(0x28, 0);
    LCD_Send(0x0C, 0);
    LCD_Send(0x06, 0);
    LCD_Send(0x01, 0);

    HAL_Delay(2);
}

void LCD_Clear(void)
{
    LCD_Send(0x01, 0);
    HAL_Delay(2);
}

void LCD_SetCursor(uint8_t row, uint8_t col)
{
    uint8_t addr = (row == 0) ? (0x80 + col) : (0xC0 + col);
    LCD_Send(addr, 0);
}

/* ---------- WRITE FUNCTIONS ---------- */

void LCD_WriteChar(char c)
{
    LCD_Send((uint8_t)c, LCD_RS);
}

void LCD_WriteString(char *str)
{
    while (*str)
    {
        LCD_WriteChar(*str++);
    }
}

void LCD_WriteInt(int num)
{
    char buf[12];
    sprintf(buf, "%d", num);
    LCD_WriteString(buf);
}

void LCD_WriteFloat(float num, uint8_t precision)
{
    char buf[20];
    char fmt[8];

    sprintf(fmt, "%%.%df", precision);
    sprintf(buf, fmt, num);

    LCD_WriteString(buf);
}

void LCD_WriteHex(uint32_t num)
{
    char buf[12];
    sprintf(buf, "0x%lX", num);
    LCD_WriteString(buf);
}

void LCD_BounceText(char *str, uint8_t row, uint16_t delay_ms)
{
    uint8_t len = strlen(str);
    if (len > 16) len = 16;

    uint8_t prev_pos = 0;

    // Move Right
    for (uint8_t pos = 0; pos <= (16 - len); pos++)
    {
        // Erase previous position
        LCD_SetCursor(row, prev_pos);
        for (uint8_t i = 0; i < len; i++)
            LCD_WriteChar(' ');

        // Write new position
        LCD_SetCursor(row, pos);
        LCD_WriteString(str);

        prev_pos = pos;
        HAL_Delay(delay_ms);
    }

    // Move Left
    for (int pos = (16 - len); pos >= 0; pos--)
    {
        LCD_SetCursor(row, prev_pos);
        for (uint8_t i = 0; i < len; i++)
            LCD_WriteChar(' ');

        LCD_SetCursor(row, pos);
        LCD_WriteString(str);

        prev_pos = pos;
        HAL_Delay(delay_ms);
    }
}


