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
  tft.println(String(device_model) + ": Initial setup");
}

// Начальная настройка

// Кнопки



void butUnPwfunc() {
  drawInitialSetupLabel();
  tft.setCursor(0, 0, 2);

  Button buttons[] = {
    {20, 150, 150, 40, "Username"},
    {200, 150, 150, 40, "Password"},
  };

  for (Button& button : buttons) {
    drawButton(button);
  }

  bool UnPass = false;
  bool PwPass = false;
  bool allPass = false;
  while (allPass != true) {
    check_but_reboot(true);
    if (tft.getTouch(&ts_x, &ts_y)) {
      // Проверяем нажатие на кнопки
      for (Button& button : buttons) {
        if (ts_x >= button.x && ts_x < button.x + button.width && ts_y >= button.y && ts_y < button.y + button.height) {
          if (strcmp(button.label, "Username") == 0) {
            // Нажата кнопка "Username"
            username = keyboard();
            if (username != "") {
              tft.fillScreen(TFT_WHITE);
              if (PwPass == false) {
                tft.setCursor(0, 0, 2);
                tft.setTextSize(2);
                tft.setTextColor(TFT_BLACK);
                tft.println("Now you need to enter a password");
                Button buttons[] = {
                  {200, 150, 150, 40, "Password"},
                };
                for (Button& button : buttons) {
                  drawButton(button);
                }
              }
              UnPass = true;
            } else {
              butUnPwfunc();
            }
          } else if (strcmp(button.label, "Password") == 0) {
            // Нажата кнопка "Password"
            password = keyboard();
            if (password != "") {
              tft.fillScreen(TFT_WHITE);
              if (UnPass == false) {
                tft.setCursor(0, 0, 2);
                tft.setTextSize(2);
                tft.setTextColor(TFT_BLACK);
                tft.println("Now you need to enter a username");
                Button buttons[] = {
                  {20, 150, 150, 40, "Username"},
                };
                for (Button& button : buttons) {
                  drawButton(button);
                }
              }
              PwPass = true;
            } else {
              butUnPwfunc();
            }
          }
        }
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
  drawSdJpeg("/initial_setup_wallpaper.jpeg", 0, 0);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(165, 100, 2);
  tft.print(device_model);
  tft.setCursor(40, 140, 2);
  tft.print("Welcome to XenoCore OS ");
  tft.println(version);
  tft.setCursor(20, 0);
  bool start_button = false;
  // Кнопка начала настройки
  Button buttons[] = {
    {175, 175, 150, 40, "Start"},
  };
  for (Button& button : buttons) {
    drawButton(button);
  }
  while (start_button != true) {
    check_but_reboot(true);
    if (tft.getTouch(&ts_x, &ts_y)) {
      for (Button& button : buttons) {
        if (ts_x >= button.x && ts_x < button.x + button.width && ts_y >= button.y && ts_y < button.y + button.height) {
          if (strcmp(button.label, "Start") == 0) {
            start_button = true;
          }
        }
      }
    }
  }
}

// Подключение к интернету
void InitialConnectNet() {
  tft.fillScreen(TFT_WHITE);
  drawInitialSetupLabel();
  tft.setCursor(0, 35, 2);
  tft.setTextSize(2);
  tft.setTextColor(TFT_BLACK);
  tft.println("You need to be connected to the internet. You need to enter a name and password");
  tft.setCursor(0, 240, 2);
  tft.print("Highly recommended not to connect to the Internet in public places");
  tft.setCursor(0, 0, 2);
  Button buttons[] = {
    {150, 125, 150, 40, "Name"},
    {150, 175, 150, 40, "Password"},
  };

  for (Button& button : buttons) {
    drawButton(button);
  }
  bool UnPassNet = false;
  bool PwPassNet = false;
  bool allPassNet = false;
  while (allPassNet != true) {
    if (tft.getTouch(&ts_x, &ts_y)) {
      // Проверяем нажатие на кнопки
      for (Button& button : buttons) {
        if (ts_x >= button.x && ts_x < button.x + button.width && ts_y >= button.y && ts_y < button.y + button.height) {
          if (strcmp(button.label, "Name") == 0) {
            // Нажата кнопка "Name"
            ssidNet = keyboard();
            if (ssidNet != "") {
              tft.fillScreen(TFT_WHITE);
              if (PwPassNet == false) {
                tft.setCursor(0, 0, 2);
                tft.setTextSize(2);
                tft.setTextColor(TFT_BLACK);
                tft.println("Now you need to enter a password");
                Button buttons[] = {
                  {150, 200, 150, 40, "Password"},
                };

                for (Button& button : buttons) {
                  drawButton(button);
                }
              }
              UnPassNet = true;
            } else {
              InitialConnectNet();
            }
          } else if (strcmp(button.label, "Password") == 0) {
            // Нажата кнопка "Password"
            passwordNet = keyboard();
            if (passwordNet != "") {
              tft.fillScreen(TFT_WHITE);
              if (UnPassNet == false) {
                tft.setCursor(0, 0, 2);
                tft.setTextSize(2);
                tft.setTextColor(TFT_BLACK);
                tft.println("Now you need to enter a name");
                Button buttons[] = {
                  {150, 150, 150, 40, "Name"},
                };

                for (Button& button : buttons) {
                  drawButton(button);
                }
              }
              PwPassNet = true;
            } else {
              InitialConnectNet();
            }
          }
        }
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
  tft.fillScreen(TFT_BLACK);
  drawSdJpeg("/xc_logo_black.jpeg", 160, 75);
  tft.setCursor(100, 225, 2);
  tft.println("Started initial setup");
  delay(2500);
  tft.setTextSize(2);
  Greetings();
  tft.setCursor(0, 0, 2);
  tft.fillScreen(TFT_BLACK);
  drawSdJpeg("/xc_logo_black.jpeg", 160, 75);
  tft.setCursor(75, 225, 2);
  tft.println("Preparing first turn on...");
  delay(1500);
  tft.fillScreen(TFT_WHITE);
  tft.setCursor(0, 35, 2);
  tft.setTextSize(2);
  tft.setTextColor(TFT_BLACK);
  tft.print(" You need to enter username and password");
  butUnPwfunc();
  tft.fillScreen(TFT_WHITE);
  drawInitialSetupLabel();
  tft.setCursor(0, 35, 2);
  tft.setTextSize(2);
  tft.setTextColor(TFT_BLACK);
  tft.print("Now you need to connect to the network");
  // Кнопки
  Button buttons[] = {
    {370, 260, 75, 40, "Skip"},
    {20, 260, 125, 40, "Continue"},
  };

  for (Button& button : buttons) {
    drawButton(button);
  }
  bool isConnectToWIFISkip = false;
  bool testpassed = false;
  while (testpassed != true) {
    if (tft.getTouch(&ts_x, &ts_y)) {
      // Проверка нажатия кнопки
      for (Button& button : buttons) {
        if (ts_x >= button.x && ts_x < button.x + button.width && ts_y >= button.y && ts_y < button.y + button.height) {
          if (strcmp(button.label, "Skip") == 0) {
            isConnectToWIFISkip = true;
            testpassed = true;
          } else if (strcmp(button.label, "Continue") == 0) {
            isConnectToWIFISkip = false;
            testpassed = true;
          }
        }
      }
    }
  }
  if (isConnectToWIFISkip == false) {
    InitialConnectNet();
  }
  timeEdit(true);
}
