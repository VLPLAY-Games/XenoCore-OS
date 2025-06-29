// Paint
void paint() {
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLACK);
  tft.setCursor(10, 35, 2);
  tft.setTextSize(2);
  tft.println("Paint");
  drawTopPanel(true);
  // Кнопка назад
  drawButBack();
  Button buttons[] = {
    {20, 270, 120, 40, "Reset"},
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
      tft.fillCircle(ts_x, ts_y, 15, TFT_BLACK);
      for (Button& button : buttons) {
        if (ts_x >= button.x && ts_x < button.x + button.width && ts_y >= button.y && ts_y < button.y + button.height) {
          if (strcmp(button.label, "Reset") == 0) {
            paint();
          }
        }
      }
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
