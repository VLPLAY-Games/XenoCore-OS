void pulse() {
  digitalWrite(17, HIGH);
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLACK);
  tft.setCursor(10, 35, 2);
  tft.setTextSize(2);
  tft.println("Pulse");
  drawTopPanel(true);
  drawButBack();
  tft.setCursor(10, 70, 2);
  drawSdJpeg("/pulse_logo.jpeg", 10, 70);
  server_flashlight();

  while (true) {
    readBT();
    check_but_reboot();
    if (pulseSensor.sawStartOfBeat()) {
      int myBPM = pulseSensor.getBeatsPerMinute();
      tft.fillRect(90, 80, 100, tft.fontHeight(), TFT_WHITE);
      tft.setCursor(90, 80, 2);
      tft.print(myBPM);
      tft.println(" BPM");
      delay(20);
    }

    if (tft.getTouch(&ts_x, &ts_y)) {
      timeToSleep = 0;
      // Проверка нажатия кнопки
      if (ts_x >= BackX && ts_x < BackX + Back_WIDTH && ts_y >= BackY && ts_y < BackY + Back_HEIGHT) {
        digitalWrite(17, LOW);
        desktop_page2();
      }
    }
    if (millis() - timing >= 1000) {
      timing = millis();
      getLastTimeTopPanel();
    }
  }
}
