void readBT() {
  if (SerialBT.available() and ble_status) {
    char c = SerialBT.read();
    // ble
    if (c == '0') {
      digitalWrite(22, LOW);
    } else if (c == '1') {
      digitalWrite(22, HIGH);
    } else if (c == '2') {  // wifi
      WiFi.disconnect();
      isConnectToWIFI = false;
    } else if (c == '3') {
      WiFi.reconnect();
      isConnectToWIFI = true;
    } else if (c == '4') {  // flashlight
      flashlightOn = false;
      digitalWrite(flashLight, LOW);
      tft.fillRect(355, 0, 32, 32, TFT_WHITE);
    }  else if (c == '5') {
      flashlightOn = true;
      digitalWrite(flashLight, HIGH);
      drawSdJpeg("/flashlight_icon.jpeg", 360, 0);
    } else if (c == '7') {  // sleep settings
      if (!sleep_off) {
          EEPROM.write(sleep_mode, 1);
          sleep_off = true;
          EEPROM.commit();
      }
    } else if (c == '6') {
      if (sleep_off) {
          EEPROM.write(sleep_mode, 0255);
          sleep_off = false;
          EEPROM.commit();
        }
    } else if (c == '8') {  // battery
      setCpuFrequencyMhz(240);
      energy_saving_mode = false;
    } else if (c == '9') {
      setCpuFrequencyMhz(80);
      energy_saving_mode = true;
    } else if (c == 'a') {  // web server
      EEPROM.write(serverAlwaysOn_ADDR + 1, 0);
      EEPROM.commit();
    } else if (c == 'b') {
      EEPROM.write(serverAlwaysOn_ADDR + 1, 1);
      EEPROM.commit();
      server.begin();
    } else if (c == 'u') {
      // Отправка данных по bluetooth
      // OS
      SerialBT.print(version);
      SerialBT.print(",");
      // WIFI
      SerialBT.print(isConnectToWIFI ? ssidNet : "Off");
      SerialBT.print(",");
      // Sleep Mode
      SerialBT.print(sleep_off ? "Off" : "On");
      SerialBT.print(",");
      // Battery Mode
      SerialBT.print(energy_saving_mode ? "Save" : "Normal");
      SerialBT.print(",");
      // Date of OS
      SerialBT.print(firmware_date);
      SerialBT.print(",");
      // BME680
      temp = String(bme.readTemperature());
      // Temperature
      SerialBT.print(temp.substring(0, temp.length() - 1) + "*C");
      SerialBT.print(",");
      temp = String(bme.readPressure()/133.32);
      // Pressure
      SerialBT.print(temp.substring(0, temp.length() - 3) + "mmHg");
      SerialBT.print(",");
      temp = String(bme.readHumidity());
      // Humidity
      SerialBT.print(temp.substring(0, temp.length() - 1) + "%");
      SerialBT.print(",");
      // IAQ
      SerialBT.print(String(bme.gas_resistance / 1000.0));
      SerialBT.print(",");
      // Flashlight
      SerialBT.print(flashlightOn ? "On" : "Off");
      SerialBT.print(",");
      // Battery state
      SerialBT.print(String(getBatLevel()));
      SerialBT.print(",");
      // Высокие значение
      float temperature = bme.readTemperature();
      if (temperature > 30){
        SerialBT.print("t!");
      }
      else if (temperature <= 30){
        SerialBT.print(" ");
      }
      SerialBT.print(",");
      float humidity = bme.readHumidity();
      if (humidity <= 25 or humidity >= 80){
        SerialBT.print("h!"); 
      }
      else if (humidity > 25 and humidity < 80){
        SerialBT.print(" ");
      }
    }
  }
}

void bluetooth_settings() {
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLACK);
  tft.setCursor(10, 35, 2);
  tft.setTextSize(2);
  tft.println("Bluetooth Settings");
  tft.println("");
  tft.print("Bluetooth status: ");
  if (ble_status) {
    tft.print("On");
  } else if (!ble_status) {
    tft.print("Off");
  }
  Button buttons[] = {
    {20, 150, 200, 40, "Off / On"},
  };

  for (Button& button : buttons) {
    drawButton(button);
  }
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
        settings_func_page2();
      }
      for (Button& button : buttons) {
        if (ts_x >= button.x && ts_x < button.x + button.width && ts_y >= button.y && ts_y < button.y + button.height) {
          if (strcmp(button.label, "Off / On") == 0) {
            if (ble_status) {
              btStop();
              ble_status = false;
              delay(200);
              if (wifi_for_bluetooth){
                WiFi.reconnect();
                isConnectToWIFI = true;
              }
              bluetooth_settings();
            } else if (!ble_status) {
              if (wifi_for_bluetooth){
                WiFi.disconnect();
                isConnectToWIFI = false;
              }
              SerialBT.begin(deviceName);
              ble_status = true;
              delay(200);
              bluetooth_settings();
            }
          }
        }
      }
    }
    if (millis() - timing >= 1000) {
      timing = millis();
      getLastTimeTopPanel();
    }
  }
}
