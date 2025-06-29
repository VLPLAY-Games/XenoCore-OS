// Необходимые библиотеки
#include "SPIFFS.h"
#include "FS.h"
#include <SPI.h>
#include <TFT_eSPI.h>
#include <Wire.h>
#include <EEPROM.h>
#include <SD.h>
#include <JPEGDecoder.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <ESPmDNS.h>
#include <Update.h>
#include <NTPClient.h>
#include <HTTPClient.h>
#include <ESPAsyncWebServer.h>
#include "Adafruit_BME680.h"
#include <PulseSensorPlayground.h>
#include <Adafruit_INA219.h>
#include "BluetoothSerial.h"
#include <IPAddress.h>

#include "config.h"

// все define
#define CALIBRATION_FILE "/calibrationData"
#define REPEAT_CAL false
#define BLACK_SPOT
#define TFT_WIDTH 480
#define TFT_HEIGHT 320
#define EEPROM_SIZE 309

#define Username_ADDR 1
#define Password_ADDR 50
#define SsidNet_ADDR 100
#define PasswordNet_ADDR 175
#define DeviceName_ADDR 250
#define utcTime_ADDR 300
#define utcTimePlus_ADDR 302
#define serverAlwaysOn_ADDR 304
#define updating_sys 306
#define sleep_mode 308

// Кнопка назад
#define Back_WIDTH 90
#define Back_HEIGHT 40
#define Back_MARGIN 10

#define BUTTON_PIN_BITMASK 0x200000000  // 2^33 in hex

// Клавиатура
#define KEY_WIDTH 40
#define KEY_HEIGHT 40
#define KEY_MARGIN 10
#define KEYPAD_ROWS 3
#define KEYPAD_COLS 9
#define BUTTON_WIDTH 160
#define BUTTON_HEIGHT 40
#define BUTTON_MARGIN 20

// Глобальные переменные
TFT_eSPI tft = TFT_eSPI();
uint16_t ts_x, ts_y;
WiFiUDP ntpUDP;
unsigned long timing;
unsigned long timing1;
unsigned long timing2;
AsyncWebServer server(80);
String dayStamp, formattedDate;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 0);
int BackX = TFT_WIDTH - Back_WIDTH - Back_MARGIN;
int BackY = TFT_HEIGHT - Back_HEIGHT - Back_MARGIN;
bool isConnectToWIFI = false;
bool wificheckPassed = false;
const char daysOfTheWeek[7][12] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
const char* months[] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
const char* daysOfWeekShort[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
unsigned long timeToSleep;
bool flashlightOn = false;
Adafruit_BME680 bme;
RTC_DATA_ATTR int bootCount = 0;
String inputString = "";  // Строка для сохранения нажатых символов
bool capsLock = false;    // Флаг режима Caps Lock
byte currentPage = 0;      // Текущая страница клавиатуры
bool sd_card_init = false;
int stopwatch_sec = 0;
int stopwatch_min = 0;
int stopwatch_hour = 0;
bool stopwatch_active = false;
uint64_t cardSize = 0;
bool new_update = false;
unsigned bme_status;
String temp = "";
int timer_sec = 0;
bool timer_active = false;
byte settings_page = 0;
PulseSensorPlayground pulseSensor;
bool pulsesensor_worked = false;
Adafruit_INA219 ina219;
bool ina219_status = false;
float current_mA = 0;
BluetoothSerial SerialBT;
bool ble_status = false;
struct Button {
  int x;
  int y;
  int width;
  int height;
  const char* label;
};
struct SystemCheck {
  const char* label;
  bool condition;
};
IPAddress allowedIPs[10];  // Массив для хранения разрешенных IP-адресов
int numAllowedIPs = 0;  // Количество разрешенных IP-адресов
byte capacity;
bool wifi_for_bluetooth = false;
HTTPClient http;
int lastState = LOW;                                        // the previous state from the input pin
int currentState;                                           // the current reading from the input pin
unsigned long pressedTime  = 0;
bool isPressing = false;
bool isLongDetected = false;



// Предопределение всех функций
void timeEdit(bool initialSetup = false);
void timeEdit2(bool initialSetup = false);
void drawButBack();
void Calibration();
String keyboard(bool unlimited = false);
char getCurrentKey(int row, int col);
void drawKeyboard();
void getLastTimeTopPanel();
void drawTopPanel(bool needTime = false);
void drawInitialSetupLabel(bool needBelow = false);
void start_button_func();
void butUnPwfunc();
void Greetings();
void InitialConnectNet();
void InitialSetupComplete();
void InitialSetup();
void system();
void Desktop();
void FactoryReset();
void FactoryResetInformation();
void ChangeUsername();
void ChangePassword();
void ChangeDeviceName();
void UpdatingSystem();
void SettingsFuncs();
void Settings();
void serverSettings();
void ServerPowerUp();
void drawppm();
void checkSystem();
void explorer();
void stopwatch();
void timer();
void calc();
void drawCalcButton(int x, int y, const char* label);
void handleCalcButtonPress(const char* button);
void ai();
void drawSdJpeg(const char *filename, int xpos, int ypos);
void jpegRender(int xpos, int ypos);
void paint();
void updateFirmware();
void downloadImage(const String& imageName);
void downloadImagesList();
void downloadFile(const char* url, const char* path);
void checkForUpdate();
String getServerCode(String fileName);
void serveImageSite(AsyncWebServerRequest *request, const char *filePath, const char *contentType);
void wifiSetting();
void check_sleep();
void settings_func_page2();
void sleep_settings();
void desktop_page2();
void pulse();
void server_flashlight();
void battery_settings();
void readBT();
void bluetooth_settings();
void drawButton(Button& button);
void removeAllowedIP(IPAddress ipAddress);
void addAllowedIP(IPAddress ipAddress);
bool isAuthenticated(IPAddress ipAddress);
void wifi_scan();
void ResetReason();
void sendChatGPTRequest(String message, String api_key);
void eventViewer();
byte getBatLevel();
void calendar();
void drawCalendar();
void check_but_reboot(bool initial = false);



// Основной цикл при запуске
void setup() {
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_27, 1);
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();
  tft.init();
  pinMode(GPIO_NUM_27, INPUT_PULLUP);
  pinMode(22, OUTPUT);
  digitalWrite(22, HIGH);
  pinMode(26, OUTPUT);
  pinMode(17, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(flashLight, OUTPUT);
  digitalWrite(17, LOW);
  digitalWrite(26, HIGH);
  digitalWrite(13, HIGH);
  Calibration();
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  sd_card_init = SD.begin(5);
  tft.setRotation(3);
  tft.setTextSize(2);
  tft.setCursor(140, 225, 2);
  if (wakeup_reason != ESP_SLEEP_WAKEUP_EXT0) {
    drawSdJpeg("/xc_logo_black.jpeg", 160, 75);
    tft.println("Loading system");
    delay(500);
    ResetReason();
  }
  EEPROM.begin(EEPROM_SIZE);
  cardSize = SD.cardSize() / 1024;
  pulseSensor.analogInput(PulseWire);
  pulseSensor.setThreshold(Threshold);
  Serial.begin(115200);

  pulsesensor_worked = pulseSensor.begin();
  Wire.begin(33, 32);
  ina219_status = ina219.begin();
  bme_status = bme.begin();
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150);  // 320*C for 150 ms

  tft.setTextFont(2);
  tft.setTextSize(2);



  int len_username = EEPROM.read(Username_ADDR);
  username.reserve(len_username);
  for (int i = 0; i < len_username; i++) {
    username += (char)EEPROM.read(Username_ADDR + 1 + i);
  }

  int len_deviceName = EEPROM.read(DeviceName_ADDR);
  deviceName.reserve(len_deviceName);
  for (int i = 0; i < len_deviceName; i++) {
    deviceName += (char)EEPROM.read(DeviceName_ADDR + 1 + i);
  }

  int len_password = EEPROM.read(Password_ADDR);
  password.reserve(len_username);
  for (int i = 0; i < len_password; i++) {
    password += (char)EEPROM.read(Password_ADDR + 1 + i);
  }

  int len_ssidNet = EEPROM.read(SsidNet_ADDR);
  username.reserve(len_ssidNet);
  for (int i = 0; i < len_ssidNet; i++) {
    ssidNet += (char)EEPROM.read(SsidNet_ADDR + 1 + i);
  }
  int len_passwordNet = EEPROM.read(PasswordNet_ADDR);
  password.reserve(len_passwordNet);
  for (int i = 0; i < len_passwordNet; i++) {
    passwordNet += (char)EEPROM.read(PasswordNet_ADDR + 1 + i);
  }

  int updating_system = EEPROM.read(updating_sys);
  if (updating_system == 1 && wakeup_reason != ESP_SLEEP_WAKEUP_EXT0) {
    tft.fillScreen(TFT_BLACK);
    drawSdJpeg("/xc_logo_black.jpeg", 160, 75);
    tft.setCursor(110, 225, 2);
    tft.println("Updating system: ");
    updateFirmware();
  }
  if (EEPROM.read(sleep_mode) == 0255) {
    sleep_off = false;
  }
  if (EEPROM.read(sleep_mode) == 1) {
    sleep_off = true;
  }
  utcTime = EEPROM.read(utcTime_ADDR + 1);
  utcTimePlus = EEPROM.read(utcTimePlus_ADDR + 1);
  serverAlwaysOn = EEPROM.read(serverAlwaysOn_ADDR + 1);
  timeClient.setTimeOffset(utcTimePlus ? utcTime * 3600 : -utcTime * 3600);
  WiFi.mode(WIFI_STA);
  WiFi.setHostname(deviceName.c_str());
  if (EEPROM.read(0) == 0255) {
    InitialSetupPassed = false;
  }
  if (EEPROM.read(0) == 1) {
    InitialSetupPassed = true;
  }
  if (InitialSetupPassed == false) {
    InitialSetup();
  } else {
      WiFi.begin(ssidNet.c_str(), passwordNet.c_str());
      if (wakeup_reason != ESP_SLEEP_WAKEUP_EXT0) {
        delay(2000);
      }
  }
}

void loop() {
  system();
}
