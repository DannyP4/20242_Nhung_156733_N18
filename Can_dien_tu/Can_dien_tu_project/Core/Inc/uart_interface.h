#ifndef UART_INTERFACE_H
#define UART_INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif


#include "main.h"


void showMainMenu(); // Giao diện chọn 1, 2, 3
void showScaleOptions(); // Giao diện chọn cân: lưu / không lưu / hủy
void showLogOptions(); // Giao diện xóa log hoặc thoát


#ifdef __cplusplus
}
#endif


#endif
