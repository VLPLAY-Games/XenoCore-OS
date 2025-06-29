// Изменение времени UTC 2
void timeEdit2(bool initialSetup) {
  tft.fillScreen(TFT_WHITE);
  drawTopPanel(true);
  tft.setTextColor(TFT_BLACK);
  if (initialSetup) {
    drawInitialSetupLabel(true);
  } else if (initialSetup == false) {  // Кнопка назад
    drawButBack();
  }
  tft.println("Change time");
  tft.print("You need to enter the hour difference in UTC format. In the format 'x' - hours.");

  Button buttons[] = {
    {150, 200, 125, 40, "Plus"},
    {150, 250, 125, 40, "Minus"},
  };

  for (Button& button : buttons) {
    drawButton(button);
  }
  if (serverAlwaysOn && isConnectToWIFI) {
    ServerPowerUp();
  }
  while (true) {
    check_but_reboot();
    if (tft.getTouch(&ts_x, &ts_y)) {
      // Проверка нажатия кнопки
      if (ts_x >= BackX && ts_x < BackX + Back_WIDTH && ts_y >= BackY && ts_y < BackY + Back_HEIGHT & initialSetup == false) {
        timeEdit();
      }
      for (Button& button : buttons) {
        if (ts_x >= button.x && ts_x < button.x + button.width && ts_y >= button.y && ts_y < button.y + button.height) {
          if (strcmp(button.label, "Plus") == 0) {
            // Плюс
            EEPROM.write(utcTimePlus_ADDR + 1, 1);
            EEPROM.commit();
            tft.fillScreen(TFT_WHITE);
            drawTopPanel(true);
            if (initialSetup == false) {
              timeClient.setTimeOffset(utcTimePlus ? utcTime * 3600 : -utcTime * 3600);
              SettingsFuncs();
            } else {
              InitialSetupComplete();
            }
          } else if (strcmp(button.label, "Minus") == 0) {
            // Минус
            EEPROM.write(utcTimePlus_ADDR + 1, 0);
            EEPROM.commit();
            tft.fillScreen(TFT_WHITE);
            drawTopPanel(true);
            if (initialSetup == false) {
              timeClient.setTimeOffset(utcTimePlus ? utcTime * 3600 : -utcTime * 3600);
              SettingsFuncs();
            } else {
              InitialSetupComplete();
            }
          }
        }
      }
    }
  }
}

// Изменение времени UTC 1
void timeEdit(bool initialSetup) {
  tft.fillScreen(TFT_WHITE);
  drawTopPanel(true);
  tft.setTextColor(TFT_BLACK);
  if (initialSetup) {
    drawInitialSetupLabel(true);
    tft.setCursor(10, 70, 2);
  } else if (initialSetup == false) {  // Кнопка назад
    drawButBack();
    tft.setCursor(10, 35, 2);
  }
  tft.println("Change time");
  tft.print("You need to enter the hour difference in UTC format. In the format 'x' - hours.");

  Button buttons[] = {
    {20, 260, 125, 40, "Continue"},
  };

  for (Button& button : buttons) {
    drawButton(button);
  }

  server_flashlight();
  while (true) {
    check_but_reboot();
    if (tft.getTouch(&ts_x, &ts_y)) {
      // Проверка нажатия кнопки
      if (ts_x >= BackX && ts_x < BackX + Back_WIDTH && ts_y >= BackY && ts_y < BackY + Back_HEIGHT && initialSetup == false) {
        SettingsFuncs();
      }
      for (Button& button : buttons) {
        if (ts_x >= button.x && ts_x < button.x + button.width && ts_y >= button.y && ts_y < button.y + button.height) {
          if (strcmp(button.label, "Continue") == 0) {
            temp = keyboard();
            if (temp != "") {
              utcTime = atoi(temp.c_str());
              EEPROM.write(utcTime_ADDR + 1, utcTime);
              EEPROM.commit();
              if (initialSetup == false) {
                timeEdit2();
              } else {
                tft.setTextSize(2);
                timeEdit2(true);
              }
            } else {
              timeEdit(initialSetup);
            }
          }
        }
      }
    }
  }
}
