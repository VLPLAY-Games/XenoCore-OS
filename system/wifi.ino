// Настройки WIFI
void wifiSetting() {
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLACK);
  tft.setCursor(10, 35, 2);
  tft.setTextSize(2);
  tft.println("WIFI Settings");
  drawTopPanel(true);
  // Кнопка назад
  drawButBack();
  tft.setCursor(10, 70, 2);
  tft.print("WIFI Status: ");
  tft.print(isConnectToWIFI ? String("Active  ") + "(" + ssidNet + ")" : "Inactive");

  Button buttons[] = {
    {20, 100, 100, 40, isConnectToWIFI ? "Off" : "On"},
    {130, 100, 170, 40, "Change WIFI"},
    {310, 100, 150, 40, "Reconnect"}
  };

  for (Button& button : buttons) {
    drawButton(button);
  }
  server_flashlight();
  wifi_scan();
  while (true) {
    readBT();
    check_sleep();
    check_but_reboot();
    if (tft.getTouch(&ts_x, &ts_y)) {
      timeToSleep = 0;
      if (ts_x >= BackX && ts_x < BackX + Back_WIDTH && ts_y >= BackY && ts_y < BackY + Back_HEIGHT) {
        SettingsFuncs();
      }
      for (Button& button : buttons) {
        if (ts_x >= button.x && ts_x < button.x + button.width && ts_y >= button.y && ts_y < button.y + button.height) {
          if (strcmp(button.label, isConnectToWIFI ? "Off" : "On") == 0) {
            if (isConnectToWIFI) {
              WiFi.disconnect();
              isConnectToWIFI = false;
              wifi_for_bluetooth = false;
              wifiSetting();
            } else if (!isConnectToWIFI) {
              WiFi.reconnect();
              isConnectToWIFI = true;
              wifi_for_bluetooth = true;
              wifiSetting();
            }
          }
          if (strcmp(button.label, "Reconnect") == 0) {
            WiFi.disconnect();
            WiFi.reconnect();
            wifiSetting();
          }
          if (strcmp(button.label, "Change WIFI") == 0) {
            InitialConnectNet();
            int len = ssidNet.length();
            EEPROM.write(SsidNet_ADDR, len);
            for (int i = 0; i < len; i++) {
              EEPROM.put(SsidNet_ADDR + 1 + i, ssidNet[i]);
            }
            len = passwordNet.length();
            EEPROM.write(PasswordNet_ADDR, len);
            for (int i = 0; i < len; i++) {
              EEPROM.put(PasswordNet_ADDR + 1 + i, passwordNet[i]);
            }
            EEPROM.commit();
            wifiSetting();
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


// Сканирование доступных сетей
void wifi_scan() {
    tft.setTextSize(1);
    tft.setCursor(20, 150, 2);
    tft.setTextColor(TFT_BLACK);
    tft.print("Scan began: ");
    // WiFi.scanNetworks will return the number of networks found.
    int n = WiFi.scanNetworks();
    tft.print("Done        ");
    if (n == 0) {
        tft.println("no networks found");
    } else {
        tft.print(n);
        tft.println(" networks found");
        tft.println("Nr | SSID                     | RSSI | CH | Encryption");
        for (int i = 0; i < n; ++i) {
            // Print SSID and RSSI for each network found
            tft.printf("%2d", i + 1);
            tft.print(" | ");
            tft.printf("%-24.24s", WiFi.SSID(i).c_str());
            tft.print(" | ");
            tft.printf("%4d", WiFi.RSSI(i));
            tft.print(" | ");
            tft.printf("%2d", WiFi.channel(i));
            tft.print(" | ");
            switch (WiFi.encryptionType(i)) {
            case WIFI_AUTH_OPEN:
                tft.print("open");
                break;
            case WIFI_AUTH_WEP:
                tft.print("WEP");
                break;
            case WIFI_AUTH_WPA_PSK:
                tft.print("WPA");
                break;
            case WIFI_AUTH_WPA2_PSK:
                tft.print("WPA2");
                break;
            case WIFI_AUTH_WPA_WPA2_PSK:
                tft.print("WPA+WPA2");
                break;
            case WIFI_AUTH_WPA2_ENTERPRISE:
                tft.print("WPA2-EAP");
                break;
            case WIFI_AUTH_WPA3_PSK:
                tft.print("WPA3");
                break;
            case WIFI_AUTH_WPA2_WPA3_PSK:
                tft.print("WPA2+WPA3");
                break;
            case WIFI_AUTH_WAPI_PSK:
                tft.print("WAPI");
                break;
            default:
                tft.print("unknown");
            }
            tft.println();
            delay(10);
        }
    }
    // Delete the scan result to free memory for code below.
    WiFi.scanDelete();
    tft.setTextSize(2);
}
