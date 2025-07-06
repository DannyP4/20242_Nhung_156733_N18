#include "sleep_process.h"
#include "stm32f4xx_hal.h"

#define WAKEUP_PIN        GPIO_PIN_0
#define WAKEUP_GPIO_PORT  GPIOA

extern UART_HandleTypeDef huart1;

void wakeup_Init(){

//    GPIO_InitTypeDef GPIO_InitStruct = {0};
//
//    // Enable clock for GPIO
//    __HAL_RCC_GPIOA_CLK_ENABLE();
//
//    // Cấu hình chân làm input + EXTI
//    GPIO_InitStruct.Pin = WAKEUP_PIN;
//    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    HAL_GPIO_Init(WAKEUP_GPIO_PORT, &GPIO_InitStruct);
//
//    // Bật ngắt EXTI
//    HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
//    HAL_NVIC_EnableIRQ(EXTI0_IRQn);

//    char *msg = "Wakeup config done\r\n";
//    HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
}

void enterSleep(){
    char *msg = "Entering STOP/SLEEP mode...\r\n";
    HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
    HAL_Delay(1000);

    // Cho phép power interface
    __HAL_RCC_PWR_CLK_ENABLE();

    // Vào STOP mode
    HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);

    // Sau khi đánh thức ta cần phải reset clock system
    SystemClock_Config();
}
