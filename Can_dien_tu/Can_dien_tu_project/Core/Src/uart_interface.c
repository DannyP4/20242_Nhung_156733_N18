#include "uart_interface.h"
#include <string.h>

extern UART_HandleTypeDef huart1;

void showMainMenu() {
    char *menu =
		"========== MENU CHINH ==========\r\n"
		"1. Can\r\n"
		"2. Xem lich su can\r\n"
		"3. Thoat\r\n"
		"================================\r\n";
    HAL_UART_Transmit(&huart1, (uint8_t *)menu, strlen(menu), HAL_MAX_DELAY);
}

void showScaleOptions() {
    char *menu =
		"======== TUY CHON CAN =========\r\n"
		"1. Luu ket qua\r\n"
		"2. Khong luu ket qua\r\n"
		"3. Huy va quay lai menu chinh\r\n"
		"================================\r\n";
    HAL_UART_Transmit(&huart1, (uint8_t *)menu, strlen(menu), HAL_MAX_DELAY);
}

void showLogOptions() {
    char *menu =
		"====== TUY CHON HIEN THI =======\r\n"
		"1. Xoa lan can moi nhat\r\n"
		"2. Xoa toan bo lich su can\r\n"
		"3. Thoat va quay lai menu chinh\r\n"
		"================================\r\n";
    HAL_UART_Transmit(&huart1, (uint8_t *)menu, strlen(menu), HAL_MAX_DELAY);
}
