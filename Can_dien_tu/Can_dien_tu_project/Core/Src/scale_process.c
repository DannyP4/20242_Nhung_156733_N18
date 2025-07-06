#include "scale_process.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

static float weight = 0.0f;
static HX711 hx;

//chân tương ứng với wiring
#define HX711_DOUT_Pin   GPIO_PIN_4
#define HX711_DOUT_Port  GPIOA
#define HX711_SCK_Pin    GPIO_PIN_5
#define HX711_SCK_Port   GPIOA

#define LCD_BACKLIGHT 0x08  // Bit điều khiển đèn nền

extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart1;

void Scale_Init(void) {
    // LCD I2C khởi tạo
    LCD_Init(&hi2c1);
    LCD_Clear(&hi2c1);

    // Khởi tạo HX711
    hx.dout_pin = HX711_DOUT_Pin;
    hx.dout_port = HX711_DOUT_Port;
    hx.sck_pin = HX711_SCK_Pin;
    hx.sck_port = HX711_SCK_Port;
    HX711_Init(&hx);
}

void Scale_Close(void) {
    LCD_Clear(&hi2c1);
    LCD_Backlight_Off(&hi2c1);  // Tắt đèn nền
}

void LCD_Backlight_Off(I2C_HandleTypeDef *hi2c) {
    uint8_t data = 0x00;  // tất cả bit = 0
    HAL_I2C_Master_Transmit(hi2c, LCD_ADDR, &data, 1, HAL_MAX_DELAY);
}

void LCD_Backlight_On(I2C_HandleTypeDef *hi2c) {
    uint8_t data = LCD_BACKLIGHT;
    HAL_I2C_Master_Transmit(hi2c, LCD_ADDR, &data, 1, HAL_MAX_DELAY);
}


void tareScale(void) {
    LCD_Clear(&hi2c1);
    LCD_Send_String(&hi2c1, "Tare...");
    const char *msg = "Tare removes...\r\n";
    HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
    HAL_Delay(2000);

    HX711_Tare(&hx, 10);

    LCD_Clear(&hi2c1);
    LCD_Send_String(&hi2c1, "Tare done.");
    msg = "Tare done...\r\n";
    HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
    HAL_Delay(2000);
    LCD_Clear(&hi2c1);
}

float weightScale(void) {
    LCD_Clear(&hi2c1);
    LCD_Send_String(&hi2c1, "Put weight...");
    HAL_UART_Transmit(&huart1, (uint8_t*)"Put weight...\r\n", 17, HAL_MAX_DELAY);
    HAL_Delay(2000);
    LCD_Clear(&hi2c1);

//    float sum = 0;
//    uint8_t sample = 10;
//    for (uint8_t i = 0; i < sample; i++) {
//        sum += HX711_Read_Average(&hx, 10);
//        HAL_Delay(200);
//    }
//    float raw = sum / sample;
//    weight = fabs(raw) / 19676.0f;  // Scale factor điều chỉnh theo cảm biến
    weight =  fabs(HX711_Get_Weight(&hx, 10));
    return weight;
}

void displayWeighResult(float weight) {
    char msg[32];
    LCD_Clear(&hi2c1);
    LCD_Send_String(&hi2c1, "Result:");
    snprintf(msg, sizeof(msg), "esult...\r\n", weight);
    HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

    snprintf(msg, sizeof(msg), "Weight: %.2f kg\r\n", weight);
    HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
    LCD_Set_Cursor(&hi2c1, 1, 0);
    snprintf(msg, sizeof(msg), "%.2f kg", weight);
    LCD_Send_String(&hi2c1, msg);
    HAL_Delay(2000);
    LCD_Clear(&hi2c1);
}

void startScaleProcess(void) {
    if (HX711_Is_Ready(&hx)) {
        HX711_Set_Scale(&hx, 19676.0f);  // Scale factor điều chỉnh theo cảm biến
        tareScale();
        weight = weightScale();
        displayWeighResult(weight);
    }
}

float getWeight(void) {
    return weight;
}


