#include "hx711.h"

static float g_scale = 1.0f;

void HX711_Init(HX711* hx) {
    HAL_GPIO_WritePin(hx->sck_port, hx->sck_pin, GPIO_PIN_RESET);
    hx->offset = 0;
    hx->scale = 1.0f;
}

uint8_t HX711_Is_Ready(HX711* hx) {
    return HAL_GPIO_ReadPin(hx->dout_port, hx->dout_pin) == GPIO_PIN_RESET;
}

int32_t HX711_Read(HX711* hx) {
    int32_t data = 0;
    while (!HX711_Is_Ready(hx));

    for (int i = 0; i < 24; i++) {
        HAL_GPIO_WritePin(hx->sck_port, hx->sck_pin, GPIO_PIN_SET);
        data = data << 1;
        HAL_GPIO_WritePin(hx->sck_port, hx->sck_pin, GPIO_PIN_RESET);
        if (HAL_GPIO_ReadPin(hx->dout_port, hx->dout_pin)) {
            data++;
        }
    }

    // Pulse 25th clock
    HAL_GPIO_WritePin(hx->sck_port, hx->sck_pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(hx->sck_port, hx->sck_pin, GPIO_PIN_RESET);

    if (data & 0x800000) data |= 0xFF000000; // sign extend
    return data;
}

float HX711_Read_Average(HX711* hx, uint8_t times) {
    int64_t sum = 0;
    for (uint8_t i = 0; i < times; i++) {
        sum += HX711_Read(hx);
    }
    return (float)(sum / times);
}

void HX711_Tare(HX711* hx, uint8_t times) {
	hx->offset = HX711_Read_Average(hx, times);
}

void HX711_Set_Scale(HX711* hx, float scale) {
	if (scale != 0) hx->scale = scale;
}

float HX711_Get_Weight(HX711* hx, uint8_t times) {
    float raw = HX711_Read_Average(hx, times);
    return (raw - hx->offset) / hx->scale;
}

