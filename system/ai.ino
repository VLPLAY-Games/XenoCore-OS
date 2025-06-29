// ИИ
void ai() {
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLACK);
  tft.setCursor(10, 35, 2);
  tft.setTextSize(2);
  tft.println("AI");
  File api = SPIFFS.open("/api.txt", SPIFFS.exists("/api.txt") ? FILE_READ : FILE_WRITE);;
  tft.println("Choose what to do");
  drawTopPanel(true);
  // Кнопка назад
  drawButBack();
  server_flashlight();

  Button buttons[] = {
    {20, 100, 100, 40, "Api"},
    {220, 100, 100, 40, "Request"}
  };
  for (Button& button : buttons) {
    drawButton(button);
  }
  while (true) {
    readBT();
    check_sleep();
    if (tft.getTouch(&ts_x, &ts_y)) {
      timeToSleep = 0;
      if (ts_x >= BackX && ts_x < BackX + Back_WIDTH && ts_y >= BackY && ts_y < BackY + Back_HEIGHT) {
        Desktop();
      }
      for (Button& button : buttons) {
        if (ts_x >= button.x && ts_x < button.x + button.width && ts_y >= button.y && ts_y < button.y + button.height) {
          if (strcmp(button.label, "Api") == 0) {
            api.print(keyboard(true));
          } else if (strcmp(button.label, "Request") == 0) {
            sendChatGPTRequest(keyboard(true), api.readString());
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



void sendChatGPTRequest(String message, String apiKey) {
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLACK);
  tft.setCursor(10, 35, 2);
  tft.setTextSize(2);
  tft.println("AI");
  tft.println("Try to send request");
  drawTopPanel(true);
  server_flashlight();

  String apiUrl = "https://api.openai.com/v1/chat/completions";

  http.begin(apiUrl);

  http.addHeader("Content-Type", "application/json");
  http.addHeader("Authorization", "Bearer " + apiKey);

  // Создаем JSON объект с запросом
  String requestBody = "{\"model\":\"text-davinci-002\",\"messages\":[{\"role\":\"user\",\"content\":\"" + message + "\"}]}";

  // Отправляем POST запрос с JSON телом
  int httpResponseCode = http.POST(requestBody);

  if (httpResponseCode > 0) {
    tft.print("HTTP Response code: ");
    tft.println(httpResponseCode);

    String response = http.getString();
    tft.println("Response:");
    tft.println(response);
  } else {
    tft.setCursor(10, 100, 2);
    tft.print("Error code: ");
    tft.println(httpResponseCode);
  }
  http.end();
  // Кнопка назад
  drawButBack();
  while (true) {
    readBT();
    check_sleep();
    check_but_reboot();
    if (tft.getTouch(&ts_x, &ts_y)) {
      timeToSleep = 0;
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
