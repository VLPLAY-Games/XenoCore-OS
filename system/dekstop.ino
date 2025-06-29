// Рабочий стол
void Desktop() {
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLACK);
  tft.setCursor(10, 35, 2);
  tft.setTextSize(2);
  tft.println("Desktop");
  drawTopPanel(true);

  drawSdJpeg("/settings_logo.jpeg", 60, 60);
  drawSdJpeg("/flashlight_logo.jpeg", 215, 60);
  drawSdJpeg("/explorer_logo.jpeg", 365, 60);
  drawSdJpeg("/stopwatch_logo.jpeg", 60, 140);
  drawSdJpeg("/timer_logo.jpeg", 215, 140);
  drawSdJpeg("/calc_logo.jpeg", 365, 140);
  drawSdJpeg("/ai_logo.jpeg", 60, 230);
  drawSdJpeg("/paint_logo.jpeg", 215, 230);

  Button buttons[] = {
    {20, 100, 130, 40, "Settings"},
    {175, 100, 130, 40, "Flashlight"},
    {325, 100, 130, 40, "Explorer"},
    {20, 180, 130, 40, "Stopwatch"},
    {175, 180, 130, 40, "Timer"},
    {325, 180, 130, 40, "Calc"},
    {20, 270, 130, 40, "AI"},
    {175, 270, 130, 40, "Paint"},
    {380, 225, 75, 40, ">>"}
  };

  // Отрисовка кнопок
  for (Button& button : buttons) {
    drawButton(button);
  }

  // Кнопка назад
  drawButBack();

  server_flashlight();

  while (true) {
    readBT();
    check_sleep();
    if (tft.getTouch(&ts_x, &ts_y)) {
      timeToSleep = 0;
      // Проверка нажатия кнопок
      for (Button& button : buttons) {
        if (ts_x >= button.x && ts_x < button.x + button.width && ts_y >= button.y && ts_y < button.y + button.height) {
          if (strcmp(button.label, "Settings") == 0) {
            Settings();
          } else if (strcmp(button.label, "Flashlight") == 0) {
            if (!flashlightOn) {
              flashlightOn = true;
              digitalWrite(flashLight, HIGH);
              drawSdJpeg("/flashlight_icon.jpeg", 360, 0);
              delay(100);
            } else {
              flashlightOn = false;
              digitalWrite(flashLight, LOW);
              tft.fillRect(355, 0, 32, 32, TFT_WHITE);
              delay(100);
            }
          } else if (strcmp(button.label, "Explorer") == 0) {
            explorer();
          } else if (strcmp(button.label, "Stopwatch") == 0) {
            stopwatch();
          } else if (strcmp(button.label, "Timer") == 0) {
            timer();
          } else if (strcmp(button.label, "Calc") == 0) {
            calc();
          } else if (strcmp(button.label, "AI") == 0) {
            ai();
          } else if (strcmp(button.label, "Paint") == 0) {
            paint();
          } else if (strcmp(button.label, ">>") == 0) {
            desktop_page2();
          }
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


void desktop_page2() {
  // Определение размеров и координат каждой кнопки
  Button buttons[] = {
    {20, 100, 130, 40, "Pulse"},
    {175, 100, 130, 40, "Events"},
    {325, 100, 130, 40, "Calendar"},
    {380, 225, 75, 40, "<<"}
  };

  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLACK);
  tft.setCursor(10, 35, 2);
  tft.setTextSize(2);
  tft.println("Desktop");
  drawTopPanel(true);

  drawSdJpeg("/pulse_logo.jpeg", 60, 60);
  drawSdJpeg("/events_logo.jpeg", 215, 60);

  // Отрисовка кнопок
  for (Button& button : buttons) {
    drawButton(button);
  }

  // Кнопка назад
  drawButBack();

  server_flashlight();
  while (true) {
    readBT();
    check_sleep();
    check_but_reboot();
    if (tft.getTouch(&ts_x, &ts_y)) {
      timeToSleep = 0;
      // Проверка нажатия кнопки
      for (Button& button : buttons) {
        if (ts_x >= button.x && ts_x < button.x + button.width && ts_y >= button.y && ts_y < button.y + button.height) {
          if (strcmp(button.label, "<<") == 0) {
            Desktop();
          } else if (strcmp(button.label, "Pulse") == 0) {
            pulse();
          } else if (strcmp(button.label, "Events") == 0) {
            eventViewer();
          } else if (strcmp(button.label, "Calendar") == 0) {
            calendar();
          }
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
