#ifndef __HX711_H
#define __HX711_H

#include "main.h"

typedef struct {
    GPIO_TypeDef* dout_port;
    uint16_t dout_pin;
    GPIO_TypeDef* sck_port;
    uint16_t sck_pin;
    float offset;
    float scale;
} HX711;

void HX711_Init(HX711* hx);
uint8_t HX711_Is_Ready(HX711* hx);
void HX711_Tare(HX711* hx, uint8_t times);
int32_t HX711_Read(HX711* hx);
float HX711_Read_Average(HX711* hx, uint8_t times);
void HX711_Set_Scale(HX711* hx, float scale);
float HX711_Get_Weight(HX711* hx, uint8_t times);

#endif

