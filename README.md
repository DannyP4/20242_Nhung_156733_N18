# Hệ thống cân điện tử có lưu trữ lịch sử và chế độ tiết kiệm năng lượng

## 📖 Giới thiệu dự án

Dự án này tập trung vào việc phát triển một hệ thống cân điện tử thông minh, không chỉ có khả năng đo lường và hiển thị trọng lượng chính xác mà còn tích hợp các tính năng nâng cao như lưu trữ lịch sử đo lường và quản lý năng lượng hiệu quả. Mục tiêu là tạo ra một thiết bị cân linh hoạt, tiện lợi, đáp ứng nhu cầu sử dụng thực tế và tối ưu hóa thời gian hoạt động.

## ✨ Tính năng chính

* **Đo lường trọng lượng chính xác:**
    * Sử dụng Load Cell 50kg và Module HX711 để chuyển đổi tín hiệu tương tự từ cảm biến thành dữ liệu số.
    * Tích hợp thuật toán hiệu chuẩn (calibration) để đảm bảo độ chính xác cao.
* **Lưu trữ lịch sử cân:**
    * Sử dụng bộ nhớ EEPROM (AT24C32) để lưu trữ kết quả cân theo thời gian thực (được cấp bởi DS3231 RTC).
    * Cho phép người dùng xem lại lịch sử cân.
* **Giao tiếp người dùng qua UART:**
    * Cung cấp giao diện dòng lệnh (CLI) qua cổng nối tiếp (UART) để người dùng tương tác, bao gồm hiệu chuẩn, xem lịch sử và quản lý cài đặt.
* **Chế độ tiết kiệm năng lượng (Sleep Mode):**
    * Hệ thống có thể chuyển sang chế độ ngủ (sleep) khi không hoạt động để giảm thiểu tiêu thụ điện năng.
    * Được kích hoạt lại bởi cảm biến rung (SW420) khi có tác động.
* **Hiển thị kết quả:**
    * Kết quả đo được hiển thị trên màn hình LCD.

## 🛠️ Công nghệ và Thành phần sử dụng

### Phần cứng
* **Vi điều khiển:** STM32F429I Discovery Kit (sử dụng chip STM32F429ZIT6)
* **Cảm biến trọng lượng:** Load Cell 50kg
* **Module đọc Load Cell:** HX711
* **Module Đồng hồ thời gian thực (RTC) và EEPROM:** DS3231 + AT24C32
* **Cảm biến rung:** SW420
* **Màn hình hiển thị:** LCD (kết nối qua I2C)

### Phần mềm
* **Ngôn ngữ lập trình:** C/C++
* **Môi trường phát triển:** STM32CubeIDE / Keil MDK (hoặc tương tự)
* **Giao tiếp:** UART, I2C, ADC, GPIO

## ⚙️ Hướng dẫn cài đặt và sử dụng (Tổng quan)

1.  **Phần cứng:**
    * Kết nối Load Cell với module HX711.
    * Kết nối HX711, DS3231+AT24C32, SW420, LCD với STM32F429I Discovery Kit theo sơ đồ mạch đã thiết kế (tham khảo báo cáo chi tiết về chân GPIO và giao tiếp).
2.  **Phần mềm:**
    * Tải mã nguồn về máy tính.
    * Sử dụng môi trường phát triển phù hợp (ví dụ: STM32CubeIDE) để biên dịch (build) và nạp (flash) firmware vào vi điều khiển STM32F429I.
    * Sử dụng công cụ terminal (ví dụ: PuTTY, Tera Term, Hercules) để giao tiếp với hệ thống qua cổng UART.

## 👥 Thành viên nhóm

* Nguyễn Bá Hoàng (MSSV: 20225844)
* Phạm Đức Long (MSSV: 20225737)
* Phạm Văn Đồng (MSSV: 20225702)

## ⚖️ Giấy phép (License)

Dự án này được cấp phép theo MIT License - xem file [LICENSE](LICENSE) để biết thêm chi tiết.
