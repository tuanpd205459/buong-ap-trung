// Thư viện cảm biến nhiệt độ và độ ẩm
#include "DHT.h"

// Thư viện I2C cho LCD
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Địa chỉ I2C của LCD
#define LCD_ADDRESS    0x27

// Số cột và số dòng của LCD
#define LCD_COLUMNS 16
#define LCD_LINES   2

// Khởi tạo đối tượng LCD
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_LINES);

// Chân kết nối cảm biến DHT
#define DHT_PIN 11

// Loại cảm biến DHT (DHT11)
#define DHT_TYPE DHT11

// Khởi tạo đối tượng cảm biến DHT
DHT dht(DHT_PIN, DHT_TYPE);

// Chân kết nối PIR, Relay, đèn LED đỏ và buzzer
const int PIR_PIN = 10;
const int RELAY_PIN = 9;
const int RED_LED_PIN = 13;
const int BUZZER_PIN = 12;

// Biến lưu trữ thời gian ấp trứng
static uint8_t incubationDays;
static uint8_t incubationHours;
static uint8_t incubationMinutes;

// Nhiệt độ ấp trứng được đặt
static uint8_t targetTemperature = 30;

// Nhiệt độ hiện tại đọc từ cảm biến DHT
static float currentTemperature;

// Trạng thái quá trình ấp trứng: 0 - chờ, 1 - ấp, 2 - tiếp tục, 3 - đặt thời gian ấp
static uint8_t processState = 0;

// Trạng thái hiện tại của quá trình
uint8_t currentState = 0;

// Các khoảng thời gian (đơn vị: milliseconds)
#define MILLIS_WAITING   1000
#define MILLIS_HATCHING  1000
#define MILLIS_BUZZER    1000 * 5
#define MILLIS_ALARM     1000 * 25

// Biến lưu trữ thời gian trước khi bật còi
unsigned long buzzerPreviousMillis = 0;
unsigned long buzzerInterval = 20 * 1000; // 20 giây

// Biến lưu trạng thái còi
bool isBuzzerOn = false;
int turnOffBuzzerStatus = 0;

// Hàm bật còi
void turnOnBuzzer() {
  tone(BUZZER_PIN, 10); // Kích hoạt còi
  turnOffBuzzer();

  isBuzzerOn = true;
}

// Hàm tắt còi
void turnOffBuzzer() {
  noTone(BUZZER_PIN); // Tắt còi
  isBuzzerOn = false;
}

// Hàm xử lý tắt còi qua cổng Serial
void handleBuzzerOff() {
  if (Serial.available()) {
    String input = Serial.readString();
    input.trim(); // Loại bỏ khoảng trắng đầu và cuối chuỗi

    if (input.startsWith("TurnOffBuzzer")) {
      turnOffBuzzer(); // Tắt còi
      turnOffBuzzerStatus = 1;
    }
  }
}

// Hàm cập nhật hiển thị thời gian ấp trứng lên LCD
void updateIncubationTime(uint8_t days, uint8_t hours, uint8_t minutes) {
  lcd.setCursor(0, 1);
  lcd.print("Time: ");

  if (days <= 9) {
    lcd.print("0");
  }
  lcd.print(days);
  lcd.print(":");

  if (hours <= 9) {
    lcd.print("0");
  }
  lcd.print(hours);
  lcd.print(":");

  if (minutes <= 9) {
    lcd.print("0");
  }
  lcd.print(minutes);
}

// Hàm cập nhật hiển thị nhiệt độ hiện tại lên LCD
void updateCurrentTemperature() {
  lcd.setCursor(0, 0);
  lcd.print("CurrentTemp: ");

  if (processState == 3) {
    lcd.print(targetTemperature);
  } else {
    lcd.print((uint8_t)dht.readTemperature());
  }
}

// Hàm điều chỉnh nhiệt độ
void adjustTemperature() {
  currentTemperature = dht.readTemperature();

  if (currentTemperature >= targetTemperature + 1) {
    digitalWrite(RED_LED_PIN, LOW);
    // Gọi hàm để chạy động cơ (điều khiển relay để chạy động cơ theo hướng cụ thể)
  } else if (currentTemperature <= targetTemperature - 1) {
    digitalWrite(RED_LED_PIN, HIGH);
    digitalWrite(RELAY_PIN, LOW);
  }
}

// Hàm gửi dữ liệu nhiệt độ và độ ẩm qua cổng Serial
void sendDataTemperatureHumidity() {
  float currentTemperature = dht.readTemperature();
  float currentHumidity = dht.readHumidity();

  if (isnan(currentTemperature) || isnan(currentHumidity)) {
    Serial.println("Failed to read from DHT");
  } else {
    Serial.print(currentTemperature);
    Serial.print(",");
    Serial.print(currentHumidity);
    Serial.print(",");
  }
}

// Hàm xử lý trạng thái đầu vào từ cổng Serial
uint8_t processInputState(String input) {
  input.trim();

  if (input.startsWith("setTemp")) {
    int targetTempIndex = input.indexOf("setTemp") + 7;
    targetTemperature = input.substring(targetTempIndex).toInt();
    return currentState;
  }

  if (input.startsWith("prState")) {
    int prStateIndex = input.indexOf("prState") + 7;
    currentState = input.substring(prStateIndex).toInt();
    return currentState;
  }
}

// Hàm chạy động cơ (đang để trống, cần phải cài đặt)
void runMotor() {
  // Điều khiển relay để chạy động cơ theo hướng cụ thể
}

// Hàm thiết lập ban đầu
void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  dht.begin();
  lcd.print("Buong ap trung");
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(PIR_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Tắt rơ le ban đầu
}

// Hàm lặp vô hạn của Arduino
void loop() {
  uint8_t days;
  uint8_t hours;
  uint8_t minutes;
  uint8_t resetStatus = 0;

  if (Serial.available()) {
    String input = Serial.readString();
    input.trim(); // Loại bỏ khoảng trắng đầu và cuối chuỗi

    if (input.startsWith("dayTimeToHatch")) {
      int dayTimeIndex = input.indexOf("dayTimeToHatch") + 14;
      int hourTimeIndex = input.indexOf("hourTimeToHatch") + 15;
      int minuteTimeIndex = input.indexOf("minuteTimeToHatch") + 17;
      int prStateIndex = input.indexOf("prState") + 7;

      days = input.substring(dayTimeIndex, hourTimeIndex).toInt();
      hours = input.substring(hourTimeIndex, minuteTimeIndex).toInt();
      minutes = input.substring(minuteTimeIndex, prStateIndex).toInt();
      currentState = input.substring(prStateIndex).toInt();
    }

    if (input.startsWith("prState")) {
      int prStateIndex = input.indexOf("prState") + 7;
      currentState = input.substring(prStateIndex).toInt();
    }
    if (input.startsWith("Reset")) {
      days = 0;
      hours = 0;
      minutes = 0;
      updateIncubationTime(days, hours, minutes);
      updateCurrentTemperature();
      adjustTemperature();
      turnOffBuzzerStatus = 0;
    }

    if (input.startsWith("setTemp")) {
      currentState = processInputState(input);
      adjustTemperature();
    }
    if (input.startsWith("TurnOffBuzzer2")) {
      turnOffBuzzer(); // Tắt còi
      turnOffBuzzerStatus = 1;
    }
  }

  unsigned long previousMillis = millis();
  unsigned long interval = 1000; // Đơn vị milliseconds (1 giây)

  while (currentState == 1) {
    if (Serial.available()) {
      String inputState = Serial.readString();
      currentState = processInputState(inputState);
      if (inputState.startsWith("Reset")) resetStatus = 1;
    }
    if (resetStatus) {
      days = 0;
      hours = 0;
      minutes = 0;
      updateIncubationTime(0, 0, 0);
      updateCurrentTemperature();
      adjustTemperature();
      break;
    }
    if (currentState == 0) {
      break;
    }

    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;

      if (minutes == 0) {
        if (hours == 0) {
          if (days == 0) {
            currentState = 2;
            break;
          } else {
            days--;
            hours = 23;
            minutes = 59;
          }
        } else {
          hours--;
          minutes = 59;
        }
      } else {
        minutes--;
      }

      // Cập nhật hiển thị trên LCD
      updateIncubationTime(days, hours, minutes);
      updateCurrentTemperature();
      adjustTemperature();

      sendDataTemperatureHumidity();
      Serial.print(days);
      Serial.print(",");
      Serial.print(hours);
      Serial.print(",");
      Serial.println(minutes);

      // Gửi tín hiệu nhiệt độ và độ ẩm về Qt liên tục sau mỗi 2 giây
      delay(1000);
    }
  }

  if (currentState == 2 && turnOffBuzzerStatus == 0) {
    unsigned long currentMillis = millis();
    if (currentMillis - buzzerPreviousMillis >= buzzerInterval) {
      buzzerPreviousMillis = currentMillis;

      if (isBuzzerOn) {
        turnOffBuzzer(); // Tắt loa
        isBuzzerOn = false;
      } else {
        turnOnBuzzer();
        delay(200); // Bật loa trong 0.5 giây
        turnOffBuzzer();
        isBuzzerOn = true;
      }
    }
  }

  sendDataTemperatureHumidity();
  Serial.println();
  delay(1000);
}


