#include "lcd_i2c.h"
#include "stm32f4xx_hal.h"

void LCD_Send_Cmd(I2C_HandleTypeDef *hi2c, uint8_t cmd) {
    uint8_t data_u = (cmd & 0xF0);
    uint8_t data_l = ((cmd << 4) & 0xF0);
    uint8_t data_t[4] = {data_u | 0x0C, data_u | 0x08, data_l | 0x0C, data_l | 0x08};
    HAL_I2C_Master_Transmit(hi2c, LCD_ADDR, data_t, 4, 100);
}

void LCD_Send_Data(I2C_HandleTypeDef *hi2c, uint8_t data) {
    uint8_t data_u = (data & 0xF0);
    uint8_t data_l = ((data << 4) & 0xF0);
    uint8_t data_t[4] = {data_u | 0x0D, data_u | 0x09, data_l | 0x0D, data_l | 0x09};
    HAL_I2C_Master_Transmit(hi2c, LCD_ADDR, data_t, 4, 100);
}

void LCD_Send_String(I2C_HandleTypeDef *hi2c, char *str) {
    while (*str) {
        LCD_Send_Data(hi2c, (uint8_t)(*str));
        str++;
    }
}

void LCD_Clear(I2C_HandleTypeDef *hi2c) {
    LCD_Send_Cmd(hi2c, 0x01);
    HAL_Delay(2);
}

void LCD_Init(I2C_HandleTypeDef *hi2c) {
    HAL_Delay(50);
    LCD_Send_Cmd(hi2c, 0x30);
    HAL_Delay(5);
    LCD_Send_Cmd(hi2c, 0x30);
    HAL_Delay(1);
    LCD_Send_Cmd(hi2c, 0x30);
    HAL_Delay(10);
    LCD_Send_Cmd(hi2c, 0x20);
    HAL_Delay(10);

    LCD_Send_Cmd(hi2c, 0x28);
    HAL_Delay(1);
    LCD_Send_Cmd(hi2c, 0x08);
    HAL_Delay(1);
    LCD_Send_Cmd(hi2c, 0x01);
    HAL_Delay(2);
    LCD_Send_Cmd(hi2c, 0x06);
    HAL_Delay(1);
    LCD_Send_Cmd(hi2c, 0x0C);
    HAL_Delay(1);
}

void LCD_Set_Cursor(I2C_HandleTypeDef *hi2c, uint8_t row, uint8_t col) {
    uint8_t row_offsets[] = {0x80, 0xC0};
    LCD_Send_Cmd(hi2c, row_offsets[row] + col);
}

void LCD_Scroll_Right(I2C_HandleTypeDef *hi2c, uint8_t steps, uint16_t delay) {
    for (uint8_t i = 0; i < steps; i++) {
        LCD_Send_Cmd(hi2c, 0x1C);
        HAL_Delay(delay);
    }
}