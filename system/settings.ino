// Все функции настроек
void SettingsFuncs() {
  tft.fillScreen(TFT_WHITE);
  drawTopPanel(true);
  tft.setTextColor(TFT_BLACK);
  tft.setCursor(10, 35, 2);
  tft.println("Settings Functions");

  // Кнопка назад
  drawButBack();

  Button buttons[] = {
    {20, 75, 160, 40, "Update OS"},
    {200, 75, 160, 40, "Fact.reset"},
    {20, 125, 160, 40, "Change UsN"},
    {200, 125, 160, 40, "Change Pw"},
    {20, 175, 160, 40, "Reboot"},
    {200, 175, 160, 40, "Device Name"},
    {20, 225, 160, 40, "WIFI"},
    {200, 225, 160, 40, "Change time"},
    {20, 275, 160, 40, "Server"},
    {200, 275, 160, 40, "Check sys"},
    {380, 75, 75, 40, ">>"}
  };

  // Отрисовка кнопок
  for (Button& button : buttons) {
    drawButton(button);
  }

  server_flashlight();
  while (true) {
    readBT();
    check_sleep();
    check_but_reboot();
    if (tft.getTouch(&ts_x, &ts_y)) {
      timeToSleep = 0;
      // Проверка нажатия кнопок
      for (Button& button : buttons) {
        if (ts_x >= button.x && ts_x < button.x + button.width && ts_y >= button.y && ts_y < button.y + button.height) {
          if (strcmp(button.label, "Update OS") == 0) {
            UpdatingSystem();
          }
          if (strcmp(button.label, "Fact.reset") == 0) {
            FactoryResetInformation();
          }
          if (strcmp(button.label, "Change UsN") == 0) {
            ChangeUsername();
          }
          if (strcmp(button.label, "Change Pw") == 0) {
            ChangePassword();
          }
          if (strcmp(button.label, "Reboot") == 0) {
            tft.fillScreen(TFT_BLACK);
            tft.setTextColor(TFT_WHITE);
            tft.setCursor(160, 225, 2);
            tft.println("Rebooting");
            drawSdJpeg("/xc_logo_black.jpeg", 160, 75);
            delay(2000);
            ESP.restart();
          }
          if (strcmp(button.label, "Device Name") == 0) {
            ChangeDeviceName();
          }
          if (strcmp(button.label, "WIFI") == 0) {
            wifiSetting();
          }
          if (strcmp(button.label, "Change time") == 0) {
            timeEdit();
          }
          if (strcmp(button.label, "Server") == 0) {
            serverSettings();
          }
          if (strcmp(button.label, "Check sys") == 0) {
            checkSystem();
          }
          if (strcmp(button.label, ">>") == 0) {
            settings_func_page2();
          }
        }
      }
      if (ts_x >= BackX && ts_x < BackX + Back_WIDTH && ts_y >= BackY && ts_y < BackY + Back_HEIGHT) {
        Settings();
      }
    }

    if (millis() - timing >= 1000) {
      timing = millis();
      getLastTimeTopPanel();
    }
  }
}

// Вторая страница функций
void settings_func_page2() {
  // Определение размеров и координат каждой кнопки
  Button buttons[] = {
    {20, 75, 160, 40, "Sleep Mode"},
    {200, 75, 160, 40, "Battery"},
    {20, 125, 160, 40, "Bluetooth"},
    {200, 125, 160, 40, "Recalibr"},
    {380, 75, 75, 40, "<<"}
  };

  tft.fillScreen(TFT_WHITE);
  drawTopPanel(true);
  tft.setTextColor(TFT_BLACK);
  tft.setCursor(10, 35, 2);
  tft.println("Settings Functions");

  // Отрисовка кнопок
  for (Button& button : buttons) {
    drawButton(button);
  }

  // Кнопка назад
  drawButBack();

  if (serverAlwaysOn && isConnectToWIFI) {
    ServerPowerUp();
  }
  if (flashlightOn) {
    drawSdJpeg("/flashlight_icon.jpeg", 80, 0);
  }
  timeToSleep = 0;
  while (true) {
    readBT();
    check_sleep();
    check_but_reboot();
    if (tft.getTouch(&ts_x, &ts_y)) {
      timeToSleep = 0;
      // Проверка нажатия кнопки
      for (Button& button : buttons) {
        if (ts_x >= button.x && ts_x < button.x + button.width && ts_y >= button.y && ts_y < button.y + button.height) {
          if (strcmp(button.label, "Sleep Mode") == 0) {
            sleep_settings();
          } else if (strcmp(button.label, "Battery") == 0) {
            battery_settings();
          } else if (strcmp(button.label, "Bluetooth") == 0) {
            bluetooth_settings();
          } else if (strcmp(button.label, "Recalibr") == 0) {
            SPIFFS.remove(CALIBRATION_FILE);
            Calibration();
            settings_func_page2();
          } else if (strcmp(button.label, "<<") == 0) {
            SettingsFuncs();
          }
        }
      }
      if (ts_x >= BackX && ts_x < BackX + Back_WIDTH && ts_y >= BackY && ts_y < BackY + Back_HEIGHT) {
        Settings();
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
  drawSdJpeg("/xc_logo_white.jpeg", 325, 40);
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

  Button buttons[] = {
    {10, TFT_HEIGHT - 50, 150, 40, "Functions"}
  };
  for (Button& button : buttons) {
    drawButton(button);
  }
  server_flashlight();
  while (true) {
    readBT();
    check_sleep();
    if (tft.getTouch(&ts_x, &ts_y)) {
      timeToSleep = 0;
      for (Button& button : buttons) {
        if (ts_x >= button.x && ts_x < button.x + button.width && ts_y >= button.y && ts_y < button.y + button.height) {
          if (strcmp(button.label, "Functions") == 0) {
            SettingsFuncs();
          }
        }
      }
      // Проверка нажатия кнопки
      if (ts_x >= BackX && ts_x < BackX + Back_WIDTH && ts_y >= BackY && ts_y < BackY + Back_HEIGHT) {
        Desktop();
      }
    }
    if (millis() - timing >= 1000) {
      timing = millis();
      getLastTimeTopPanel();
    }
  }
}
