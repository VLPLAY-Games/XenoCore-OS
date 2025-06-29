// Секундомер
void stopwatch() {
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLACK);
  tft.setCursor(10, 35, 2);
  tft.setTextSize(2);
  tft.println("Stopwatch");
  drawTopPanel(true);
  // Кнопка назад
  drawButBack();

  Button buttons[] = {
    {20, 150, 120, 40, "Start"},
    {150, 150, 120, 40, "Stop"},
    {300, 150, 120, 40, "Reset"}
  };

  for (Button& button : buttons) {
    drawButton(button);
  }

  tft.setCursor(10, 70, 2);
  tft.setTextColor(TFT_BLACK);
  tft.print(String(stopwatch_sec) + " sec " + String(stopwatch_min) + " min " + String(stopwatch_hour) + " hour ");
  server_flashlight();
  while (true) {
    readBT();
    check_but_reboot();
    if (millis() - timing2 >= 1000) {
      timing2 = millis();
      if (stopwatch_active) {
        tft.fillRect(10, 70, TFT_WIDTH, tft.fontHeight(), TFT_WHITE);
        stopwatch_sec++;
        tft.setCursor(10, 70, 2);
        if (stopwatch_sec == 60) {
          stopwatch_min++;
          stopwatch_sec = 0;
        }
        if (stopwatch_min == 60) {
          stopwatch_min = 0;
          stopwatch_hour++;
        }
        tft.print(String(stopwatch_sec) + " sec " + String(stopwatch_min) + " min " + String(stopwatch_hour) + " hour");
      }
    }
    if (tft.getTouch(&ts_x, &ts_y)) {
      if (ts_x >= BackX && ts_x < BackX + Back_WIDTH && ts_y >= BackY && ts_y < BackY + Back_HEIGHT) {
        Desktop();
      }
      for (Button& button : buttons) {
        if (ts_x >= button.x && ts_x < button.x + button.width && ts_y >= button.y && ts_y < button.y + button.height) {
          if (strcmp(button.label, "Start") == 0) {
            stopwatch_active = true;
          }
          if (strcmp(button.label, "Stop") == 0) {
            stopwatch_active = false;
          }
          if (strcmp(button.label, "Reset") == 0) {
            stopwatch_sec = 0;
            stopwatch_min = 0;
            stopwatch_hour = 0;
            tft.fillRect(10, 70, TFT_WIDTH, tft.fontHeight(), TFT_WHITE);
            tft.setCursor(10, 70, 2);
            tft.print(String(stopwatch_sec) + " sec " + String(stopwatch_min) + " min " + String(stopwatch_hour) + " hour");
          }
          delay(50);
        }
      }
    }
    if (millis() - timing >= 1000) {
      timing = millis();
      getLastTimeTopPanel();
    }
  }
}
