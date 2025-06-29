// Получить время для верхней панели и нарисовать
void getLastTimeTopPanel() {
  if (isConnectToWIFI) {
    tft.setTextSize(2);
    tft.setTextColor(TFT_BLACK);
    tft.setCursor(400, 0, 2);
    tft.fillRect(400, 0, tft.width(), tft.fontHeight(), TFT_WHITE);
    timeClient.update();
    byte hours = timeClient.getHours();
    if (hours >= 0 && hours <= 9) {
      tft.print("0");
    }
    tft.print(hours);
    tft.print(":");
    byte minutes = timeClient.getMinutes();
    if (minutes >= 0 && minutes <= 9) {
      tft.print("0");
    }
    tft.print(minutes);
  }
}

// Нарисовать верхнюю панель
void drawTopPanel(bool needTime) {
  drawSdJpeg("/battery_level.jpeg", -5, 0);
  tft.setCursor(25, 0, 2);
  tft.print(String(getBatLevel()) + "%");
  if (isConnectToWIFI) {
    drawSdJpeg("/wifi_on_icon.jpeg", 80, 0);
    tft.setCursor(180, 0, 2);
    tft.setTextSize(2);
    tft.setTextColor(TFT_BLACK);
    tft.print(daysOfTheWeek[timeClient.getDay()]);
  }
  if (serverAlwaysOn && isConnectToWIFI) {
    drawSdJpeg("/server_icon.jpeg", 320, 0);
  } else if (isConnectToWIFI == false) {
    drawSdJpeg("/wifi_off_icon.jpeg", 80, 0);
  }
  if (ble_status) {
    drawSdJpeg("/bluetooth_icon.jpeg", 120, 0);
  }

  if (needTime) {
    getLastTimeTopPanel();
  }
}
