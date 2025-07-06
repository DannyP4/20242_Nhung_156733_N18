#ifndef LOG_PROCESS_H
#define LOG_PROCESS_H


#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/*
AT24C32 trong DS3231 có tổng cộng 4.096 byte bộ nhớ EEPROM và có 3 byte đầu có ý nghĩa đăc biệt:
- byte số 0: lưu giá trị cờ (flag) để đánh dấu 1 số thứ nếu cần
- byte số 1: lưu giá trị byte cao địa chỉ con trỏ stack 
- byte số 2: lưu giá trị byte thấp địa chỉ con trỏ stack
	(vì con trỏ có 16 bit ứng với 2 byte)
*/

//*************** các kiểu dữ liệu cần thiết ***************//
// Cấu trúc thời gian
typedef struct {
	uint8_t sec;
	uint8_t min;
	uint8_t hour;
	uint8_t weekday; // range từ 1 -> 7
	uint8_t day; // range từ 01 -> 31
	uint8_t month;
	uint8_t year; //range từ 00 -> 99
} Time;

// Cấu trúc lưu dữ liệu cân nặng kèm thời gian
typedef struct {
	float weight;     // Khối lượng cân được
	Time timestamp;   // Thời điểm cân
} WeightLog;

//*************** các hàm nội bộ dùng trong thư viện ***************//
// ==========================
// Các hàm nội bộ cho thời gian
// ==========================
uint8_t Decimal2BCD(uint8_t val);
uint8_t BCD2Decimal(uint8_t val);


// ==========================
// Các hàm nội bộ cho bộ nhớ EEPROM
// ==========================
void eepromWriteByte(uint16_t addr, uint8_t data);
uint8_t eepromReadByte(uint16_t addr);

void eepromWriteFloat(uint16_t addr, float val);
float eepromReadFloat(uint16_t addr);
void eepromWriteTime(uint16_t addr, Time* time);
Time eepromReadTime(uint16_t addr);



//*************** các hàm thực hiện chức năng ***************//
// ==========================
// hàm khởi tạo
// ==========================
void DS3231_Module_Init(); // Khởi tạo I2C hoặc thiết lập ban đầu
void DS3231_Module_Close(); // kết thúc khi vào chế độ sleep

void InitialTime();
// void resetInitialTime();
// ==========================
// Các hàm quản lý thời gian
// ==========================
void setTime(Time* time);	// Cài đặt thời gian cho DS3231
void getTime(Time* time);	// Đọc thời gian hiện tại từ DS3231


// ==========================
// Các hàm quản lý log EEPROM
// ==========================
void writeWeightLog(uint16_t addr, WeightLog* log);
float readWeightLog(uint16_t addr, WeightLog* log);

void clearAllLogs();
void clearLastLog();
uint16_t getLastLogAddress();


// ==========================
// Các hàm tiện ích
// ==========================
uint16_t getTotalLogs();            // Lấy số bản ghi đã lưu
bool isStorageFull();               // Kiểm tra ROM đầy chưa
void printLog(WeightLog* log);  	// In ra log qua Serial (để debug)
void printAllLogs(); 				// In tất cả log qua Serial


#ifdef __cplusplus
}
#endif

#endif
