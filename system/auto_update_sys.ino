// Для автоматического обновления системы
void downloadImage(const String& imageName) {
  String imageUrl = imagesBaseUrl + imageName;

  // Выполнение HTTP GET запроса к серверу
  http.begin(imageUrl);
  int httpCode = http.GET();
  int contentLength = http.getSize();
  Serial.println(contentLength);
  if (httpCode == HTTP_CODE_OK) {
    String imagePath = "/" + imageName;
    if (SD.exists(imagePath)) {
      File imageFile = SD.open(imagePath);
      if (imageFile.size() == contentLength){
        imageFile.close();
        // Serial.println("File already exists and size matches: " + imageName);
        return;
      }
      imageFile.close();
      SD.remove(imagePath);
    }
    File imageFile = SD.open(imagePath, FILE_WRITE);
    http.writeToStream(&imageFile);
    imageFile.close();
    // Serial.println("Downloaded image: " + imageName);
  }  // else {
  //   tft.println("Failed to download image: " + imageName);
  // }

  http.end();
}


void downloadImagesList() {
  http.begin(imagesListUrl);
  int httpCode = http.GET();
  // Serial.print(httpCode);

  if (httpCode == HTTP_CODE_OK) {
    String imagesList = http.getString();
    // Serial.println("Received images list: " + imagesList);

    // Разделение строки на отдельные имена файлов
    int startIndex = 0;
    int endIndex = imagesList.indexOf('\n');
    while (endIndex != -1) {
      String imageName = imagesList.substring(startIndex, endIndex);
      downloadImage(imageName);

      startIndex = endIndex + 1;
      endIndex = imagesList.indexOf('\n', startIndex);
    }
  } else {
    tft.println("Failed to download images list");
    // Serial.println(http.errorToString(httpCode).c_str());
  }

  http.end();
}
void downloadFile(const char* url, const char* path) {
  // Выполнение HTTP GET запроса к серверу
  http.begin(url);
  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK) {
    File file = SD.open(path, FILE_WRITE);
    http.writeToStream(&file);
    file.close();
    // Serial.println("File downloaded successfully");
  } else {
    tft.println("Failed to download file");
  }

  http.end();
}
void checkForUpdate() {
  // Выполнение HTTP GET запроса к серверу
  http.begin(serverUrl);
  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    // Serial.println("Received update info: " + payload);

    // Парсим строку с версией и хешем
    int index = payload.indexOf(' ');
    if (index != -1) {
      String versionOnServer = payload.substring(0, index);
      String md5HashOnServer = payload.substring(index + 1);

      if (versionOnServer > version_for_upd) {
        // Serial.println("Updating firmware...");
        tft.setCursor(140, 225, 2);
        tft.println("Downloading the update...");
        downloadFile(firmwareUrl, "/firmware.bin");
        downloadImagesList();
        new_update = true;
      } else {
        new_update = false;
      }
    }  // else {
    //   Serial.println("Invalid update info format");
    // }
  } else {
    tft.println("Failed to connect to server");
  }

  http.end();
}
void updateFirmware() {
  // Serial.print(F("\nSearch for firmware.."));
  File firmware =  SD.open("/firmware.bin");
  if (firmware) {
      // Serial.println(F("found!"));
      // Serial.println(F("Try to update!"));

      Update.begin(firmware.size(), U_FLASH);
      Update.onProgress([](int progress, int total) {
        tft.setTextColor(TFT_WHITE);
        int percentage = (progress * 100) / total;
        tft.fillRect(325, 225, 80, tft.fontHeight(), TFT_BLACK);
        tft.setCursor(325, 225, 2);
        tft.print(percentage);
        tft.print("%");
      });
      Update.writeStream(firmware);
      if (Update.end()) {
          // Serial.println(F("Update finished!"));
          tft.fillScreen(TFT_BLACK);
          drawSdJpeg("/xc_logo_black.jpeg", 160, 75);
          tft.setCursor(140, 225, 2);
          tft.println("Updating complete!");
          EEPROM.write(updating_sys, 0);
          EEPROM.commit();
          delay(2000);
      } else {
          EEPROM.write(updating_sys, 0);
          EEPROM.commit();
          tft.println(F("Update error!"));
          tft.println(Update.getError());
      }

      firmware.close();

      SD.remove("/firmware.bin");
      delay(2000);

      ESP.restart();
  } else {
      EEPROM.write(updating_sys, 0);
      EEPROM.commit();
      tft.println(F("Firmware file not found!"));
  }
}


// Обновление системы
void UpdatingSystem() {
  tft.fillScreen(TFT_WHITE);
  drawTopPanel(true);
  drawButBack();
  drawSdJpeg("/xc_logo_white.jpeg", 175, 50);
  tft.setCursor(0, 35, 2);
  tft.println("Update System");
  tft.setCursor(120, 175, 2);
  tft.println("XenoCoreOS " + String(version));
  tft.setCursor(140, 200, 2);
  tft.println("Checking update");
  checkForUpdate();
  if (new_update) {
    tft.setCursor(75, 275, 2);
    tft.println("Complete. Reboot required!");
    EEPROM.write(updating_sys, 1);
    EEPROM.commit();
  } else if (!new_update) {
    tft.setCursor(120, 225, 2);
    tft.println("No update available!");
  }
  while (true) {
    if (isConnectToWIFI) {
    } else if (!isConnectToWIFI) {
      tft.print("You are not connected to WIFI");
    }
    if (tft.getTouch(&ts_x, &ts_y)) {
      // Проверка нажатия кнопки
      if (ts_x >= BackX && ts_x < BackX + Back_WIDTH && ts_y >= BackY && ts_y < BackY + Back_HEIGHT) {
        SettingsFuncs();
      }
    }
  }
}
