bool session_active = false;

// Функция для чтения данных сайта с SD
String getServerCode(String fileName = "") {
  File file = SD.open("/" + fileName);
    if (file) {
      // Чтение содержимого файла
      String content = "";
      while (file.available()) {
        content += (char)file.read();
      }
      String wifi_info = "";
      if (isConnectToWIFI) {
        wifi_info = wifi_info + ssidNet;
      }
      content.replace("%USERNAME%", username);
      content.replace("%PASSWORD%", password);
      content.replace("%MODEL%", device_model);
      content.replace("%DEVICE_NAME%", deviceName);
      content.replace("%FIRMWARE_VERSION%", "XenoCoreOS " + String(version));
      content.replace("%WIFI%", wifi_info);
      content.replace("%FIRMWARE_RELEASE_DATE%", firmware_date);
      temp = String(bme.readTemperature());
      content.replace("%BMP680_TEMP%", temp.substring(0, temp.length() - 1) + "*C");
      temp = String(bme.readPressure()/133.32);
      content.replace("%BMP680_PRES%", temp.substring(0, temp.length() - 3) + "mmHg");
      temp = String(bme.readHumidity());
      content.replace("%BMP680_HUM%", temp.substring(0, temp.length() - 1) + "%");
      content.replace("%BMP680_IAQ%", String(bme.gas_resistance / 1000.0) + "IAQ");

      content.replace("%SLEEP_MODE%", sleep_off ? "Inactive" : "Active");
      content.replace("%BAT_MODE%", energy_saving_mode ? "Energy saving" : "Normal");
      content.replace("%BL_STATE%", ble_status ? "On" : "Off");
      content.replace("%SERVER_STATE%", serverAlwaysOn ? "On" : "Off");
      content.replace("%BAT_STATE%", energy_saving_mode ? "On" : "Off");
      content.replace("%SLEEP_STATE%", sleep_off ? "Off" : "On");
      if (ina219_status) {
        byte capacity;
        current_mA = ina219.getCurrent_mA();
        if (current_mA >= capacity_mah) {
          capacity = 99;
        } else if (current_mA <= 10) {
          capacity = 0;
        } else {
          capacity = floor(current_mA / 7);
        }
        content.replace("%BAT_STATE%", String(capacity) + "%");
      }

      // Закрытие файла
      file.close();
      // Отправка содержимого клиенту
      return content;
    }
}
// Функция для чтения картинок для сайта с SD
void serveImageSite(AsyncWebServerRequest *request, const char *filePath, const char *contentType) {
  File file = SD.open(filePath);
  if (file) {
    request->send(SD, filePath, contentType);
    file.close();
  } else {
    request->send(404, "text/plain", "File Not Found");
  }
}

// Функция запуска сервера
void ServerPowerUp() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    if (isAuthenticated(request->client()->remoteIP())) {
      request->send(200, "text/html", getServerCode("mainPage.html"));
    } else {
      request->send(200, "text/html", getServerCode("loginPage.html"));
    }
  });

  server.on("/login", HTTP_GET, [](AsyncWebServerRequest *request){
    if (!isAuthenticated(request->client()->remoteIP())) {
      // Выполняйте вход только если пользователь не аутентифицирован
      String username_site = request->arg("username_site");
      String password_site = request->arg("password_site");
      if (username_site == username && password_site == password) {
        addAllowedIP(request->client()->remoteIP());  // Добавление IP-адреса в список разрешенных
        request->send(200, "text/plain", "OK");
        request->send(200, "text/html", getServerCode("mainPage.html"));
      } else {
        request->send(200, "text/plain", "FAIL");
      }
    } else {
      request->send(403);  // Ошибка доступа (Forbidden)
    }
  });
  server.on("/updatesystem", HTTP_GET, [](AsyncWebServerRequest *request){
    if (isAuthenticated(request->client()->remoteIP())) {
      request->send(200, "text/html", getServerCode("serverUpdateSystem.html"));
    } else {
      request->send(200, "text/html", getServerCode("loginPage.html"));
    }
  });
  server.on("/mainpage", HTTP_GET, [](AsyncWebServerRequest *request){
    if (isAuthenticated(request->client()->remoteIP())) {
      request->send(200, "text/html", getServerCode("mainPage.html"));
    } else {
      request->send(200, "text/html", getServerCode("loginPage.html"));
    }
  });
  server.on("/about", HTTP_GET, [](AsyncWebServerRequest *request){
    if (isAuthenticated(request->client()->remoteIP())) {
      request->send(200, "text/html", getServerCode("about.html"));
    } else {
      request->send(200, "text/html", getServerCode("loginPage.html"));
    }
  });
  server.on("/sensors", HTTP_GET, [](AsyncWebServerRequest *request){
    if (isAuthenticated(request->client()->remoteIP())) {
      request->send(200, "text/html", getServerCode("sensors.html"));
    } else {
      request->send(200, "text/html", getServerCode("loginPage.html"));
    }
  });
  server.on("/settings", HTTP_GET, [](AsyncWebServerRequest *request){
    if (isAuthenticated(request->client()->remoteIP())) {
      request->send(200, "text/html", getServerCode("settings.html"));
    } else {
      request->send(200, "text/html", getServerCode("loginPage.html"));
    }
  });
  server.on("/bluetooth_on", HTTP_GET, [](AsyncWebServerRequest *request){
    if (isAuthenticated(request->client()->remoteIP())) {
      SerialBT.begin(deviceName);
      ble_status = true;
      request->send(200, "text/html", getServerCode("settings.html"));
    } else {
      request->send(200, "text/html", getServerCode("loginPage.html"));
    }
  });
  server.on("/bluetooth_off", HTTP_GET, [](AsyncWebServerRequest *request){
    if (isAuthenticated(request->client()->remoteIP())) {
      btStop();
      ble_status = false;
      request->send(200, "text/html", getServerCode("settings.html"));
    } else {
      request->send(200, "text/html", getServerCode("loginPage.html"));
    }
  });
  server.on("/web_server_on", HTTP_GET, [](AsyncWebServerRequest *request){
    if (isAuthenticated(request->client()->remoteIP())) {
      EEPROM.write(serverAlwaysOn_ADDR + 1, 1);
      EEPROM.commit();
      request->send(200, "text/html", getServerCode("settings.html"));
    } else {
      request->send(200, "text/html", getServerCode("loginPage.html"));
    }
  });
  server.on("/web_server_off", HTTP_GET, [](AsyncWebServerRequest *request){
    if (isAuthenticated(request->client()->remoteIP())) {
      EEPROM.write(serverAlwaysOn_ADDR + 1, 0);
      EEPROM.commit();
      request->send(200, "text/html", getServerCode("settings.html"));
    } else {
      request->send(200, "text/html", getServerCode("loginPage.html"));
    }
  });
  server.on("/battery_safe_on", HTTP_GET, [](AsyncWebServerRequest *request){
    if (isAuthenticated(request->client()->remoteIP())) {
      energy_saving_mode = true;
      setCpuFrequencyMhz(80);
      request->send(200, "text/html", getServerCode("settings.html"));
    } else {
      request->send(200, "text/html", getServerCode("loginPage.html"));
    }
  });
  server.on("/battery_safe_off", HTTP_GET, [](AsyncWebServerRequest *request){
    if (isAuthenticated(request->client()->remoteIP())) {
      energy_saving_mode = false;
      setCpuFrequencyMhz(240);
      request->send(200, "text/html", getServerCode("settings.html"));
    } else {
      request->send(200, "text/html", getServerCode("loginPage.html"));
    }
  });
  server.on("/sleep_mode_activate", HTTP_GET, [](AsyncWebServerRequest *request){
    if (isAuthenticated(request->client()->remoteIP())) {
      EEPROM.write(sleep_mode, 0255);
      sleep_off = false;
      request->send(200, "text/html", getServerCode("settings.html"));
    } else {
      request->send(200, "text/html", getServerCode("loginPage.html"));
    }
  });
  server.on("/sleep_mode_inactivate", HTTP_GET, [](AsyncWebServerRequest *request){
    if (isAuthenticated(request->client()->remoteIP())) {
      EEPROM.write(sleep_mode, 1);
      sleep_off = true;
      request->send(200, "text/html", getServerCode("settings.html"));
    } else {
      request->send(200, "text/html", getServerCode("loginPage.html"));
    }
  });
  server.on("/logout", HTTP_GET, [](AsyncWebServerRequest *request){
    removeAllowedIP(request->client()->remoteIP());  // Удаление IP-адреса из списка разрешенных
    request->send(200, "text/html", getServerCode("loginPage.html"));
  });

  server.on("/xc_logo_white.jpeg", HTTP_GET, [](AsyncWebServerRequest *request) {
    serveImageSite(request, "/xc_logo_white.jpeg", "image/jpeg");
  });
  server.on("/xc_logo_site.ico", HTTP_GET, [](AsyncWebServerRequest *request) {
    serveImageSite(request, "/xc_logo_site.ico", "image/x-icon");
  });
  /*handling uploading firmware file */
  server.on("/update", HTTP_POST, [](AsyncWebServerRequest *request){
    AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    request->send(response);
    ESP.restart();
  }, [](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final){
    if (!index) {
      tft.fillScreen(TFT_BLACK);
      tft.setTextColor(TFT_WHITE);
      tft.setTextFont(2);
      tft.setTextSize(2);
      drawSdJpeg("/xc_logo_black.jpeg", 160, 75);
      tft.setCursor(140, 225, 2);
      tft.println("Updating system");
      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) {
        Update.printError(Serial);
      }
    }

    if (Update.write(data, len) != len) {
      Update.printError(Serial);
    }

    if (final) {
      if (Update.end(true)) {
        ESP.restart();
      } else {
        Update.printError(Serial);
      }
    }
  });
  server.begin();
}

// Настройки сервера
void serverSettings() {
  tft.fillScreen(TFT_WHITE);
  drawTopPanel(true);
  tft.setTextColor(TFT_BLACK);
  tft.setCursor(10, 35, 2);
  tft.println("Server Settings");
  tft.print("Server autostart: ");
  tft.println(serverAlwaysOn ? "On" : "Off");
  tft.print("Will the server be on all the time?");
  Button buttons[] = {
    {20, 150, 200, 40, "Off / On"},
  };
  for (Button& button : buttons) {
    drawButton(button);
  }
  server_flashlight();
  // Кнопка назад
  drawButBack();
  while (true) {
    readBT();
    check_sleep();
    check_but_reboot();
    if (tft.getTouch(&ts_x, &ts_y)) {
      timeToSleep = 0;
      // Проверка нажатия кнопки
      if (ts_x >= BackX && ts_x < BackX + Back_WIDTH && ts_y >= BackY && ts_y < BackY + Back_HEIGHT) {
        SettingsFuncs();
      }
      for (Button& button : buttons) {
        if (ts_x >= button.x && ts_x < button.x + button.width && ts_y >= button.y && ts_y < button.y + button.height) {
          if (strcmp(button.label, "Off / On") == 0) {
            if (!serverAlwaysOn) {
              // Да
              EEPROM.write(serverAlwaysOn_ADDR + 1, 1);
              EEPROM.commit();
              timeClient.setTimeOffset(utcTimePlus ? utcTime * 3600 : -utcTime * 3600);
              server.begin();
              serverAlwaysOn = true;
            } else if (sleep_off) {
              // Нет
              EEPROM.write(serverAlwaysOn_ADDR + 1, 0);
              EEPROM.commit();
              serverAlwaysOn = false;
            }
            EEPROM.commit();
            delay(200);
            serverSettings();
          }
        }
      }
    }
  }
}

bool isAuthenticated(IPAddress ipAddress) {
  for (int i = 0; i < numAllowedIPs; ++i) {
    if (allowedIPs[i] == ipAddress) {
      return true;
    }
  }
  return false;
}

void addAllowedIP(IPAddress ipAddress) {
  if (numAllowedIPs < sizeof(allowedIPs) / sizeof(allowedIPs[0])) {
    allowedIPs[numAllowedIPs++] = ipAddress;
  }
}

void removeAllowedIP(IPAddress ipAddress) {
  for (int i = 0; i < numAllowedIPs; ++i) {
    if (allowedIPs[i] == ipAddress) {
      for (int j = i; j < numAllowedIPs - 1; ++j) {
        allowedIPs[j] = allowedIPs[j + 1];
      }
      numAllowedIPs--;
      break;
    }
  }
}
