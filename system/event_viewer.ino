void eventViewer() {
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLACK);
  tft.setCursor(10, 35, 2);
  tft.setTextSize(2);
  tft.println("Event viewer");
  File file = SPIFFS.open("/log.txt", FILE_READ);
  // Считываем и выводим содержимое файла построчно
  tft.setTextSize(1);
  tft.setCursor(0, 70, 1);
  byte count_files = 0;
  byte pos = 70;
  while (file.available()) {
    count_files ++;
    String line = file.readStringUntil('\n');  // Считываем строку до символа новой строки
    if (count_files >= 30){
      tft.setCursor(180, pos, 1);
      pos += 10;
    }
    tft.println(line);  // Выводим строку на TFT дисплей
  }
  tft.setTextSize(2);
  // Закрываем файл
  file.close();
  drawTopPanel(true);
  // Кнопка назад
  drawButBack();
  server_flashlight();

  while (true) {
    readBT();
    check_sleep();
    check_but_reboot();
    if (tft.getTouch(&ts_x, &ts_y)) {
      timeToSleep = 0;
      if (ts_x >= BackX && ts_x < BackX + Back_WIDTH && ts_y >= BackY && ts_y < BackY + Back_HEIGHT) {
        desktop_page2();
      }
    }
    if (millis() - timing >= 1000) {
      timing = millis();
      getLastTimeTopPanel();
    }
  }
}
