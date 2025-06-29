void checkSystem() {
  tft.fillScreen(TFT_WHITE);
  drawTopPanel(true);
  tft.setTextColor(TFT_BLACK);
  tft.setCursor(10, 35, 2);
  tft.println("Check System");

  SystemCheck checks[] = {
    {"File system", SPIFFS.begin()},
    {"BME680", bme.readTemperature() > 0.00 && bme.readTemperature() < 80.00},
    {"SD", sd_card_init},
    {"Pulse sensor", pulsesensor_worked},
    {"Charge sensor", ina219_status}
  };

  bool success = true;

  for (const SystemCheck& check : checks) {
    tft.print(check.label);
    tft.print(": ");
    if (check.condition) {
      tft.println("OK");
    } else {
      success = false;
      tft.println("ERROR");
    }
  }

  tft.println("Checking complete!");
  if (success) {
    tft.println("No violations found.");
  } else {
    tft.println("Violations found.");
  }

  delay(3000);
  SettingsFuncs();
}


void server_flashlight() {
  if (flashlightOn) {
    drawSdJpeg("/flashlight_icon.jpeg", 360, 0);
  }
  timeToSleep = 0;
}


void check_but_reboot(bool initial){
  currentState = digitalRead(GPIO_NUM_27);                   // read the state of the switch/button:

  if(lastState == HIGH && currentState == LOW) {            // button is pressed
    pressedTime = millis();
    isPressing = true;
    isLongDetected = false;
  } else if(lastState == LOW && currentState == HIGH) {     // button is released
    isPressing = false;
  }

  if(isPressing == true && isLongDetected == false) {
    long pressDuration = millis() - pressedTime;

    if( pressDuration > LONG_PRESS_TIME ) {
      isLongDetected = true;
    }
  }
  if (isLongDetected && initial == false){
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    tft.setCursor(160, 225, 2);
    tft.println("Rebooting");
    drawSdJpeg("/xc_logo_black.jpeg", 160, 75);
    delay(2000);
    ESP.restart();
  }
  if (isLongDetected && initial){
    SPIFFS.remove(CALIBRATION_FILE);
    ESP.restart();
  }

  lastState = currentState;                                 // save the the last state
}
