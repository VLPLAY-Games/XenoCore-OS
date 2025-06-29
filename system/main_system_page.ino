// Система
void system() {
  tft.fillScreen(TFT_WHITE);
  tft.setCursor(0, 0, 2);

  Button buttons[] = {
    {TFT_WIDTH - 160, TFT_HEIGHT - 60, 140, 40, "Desktop"},
  };

  for (Button& button : buttons) {
    drawButton(button);
  }

  char tempBuffer[10];  // Буфер для хранения значения температуры
  char pressureBuffer[10];  // Буфер для хранения значения давления
  char humidityBuffer[10];  // Буфер для хранения значения влажности

  tft.setTextColor(TFT_BLACK);
  float temperature = bme.readTemperature();
  drawSdJpeg("/temp_icon.jpeg", 0, 210);
  if (temperature >= 26 and temperature <= 29) {
    tft.fillRect(35, 210, 100, tft.fontHeight(), TFT_YELLOW);
  } else if (temperature > 29) {
    tft.setTextColor(TFT_WHITE);
    tft.fillRect(35, 210, 100, tft.fontHeight(), TFT_RED);
  } else if (temperature <  26) {
    tft.fillRect(35, 210, 100, tft.fontHeight(), TFT_WHITE);
  }
  tft.setCursor(35, 210, 2);
  dtostrf(temperature, 4, 1, tempBuffer);  // Преобразование значения температуры в строку
  tft.print(tempBuffer);
  tft.print("*C");
  tft.setTextColor(TFT_BLACK);

  float pressure = bme.readPressure() / 133.32;  // Преобразование значения давления
  drawSdJpeg("/pressure_icon.jpeg", 185, 210);
  tft.fillRect(220, 210, 45, tft.fontHeight(), TFT_WHITE);
  tft.setCursor(220, 210, 2);
  dtostrf(pressure, 3, 0, pressureBuffer);  // Преобразование значения давления в строку
  tft.print(pressureBuffer);
  tft.print("mmHg");

  drawSdJpeg("/co2_icon.jpeg", 0, 245);
  drawppm();

  float humidity = bme.readHumidity();
  drawSdJpeg("/humidity_icon.jpeg", 0, 280);
  if (humidity <= 25 or humidity >= 80) {
    tft.fillRect(35, 280, 100, tft.fontHeight(), TFT_RED);
    tft.setTextColor(TFT_WHITE);
  } else if ((humidity > 25 and humidity <= 50)or(humidity >= 70 and humidity < 80)) {
    tft.fillRect(35, 280, 100, tft.fontHeight(), TFT_YELLOW);
  } else if (humidity > 50 and humidity < 70) {
    tft.fillRect(35, 280, 100, tft.fontHeight(), TFT_WHITE);
  }
  tft.setCursor(35, 280, 2);
  dtostrf(humidity, 4, 1, humidityBuffer);  // Преобразование значения влажности в строку
  tft.print(humidityBuffer);
  tft.print("%");
  uint8_t time = 0;
  if (WiFi.status() != WL_CONNECTED && wificheckPassed == false) {
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      time++;
      if (time > 7) {
        isConnectToWIFI = false;
        wifi_for_bluetooth = false;
        wificheckPassed = true;
        break;
      }
    }
  }
  if (WiFi.status() == WL_CONNECTED) {
    timeClient.begin();
    isConnectToWIFI = true;
    wifi_for_bluetooth = true;
  } else if (WiFi.status() != WL_CONNECTED) {
    isConnectToWIFI = false;
    wifi_for_bluetooth = false;
  }
  drawTopPanel();
  tft.setCursor(0, 0, 2);
  tft.setTextSize(4);
  tft.setTextColor(TFT_BLACK);
  if (serverAlwaysOn && isConnectToWIFI) {
    ServerPowerUp();
  }
  server_flashlight();
  while (true) {
    readBT();
    check_but_reboot();

    if (millis() - timing1 >= 10000) {
      timing1 = millis();
      drawppm();
      tft.setTextColor(TFT_BLACK);

      float temperature = bme.readTemperature();
      drawSdJpeg("/temp_icon.jpeg", 0, 210);
      if (temperature >= 26 and temperature <= 29) {
        tft.fillRect(35, 210, 100, tft.fontHeight(), TFT_YELLOW);
      } else if (temperature > 29) {
        tft.setTextColor(TFT_WHITE);
        tft.fillRect(35, 210, 100, tft.fontHeight(), TFT_RED);
      } else if (temperature <  26) {
        tft.fillRect(35, 210, 100, tft.fontHeight(), TFT_WHITE);
      }
      tft.setCursor(35, 210, 2);
      dtostrf(temperature, 4, 1, tempBuffer);  // Преобразование значения температуры в строку
      tft.print(tempBuffer);
      tft.print("*C");
      tft.setTextColor(TFT_BLACK);

      tft.fillRect(220, 210, 45, tft.fontHeight(), TFT_WHITE);
      tft.setCursor(220, 210, 2);
      float pressure = bme.readPressure() / 133.32;  // Преобразование значения давления
      dtostrf(pressure, 3, 0, pressureBuffer);  // Преобразование значения давления в строку
      tft.print(pressureBuffer);

      float humidity = bme.readHumidity();
      drawSdJpeg("/humidity_icon.jpeg", 0, 280);
      if (humidity <= 25 or humidity >= 80) {
        tft.fillRect(35, 280, 100, tft.fontHeight(), TFT_RED);
        tft.setTextColor(TFT_WHITE);
      } else if ((humidity > 25 and humidity <= 50)or(humidity >= 70 and humidity < 80)) {
        tft.fillRect(35, 280, 100, tft.fontHeight(), TFT_YELLOW);
      } else if (humidity > 50 and humidity < 70) {
        tft.fillRect(35, 280, 100, tft.fontHeight(), TFT_WHITE);
      }
      tft.setCursor(35, 280, 2);
      dtostrf(humidity, 4, 1, humidityBuffer);  // Преобразование значения влажности в строку
      tft.print(humidityBuffer);
      tft.print("%");
      tft.setTextColor(TFT_BLACK);
      tft.setTextSize(4);
    }
    check_sleep();
    if (isConnectToWIFI) {
      if (millis() - timing >= 1000) {
        timing = millis();
        tft.setCursor(35, 105, 4);
        tft.fillRect(35, 105, tft.width(), tft.fontHeight(), TFT_WHITE);
        timeClient.update();
        tft.print(timeClient.getFormattedTime());
      }
    }
    if (tft.getTouch(&ts_x, &ts_y)) {
      timeToSleep = 0;
      for (Button& button : buttons) {
        if (ts_x >= button.x && ts_x < button.x + button.width && ts_y >= button.y && ts_y < button.y + button.height) {
          if (strcmp(button.label, "Desktop") == 0) {
            Desktop();
          }
        }
      }
    }
  }
}
