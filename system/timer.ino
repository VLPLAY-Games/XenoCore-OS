#define timer_c_WIDTH 50
#define timer_c_HEIGHT 40
#define timer_c_MARGIN 20

// Define the positions of the timer buttons
int timerX[] = {timer_c_MARGIN, 75, 140, timer_c_MARGIN, 75, 140, timer_c_MARGIN, 75, 140};
int timerY[] = {125, 125, 125, 175, 175, 175, 225, 225, 225};

void drawTimerButton(int x, int y, const char* label) {
  tft.fillRoundRect(x, y, timer_c_WIDTH, timer_c_HEIGHT, 10, TFT_LIGHTGREEN);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(x + (timer_c_WIDTH / 2), y + (timer_c_HEIGHT / 2) - 15, 2);
  tft.println(label);
}

void timer() {
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLACK);
  tft.setCursor(10, 35, 2);
  tft.setTextSize(2);
  tft.println("Timer");
  drawTopPanel(true);
  drawButBack();

  // Draw timer buttons using a loop
  for (int i = 0; i < 9; i++) {
    drawTimerButton(timerX[i], timerY[i], String(i + 1).c_str());
  }

  Button buttons[] = {
    {250, 125, 120, 40, "Start"},
    {250, 175, 120, 40, "Reset"}
  };

  // Отрисовка кнопок
  for (Button& button : buttons) {
    drawButton(button);
  }

  tft.setCursor(20, 75, 2);
  tft.setTextColor(TFT_BLACK);
  tft.print(String(timer_sec) + " sec");

  temp = "";
  tft.setCursor(200, 75, 2);
  tft.print("Selected: " + temp + " sec");
  server_flashlight();
  while (true) {
    readBT();
    check_but_reboot();
    if (millis() - timing2 >= 1000) {
      timing2 = millis();
      if (timer_active && timer_sec != 0) {
        tft.fillRect(20, 75, 100, tft.fontHeight(), TFT_WHITE);
        timer_sec--;
        tft.setCursor(20, 75, 2);
        tft.print(String(timer_sec) + " sec");
      } else {
        timer_active = false;
      }
    }

    if (tft.getTouch(&ts_x, &ts_y)) {
      // Check if any timer button is pressed
      for (int i = 0; i < 9; i++) {
        if (ts_x >= timerX[i] && ts_x < timerX[i] + timer_c_WIDTH && ts_y >= timerY[i] && ts_y < timerY[i] + timer_c_HEIGHT) {
          temp += String(i + 1);
          tft.fillRect(200, 75, 200, tft.fontHeight(), TFT_WHITE);
          tft.setCursor(200, 75, 2);
          tft.print("Selected: " + temp + " sec");
          delay(50);
          break;
        }
      }
      for (Button& button : buttons) {
        if (ts_x >= button.x && ts_x < button.x + button.width && ts_y >= button.y && ts_y < button.y + button.height) {
          if (strcmp(button.label, "Start") == 0) {
            timer_active = true;
            timer_sec = temp.toInt();
            tft.fillRect(20, 75, 100, tft.fontHeight(), TFT_WHITE);
            tft.setCursor(20, 75, 2);
            tft.print(String(timer_sec) + " sec");
          } else if (strcmp(button.label, "Reset") == 0) {
            tft.fillRect(20, 75, TFT_WIDTH, tft.fontHeight(), TFT_WHITE);
            timer_active = false;
            tft.setCursor(20, 75, 2);
            temp = "";
            timer_sec = 0;
            tft.print(String(timer_sec) + " sec");
            tft.setCursor(200, 75, 2);
            tft.print("Selected: " + temp + " sec");
          }
        }
      }
       if (ts_x >= BackX && ts_x < BackX + Back_WIDTH && ts_y >= BackY && ts_y < BackY + Back_HEIGHT) {
        Desktop();
      }
    }

    if (millis() - timing >= 1000) {
      timing = millis();
      getLastTimeTopPanel();
    }
  }
}
