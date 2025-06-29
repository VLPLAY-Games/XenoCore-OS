// Сброс настроек к заводскому состоянию
void FactoryReset() {
  tft.fillScreen(TFT_WHITE);
  drawTopPanel(true);
  tft.setTextColor(TFT_BLACK);
  tft.setCursor(0, 35, 2);
  tft.println("Factory Reset Started");
  tft.println("Progress: 0%");
  delay(1500);
  for (int i = 0; i <= EEPROM_SIZE; i++) {
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

  Button buttons[] = {
    {20, 240, 130, 40, "Continue"},
  };

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
      for (Button& button : buttons) {
        if (ts_x >= button.x && ts_x < button.x + button.width && ts_y >= button.y && ts_y < button.y + button.height) {
          if (strcmp(button.label, "Continue")  == 0) {
            FactoryReset();
          }
        }
      }
      // Проверка нажатия кнопки
      if (ts_x >= BackX && ts_x < BackX + Back_WIDTH && ts_y >= BackY && ts_y < BackY + Back_HEIGHT) {
        SettingsFuncs();
      }
    }
  }
}
