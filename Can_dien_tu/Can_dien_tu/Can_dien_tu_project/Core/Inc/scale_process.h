#ifndef __SCALE_PROCESS_H
#define __SCALE_PROCESS_H

#include "main.h"
#include "hx711.h"
#include "lcd_i2c.h"

// Khởi tạo LCD và HX711
void Scale_Init(void);

// Tắt đèn hoặc clear LCD
void Scale_Close(void);
void LCD_Backlight_Off(I2C_HandleTypeDef *hi2c);
void LCD_Backlight_On(I2C_HandleTypeDef *hi2c);

// Đặt lại khối lượng về 0 (Tare)
void tareScale(void);

// Đọc khối lượng trung bình
float weightScale(void);

// Hiển thị kết quả lên LCD và UART
void displayWeighResult(float weight);

// Quy trình cân hoàn chỉnh (gồm tare, đo, hiển thị)
void startScaleProcess(void);

// Lấy khối lượng cuối cùng đo được
float getWeight(void);

#endif

