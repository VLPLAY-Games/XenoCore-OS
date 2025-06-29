#define CALC_BUTTON_WIDTH 50
#define CALC_BUTTON_HEIGHT 40
#define CALC_BUTTON_MARGIN 20
#define CALC_BUTTON_ROWS 4
#define CALC_BUTTON_COLS 4

int calcButtonX[CALC_BUTTON_ROWS * CALC_BUTTON_COLS] = {CALC_BUTTON_MARGIN, 75, 140, 205, CALC_BUTTON_MARGIN, 75, 140, 205, CALC_BUTTON_MARGIN, 75, 140, 205, CALC_BUTTON_MARGIN, 75, 140, 205};
int calcButtonY[CALC_BUTTON_ROWS * CALC_BUTTON_COLS] = {125, 125, 125, 125, 175, 175, 175, 175, 225, 225, 225, 225, 275, 275, 275, 275};

const char* calcButtons[] = {"7", "8", "9", "/",
                              "4", "5", "6", "*",
                              "1", "2", "3", "-",
                              "C", "0", "=", "+"};
String inputBuffer = "";
double num1 = 0;
double num2 = 0;
char operand = ' ';


void drawCalcButton(int x, int y, const char* label) {
  tft.fillRoundRect(x, y, CALC_BUTTON_WIDTH, CALC_BUTTON_HEIGHT, 10, TFT_LIGHTGREEN);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(x + (CALC_BUTTON_WIDTH / 2), y + (CALC_BUTTON_HEIGHT / 2) - 15, 2);
  tft.println(label);
  tft.setTextColor(TFT_BLACK);
}

void handleCalcButtonPress(const char* button) {
  if (isdigit(button[0]) || button[0] == '.') {
    inputBuffer += button;
  } else if (button[0] == 'C') {
    inputBuffer = "";
    num1 = 0;
    num2 = 0;
    operand = ' ';
  } else if (button[0] == '=') {
    if (!inputBuffer.isEmpty()) {
      num2 = inputBuffer.toDouble();
      inputBuffer = "";

      switch (operand) {
        case '+':
          num1 += num2;
          break;
        case '-':
          num1 -= num2;
          break;
        case '*':
          num1 *= num2;
          break;
        case '/':
          if (num2 != 0) {
            num1 /= num2;
          } else {
            // Handle division by zero error
            inputBuffer = "Error";
            num1 = 0;
            num2 = 0;
            operand = ' ';
            return;
          }
          break;
        default:
          break;
      }

      // Convert the result to a string and display it
      inputBuffer = String(num1);
    }
  } else {
    if (!inputBuffer.isEmpty()) {
      num1 = inputBuffer.toDouble();
      operand = button[0];
      inputBuffer = "";
    }
  }

  // Update the display
  tft.fillRect(10, 75, TFT_WIDTH - 20, tft.fontHeight(), TFT_WHITE);
  tft.setCursor(10, 75, 2);
  tft.setTextColor(TFT_BLACK);
  tft.print(inputBuffer);
}

void calc() {
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLACK);
  tft.setCursor(10, 35, 2);
  tft.setTextSize(2);
  tft.println("Calculator");
  drawTopPanel(true);
  drawButBack();
  // Draw calculator buttons using a loop
  for (int i = 0; i < CALC_BUTTON_ROWS * CALC_BUTTON_COLS; i++) {
    drawCalcButton(calcButtonX[i], calcButtonY[i], calcButtons[i]);
  }
  server_flashlight();
  while (true) {
    readBT();
    check_sleep();
    check_but_reboot();
    if (millis() - timing >= 1000) {
      timing = millis();
      getLastTimeTopPanel();
    }
    if (tft.getTouch(&ts_x, &ts_y)) {
      timeToSleep = 0;
      if (ts_x >= BackX && ts_x < BackX + Back_WIDTH && ts_y >= BackY && ts_y < BackY + Back_HEIGHT) {
        Desktop();
      }
      for (int i = 0; i < CALC_BUTTON_ROWS * CALC_BUTTON_COLS; i++) {
        if (ts_x >= calcButtonX[i] && ts_x < calcButtonX[i] + CALC_BUTTON_WIDTH && ts_y >= calcButtonY[i] && ts_y < calcButtonY[i] + CALC_BUTTON_HEIGHT) {
          handleCalcButtonPress(calcButtons[i]);
          delay(50);
        }
      }
    }
  }
}
