void sleep_settings() {
  tft.fillScreen(TFT_WHITE);
  drawTopPanel(true);
  tft.setTextColor(TFT_BLACK);
  tft.setCursor(10, 35, 2);
  tft.println("Sleep Settings");
  // Кнопка назад
  drawButBack();
  server_flashlight();

  tft.setCursor(10, 70, 2);
  tft.print("Sleep mode status: ");
  tft.print(sleep_off ? "Inactive" : "Active");

  Button buttons[] = {
    {20, 150, 200, 40, "Off / On"},
  };

  for (Button& button : buttons) {
    drawButton(button);
  }
  while (true) {
    readBT();
    check_sleep();
    check_but_reboot();
    if (tft.getTouch(&ts_x, &ts_y)) {
      timeToSleep = 0;
      // Проверка нажатия кнопки
      if (ts_x >= BackX && ts_x < BackX + Back_WIDTH && ts_y >= BackY && ts_y < BackY + Back_HEIGHT) {
        settings_func_page2();
      }
      for (Button& button : buttons) {
        if (ts_x >= button.x && ts_x < button.x + button.width && ts_y >= button.y && ts_y < button.y + button.height) {
          if (strcmp(button.label, "Off / On") == 0) {
            if (!sleep_off) {
              EEPROM.write(sleep_mode, 1);
              sleep_off = true;
            } else if (sleep_off) {
              EEPROM.write(sleep_mode, 0255);
              sleep_off = false;
            }
            EEPROM.commit();
            delay(200);
            sleep_settings();
          }
        }
      }
    }
    if (millis() - timing >= 1000) {
      timing = millis();
      getLastTimeTopPanel();
    }
  }
}


void check_sleep() {
  if (!sleep_off) {
    if (millis() - timing2 >= 1000) {
      timing2 = millis();
      timeToSleep++;
      if (timeToSleep > 20) {
        esp_deep_sleep_start();
      }
    }
  }
}
