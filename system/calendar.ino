void calendar(){
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLACK);
  tft.setCursor(10, 35, 2);
  tft.setTextSize(2);
  tft.println("Calendar");
  drawTopPanel(true);
  drawButBack();
  server_flashlight();
  drawCalendar();


  while (true) {
    readBT();
    check_sleep();
    check_but_reboot();
    if (tft.getTouch(&ts_x, &ts_y)) {
      timeToSleep = 0;
      // Проверка нажатия кнопки
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


void drawCalendar() {
  // Set text color and size
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.setTextSize(2);

  // Get current time
  time_t rawTime = timeClient.getEpochTime();
  struct tm* timeInfo = localtime(&rawTime);

  // Extract year and month
  int year = timeInfo->tm_year + 1900;
  int month = timeInfo->tm_mon + 1;

  // Get the first day of the month and the number of days in the month
  timeInfo->tm_mday = 1; // Set to the first day of the month
  mktime(timeInfo); // Normalize struct tm

  int firstDayOfWeek = timeInfo->tm_wday; // 0 = Sunday, 1 = Monday, ..., 6 = Saturday
  int daysInMonth = 31;

  // Adjust days in month based on the month and year
  if (month == 4 || month == 6 || month == 9 || month == 11) {
    daysInMonth = 30;
  } else if (month == 2) {
    // Check for leap year
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
      daysInMonth = 29;
    } else {
      daysInMonth = 28;
    }
  }

  // Draw the header (Month and Year)
  tft.setCursor(20, 70);
  tft.printf("%s %d", months[month - 1], year);

  // Draw the days of the week

  int x = 20;
  int y = 100;

  for (int i = 0; i < 7; i++) {
    tft.setCursor(x, y);
    tft.print(daysOfWeekShort[i]);
    x += 50; // Adjust spacing between days
  }

  // Draw the days in the month
  x = 20 + firstDayOfWeek * 50; // Start position based on the first day of the month
  y = 130;

  for (int day = 1; day <= daysInMonth; day++) {
    tft.setCursor(x, y);
    tft.print(day);

    x += 50; // Move to the next column
    if ((day + firstDayOfWeek) % 7 == 0) { // Move to the next row after Saturday
      x = 20;
      y += 30; // Adjust row spacing
    }
  }
}
