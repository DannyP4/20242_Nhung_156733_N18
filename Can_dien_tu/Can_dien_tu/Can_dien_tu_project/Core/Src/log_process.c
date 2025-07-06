#include "log_process.h"

extern I2C_HandleTypeDef hi2c3;
extern UART_HandleTypeDef huart1;

//*************** EEPROM & DS3231 thông số & địa chỉ ***************//
// các define
#define EEPROM_SIZE 4096 		// eeprom có tối đa 4 Kbyte => 4096 byte
#define LOG_SIZE 11				// 4 byte đầu lưu weight, 7 byte sau lưu thời gian cân
#define INIT_FLAG_ADDR 0x00     // đặt địa chỉ byte đầu EEPROM lưu cờ đánh dấu khi khởi tạo
#define INIT_FLAG_VALUE 0x01 	// giá trị đánh dấu đã khởi tạo giá trị thời gian ban đầu (Initial Time) (0000 0001)
#define EEPROM_ADDR        (0x57 << 1)  // EEPROM AT24C32 I2C address
#define DS3231_ADDR        (0x68 << 1)  // RTC DS3231 I2C address


// các biến toàn cục
uint16_t nextLogAddr;			// địa chỉ con trỏ tiếp theo trong eeprom (con trỏ stack)


// Cấu trúc lưu dữ liệu cân nặng dạng float vào eeprom
static union {
	float floatVal;
	uint8_t bytes[4];
} FloatToByte;

// Cấu trúc lấy dữ liệu cân nặng dạng float từ eeprom
static union {
	uint8_t bytes[4];
	float floatVal;
} ByteToFloat;



//*************** các hàm nội bộ dùng trong thư viện ***************//
// ==========================
// Các hàm nội bộ cho thời gian
// ==========================
uint8_t Decimal2BCD(uint8_t val){
	return (val/10)<<4 | (val%10);
}

uint8_t BCD2Decimal(uint8_t val){
	return (val>>4)*10 + (val&0x0F);
}



// ==========================
// Các hàm nội bộ cho bộ nhớ EEPROM
// ==========================
// hàm nội bộ bậc 1
void eepromWriteByte(uint16_t addr, uint8_t data){
	uint8_t buf[3] = { addr >> 8, addr & 0xFF, data };
	HAL_I2C_Master_Transmit(&hi2c3, EEPROM_ADDR, buf, 3, HAL_MAX_DELAY);
	HAL_Delay(5);
}

uint8_t eepromReadByte(uint16_t addr){
	uint8_t reg[2] = { addr >> 8, addr & 0xFF };
	uint8_t val = 0xFF;  // Trả về giá trị mặc định nếu không đọc được

	HAL_I2C_Master_Transmit(&hi2c3, EEPROM_ADDR, reg, 2, HAL_MAX_DELAY);
	HAL_I2C_Master_Receive(&hi2c3, EEPROM_ADDR, &val, 1, HAL_MAX_DELAY);
	return val;
}


// hàm nội bộ bậc 2
void eepromWriteFloat(uint16_t addr, float val){
	FloatToByte.floatVal = val;

	for (uint8_t i = 0; i < 4; i++) {
			eepromWriteByte(addr + i, FloatToByte.bytes[i]);
	}
}

float eepromReadFloat(uint16_t addr){
	for (uint8_t i = 0; i < 4; i++) {
			ByteToFloat.bytes[i] = eepromReadByte(addr + i);
	}

	return ByteToFloat.floatVal;
}

void eepromWriteTime(uint16_t addr, Time* time){
	eepromWriteByte(addr + 0, time->sec);
	eepromWriteByte(addr + 1, time->min);
	eepromWriteByte(addr + 2, time->hour);
	eepromWriteByte(addr + 3, time->weekday);
	eepromWriteByte(addr + 4, time->day);
	eepromWriteByte(addr + 5, time->month);
	eepromWriteByte(addr + 6, time->year);
}

Time eepromReadTime(uint16_t addr){
	Time time;

	time.sec     = eepromReadByte(addr + 0);
	time.min     = eepromReadByte(addr + 1);
	time.hour    = eepromReadByte(addr + 2);
	time.weekday = eepromReadByte(addr + 3);
	time.day     = eepromReadByte(addr + 4);
	time.month   = eepromReadByte(addr + 5);
	time.year    = eepromReadByte(addr + 6);

	return time;
}



//*************** các hàm thực hiện chức năng ***************//
// ==========================
// hàm khởi tạo
// ==========================
void DS3231_Module_Init(){ // Khởi tạo I2C hoặc thiết lập ban đầu

//	// Khởi tạo thời gian
//	InitialTime();

	// Đọc địa chỉ ghi tiếp theo từ 2 byte (tại vị trí 1 và 2) trong EEPROM
	uint8_t highByte = eepromReadByte(1);
	uint8_t lowByte = eepromReadByte(2);
	nextLogAddr = (highByte << 8) | lowByte;

	// Nếu EEPROM chưa từng ghi (giá trị mặc định là 0xFFFF) hoặc sắp hết bộ nhớ
	if (nextLogAddr == 0xFFFF || nextLogAddr >= EEPROM_SIZE - LOG_SIZE) {
			nextLogAddr = 3; // bắt đầu ghi sau 3 byte đầu tiên
	}

}


void DS3231_Module_Close(){
	// Lưu giá trị địa chỉ tiếp theo vào eeprom để sau lấy lại
	eepromWriteByte(1, (nextLogAddr >> 8) & 0xFF);  // Byte cao
	eepromWriteByte(2, nextLogAddr & 0xFF);         // Byte thấp
}


void InitialTime() {
	Time Set_IniTime;

    Set_IniTime.sec = 00;
    Set_IniTime.min = 12;
    Set_IniTime.hour = 21;
    Set_IniTime.day = 30;
    Set_IniTime.month = 6;
    Set_IniTime.year = 25;
    Set_IniTime.weekday = 10;
    setTime(&Set_IniTime);
    eepromWriteByte(INIT_FLAG_ADDR, INIT_FLAG_VALUE);
}

// ==========================
// Các hàm quản lý thời gian
// ==========================
void setTime(Time* time){
    uint8_t data[8] = {
        0x00, // Thanh ghi bắt đầu
        Decimal2BCD(time->sec), Decimal2BCD(time->min), Decimal2BCD(time->hour),
        Decimal2BCD(time->weekday), Decimal2BCD(time->day), Decimal2BCD(time->month), Decimal2BCD(time->year)
    };
    HAL_I2C_Master_Transmit(&hi2c3, DS3231_ADDR, data, 8, HAL_MAX_DELAY); // ghi vào địa chỉ của DS3231
};


void getTime(Time* time){
    uint8_t reg = 0x00; // Thanh ghi bắt đầu
    uint8_t buffer[7];
    HAL_I2C_Master_Transmit(&hi2c3, DS3231_ADDR, &reg, 1, HAL_MAX_DELAY); // Trước khi đọc thì phải gửi tìm I2C có địa chỉ cần đọc
    HAL_I2C_Master_Receive(&hi2c3, DS3231_ADDR, buffer, 7, HAL_MAX_DELAY);  // Yêu cầu đọc 7 byte

    // đọc 7 byte và chuyển đổi BCD
    time->sec     = BCD2Decimal(buffer[0]);
    time->min     = BCD2Decimal(buffer[1]);
    time->hour    = BCD2Decimal(buffer[2]);
    time->weekday = BCD2Decimal(buffer[3]);
    time->day     = BCD2Decimal(buffer[4]);
    time->month   = BCD2Decimal(buffer[5]);
    time->year    = BCD2Decimal(buffer[6]);
};


// ==========================
// Các hàm quản lý log EEPROM (hàm bậc 3)
// ==========================
void writeWeightLog(uint16_t addr, WeightLog* log){
	// Ghi float weight (4 byte)
	eepromWriteFloat(addr, log->weight);           // 0 -> 3

	// Ghi struct Time (7 byte)
	eepromWriteTime(addr + 4, &(log->timestamp));     // 4 -> 10

	// Dịch chuyển đến địa chỉ tiếp theo
	nextLogAddr = addr + LOG_SIZE;
}

float readWeightLog(uint16_t addr, WeightLog* log){
	if(addr >= 0){ // addr thg truyền vào để đọc = nextLogAddr - i*LOG_SIZE (với i là vị trí log tính từ nextLogAddr đổ về trước) => có thể có giá trị âm khi addr == 3
		// Đọc float weight
		log->weight = eepromReadFloat(addr);           // 0 -> 3

		// Đọc struct Time
		log->timestamp = eepromReadTime(addr + 4);     // 4 -> 10

		return log->weight;
	}
	return -1;
}

void clearAllLogs(){
	/*
	thay vì xóa thì ta chỉ cần cập nhật vị trí con trỏ tiếp theo rồi ghi đè giá trị lưu trước đó là được 
	=> EEPROM thường giới hạn số lần ghi (~100,000 lần) nên làm thế để tiết kiệm số lần ghi
	*/

	// Cập nhật con trỏ địa chỉ tiếp theo 
	nextLogAddr = 3;
	eepromWriteByte(1, (nextLogAddr >> 8) & 0xFF);  // Byte cao
	eepromWriteByte(2, nextLogAddr & 0xFF);         // Byte thấp
}

void clearLastLog(){
	if(getTotalLogs() != 0){
		nextLogAddr = nextLogAddr - LOG_SIZE; //dịch con trỏ stack xuống (không xóa mà chỉ ghi đè)
		eepromWriteByte(1, (nextLogAddr >> 8) & 0xFF);  // Byte cao
		eepromWriteByte(2, nextLogAddr & 0xFF);         // Byte thấp
	}
};

uint16_t getLastLogAddress(){
	return nextLogAddr;
}


// ==========================
// Các hàm tiện ích
// ==========================
uint16_t getTotalLogs(){// Lấy số bản ghi đã lưu
	return (nextLogAddr-3)/LOG_SIZE;
}    

bool isStorageFull(){// Kiểm tra ROM đầy chưa
 	return (nextLogAddr + LOG_SIZE > EEPROM_SIZE); // nếu nextLogAddr tiếp theo bị vượt quá size eeprom tối đa
} 

void printLog(WeightLog* log){// In ra log qua Serial (để debug)
	char buf[128];
	    sprintf(buf, "Weight: %.2f kg | Time: %02d:%02d:%02d Date: %02d/%02d/20%02d\r\n",
	            log->weight, // in cân nặng
	            log->timestamp.hour, log->timestamp.min, log->timestamp.sec,
	            log->timestamp.day, log->timestamp.month, log->timestamp.year); // in thời gian
	    HAL_UART_Transmit(&huart1, (uint8_t*)buf, strlen(buf), HAL_MAX_DELAY);

}  		

void printAllLogs(){
	HAL_UART_Transmit(&huart1, (uint8_t*)"----------- Log List -----------\r\n", 34, HAL_MAX_DELAY);
	for (uint16_t addr = 3; addr + LOG_SIZE <= nextLogAddr; addr += LOG_SIZE) {
			WeightLog log;
			readWeightLog(addr, &log);
			printLog(&log);
	}
}
