void explorer() {
  tft.fillScreen(TFT_WHITE);
  drawTopPanel(true);
  // Отображение заголовка
  tft.setTextColor(TFT_BLACK);
  tft.setCursor(10, 35, 2);
  tft.println("Explorer");
  // Отображение кнопки назад
  drawButBack();
  server_flashlight();

  tft.setTextSize(1);
  // Отображение файлов и папок на SPIFFS
  tft.setTextColor(TFT_BLUE);
  tft.setCursor(10, 70, 2);
  tft.println("SPIFFS:");

  tft.setTextColor(TFT_BLACK);
  File rootSPIFFS = SPIFFS.open("/");
  File fileSPIFFS = rootSPIFFS.openNextFile();
  while (fileSPIFFS) {
    tft.print("FILE: ");
    tft.print(fileSPIFFS.name());
    tft.print(", size: ");
    tft.print(fileSPIFFS.size());
    tft.println("B");

    fileSPIFFS = rootSPIFFS.openNextFile();
  }
  rootSPIFFS.close();

  // Отображение разделителя
  tft.drawLine(0, 135, tft.width(), 135, TFT_BLACK);

  // Отображение файлов и папок на SD-карте
  tft.setTextColor(TFT_BLUE);
  tft.setCursor(10, 140, 2);
  tft.println("SD Card:");
  tft.setCursor(0, 160, 1);
  tft.setTextColor(TFT_BLACK);
  File rootSD = SD.open("/");
  File fileSD = rootSD.openNextFile();
  byte count_files = 0;
  byte pos = 145;
  while (fileSD) {
    count_files += 1;
    // Получение имени файла и его длины
    String fileName = fileSD.name();
    if (fileName.length() > 15) {
      fileName = fileName.substring(0, 12) + "...";
    }
    if (count_files >= 21) {
      tft.setCursor(240, pos, 1);
      pos += 10;
    }
    // Отображение имени файла и его размера
    tft.print("FILE: ");
    tft.print(fileName);
    tft.print(", size: ");
    tft.print(fileSD.size());
    tft.println("B");

    fileSD = rootSD.openNextFile();
  }
  rootSD.close();

  // Отображение информации о памяти
  tft.setTextColor(TFT_BLUE);
  tft.setCursor(240, 70, 2);
  tft.print("Memory:");

  tft.setTextColor(TFT_BLACK);
  uint32_t flashSize = ESP.getFlashChipSize();
  tft.setCursor(240, 90, 2);
  tft.print("Flash Size: ");
  tft.print(flashSize);
  tft.println(" Bytes");

  // Получаем количество свободной памяти (heap)
  uint32_t freeHeap = ESP.getFreeHeap();
  tft.setCursor(240, 110, 2);
  tft.print("Free Heap: ");
  tft.print(freeHeap);
  tft.println(" Bytes");

  // Отображение информации о карте SD
  tft.setCursor(240, 275, 2);
  tft.print("SD total: ");
  tft.print(cardSize);
  tft.println("KB");
  tft.setCursor(240, 290, 2);
  tft.print("SD used: ");
  tft.print(SD.usedBytes() / 1024);
  tft.println("KB");
  tft.setCursor(240, 305, 2);
  tft.print("SD total files: ");
  tft.println(count_files);

  while (true) {
    readBT();
    check_sleep();
    check_but_reboot();
    if (tft.getTouch(&ts_x, &ts_y)) {
      timeToSleep = 0;
      // Проверка нажатия кнопки
      if (ts_x >= BackX && ts_x < BackX + Back_WIDTH && ts_y >= BackY && ts_y < BackY + Back_HEIGHT) {
        tft.setCursor(0, 0, 2);
        tft.setTextSize(2);
        Desktop();
      }
    }
    if (millis() - timing >= 1000) {
      timing = millis();
      getLastTimeTopPanel();
    }
  }
}
