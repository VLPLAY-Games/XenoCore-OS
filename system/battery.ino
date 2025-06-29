void battery_settings() {
  tft.fillScreen(TFT_WHITE);
  drawTopPanel(true);
  tft.setTextColor(TFT_BLACK);
  tft.setCursor(10, 35, 2);
  tft.println("Battery settings");
  // Кнопка назад
  drawButBack();
  tft.setCursor(10, 70, 2);
  tft.print("Battery mode: ");
  tft.println(energy_saving_mode ? "Energy saving" : "Normal");

  // Кнопка off / on
  Button buttons[] = {
    {20, 150, 200, 40, "Change mode"}
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
      for (Button& button : buttons) {
        if (ts_x >= button.x && ts_x < button.x + button.width && ts_y >= button.y && ts_y < button.y + button.height) {
          if (strcmp(button.label, "Change mode") == 0) {
            if (energy_saving_mode) {
              setCpuFrequencyMhz(240);
            } else {
              setCpuFrequencyMhz(80);
            }
            energy_saving_mode = !energy_saving_mode;
            delay(200);
            battery_settings();
          }
        }
      }
      if (ts_x >= BackX && ts_x < BackX + Back_WIDTH && ts_y >= BackY && ts_y < BackY + Back_HEIGHT) {
        settings_func_page2();
      }
    }
    if (millis() - timing >= 1000) {
      timing = millis();
      getLastTimeTopPanel();
    }
  }
}


byte getBatLevel(){
  if (ina219_status) {
    current_mA = ina219.getCurrent_mA();
    if (current_mA >= capacity_mah) {
      capacity = 99;
    } else if (current_mA <= 10) {
      capacity = 0;
    } else {
      capacity = floor(current_mA / 7);
    }
  }
  return capacity;
}