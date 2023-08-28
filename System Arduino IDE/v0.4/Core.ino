// Доп библиотеки
#include "config.h"
 #include "server.h"
 #include "images.h"

// Необходимые библиотеки
#include "FS.h"
 #include <SPI.h>
 #include <TFT_eSPI.h>

 #include <EEPROM.h>

 #include <WiFi.h>
 #include <WiFiClient.h>
 #include <WebServer.h>
 #include <ESPmDNS.h>
 #include <Update.h>
 #include <NTPClient.h>

 //#include <Adafruit_Sensor.h>
 //#include <Adafruit_BME280.h>
 //#include <Adafruit_BMP280.h>

// все define
 #define CALIBRATION_FILE "/calibrationData"
 #define REPEAT_CAL false
 #define BLACK_SPOT
 #define TFT_WIDTH 480
 #define TFT_HEIGHT 320
 #define EEPROM_SIZE 306

 #define Username_ADDR 1
 #define Password_ADDR 50
 #define SsidNet_ADDR 100
 #define PasswordNet_ADDR 175
 #define DeviceName_ADDR 250
 #define utcTime_ADDR 300
 #define utcTimePlus_ADDR 302
 #define serverAlwaysOn_ADDR 304

 // Кнопка назад
 #define Back_WIDTH 90
 #define Back_HEIGHT 40
 #define Back_MARGIN 10

 #define BUTTON_PIN_BITMASK 0x200000000 // 2^33 in hex

 // Кнопка начала всего
 #define start_buttonFRAME_X 180
 #define start_buttonFRAME_Y 196
 #define start_buttonFRAME_W 120
 #define start_buttonFRAME_H 50
 #define start_buttonX (start_buttonFRAME_X * 2)
 #define start_buttonY start_buttonFRAME_Y
 #define start_buttonW (start_buttonFRAME_W / 2)
 #define start_buttonH start_buttonFRAME_H

 // Клавиатура
 #define KEY_WIDTH 40
 #define KEY_HEIGHT 40
 #define KEY_MARGIN 10
 #define KEYPAD_ROWS 3
 #define KEYPAD_COLS 9
 #define BUTTON_WIDTH 160
 #define BUTTON_HEIGHT 40
 #define BUTTON_MARGIN 20

 // Кнопки имени пользователя и пароля, настройки сервера, изменение времени UTC 2
 #define butUnPw_WIDTH 160
 #define butUnPw_HEIGHT 40
 #define butUnPw_MARGIN 20

 // Подключение к интернету
 #define butUnPwNet_WIDTH 160
 #define butUnPwNet_HEIGHT 40
 #define butUnPwNet_MARGIN 20

 // Кнопка пропустить подключение к интернету
 #define Skip_WIDTH 90
 #define Skip_HEIGHT 40
 #define Skip_MARGIN 20

 // Кнопка рабочего стола
 #define Desktop_WIDTH 140
 #define Desktop_HEIGHT 40
 #define Desktop_MARGIN 20

 // Кнопка настроек
 #define Settings_WIDTH 130
 #define Settings_HEIGHT 40
 #define Settings_MARGIN 20

 // Кнопка фонарика
 #define FlashLight_WIDTH 130
 #define FlashLight_HEIGHT 40
 #define FlashLight_MARGIN 20

 // Кнопка продолжить, изменение времени UTC 1, инструкция по обновлению системы
 #define continueupdate_WIDTH 130
 #define continueupdate_HEIGHT 40
 #define continueupdate_MARGIN 20

 // Кнопки функций настроек
 // Кнопка обновления
 #define updateos_WIDTH 160
 #define updateos_HEIGHT 40
 #define updateos_MARGIN 20

 // Кнопка сброса
 #define FactoryReset_WIDTH 160
 #define FactoryReset_HEIGHT 40
 #define FactoryReset_MARGIN 20

 // Кнопка изменения имени пользователя
 #define UsernameEdit_WIDTH 160
 #define UsernameEdit_HEIGHT 40
 #define UsernameEdit_MARGIN 20

 // Кнопка изменения пароля
 #define PasswordEdit_WIDTH 160
 #define PasswordEdit_HEIGHT 40
 #define PasswordEdit_MARGIN 20

 // Кнопка перезагрузки
 #define Reboot_WIDTH 160
 #define Reboot_HEIGHT 40
 #define Reboot_MARGIN 20

 // Кнопка изменения имени устройства
 #define DeviceNameEdit_WIDTH 160
 #define DeviceNameEdit_HEIGHT 40
 #define DeviceNameEdit_MARGIN 20

 // Кнопка изменения WIFI
 #define WIFIEdit_WIDTH 160
 #define WIFIEdit_HEIGHT 40
 #define WIFIEdit_MARGIN 20

 // Кнопка изменения времени
 #define utcTimeEdit_WIDTH 160
 #define utcTimeEdit_HEIGHT 40
 #define utcTimeEdit_MARGIN 20

 // Кнопка изменения настройки сервера
 #define serverAlwaysOnEdit_WIDTH 160
 #define serverAlwaysOnEdit_HEIGHT 40
 #define serverAlwaysOnEdit_MARGIN 20

 // Кнопка функций настроек
 #define funcsettings_WIDTH 150
 #define funcsettings_HEIGHT 40
 #define funcsettings_MARGIN 10

// Глобальные переменные
TFT_eSPI tft = TFT_eSPI();
 uint16_t ts_x, ts_y;
 WiFiUDP ntpUDP;
 unsigned long timing;
 unsigned long timing1;
 unsigned long timing2;
 WebServer server(80);
 String dayStamp, formattedDate;
 NTPClient timeClient(ntpUDP, "pool.ntp.org", 0);
 int BackX = TFT_WIDTH - Back_WIDTH - Back_MARGIN;
 int BackY = TFT_HEIGHT - Back_HEIGHT - Back_MARGIN;
 bool isConnectToWIFI = false;
 bool wificheckPassed = false;
 char daysOfTheWeek[7][12] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
 unsigned long timeToSleep;
 const int flashLight = 25;
 bool flashlightOn = false;
 //Adafruit_BME280 bme;
 RTC_DATA_ATTR int bootCount = 0;
 String inputString = "";  // Строка для сохранения нажатых символов
 bool capsLock = false;    // Флаг режима Caps Lock
 int currentPage = 0;      // Текущая страница клавиатуры

// Предопределение всех функций
void timeEdit(bool initialSetup = false);
 void timeEdit2(bool initialSetup = false);
 void drawButBack();
 void Calibration();
 String keyboard();
 char getCurrentKey(int row, int col);
 void drawKeyboard();
 void getLastTimeTopPanel();
 void drawTopPanel(bool needTime = false);
 void drawInitialSetupLabel(bool needBelow = false);
 void start_button_func();
 void drawbutUnPw(int x, int y, int width, int height, const char* label);
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
 void InstructionUpdateSystem();
 void serverSettings();
 void ServerPowerUp();
 void drawppm();

// Функции

// Отрисовка кнопки назад
void drawButBack() {
  tft.fillRoundRect(BackX, BackY, Back_WIDTH, Back_HEIGHT, 10, TFT_RED);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(BackX + (Back_WIDTH / 2) - 30, BackY + (Back_HEIGHT / 2) - 15);
  tft.println("Back");
  tft.setTextColor(TFT_BLACK);
}

// Калибровка
void Calibration() {
  uint16_t calibrationData[5];
  uint8_t calDataOK = 0;

  tft.setRotation(3);
  tft.fillScreen((0xFFFF));

  tft.setCursor(20, 0, 2);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.setTextSize(1);
  tft.println("calibration run");

  // check file system
  if (!SPIFFS.begin()) {
    Serial.println("formating file system");

    SPIFFS.format();
    SPIFFS.begin();
  }

  // check if calibration file exists
  if (SPIFFS.exists(CALIBRATION_FILE)) {
    File f = SPIFFS.open(CALIBRATION_FILE, "r");
    if (f) {
      if (f.readBytes((char*)calibrationData, 14) == 14)
        calDataOK = 1;
      f.close();
    }
  }
  if (calDataOK) {
    // calibration data valid
    tft.setTouch(calibrationData);
  } else {
    // data not valid. recalibrate
    tft.calibrateTouch(calibrationData, TFT_WHITE, TFT_RED, 15);
    // store data
    File f = SPIFFS.open(CALIBRATION_FILE, "w");
    if (f) {
      f.write((const unsigned char*)calibrationData, 14);
      f.close();
    }
  }

  tft.fillScreen((0xFFFF));
}

// Клавиатура
const char keyboardKeys1[KEYPAD_ROWS][KEYPAD_COLS] = {
  { 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o' },
  { 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l' },
  { 'z', 'x', 'c', 'v', 'b', 'n', 'm', 'p', ' ' },
};

const char keyboardKeys2[KEYPAD_ROWS][KEYPAD_COLS] = {
  { '1', '2', '3', '4', '5', '6', '7', '8', '9' },
  { '0', '@', '#', '$', '%', '&', '*', '(', ')' },
  { '-', '+', '=', '_', '/', '\\', '^', '<', '>' },
};

// Получить строку после ввода
String keyboard() {
  tft.setCursor(0, 0, 2);
  tft.fillScreen(TFT_BLACK);

  // Рисуем клавиатуру
  drawKeyboard();

  // Настраиваем цвета
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  inputString = "";
  while (true) {
    if (tft.getTouch(&ts_x, &ts_y)) {
      // Обработка нажатия на клавиатуру
      if (ts_x >= 0 && ts_y >= 0 && ts_x < TFT_WIDTH && ts_y < TFT_HEIGHT) {
        int col = (ts_x - KEY_MARGIN) / (KEY_WIDTH + KEY_MARGIN);
        int row = (ts_y - KEY_MARGIN) / (KEY_HEIGHT + KEY_MARGIN);

        // Проверяем, находится ли нажатие в пределах клавиатуры
        if (col >= 0 && col < KEYPAD_COLS && row >= 0 && row < KEYPAD_ROWS) {
          // Выводим нажатый символ
          char key = getCurrentKey(row, col);
          if (key != ' ') {
            if (inputString.length() <= 20) {
              inputString += key;
            }
          }
        } else {
          int buttonY = TFT_HEIGHT - (2 * KEY_HEIGHT) - KEY_MARGIN;
          int buttonWidth = KEY_WIDTH * 2 + KEY_MARGIN;
          int buttonHeight = KEY_HEIGHT + KEY_MARGIN;
          int button1X = KEY_MARGIN;
          int button2X = button1X + buttonWidth + KEY_MARGIN;
          int button3X = button2X + buttonWidth + KEY_MARGIN;
          int button4X = button3X + buttonWidth + KEY_MARGIN;

          if (ts_x >= button1X && ts_x < button1X + buttonWidth && ts_y >= buttonY && ts_y < buttonY + buttonHeight) {
            // Нажата кнопка "Del"
            if (!inputString.isEmpty()) {
              inputString.remove(inputString.length() - 1);
            }
          } else if (ts_x >= button2X && ts_x < button2X + buttonWidth && ts_y >= buttonY && ts_y < buttonY + buttonHeight) {
            // Нажата кнопка "Conf"
            if (!inputString.isEmpty())
              return inputString;
          } else if (ts_x >= button3X && ts_x < button3X + buttonWidth && ts_y >= buttonY && ts_y < buttonY + buttonHeight) {
            // Нажата кнопка "C Lock"
            if (currentPage == 0)
              capsLock = !capsLock;
            drawKeyboard();
          } else if (ts_x >= button4X && ts_x < button4X + buttonWidth && ts_y >= buttonY && ts_y < buttonY + buttonHeight) {
            // Нажата кнопка "N/P Page"
            currentPage = (currentPage + 1) % 2;
            drawKeyboard();
          }
        }
        tft.fillRect(0, TFT_HEIGHT - KEY_HEIGHT, TFT_WIDTH, KEY_HEIGHT, TFT_BLACK);
        tft.setCursor(0, TFT_HEIGHT - KEY_HEIGHT);
        tft.setTextSize(2);
        tft.print(inputString);
        tft.setTextSize(1);
      }
    }
  }
}

// Получить текущую кнопку
char getCurrentKey(int row, int col) {
  if (currentPage == 0 && capsLock == false)
    return keyboardKeys1[row][col];
  else if (currentPage == 0 && capsLock)
    return (char)toupper(keyboardKeys1[row][col]);
  else {
    return keyboardKeys2[row][col];
  }
}

// Нарисовать клавиатуру
void drawKeyboard() {
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(1);
  tft.fillScreen(TFT_BLACK);

  for (int row = 0; row < KEYPAD_ROWS; row++) {
    for (int col = 0; col < KEYPAD_COLS; col++) {
      int keyX = col * (KEY_WIDTH + KEY_MARGIN) + KEY_MARGIN;
      int keyY = row * (KEY_HEIGHT + KEY_MARGIN) + KEY_MARGIN;

      tft.fillRoundRect(keyX, keyY, KEY_WIDTH, KEY_HEIGHT, 10, TFT_DARKGREY);
      tft.setCursor(keyX + (KEY_WIDTH / 2) - 6, keyY + (KEY_HEIGHT / 2) - 6);
      tft.print(getCurrentKey(row, col));
    }
  }

  // Рисуем кнопки под клавиатурой
  int buttonY = TFT_HEIGHT - (2 * KEY_HEIGHT) - KEY_MARGIN;
  int buttonWidth = KEY_WIDTH * 2 + KEY_MARGIN;
  int buttonHeight = KEY_HEIGHT + KEY_MARGIN;

  tft.fillRoundRect(KEY_MARGIN, buttonY, buttonWidth, buttonHeight, 10, TFT_LIGHTGREEN);
  tft.setCursor(KEY_MARGIN + (buttonWidth / 2) - 18, buttonY + (buttonHeight / 2) - 6);
  tft.print("Delete");

  tft.fillRoundRect(KEY_MARGIN + buttonWidth + KEY_MARGIN, buttonY, buttonWidth, buttonHeight, 10, TFT_LIGHTGREEN);
  tft.setCursor(KEY_MARGIN + buttonWidth + KEY_MARGIN + (buttonWidth / 2) - 24, buttonY + (buttonHeight / 2) - 6);
  tft.print("Confirm");
  if (currentPage == 0) {
    tft.fillRoundRect(KEY_MARGIN + (2 * buttonWidth) + (2 * KEY_MARGIN), buttonY, buttonWidth, buttonHeight, 10, TFT_LIGHTGREEN);
    tft.setCursor(KEY_MARGIN + (2 * buttonWidth) + (2 * KEY_MARGIN) + (buttonWidth / 2) - 24, buttonY + (buttonHeight / 2) - 6);
    tft.print("Caps");
  }
  tft.fillRoundRect(KEY_MARGIN + (3 * buttonWidth) + (3 * KEY_MARGIN), buttonY, buttonWidth, buttonHeight, 10, TFT_LIGHTGREEN);
  tft.setCursor(KEY_MARGIN + (3 * buttonWidth) + (3 * KEY_MARGIN) + (buttonWidth / 2) - 18, buttonY + (buttonHeight / 2) - 6);
  tft.print("N/P Page");
}

// Получить время для верхней панели и нарисовать
void getLastTimeTopPanel() {
  if (isConnectToWIFI) {
    tft.setTextSize(2);
    tft.setTextColor(TFT_BLACK);
    tft.setCursor(400, 0, 2);
    tft.fillRect(400, 0, tft.width(), tft.fontHeight(), TFT_WHITE);
    timeClient.update();
    byte hours = timeClient.getHours();
    if (hours >= 0 && hours <= 9) {
      tft.print("0");
    }
    tft.print(hours);
    tft.print(":");
    byte minutes = timeClient.getMinutes();
    if (minutes >= 0 && minutes <= 9) {
      tft.print("0");
    }
    tft.print(minutes);
  }
}

// Нарисовать верхнюю панель
void drawTopPanel(bool needTime) {
  if (isConnectToWIFI) {
    tft.drawBitmap(0, 0, wifiIcon, 32, 32, TFT_BLACK);
    if (needTime){
      tft.setCursor(250, 0, 2);
    }
    else{
      tft.setCursor(180, 0, 2);
    }
    tft.setTextSize(2);
    tft.setTextColor(TFT_BLACK);
    tft.print(daysOfTheWeek[timeClient.getDay()]);
  }
  if (serverAlwaysOn && isConnectToWIFI) {
    tft.drawBitmap(40, 0, serverIcon, 32, 32, TFT_BLACK);
  } else if (isConnectToWIFI == false) {
    tft.setCursor(0, 0, 2);
    tft.setTextColor(TFT_BLACK);
    tft.print("X");
  }
  if (needTime) {
    getLastTimeTopPanel();
    tft.drawBitmap(170, 0, tempIcon, 32, 32, TFT_BLACK);
  }
}

// Надпись Initial setup в первоначальной настройке + верхняя панель
void drawInitialSetupLabel(bool needBelow) {
  tft.setTextColor(TFT_BLACK);
  if (needBelow) {
    drawTopPanel(true);
    tft.setCursor(10, 35, 2);
  } else {
    tft.setCursor(10, 0, 2);
  }
  tft.setTextSize(2);
  tft.println("Initial setup");
}

// Начальная настройка

// Кнопки

// Кнопка начала всего
void start_button_func() {
  tft.fillRoundRect(start_buttonX / 2, start_buttonY, start_buttonW * 2, start_buttonH, 10, TFT_LIGHTGREEN);
  tft.fillRoundRect(start_buttonFRAME_X, start_buttonFRAME_Y, start_buttonFRAME_W, start_buttonFRAME_H, 10, TFT_LIGHTGREEN);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("Begin!", (start_buttonX / 2) + start_buttonW, start_buttonY + (start_buttonH / 2));
}

// Кнопки имени пользователя и пароля
void drawbutUnPw(int x, int y, int width, int height, const char* label) {
  tft.fillRoundRect(x, y, width, height, 10, TFT_LIGHTGREEN);
  tft.fillRoundRect(x, y, width, height, 10, TFT_LIGHTGREEN);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(x + (width - strlen(label) * 12) / 2, y + (height - 16) / 2 - 12);
  tft.print(label);
}

void butUnPwfunc() {
  drawInitialSetupLabel();
  tft.setCursor(0, 0, 2);
  uint8_t butUnPw1X = (TFT_WIDTH - butUnPw_WIDTH) / 2;
  uint8_t butUnPw1Y = (TFT_HEIGHT - 2 * butUnPw_HEIGHT - butUnPw_MARGIN) / 2;
  uint8_t butUnPw2X = butUnPw1X;
  uint8_t butUnPw2Y = butUnPw1Y + butUnPw_HEIGHT + butUnPw_MARGIN;
  drawbutUnPw(butUnPw1X, butUnPw1Y, butUnPw_WIDTH, butUnPw_HEIGHT, "Username");
  drawbutUnPw(butUnPw2X, butUnPw2Y, butUnPw_WIDTH, butUnPw_HEIGHT, "Password");
  bool UnPass = false;
  bool PwPass = false;
  bool allPass = false;
  while (allPass != true) {
    if (tft.getTouch(&ts_x, &ts_y)) {
      // Проверяем нажатие на кнопки
      if (ts_x >= butUnPw1X && ts_x < butUnPw1X + butUnPw_WIDTH && ts_y >= butUnPw1Y && ts_y < butUnPw1Y + butUnPw_HEIGHT) {
        // Нажата кнопка "Username"
        username = keyboard();
        tft.fillScreen(TFT_WHITE);
        if (PwPass == false) {
          tft.setCursor(0, 0, 2);
          tft.setTextSize(2);
          tft.setTextColor(TFT_BLACK);
          tft.println("Now you need to enter a password");
          drawbutUnPw(butUnPw2X, butUnPw2Y, butUnPw_WIDTH, butUnPw_HEIGHT, "Password");
        }
        UnPass = true;
      } else if (ts_x >= butUnPw2X && ts_x < butUnPw2X + butUnPw_WIDTH && ts_y >= butUnPw2Y && ts_y < butUnPw2Y + butUnPw_HEIGHT) {
        // Нажата кнопка "Password"
        password = keyboard();
        tft.fillScreen(TFT_WHITE);
        if (UnPass == false) {
          tft.setCursor(0, 0, 2);
          tft.setTextSize(2);
          tft.setTextColor(TFT_BLACK);
          tft.println("Now you need to enter a username");
          drawbutUnPw(butUnPw1X, butUnPw1Y, butUnPw_WIDTH, butUnPw_HEIGHT, "Username");
        }
        PwPass = true;
      }
    }
    if (UnPass && PwPass)
      allPass = true;
  }
  int len = username.length();
  EEPROM.write(Username_ADDR, len);
  for (int i = 0; i < len; i++) {
    EEPROM.write(Username_ADDR + 1 + i, username[i]);
  }
  len = password.length();
  EEPROM.write(Password_ADDR, len);
  for (int i = 0; i < len; i++) {
    EEPROM.write(Password_ADDR + 1 + i, password[i]);
  }
}

// Приветствие
void Greetings() {
  tft.fillScreen(TFT_WHITE);
  tft.setTextFont(10);
  tft.setTextSize(2);
  tft.setCursor(50, 100, 2);
  tft.print("Welcome to XenoCore OS ");
  tft.println(version);
  tft.setCursor(20, 0);
  bool start_button = false;
  start_button_func();
  while (start_button != true) {
    if (tft.getTouch(&ts_x, &ts_y)) {
      if (start_button == false) {
        if ((ts_x > start_buttonX / 2 - 128 + start_buttonW * 2) && (ts_x < (start_buttonX / 2 - 128 + start_buttonW * 2 + start_buttonW * 2))) {
          if ((ts_y > start_buttonY) && (ts_y <= (start_buttonY + start_buttonW))) {
            start_button = true;
          }
        }
      }
    }
  }
  tft.fillScreen(TFT_WHITE);
}

// Подключение к интернету
void InitialConnectNet() {
  tft.fillScreen(TFT_WHITE);
  drawInitialSetupLabel();
  tft.setCursor(0, 35, 2);
  tft.setTextSize(2);
  tft.setTextColor(TFT_BLACK);
  tft.println("You need to be connected to the internet");
  tft.print("You need to enter a name and password");
  tft.setCursor(0, 0, 2);
  uint8_t butUnPwNet1X = (TFT_WIDTH - butUnPwNet_WIDTH) / 2;
  uint8_t butUnPwNet1Y = (TFT_HEIGHT - 2 * butUnPwNet_HEIGHT - butUnPwNet_MARGIN) / 2;
  uint8_t butUnPwNet2X = butUnPwNet1X;
  uint8_t butUnPwNet2Y = butUnPwNet1Y + butUnPwNet_HEIGHT + butUnPwNet_MARGIN;
  drawbutUnPw(butUnPwNet1X, butUnPwNet1Y, butUnPwNet_WIDTH, butUnPwNet_HEIGHT, "Name");
  drawbutUnPw(butUnPwNet2X, butUnPwNet2Y, butUnPwNet_WIDTH, butUnPwNet_HEIGHT, "Password");
  bool UnPassNet = false;
  bool PwPassNet = false;
  bool allPassNet = false;
  while (allPassNet != true) {
    if (tft.getTouch(&ts_x, &ts_y)) {
      // Проверяем нажатие на кнопки
      if (ts_x >= butUnPwNet1X && ts_x < butUnPwNet1X + butUnPwNet_WIDTH && ts_y >= butUnPwNet1Y && ts_y < butUnPwNet1Y + butUnPwNet_HEIGHT) {
        // Нажата кнопка "Name"
        ssidNet = keyboard();
        tft.fillScreen(TFT_WHITE);
        if (PwPassNet == false) {
          tft.setCursor(0, 0, 2);
          tft.setTextSize(2);
          tft.setTextColor(TFT_BLACK);
          tft.println("Now you need to enter a password");
          drawbutUnPw(butUnPwNet2X, butUnPwNet2Y, butUnPwNet_WIDTH, butUnPwNet_HEIGHT, "Password");
        }
        UnPassNet = true;
      } else if (ts_x >= butUnPwNet2X && ts_x < butUnPwNet2X + butUnPwNet_WIDTH && ts_y >= butUnPwNet2Y && ts_y < butUnPwNet2Y + butUnPwNet_HEIGHT) {
        // Нажата кнопка "Password"
        passwordNet = keyboard();
        tft.fillScreen(TFT_WHITE);
        if (UnPassNet == false) {
          tft.setCursor(0, 0, 2);
          tft.setTextSize(2);
          tft.setTextColor(TFT_BLACK);
          tft.println("Now you need to enter a name");
          drawbutUnPw(butUnPwNet1X, butUnPwNet1Y, butUnPwNet_WIDTH, butUnPwNet_HEIGHT, "Name");
        }
        PwPassNet = true;
      }
    }
    if (UnPassNet && PwPassNet) {
      tft.fillScreen(TFT_WHITE);
      tft.setCursor(0, 0, 2);
      tft.setTextSize(2);
      tft.setTextColor(TFT_BLACK);
      tft.println("Connecting");
      WiFi.begin(ssidNet.c_str(), passwordNet.c_str());
      uint8_t time = 0;
      while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        time++;
        tft.print(".");
        if (time > 30) {
          UnPassNet = false;
          PwPassNet = false;
          allPassNet = false;
          tft.println("Connection failure");
          delay(2500);
          InitialConnectNet();
        }
      }
      tft.println("");
      tft.print("Connected to ");
      tft.println(ssidNet);
      tft.print("IP address: ");
      tft.print(WiFi.localIP());
      allPassNet = true;
      timeClient.begin();
      delay(3500);
    }
  }
  int len = ssidNet.length();
  EEPROM.write(SsidNet_ADDR, len);
  for (int i = 0; i < len; i++) {
    EEPROM.write(SsidNet_ADDR + 1 + i, ssidNet[i]);
  }
  len = passwordNet.length();
  EEPROM.write(PasswordNet_ADDR, len);
  for (int i = 0; i < len; i++) {
    EEPROM.write(PasswordNet_ADDR + 1 + i, passwordNet[i]);
  }
}

// Функция показа, что первоначальная настройка пройдена
void InitialSetupComplete() {
  tft.fillScreen(TFT_WHITE);
  tft.setCursor(0, 0, 2);
  tft.setTextSize(2);
  tft.setTextColor(TFT_BLACK);
  tft.println("Initial setup completed successfully!");
  delay(1500);
  tft.print("Rebooting...");
  delay(2000);
  EEPROM.write(0, 1);
  EEPROM.commit();
  ESP.restart();
}

// Первоначальная настройка
void InitialSetup() {
  tft.setTextFont(2);
  tft.setTextColor(TFT_BLACK);
  tft.println("Started initial setup");
  delay(2500);
  tft.fillScreen(TFT_WHITE);
  Calibration();
  tft.setTextSize(2);
  tft.println("Calibration Successfull");
  delay(1500);
  tft.fillScreen(TFT_WHITE);
  Greetings();
  tft.setCursor(0, 0, 2);
  tft.setTextColor(TFT_BLACK);
  tft.println("Preparing first turn on...");
  delay(1500);
  tft.fillScreen(TFT_WHITE);
  tft.setCursor(0, 35, 2);
  tft.setTextSize(2);
  tft.setTextColor(TFT_BLACK);
  tft.print("You need to enter username and password");
  butUnPwfunc();
  tft.fillScreen(TFT_WHITE);
  drawInitialSetupLabel();
  tft.setCursor(0, 35, 2);
  tft.setTextSize(2);
  tft.setTextColor(TFT_BLACK);
  tft.print("Now you need to connect to the network");
  // Кнопка пропустить
  int SkipX = TFT_WIDTH - Skip_WIDTH - Skip_MARGIN;
  int SkipY = TFT_HEIGHT - Skip_HEIGHT - Skip_MARGIN;
  tft.fillRoundRect(SkipX, SkipY, Skip_WIDTH, Skip_HEIGHT, 10, TFT_LIGHTGREEN);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(SkipX + (Skip_WIDTH / 2) - 30, SkipY + (Skip_HEIGHT / 2) - 15);
  tft.println("Skip");
  // Кнопка продолжить
  #define contsettings_WIDTH 150
  #define contsettings_HEIGHT 40
  #define contsettings_MARGIN 20
  int contsettingsX = contsettings_MARGIN;
  int contsettingsY = TFT_HEIGHT - contsettings_HEIGHT - contsettings_MARGIN;
  tft.fillRoundRect(contsettingsX, contsettingsY, contsettings_WIDTH, contsettings_HEIGHT, 10, TFT_LIGHTGREEN);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(contsettingsX + (contsettings_WIDTH / 2) - 60, contsettingsY + (contsettings_HEIGHT / 2) - 10);
  tft.println("Continue");
  bool isConnectToWIFISkip = false;
  bool testpassed = false;
  while (testpassed != true) {
    if (tft.getTouch(&ts_x, &ts_y)) {
      // Проверка нажатия кнопки
      if (ts_x >= SkipX && ts_x < SkipX + Skip_WIDTH && ts_y >= SkipY && ts_y < SkipY + Skip_HEIGHT) {
        isConnectToWIFISkip = true;
        testpassed = true;
      }
      if (ts_x >= contsettingsX && ts_x < contsettingsX + contsettings_WIDTH && ts_y >= contsettingsY && ts_y < contsettingsY + contsettings_HEIGHT) {
        isConnectToWIFISkip = false;
        testpassed = true;
      }
    }
  }
  if (isConnectToWIFISkip == false) {
    InitialConnectNet();
  }
  timeEdit(true);
}

// Рисовка значений с MQ135
void drawppm(){
  int MQ135_data = analogRead(34);
  tft.setTextSize(2);
  tft.setCursor(0, 250, 2);
  tft.setCursor(35, 250, 2);
  tft.fillRect(35, 250, 150, tft.fontHeight(), TFT_WHITE);
  tft.print(MQ135_data); // analog data
  tft.print(" ppm"); // Unit = part per million
}

// Система
void system() {
  tft.fillScreen(TFT_WHITE);
  tft.setCursor(0, 0, 2);
  int DesktopX = TFT_WIDTH - Desktop_WIDTH - Desktop_MARGIN;
  int DesktopY = TFT_HEIGHT - Desktop_HEIGHT - Desktop_MARGIN;
  tft.fillRoundRect(DesktopX, DesktopY, Desktop_WIDTH, Desktop_HEIGHT, 10, TFT_LIGHTGREEN);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(DesktopX + (Desktop_WIDTH / 2) - 40, DesktopY + (Desktop_HEIGHT / 2) - 15);
  tft.println("Desktop");
  WiFi.begin(ssidNet.c_str(), passwordNet.c_str());
  uint8_t time = 0;
  if (WiFi.status() != WL_CONNECTED && wificheckPassed == false) {
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      time++;
      if (time > 10) {
        isConnectToWIFI = false;
        wificheckPassed = true;
        break;
      }
    }
  }
  if (WiFi.status() == WL_CONNECTED) {
    timeClient.begin();
    isConnectToWIFI = true;
  } else if (WiFi.status() != WL_CONNECTED) {
    isConnectToWIFI = false;
  }
  drawTopPanel();
  tft.setCursor(0, 0, 2);
  tft.setTextSize(4);
  tft.setTextColor(TFT_BLACK);
  if (serverAlwaysOn && isConnectToWIFI) {
    server.begin();
    ServerPowerUp();
  }
  tft.drawBitmap(0, 210, tempIcon, 32, 32, TFT_BLACK);
  tft.drawBitmap(0, 245, co2ppmIcom, 32, 32, TFT_BLACK);
  tft.drawBitmap(0, 280, humidityIcon, 32, 32, TFT_BLACK);
  if (flashlightOn){
    tft.drawBitmap(80, 0, flashlightIcon, 32, 32, TFT_BLACK);
  }
  timeToSleep = 0;
  while (true) {
    //Serial.println(analogRead(mq135));
    if (millis() - timing1 >= 10000){
      timing1 = millis();
      drawppm();
      tft.setTextSize(4);
    }
    if (millis() - timing2 >= 1000){
      timing2 = millis();
      timeToSleep++;
    }

    if (timeToSleep > 20){
      esp_deep_sleep_start();
    }

    if (serverAlwaysOn && isConnectToWIFI) {
      server.handleClient();
    }
    if (isConnectToWIFI) {
      if (millis() - timing >= 1000) {
        timing = millis();
        tft.setCursor(35, 105, 4);
        tft.fillRect(35, 105, tft.width(), tft.fontHeight(), TFT_WHITE);
        timeClient.update();
        tft.print(timeClient.getFormattedTime());
      }
    }
    if (tft.getTouch(&ts_x, &ts_y)) {
      timeToSleep = 0;
      // Проверка нажатия кнопки
      if (ts_x >= DesktopX && ts_x < DesktopX + Desktop_WIDTH && ts_y >= DesktopY && ts_y < DesktopY + Desktop_HEIGHT) {
        Desktop();
      }
    }
  }
}

// Рабочий стол
void Desktop() {
  tft.fillScreen(TFT_WHITE);
  drawTopPanel(true);
  tft.setTextColor(TFT_BLACK);
  tft.setCursor(10, 35, 2);
  tft.setTextSize(2);
  tft.println("Desktop");
  // Кнопки
  // Кнопка настроек
  int SettingsX = Settings_MARGIN;
  int SettingsY = 82;
  tft.fillRoundRect(SettingsX, SettingsY, Settings_WIDTH, Settings_HEIGHT, 10, TFT_LIGHTGREEN);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(SettingsX + (Settings_WIDTH / 2) - 50, SettingsY + (Settings_HEIGHT / 2) - 15, 2);
  tft.println("Settings");
  // Кнопка фонарика
  int FlashLightX = 175;
  int FlashLightY = 82;
  tft.fillRoundRect(FlashLightX, FlashLightY, FlashLight_WIDTH, FlashLight_HEIGHT, 10, TFT_LIGHTGREEN);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(FlashLightX + (FlashLight_WIDTH / 2) - 50, FlashLightY + (FlashLight_HEIGHT / 2) - 15, 2);
  tft.println("Flashlight");
  // Кнопка назад
  drawButBack();
  if (serverAlwaysOn && isConnectToWIFI) {
    ServerPowerUp();
  }
  if (flashlightOn){
    tft.drawBitmap(80, 0, flashlightIcon, 32, 32, TFT_BLACK);
  }
  timeToSleep = 0;
  while (true) {
    if (serverAlwaysOn && isConnectToWIFI) {
      server.handleClient();
    }
    if (millis() - timing2 >= 1000){
      timing2 = millis();
      timeToSleep++;
    }

    if (timeToSleep > 20){
      esp_deep_sleep_start();
    }
    if (tft.getTouch(&ts_x, &ts_y)) {
      timeToSleep = 0;
      // Проверка нажатия кнопки
      if (ts_x >= SettingsX && ts_x < SettingsX + Settings_WIDTH && ts_y >= SettingsY && ts_y < SettingsY + Settings_HEIGHT) {
        Settings();
      }
      if (ts_x >= FlashLightX && ts_x < FlashLightX + FlashLight_WIDTH && ts_y >= FlashLightY && ts_y < FlashLightY + FlashLight_HEIGHT) {
        if (flashlightOn == false){
          flashlightOn = true;
          digitalWrite(flashLight, HIGH);
          tft.drawBitmap(80, 0, flashlightIcon, 32, 32, TFT_BLACK);
          delay(100);
        }
        else{
          flashlightOn = false;
          digitalWrite(flashLight, LOW);
          tft.fillRect(80, 0, 32, 32, TFT_WHITE);
          delay(100);
        }
      }
      if (ts_x >= BackX && ts_x < BackX + Back_WIDTH && ts_y >= BackY && ts_y < BackY + Back_HEIGHT) {
        system();
      }
    }
    if (millis() - timing >= 1000) {
      timing = millis();
      getLastTimeTopPanel();
    }
  }
}

// Сброс настроек к заводскому состоянию
void FactoryReset() {
  tft.fillScreen(TFT_WHITE);
  drawTopPanel(true);
  tft.setTextColor(TFT_BLACK);
  tft.setCursor(0, 35, 2);
  tft.println("Factory Reset Started");
  tft.println("Progress: 0%");
  delay(1500);
  for (int i = 0; i < 307; i++) {
    if (i == 50) tft.println("Progress: 20%");
    if (i == 125) tft.println("Progress: 40%");
    if (i == 200) tft.println("Progress: 60%");
    if (i == 250) tft.println("Progress: 80%");
    EEPROM.put(i, 0);
  }
  SPIFFS.format();
  tft.println("Progress: 100%");
  EEPROM.commit();
  tft.println("Rebooting...");
  delay(2000);
  ESP.restart();
}

// Информирование о сбросе настроек
void FactoryResetInformation() {
  tft.fillScreen(TFT_WHITE);
  drawTopPanel(true);
  tft.setTextColor(TFT_BLACK);
  tft.setCursor(0, 35, 2);
  tft.println("Factory reset warning");
  tft.println("When you click the 'Continue' button, all device settings will be reset to factory settings. All user data will be deleted. Are you sure you want to reset your device to factory settings?");
  // Кнопка назад
  drawButBack();
  // Кнопка продолжить
  int continueupdateX = continueupdate_MARGIN;
  int continueupdateY = TFT_HEIGHT - continueupdate_HEIGHT - continueupdate_MARGIN;
  tft.fillRoundRect(continueupdateX, continueupdateY, continueupdate_WIDTH, continueupdate_HEIGHT, 10, TFT_LIGHTGREEN);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(continueupdateX + (continueupdate_WIDTH / 2) - 50, continueupdateY + (continueupdate_HEIGHT / 2) - 10);
  tft.println("Continue");
  if (serverAlwaysOn && isConnectToWIFI) {
    ServerPowerUp();
  }
  if (flashlightOn){
    tft.drawBitmap(80, 0, flashlightIcon, 32, 32, TFT_BLACK);
  }
  timeToSleep = 0;
  while (true) {
    if (serverAlwaysOn && isConnectToWIFI) {
      server.handleClient();
    }
    if (millis() - timing2 >= 1000){
      timing2 = millis();
      timeToSleep++;
    }

    if (timeToSleep > 20){
      esp_deep_sleep_start();
    }
    if (tft.getTouch(&ts_x, &ts_y)) {
      timeToSleep = 0;
      // Проверка нажатия кнопки
      if (ts_x >= BackX && ts_x < BackX + Back_WIDTH && ts_y >= BackY && ts_y < BackY + Back_HEIGHT) {
        SettingsFuncs();
      }
      if (ts_x >= continueupdateX && ts_x < continueupdateX + continueupdate_WIDTH && ts_y >= continueupdateY && ts_y < continueupdateY + continueupdate_HEIGHT) {
        FactoryReset();
      }
    }
  }
}

// Изменение имени пользователя
void ChangeUsername() {
  username = keyboard();
  int len = username.length();
  EEPROM.write(Username_ADDR, len);
  for (int i = 0; i < len; i++) {
    EEPROM.put(Username_ADDR + 1 + i, username[i]);
  }
  EEPROM.commit();
  tft.setTextColor(TFT_BLACK);
  tft.setCursor(0, 0, 2);
  tft.setTextSize(2);
  Settings();
}

// Изменение пароля
void ChangePassword() {
  password = keyboard();
  int len = password.length();
  EEPROM.write(Password_ADDR, len);
  for (int i = 0; i < len; i++) {
    EEPROM.put(Password_ADDR + 1 + i, password[i]);
  }
  EEPROM.commit();
  tft.setTextColor(TFT_BLACK);
  tft.setCursor(0, 0, 2);
  tft.setTextSize(2);
  Settings();
}

// Изменение имени устройства
void ChangeDeviceName() {
  deviceName = keyboard();
  int len = deviceName.length();
  EEPROM.write(DeviceName_ADDR, len);
  for (int i = 0; i < len; i++) {
    EEPROM.put(DeviceName_ADDR + 1 + i, deviceName[i]);
  }
  EEPROM.commit();
  tft.setTextColor(TFT_BLACK);
  tft.setCursor(0, 0, 2);
  tft.setTextSize(2);
  Settings();
}

// Функция запуска сервера
void ServerPowerUp() {
  server.on("/", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html",
                "<title>Login Page</title>"
                "<form name='loginForm'>"
                "<table width='20%' bgcolor='A09F9F' align='center'>"
                "<tr>"
                "<td colspan=2>"
                "<center><font size=4><b>XenoClock Login Page</b></font></center>"
                "<br>"
                "</td>"
                "<br>"
                "<br>"
                "</tr>"
                "<tr>"
                "<td>Username:</td>"
                "<td><input type='text' size=25 name='userid'><br></td>"
                "</tr>"
                "<br>"
                "<br>"
                "<tr>"
                "<td>Password:</td>"
                "<td><input type='Password' size=25 name='pwd'><br></td>"
                "<br>"
                "<br>"
                "</tr>"
                "<tr>"
                "<td><input type='submit' onclick='check(this.form)' value='Login'></td>"
                "</tr>"
                "</table>"
                "</form>"
                "<style>"
                "body {"
                "font-family: Arial, sans-serif;"
                "background-color: #f2f2f2;"
                "}"
                ""
                "table {"
                "border-collapse: collapse;"
                "box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);"
                "background-color: #ffffff;"
                "}"
                ""
                "td {"
                "padding: 10px;"
                "  }"
                ""
                "td:first-child {"
                "text-align: right;"
                "font-weight: bold;"
                "}"
                ""
                "input[type='text'],"
                "input[type='password'] {"
                "width: 100%;"
                "padding: 8px;"
                "border: 1px solid #ccc;"
                "border-radius: 4px;"
                "}"
                ""
                "input[type='submit'] {"
                "width: 100%;"
                "padding: 10px;"
                "background-color: #4CAF50;"
                "color: #fff;"
                "border: none;"
                "border-radius: 4px;"
                "cursor: pointer;"
                "font-weight: bold;"
                "}"
                ""
                "input[type='submit']:hover {"
                "background-color: #45a049;"
                "}"
                ""
                "center {"
                "margin-top: 20px;"
                "margin-bottom: 20px;"
                "}"
                "</style>"
                "<script>"
                "function check(form)"
                "{"
                "if(form.userid.value=='"
                  + username + "' && form.pwd.value=='" + password + "')"
                                                                     "{"
                                                                     "window.open('/mainpage')"
                                                                     "}"
                                                                     "else"
                                                                     "{"
                                                                     " alert('Error Password or Username')/*displays error message*/"
                                                                     "}"
                                                                     "}"
                                                                     "</script>");
  });
  server.on("/updatesystem", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", serverUpdateSystem);
  });
  server.on("/mainpage", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", serverMainPage);
  });
  /*handling uploading firmware file */
  server.on(
    "/update", HTTP_POST, []() {
      server.sendHeader("Connection", "close");
      server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
      ESP.restart();
    },
    []() {
      HTTPUpload& upload = server.upload();
      if (upload.status == UPLOAD_FILE_START) {
        tft.printf("Update: %s\n", upload.filename.c_str());
        if (!Update.begin(UPDATE_SIZE_UNKNOWN)) {  //start with max available size
          Update.printError(Serial);
        }
      } else if (upload.status == UPLOAD_FILE_WRITE) {
        /* flashing firmware to ESP*/
        if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
          Update.printError(Serial);
        }
      } else if (upload.status == UPLOAD_FILE_END) {
        if (Update.end(true)) {  //true to set the size to the current progress
          tft.println("");
          tft.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
        } else {
          Update.printError(Serial);
        }
      }
    });
}

// Обновление системы
void UpdatingSystem() {
  tft.fillScreen(TFT_WHITE);
  drawTopPanel(true);
  drawButBack();
  tft.setCursor(0, 35, 2);
  tft.println("Update System");
  tft.println("");
  tft.println("mDNS responder started");
  tft.print("Link: ");
  tft.print(WiFi.localIP());
  tft.println(":80");
  ServerPowerUp();
  server.begin();
  while (true) {
    if (isConnectToWIFI) {
      server.handleClient();
    } else if (!isConnectToWIFI) {
      tft.print("You are not connected to WIFI");
    }
    if (tft.getTouch(&ts_x, &ts_y)) {
      // Проверка нажатия кнопки
      if (ts_x >= BackX && ts_x < BackX + Back_WIDTH && ts_y >= BackY && ts_y < BackY + Back_HEIGHT) {
        InstructionUpdateSystem();
      }
      delay(1);
    }
  }
}

// Настройки сервера
void serverSettings() {
  tft.fillScreen(TFT_WHITE);
  drawTopPanel(true);
  tft.setTextColor(TFT_BLACK);
  tft.setCursor(10, 35, 2);
  tft.println("Server Settings");
  tft.print("Will the server be on all the time?");
  uint8_t butUnPw1X = (TFT_WIDTH - butUnPw_WIDTH) / 2;
  uint8_t butUnPw1Y = (TFT_HEIGHT - 2 * butUnPw_HEIGHT - butUnPw_MARGIN) / 2;
  uint8_t butUnPw2X = butUnPw1X;
  uint8_t butUnPw2Y = butUnPw1Y + butUnPw_HEIGHT + butUnPw_MARGIN;
  drawbutUnPw(butUnPw1X, butUnPw1Y, butUnPw_WIDTH, butUnPw_HEIGHT, "On");
  drawbutUnPw(butUnPw2X, butUnPw2Y, butUnPw_WIDTH, butUnPw_HEIGHT, "Off");
  if (serverAlwaysOn && isConnectToWIFI) {
    ServerPowerUp();
  }
  if (flashlightOn){
    tft.drawBitmap(80, 0, flashlightIcon, 32, 32, TFT_BLACK);
  }
  // Кнопка назад
  drawButBack();
  timeToSleep = 0;
  while (true) {
    if (serverAlwaysOn && isConnectToWIFI) {
      server.handleClient();
    }
    if (millis() - timing2 >= 1000){
      timing2 = millis();
      timeToSleep++;
    }

    if (timeToSleep > 20){
      esp_deep_sleep_start();
    }
    if (tft.getTouch(&ts_x, &ts_y)) {
      timeToSleep = 0;
      // Проверка нажатия кнопки
      if (ts_x >= BackX && ts_x < BackX + Back_WIDTH && ts_y >= BackY && ts_y < BackY + Back_HEIGHT) {
        timeEdit();
      }
      if (ts_x >= BackX && ts_x < BackX + Back_WIDTH && ts_y >= BackY && ts_y < BackY + Back_HEIGHT) {
        SettingsFuncs();
      }
      if (ts_x >= butUnPw1X && ts_x < butUnPw1X + butUnPw_WIDTH && ts_y >= butUnPw1Y && ts_y < butUnPw1Y + butUnPw_HEIGHT) {
        // Да
        EEPROM.write(serverAlwaysOn_ADDR + 1, 1);
        EEPROM.commit();
        timeClient.setTimeOffset(utcTimePlus ? utcTime * 3600 : -utcTime * 3600);
        server.begin();
        SettingsFuncs();

      } else if (ts_x >= butUnPw2X && ts_x < butUnPw2X + butUnPw_WIDTH && ts_y >= butUnPw2Y && ts_y < butUnPw2Y + butUnPw_HEIGHT) {
        // Нет
        EEPROM.write(serverAlwaysOn_ADDR + 1, 0);
        EEPROM.commit();
        SettingsFuncs();
      }
    }
  }
}

// Изменение времени UTC 2
void timeEdit2(bool initialSetup) {
  tft.fillScreen(TFT_WHITE);
  drawTopPanel(true);
  tft.setTextColor(TFT_BLACK);
  if (initialSetup) {
    drawInitialSetupLabel(true);
  }
  // Кнопка назад
  else if (initialSetup == false) {
    drawButBack();
  }
  tft.setCursor(10, 200, 2);
  tft.println("Change time");
  tft.print("You need to enter the hour difference in UTC format. In the format 'x' - hours.");
  uint8_t butUnPw1X = (TFT_WIDTH - butUnPw_WIDTH) / 2;
  uint8_t butUnPw1Y = (TFT_HEIGHT - 2 * butUnPw_HEIGHT - butUnPw_MARGIN) / 2;
  uint8_t butUnPw2X = butUnPw1X;
  uint8_t butUnPw2Y = butUnPw1Y + butUnPw_HEIGHT + butUnPw_MARGIN;
  drawbutUnPw(butUnPw1X, butUnPw1Y, butUnPw_WIDTH, butUnPw_HEIGHT, "Plus");
  drawbutUnPw(butUnPw2X, butUnPw2Y, butUnPw_WIDTH, butUnPw_HEIGHT, "Minus");
  if (serverAlwaysOn && isConnectToWIFI) {
    ServerPowerUp();
  }
  while (true) {
    if (serverAlwaysOn && isConnectToWIFI) {
      server.handleClient();
    }
    if (tft.getTouch(&ts_x, &ts_y)) {
      // Проверка нажатия кнопки
      if (ts_x >= BackX && ts_x < BackX + Back_WIDTH && ts_y >= BackY && ts_y < BackY + Back_HEIGHT & initialSetup == false) {
        timeEdit();
      }
      if (ts_x >= butUnPw1X && ts_x < butUnPw1X + butUnPw_WIDTH && ts_y >= butUnPw1Y && ts_y < butUnPw1Y + butUnPw_HEIGHT) {
        // Плюс
        EEPROM.write(utcTimePlus_ADDR + 1, 1);
        EEPROM.commit();
        tft.fillScreen(TFT_WHITE);
        drawTopPanel(true);
        if (initialSetup == false) {
          timeClient.setTimeOffset(utcTimePlus ? utcTime * 3600 : -utcTime * 3600);
          SettingsFuncs();
        } else {
          InitialSetupComplete();
        }
      } else if (ts_x >= butUnPw2X && ts_x < butUnPw2X + butUnPw_WIDTH && ts_y >= butUnPw2Y && ts_y < butUnPw2Y + butUnPw_HEIGHT) {
        // Минус
        EEPROM.write(utcTimePlus_ADDR + 1, 0);
        EEPROM.commit();
        tft.fillScreen(TFT_WHITE);
        drawTopPanel(true);
        if (initialSetup == false) {
          timeClient.setTimeOffset(utcTimePlus ? utcTime * 3600 : -utcTime * 3600);
          SettingsFuncs();
        } else {
          InitialSetupComplete();
        }
      }
    }
  }
}

// Изменение времени UTC 1
void timeEdit(bool initialSetup) {
  tft.fillScreen(TFT_WHITE);
  drawTopPanel(true);
  tft.setTextColor(TFT_BLACK);
  if (initialSetup) {
    drawInitialSetupLabel(true);
    tft.setCursor(10, 70, 2);
  }
  // Кнопка назад
  else if (initialSetup == false) {
    drawButBack();
    tft.setCursor(10, 35, 2);
  }
  tft.println("Change time");
  tft.print("You need to enter the hour difference in UTC format. In the format 'x' - hours.");


  // Кнопка продолжить
  int continueupdateX = continueupdate_MARGIN;
  int continueupdateY = TFT_HEIGHT - continueupdate_HEIGHT - continueupdate_MARGIN;
  tft.fillRoundRect(continueupdateX, continueupdateY, continueupdate_WIDTH, continueupdate_HEIGHT, 10, TFT_LIGHTGREEN);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(continueupdateX + (continueupdate_WIDTH / 2) - 50, continueupdateY + (continueupdate_HEIGHT / 2) - 10);
  tft.println("Continue");
  if (serverAlwaysOn && isConnectToWIFI) {
    ServerPowerUp();
  }
  while (true) {
    if (serverAlwaysOn && isConnectToWIFI) {
      server.handleClient();
    }
    if (tft.getTouch(&ts_x, &ts_y)) {
      // Проверка нажатия кнопки
      if (ts_x >= BackX && ts_x < BackX + Back_WIDTH && ts_y >= BackY && ts_y < BackY + Back_HEIGHT && initialSetup == false) {
        SettingsFuncs();
      }
      if (ts_x >= continueupdateX && ts_x < continueupdateX + continueupdate_WIDTH && ts_y >= continueupdateY && ts_y < continueupdateY + continueupdate_HEIGHT) {
        utcTime = atoi((keyboard()).c_str());
        Serial.println(utcTime);
        EEPROM.write(utcTime_ADDR + 1, utcTime);
        EEPROM.commit();
        if (initialSetup == false) {
          timeEdit2();
        } else {
          tft.setTextSize(2);
          timeEdit2(true);
        }
      }
    }
  }
}

// Все функции настроек
void SettingsFuncs() {
  tft.fillScreen(TFT_WHITE);
  drawTopPanel(true);
  tft.setTextColor(TFT_BLACK);
  tft.setCursor(10, 35, 2);
  tft.println("Settings Functions");
  // Кнопка обновления
  int updateosX = updateos_MARGIN;
  int updateosY = 75;
  tft.fillRoundRect(updateosX, updateosY, updateos_WIDTH, updateos_HEIGHT, 10, TFT_LIGHTGREEN);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(updateosX + (updateos_WIDTH / 2) - 60, updateosY + (updateos_HEIGHT / 2) - 10);
  tft.println("Update OS");

  // Кнопка назад
  drawButBack();

  // Кнопка сброса
  int FactoryResetX = 200;
  int FactoryResetY = 75;
  tft.fillRoundRect(FactoryResetX, FactoryResetY, FactoryReset_WIDTH, FactoryReset_HEIGHT, 10, TFT_LIGHTGREEN);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(FactoryResetX + (FactoryReset_WIDTH / 2) - 70, FactoryResetY + (FactoryReset_HEIGHT / 2) - 15);
  tft.println("Fact.reset");

  // Кнопка изменения имени пользователя
  int UsernameEditX = UsernameEdit_MARGIN;
  int UsernameEditY = 125;
  tft.fillRoundRect(UsernameEditX, UsernameEditY, UsernameEdit_WIDTH, UsernameEdit_HEIGHT, 10, TFT_LIGHTGREEN);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(UsernameEditX + (UsernameEdit_WIDTH / 2) - 70, UsernameEditY + (UsernameEdit_HEIGHT / 2) - 15);
  tft.println("Change UsN");

  // Кнопка изменения пароля
  int PasswordEditX = 200;
  int PasswordEditY = 125;
  tft.fillRoundRect(PasswordEditX, PasswordEditY, PasswordEdit_WIDTH, PasswordEdit_HEIGHT, 10, TFT_LIGHTGREEN);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(PasswordEditX + (PasswordEdit_WIDTH / 2) - 70, PasswordEditY + (PasswordEdit_HEIGHT / 2) - 15);
  tft.println("Change Pw");

  // Кнопка перезагрузки
  int RebootX = Reboot_MARGIN;
  int RebootY = 175;
  tft.fillRoundRect(RebootX, RebootY, Reboot_WIDTH, Reboot_HEIGHT, 10, TFT_LIGHTGREEN);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(RebootX + (Reboot_WIDTH / 2) - 70, RebootY + (Reboot_HEIGHT / 2) - 15);
  tft.println("Reboot");

  // Кнопка изменения имени устройства
  int DeviceNameEditX = 200;
  int DeviceNameEditY = 175;
  tft.fillRoundRect(DeviceNameEditX, DeviceNameEditY, DeviceNameEdit_WIDTH, DeviceNameEdit_HEIGHT, 10, TFT_LIGHTGREEN);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(DeviceNameEditX + (DeviceNameEdit_WIDTH / 2) - 70, DeviceNameEditY + (DeviceNameEdit_HEIGHT / 2) - 15);
  tft.println("Device Name");

  // Кнопка изменения WIFI
  int WIFIEditX = WIFIEdit_MARGIN;
  int WIFIEditY = 225;
  tft.fillRoundRect(WIFIEditX, WIFIEditY, WIFIEdit_WIDTH, WIFIEdit_HEIGHT, 10, TFT_LIGHTGREEN);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(WIFIEditX + (WIFIEdit_WIDTH / 2) - 70, WIFIEditY + (WIFIEdit_HEIGHT / 2) - 15);
  tft.println("Change WIFI");

  // Кнопка изменения времени
  int utcTimeEditX = 200;
  int utcTimeEditY = 225;
  tft.fillRoundRect(utcTimeEditX, utcTimeEditY, utcTimeEdit_WIDTH, utcTimeEdit_HEIGHT, 10, TFT_LIGHTGREEN);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(utcTimeEditX + (utcTimeEdit_WIDTH / 2) - 70, utcTimeEditY + (utcTimeEdit_HEIGHT / 2) - 15);
  tft.println("Change time");

  // Кнопка изменения настройки сервера
  int serverAlwaysOnEditX = serverAlwaysOnEdit_MARGIN;
  int serverAlwaysOnEditY = 275;
  tft.fillRoundRect(serverAlwaysOnEditX, serverAlwaysOnEditY, serverAlwaysOnEdit_WIDTH, serverAlwaysOnEdit_HEIGHT, 10, TFT_LIGHTGREEN);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(serverAlwaysOnEditX + (serverAlwaysOnEdit_WIDTH / 2) - 70, serverAlwaysOnEditY + (serverAlwaysOnEdit_HEIGHT / 2) - 15);
  tft.println("Server");
  if (serverAlwaysOn && isConnectToWIFI) {
    ServerPowerUp();
  }
  if (flashlightOn){
    tft.drawBitmap(80, 0, flashlightIcon, 32, 32, TFT_BLACK);
  }
  timeToSleep = 0;
  while (true) {
    if (serverAlwaysOn && isConnectToWIFI) {
      server.handleClient();
    }
    if (millis() - timing2 >= 1000){
      timing2 = millis();
      timeToSleep++;
    }

    if (timeToSleep > 20){
      esp_deep_sleep_start();
    }
    if (tft.getTouch(&ts_x, &ts_y)) {
      timeToSleep = 0;
      // Проверка нажатия кнопки
      if (ts_x >= BackX && ts_x < BackX + Back_WIDTH && ts_y >= BackY && ts_y < BackY + Back_HEIGHT) {
        Settings();
      }
      if (ts_x >= updateosX && ts_x < updateosX + updateos_WIDTH && ts_y >= updateosY && ts_y < updateosY + updateos_HEIGHT) {
        InstructionUpdateSystem();
      }
      if (ts_x >= FactoryResetX && ts_x < FactoryResetX + FactoryReset_WIDTH && ts_y >= FactoryResetY && ts_y < FactoryResetY + FactoryReset_HEIGHT) {
        FactoryResetInformation();
      }
      if (ts_x >= UsernameEditX && ts_x < UsernameEditX + UsernameEdit_WIDTH && ts_y >= UsernameEditY && ts_y < UsernameEditY + UsernameEdit_HEIGHT) {
        ChangeUsername();
      }
      if (ts_x >= PasswordEditX && ts_x < PasswordEditX + PasswordEdit_WIDTH && ts_y >= PasswordEditY && ts_y < PasswordEditY + PasswordEdit_HEIGHT) {
        ChangePassword();
      }
      if (ts_x >= RebootX && ts_x < RebootX + Reboot_WIDTH && ts_y >= RebootY && ts_y < RebootY + Reboot_HEIGHT) {
        tft.fillScreen(TFT_WHITE);
        tft.setCursor(0, 0, 2);
        tft.println("Rebooting...");
        delay(1500);
        ESP.restart();
      }
      if (ts_x >= DeviceNameEditX && ts_x < DeviceNameEditX + DeviceNameEdit_WIDTH && ts_y >= DeviceNameEditY && ts_y < DeviceNameEditY + DeviceNameEdit_HEIGHT) {
        ChangeDeviceName();
      }
      if (ts_x >= WIFIEditX && ts_x < WIFIEditX + WIFIEdit_WIDTH && ts_y >= WIFIEditY && ts_y < WIFIEditY + WIFIEdit_HEIGHT) {
        InitialConnectNet();
        int len = ssidNet.length();
        EEPROM.write(SsidNet_ADDR, len);
        for (int i = 0; i < len; i++) {
          EEPROM.put(SsidNet_ADDR + 1 + i, ssidNet[i]);
        }
        len = passwordNet.length();
        EEPROM.write(PasswordNet_ADDR, len);
        for (int i = 0; i < len; i++) {
          EEPROM.put(PasswordNet_ADDR + 1 + i, passwordNet[i]);
        }
        EEPROM.commit();
        SettingsFuncs();
      }
      if (ts_x >= utcTimeEditX && ts_x < utcTimeEditX + utcTimeEdit_WIDTH && ts_y >= utcTimeEditY && ts_y < utcTimeEditY + utcTimeEdit_HEIGHT) {
        timeEdit();
      }
      if (ts_x >= serverAlwaysOnEditX && ts_x < serverAlwaysOnEditX + serverAlwaysOnEdit_WIDTH && ts_y >= serverAlwaysOnEditY && ts_y < serverAlwaysOnEditY + serverAlwaysOnEdit_HEIGHT) {
        serverSettings();
      }
    }
    if (millis() - timing >= 1000) {
      timing = millis();
      getLastTimeTopPanel();
    }
  }
}

// Настройки
void Settings() {
  String versionSettings = version;
  tft.fillScreen(TFT_WHITE);
  drawTopPanel(true);
  tft.setTextColor(TFT_BLACK);
  tft.setCursor(10, 35, 2);
  tft.println("Settings");
  tft.println(" Device: XenoClock 1");
  tft.print(" Device name: ");
  tft.println(deviceName);
  tft.print(" OS: XenoCore OS ");
  tft.println(versionSettings);
  tft.print(" WIFI: ");
  if (isConnectToWIFI) {
    tft.print(ssidNet);
  }
  tft.print(", ");
  if (isConnectToWIFI) {
    tft.print(WiFi.localIP());
  }
  tft.println("");
  tft.print(" Username: ");
  tft.println(username);
  tft.print(" Password: ");
  tft.println(password);
  // Кнопка назад
  drawButBack();

  #define funcsettings_WIDTH 150
  #define funcsettings_HEIGHT 40
  #define funcsettings_MARGIN 10
  int funcsettingsX = funcsettings_MARGIN;
  int funcsettingsY = TFT_HEIGHT - funcsettings_HEIGHT - funcsettings_MARGIN;
  tft.fillRoundRect(funcsettingsX, funcsettingsY, funcsettings_WIDTH, funcsettings_HEIGHT, 10, TFT_LIGHTGREEN);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(funcsettingsX + (funcsettings_WIDTH / 2) - 60, funcsettingsY + (funcsettings_HEIGHT / 2) - 10);
  tft.println("Functions");
  if (serverAlwaysOn && isConnectToWIFI) {
    ServerPowerUp();
  }
  if (flashlightOn){
    tft.drawBitmap(80, 0, flashlightIcon, 32, 32, TFT_BLACK);
  }
  timeToSleep = 0;
  while (true) {
    if (serverAlwaysOn && isConnectToWIFI) {
      server.handleClient();
    }
    if (millis() - timing2 >= 1000){
      timing2 = millis();
      timeToSleep++;
    }

    if (timeToSleep > 20){
      esp_deep_sleep_start();
    }
    if (tft.getTouch(&ts_x, &ts_y)) {
      timeToSleep = 0;
      // Проверка нажатия кнопки
      if (ts_x >= BackX && ts_x < BackX + Back_WIDTH && ts_y >= BackY && ts_y < BackY + Back_HEIGHT) {
        Desktop();
      }
      if (ts_x >= funcsettingsX && ts_x < funcsettingsX + funcsettings_WIDTH && ts_y >= funcsettingsY && ts_y < funcsettingsY + funcsettings_HEIGHT) {
        SettingsFuncs();
      }
    }
    if (millis() - timing >= 1000) {
      timing = millis();
      getLastTimeTopPanel();
    }
  }
}

// Инструкция по обновлению системы
void InstructionUpdateSystem() {
  tft.fillScreen(TFT_WHITE);
  drawTopPanel(true);
  tft.setTextColor(TFT_BLACK);
  tft.setCursor(10, 35, 2);
  tft.println("Instruction Update System");
  tft.print("To update the system, you need to download ");
  tft.setTextSize(1);
  tft.print("(github.com/VLPLAY01/XenoCore-OS) ");
  tft.setTextSize(2);
  tft.print(".bin file. Then follow the link in watch and enter your username and password, then upload the .bin file and click the 'Update'. Your watch will restart.");
  // Кнопка назад
  drawButBack();
  // Кнопка продолжить
  int continueupdateX = continueupdate_MARGIN;
  int continueupdateY = TFT_HEIGHT - continueupdate_HEIGHT - continueupdate_MARGIN;
  tft.fillRoundRect(continueupdateX, continueupdateY, continueupdate_WIDTH, continueupdate_HEIGHT, 10, TFT_LIGHTGREEN);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(continueupdateX + (continueupdate_WIDTH / 2) - 50, continueupdateY + (continueupdate_HEIGHT / 2) - 10);
  tft.println("Continue");
  if (serverAlwaysOn && isConnectToWIFI) {
    ServerPowerUp();
  }
  if (flashlightOn){
    tft.drawBitmap(80, 0, flashlightIcon, 32, 32, TFT_BLACK);
  }
  timeToSleep = 0;
  while (true) {
    if (serverAlwaysOn && isConnectToWIFI) {
      server.handleClient();
    }
    if (millis() - timing2 >= 1000){
      timing2 = millis();
      timeToSleep++;
    }

    if (timeToSleep > 20){
      esp_deep_sleep_start();
    }
    if (tft.getTouch(&ts_x, &ts_y)) {
      timeToSleep = 0;
      // Проверка нажатия кнопки
      if (ts_x >= BackX && ts_x < BackX + Back_WIDTH && ts_y >= BackY && ts_y < BackY + Back_HEIGHT) {
        SettingsFuncs();
      }
      if (ts_x >= continueupdateX && ts_x < continueupdateX + continueupdate_WIDTH && ts_y >= continueupdateY && ts_y < continueupdateY + continueupdate_HEIGHT) {
        UpdatingSystem();
      }
    }
  }
}

// Основной цикл при запуске
void setup() {
  pinMode(22, OUTPUT);
  pinMode(flashLight, OUTPUT);
  digitalWrite(22, HIGH);
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_13,1);
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();
  tft.init();
  Calibration();
  tft.setCursor(0, 0, 2);
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLACK);
  Serial.begin(115200);
  EEPROM.begin(EEPROM_SIZE);
  tft.setRotation(3);
  tft.setTextSize(2);
  tft.setCursor(0, 0, 2);
  if (wakeup_reason != ESP_SLEEP_WAKEUP_EXT0){
    tft.println("Booting...");
    delay(500);
  }
  tft.setTextColor(TFT_BLACK);
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
  utcTime = EEPROM.read(utcTime_ADDR + 1);
  utcTimePlus = EEPROM.read(utcTimePlus_ADDR + 1);
  serverAlwaysOn = EEPROM.read(serverAlwaysOn_ADDR + 1);
  timeClient.setTimeOffset(utcTimePlus ? utcTime * 3600 : -utcTime * 3600);

  WiFi.mode(WIFI_STA);
  WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
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
    if (wakeup_reason != ESP_SLEEP_WAKEUP_EXT0){
      tft.setTextColor(TFT_BLACK);
      tft.setTextFont(2);
      tft.println("Booting Successfull");
      tft.println("The system has been rebooted");
      delay(2000);
    }
  }
}

void loop() {
  system();
}