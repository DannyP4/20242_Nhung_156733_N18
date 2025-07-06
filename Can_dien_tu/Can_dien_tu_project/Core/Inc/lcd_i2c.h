#ifndef __LCD_I2C_H
#define __LCD_I2C_H

#ifdef __cplusplus
extern "C" {
#endif


#include "main.h"

// Địa chỉ mặc định của LCD I2C
#define LCD_ADDR        (0x27 << 1)  // Địa chỉ I2C dịch trái 1 bit
#define LCD_BACKLIGHT   0x08
#define LCD_ENABLE      0x04
#define LCD_RW          0x02
#define LCD_RS          0x01


void LCD_Init(I2C_HandleTypeDef *hi2c); // Hàm khởi tạo LCD

void LCD_Clear(I2C_HandleTypeDef *hi2c); // Xóa màn hình

void LCD_Send_Cmd(I2C_HandleTypeDef *hi2c, uint8_t cmd); // Gửi lệnh đến LCD

void LCD_Send_Data(I2C_HandleTypeDef *hi2c, uint8_t data); // Gửi dữ liệu (ký tự) đến LCD

void LCD_Send_String(I2C_HandleTypeDef *hi2c, char *str); // Gửi chuỗi ký tự

void LCD_Set_Cursor(I2C_HandleTypeDef *hi2c, uint8_t row, uint8_t col); // Đặt vị trí con trỏ (hàng, cột)

void LCD_Scroll_Right(I2C_HandleTypeDef *hi2c, uint8_t steps, uint16_t delay); // Cuộn màn hình sang phải


#ifdef __cplusplus
}
#endif


#endif
