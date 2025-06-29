// Рисовка значений с MQ135
void drawppm() {
  tft.setTextSize(2);
  tft.setCursor(0, 250, 2);
  tft.setCursor(35, 245, 2);
  tft.fillRect(35, 250, 150, tft.fontHeight(), TFT_WHITE);
  tft.print(bme.gas_resistance / 1000.0);  // analog data
  tft.print("IAQ");  // Unit = part per million
}

// Отрисовка кнопки назад
void drawButBack() {
  tft.fillRoundRect(BackX, BackY, Back_WIDTH, Back_HEIGHT, 10, TFT_RED);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(BackX + (Back_WIDTH / 2) - 25, BackY + (Back_HEIGHT / 2) - 15);
  tft.println("Back");
  tft.setTextColor(TFT_BLACK);
}

// Отрисовка кнопок
void drawButton(Button& button) {
  tft.fillRoundRect(button.x, button.y, button.width, button.height, 10, TFT_LIGHTGREEN);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(button.x + (button.width / 2) - (strlen(button.label) * 6), button.y + (button.height / 2) - 15, 2);
  tft.println(button.label);
}
