# lcd-i2c-driver

A lightweight STM32 HAL-based driver for controlling 16x2 LCD displays via the PCF8574 I2C I/O expander. Supports 4-bit mode communication, basic display control, and utility write functions.

---

## Features

- 4-bit LCD communication over I2C using PCF8574 expander
- Display initialization, clear, and cursor positioning
- Write functions for characters, strings, integers, floats, and hex values
- Animated bounce text effect

---

## Hardware Requirements

| Component | Details |
|---|---|
| MCU | STM32 (any with HAL I2C support) |
| LCD | 16x2 HD44780-compatible |
| I2C Expander | PCF8574 |
| Interface | I2C (default: `hi2c1`) |

### PCF8574 Pin Mapping

| PCF8574 Bit | LCD Pin | Description |
|---|---|---|
| `0x01` | RS | Register Select |
| `0x02` | RW | Read/Write |
| `0x04` | EN | Enable |
| `0x08` | BL | Backlight |

---

## File Structure

```
lcd-i2c-driver/
├── lcd_i2c.c       # Driver implementation
├── lcd_i2c.h       # Header file with function declarations and config
└── README.md
```

---

## Configuration

In `lcd_i2c.h`, set the I2C address of your PCF8574 module:

```c
#define LCD_I2C_ADDR   0x4E   // Default for PCF8574 (0x27 << 1)
```

The driver uses `hi2c1` by default (declared as `extern` in `lcd_i2c.c`). Change this in the source file if you use a different I2C peripheral.

---

## API Reference

### Initialization & Control

```c
void LCD_Init(void);
```
Initializes the LCD in 4-bit mode. Call once after system boot. Internally sequences the standard HD44780 initialization commands.

```c
void LCD_Clear(void);
```
Clears the display and resets the cursor to home position.

```c
void LCD_SetCursor(uint8_t row, uint8_t col);
```
Moves the cursor to the specified row (0 or 1) and column (0–15).

---

### Write Functions

```c
void LCD_WriteChar(char c);
```
Writes a single character at the current cursor position.

```c
void LCD_WriteString(char *str);
```
Writes a null-terminated string starting at the current cursor position.

```c
void LCD_WriteInt(int num);
```
Writes a signed integer value to the display.

```c
void LCD_WriteFloat(float num, uint8_t precision);
```
Writes a floating-point number with the specified number of decimal places.

```c
void LCD_WriteHex(uint32_t num);
```
Writes a 32-bit unsigned integer in `0xHEX` format.

---

### Animation

```c
void LCD_BounceText(char *str, uint8_t row, uint16_t delay_ms);
```
Animates text by sliding it left and right across the specified row. Strings longer than 16 characters are truncated.

---

## Usage Example

```c
#include "lcd_i2c.h"

int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_I2C1_Init();

    LCD_Init();

    // Display a string
    LCD_SetCursor(0, 0);
    LCD_WriteString("Hello, World!");

    // Display a float on row 2
    LCD_SetCursor(1, 0);
    LCD_WriteFloat(3.14159, 2);   // Displays: 3.14

    // Bounce animation
    while (1) {
        LCD_BounceText("STM32 LCD", 0, 200);
    }
}
```

---

## Notes

- The driver does not implement the **Read/Write (RW)** line; it is always pulled low (write-only mode).
- `HAL_Delay()` is used for timing — ensure SysTick is initialized before calling `LCD_Init()`.
- Backlight is always enabled (`LCD_BL` is ORed into every write).

---

## License

MIT License. Free to use, modify, and distribute.

---

## Author

**Aryan** — February 2026
