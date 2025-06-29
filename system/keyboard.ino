// Клавиатура
const char keyboardKeys1[KEYPAD_ROWS][KEYPAD_COLS] = {
  { 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o' },
  { 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l' },
  { 'z', 'x', 'c', 'v', 'b', 'n', 'm', 'p', '!' },
};

const char keyboardKeys2[KEYPAD_ROWS][KEYPAD_COLS] = {
  { '1', '2', '3', '4', '5', '6', '7', '8', '9' },
  { '0', '@', '#', '$', '%', '&', '*', '(', ')' },
  { '-', '+', '=', '_', '/', '\\', '^', '<', '>' },
};

// Получить строку после ввода
String keyboard(bool unlimited) {
  tft.setCursor(0, 0, 2);
  tft.fillScreen(TFT_BLACK);

  // Рисуем клавиатуру
  drawKeyboard();

  // Настраиваем цвета
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  inputString = "";
  while (true) {
    if (tft.getTouch(&ts_x, &ts_y)) {
      // Обработка нажатия на клавиатуру
      if (ts_x >= 0 && ts_y >= 0 && ts_x < TFT_WIDTH && ts_y < TFT_HEIGHT) {
        int col = (ts_x - KEY_MARGIN) / (KEY_WIDTH + KEY_MARGIN);
        int row = (ts_y - KEY_MARGIN) / (KEY_HEIGHT + KEY_MARGIN);

        // Проверяем, находится ли нажатие в пределах клавиатуры
        if (col >= 0 && col < KEYPAD_COLS && row >= 0 && row < KEYPAD_ROWS) {
          // Выводим нажатый символ
          char key = getCurrentKey(row, col);
          if (key != ' ') {
            if (inputString.length() <= 20 and !unlimited) {
              inputString += key;
            } else if (unlimited) {
              inputString += key;
            }
          }
        } else {
          int buttonY = TFT_HEIGHT - (2 * KEY_HEIGHT) - KEY_MARGIN;
          int buttonWidth = KEY_WIDTH * 2 + KEY_MARGIN - 12;
          int buttonHeight = KEY_HEIGHT + KEY_MARGIN - 10;
          int button1X = KEY_MARGIN;
          int button2X = button1X + buttonWidth + KEY_MARGIN;
          int button3X = button2X + buttonWidth + KEY_MARGIN;
          int button4X = button3X + buttonWidth + KEY_MARGIN;
          int button5X = button4X + buttonWidth + KEY_MARGIN;

          if (ts_x >= button1X && ts_x < button1X + buttonWidth && ts_y >= buttonY && ts_y < buttonY + buttonHeight) {
            // Нажата кнопка "Del"
            if (!inputString.isEmpty()) {
              inputString.remove(inputString.length() - 1);
            }
          } else if (ts_x >= button2X && ts_x < button2X + buttonWidth && ts_y >= buttonY && ts_y < buttonY + buttonHeight) {
            // Нажата кнопка "Conf"
            if (!inputString.isEmpty())
              return inputString;
          } else if (ts_x >= button3X && ts_x < button3X + buttonWidth && ts_y >= buttonY && ts_y < buttonY + buttonHeight) {
            // Нажата кнопка "C Lock"
            if (currentPage == 0)
              capsLock = !capsLock;
            drawKeyboard();
          } else if (ts_x >= button4X && ts_x < button4X + buttonWidth && ts_y >= buttonY && ts_y < buttonY + buttonHeight) {
            // Нажата кнопка "N/P Page"
            currentPage = (currentPage + 1) % 2;
            drawKeyboard();
          } else if (ts_x >= button5X && ts_x < button5X + buttonWidth && ts_y >= buttonY && ts_y < buttonY + buttonHeight) {
            // Нажата кнопка "Back"
            return "";
          }
        }
        tft.fillRect(0, TFT_HEIGHT - KEY_HEIGHT, TFT_WIDTH, KEY_HEIGHT, TFT_BLACK);
        tft.setCursor(0, TFT_HEIGHT - KEY_HEIGHT);
        tft.setTextSize(2);
        tft.print(inputString);
        tft.setTextSize(1);
        delay(50);
      }
    }
  }
}

// Получить текущую кнопку
char getCurrentKey(int row, int col) {
  if (currentPage == 0 && capsLock == false)
    return keyboardKeys1[row][col];
  else if (currentPage == 0 && capsLock)
    return (char)toupper(keyboardKeys1[row][col]);
  else
    return keyboardKeys2[row][col];
}

// Нарисовать клавиатуру
void drawKeyboard() {
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(1);
  tft.fillScreen(TFT_BLACK);

  for (int row = 0; row < KEYPAD_ROWS; row++) {
    for (int col = 0; col < KEYPAD_COLS; col++) {
      int keyX = col * (KEY_WIDTH + KEY_MARGIN) + KEY_MARGIN;
      int keyY = row * (KEY_HEIGHT + KEY_MARGIN) + KEY_MARGIN;

      tft.fillRoundRect(keyX, keyY, KEY_WIDTH, KEY_HEIGHT, 10, TFT_DARKGREY);
      tft.setCursor(keyX + (KEY_WIDTH / 2) - 6, keyY + (KEY_HEIGHT / 2) - 6);
      tft.print(getCurrentKey(row, col));
    }
  }

  // Рисуем кнопки под клавиатурой
  int buttonY = TFT_HEIGHT - (2 * KEY_HEIGHT) - KEY_MARGIN;
  int buttonWidth = KEY_WIDTH * 2 + KEY_MARGIN - 12;
  int buttonHeight = KEY_HEIGHT + KEY_MARGIN - 10;

  tft.fillRoundRect(KEY_MARGIN, buttonY, buttonWidth, buttonHeight, 10, TFT_LIGHTGREEN);
  tft.setCursor(KEY_MARGIN + (buttonWidth / 2) - 18, buttonY + (buttonHeight / 2) - 6);
  tft.print("Delete");

  tft.fillRoundRect(KEY_MARGIN + buttonWidth + KEY_MARGIN, buttonY, buttonWidth, buttonHeight, 10, TFT_LIGHTGREEN);
  tft.setCursor(KEY_MARGIN + buttonWidth + KEY_MARGIN + (buttonWidth / 2) - 24, buttonY + (buttonHeight / 2) - 6);
  tft.print("Confirm");
  if (currentPage == 0) {
    tft.fillRoundRect(KEY_MARGIN + (2 * buttonWidth) + (2 * KEY_MARGIN), buttonY, buttonWidth, buttonHeight, 10, TFT_LIGHTGREEN);
    tft.setCursor(KEY_MARGIN + (2 * buttonWidth) + (2 * KEY_MARGIN) + (buttonWidth / 2) - 24, buttonY + (buttonHeight / 2) - 6);
    tft.print("Caps");
  }
  tft.fillRoundRect(KEY_MARGIN + (3 * buttonWidth) + (3 * KEY_MARGIN), buttonY, buttonWidth, buttonHeight, 10, TFT_LIGHTGREEN);
  tft.setCursor(KEY_MARGIN + (3 * buttonWidth) + (3 * KEY_MARGIN) + (buttonWidth / 2) - 18, buttonY + (buttonHeight / 2) - 6);
  tft.print("N/P Page");

  tft.fillRoundRect(KEY_MARGIN + (4 * buttonWidth) + (4 * KEY_MARGIN), buttonY, buttonWidth, buttonHeight, 10, TFT_LIGHTGREEN);
  tft.setCursor(KEY_MARGIN + (4 * buttonWidth) + (4 * KEY_MARGIN) + (buttonWidth / 2) - 18, buttonY + (buttonHeight / 2) - 6);
  tft.print("Back");
}
