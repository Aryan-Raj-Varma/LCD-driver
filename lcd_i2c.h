#ifndef INC_LCD_I2C_H_
#define INC_LCD_I2C_H_

#include <stdint.h>
#include "stm32f1xx_hal.h"       // ✅ HAL + HAL_Delay()

/* Change if your PCF8574 address is different */
#define LCD_I2C_ADDR   (0x27 << 1)

/* Public API */
void LCD_Init(void);
void LCD_Clear(void);
void LCD_SetCursor(uint8_t row, uint8_t col);

void LCD_WriteChar(char c);
void LCD_WriteString(char *str);
void LCD_WriteInt(int num);
void LCD_WriteFloat(float num, uint8_t precision);
void LCD_WriteHex(uint32_t num);
void LCD_BounceText(char *str, uint8_t row, uint16_t delay_ms);


#endif /* INC_LCD_I2C_H_ */
