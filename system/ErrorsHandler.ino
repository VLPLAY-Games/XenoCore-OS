// Функция для вывода причины перезагрузки на TFT дисплей
void ResetReason() {
  esp_reset_reason_t reason = esp_reset_reason();
  const char* reset_reason_str = "";
  File check, file;
  check = SPIFFS.open("/log.txt", FILE_APPEND);
  // Проверка количества записей
  if (check.size() > 25 * 32) {  // Ограничение на 25 записей по 32 символов каждая
    check.close();
    // Удаляем исходный файл
    SPIFFS.remove("/log.txt");

    // Создаем новый файл
    file = SPIFFS.open("/log.txt", FILE_APPEND);
    file.close();
  }
  check.close();
  file = SPIFFS.open("/log.txt", FILE_APPEND);

  switch (reason) {
    case ESP_RST_UNKNOWN: reset_reason_str = "Unknown"; break;
    case ESP_RST_POWERON: reset_reason_str = "Power-on Reset"; break;
    case ESP_RST_EXT: reset_reason_str = "External Reset"; break;
    case ESP_RST_SW: reset_reason_str = "Software Reset"; break;
    case ESP_RST_PANIC: reset_reason_str = "Panic Reset"; break;
    case ESP_RST_INT_WDT: reset_reason_str = "Watchdog Reset (interrupt)"; break;
    case ESP_RST_TASK_WDT: reset_reason_str = "Watchdog Reset (task)"; break;
    case ESP_RST_WDT: reset_reason_str = "Watchdog Reset"; break;
    case ESP_RST_DEEPSLEEP: reset_reason_str = "Deep Sleep Reset"; break;
    case ESP_RST_BROWNOUT: reset_reason_str = "Brownout Reset"; break;
    case ESP_RST_SDIO: reset_reason_str = "SDIO Reset"; break;
  }

  // Вывод причины перезагрузки на TFT дисплей
  if (strcmp(reset_reason_str, "Power-on Reset") != 0 && strcmp(reset_reason_str, "Software Reset") != 0) {
    tft.fillScreen(TFT_BLACK);  // Очистка экрана
    tft.setCursor(0, 0);
    tft.println("A critical error has occurred in the system");
    tft.println("The system has been rebooted");
    tft.print("Error type: ");
    tft.println(reset_reason_str);
    file.print("Error: ");
    delay(3000);
  } else {
    file.print("Note: ");
  }
  file.println(reset_reason_str);
  file.close();
}
