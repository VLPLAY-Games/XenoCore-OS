// Изменение имени пользователя
void ChangeUsername() {
  temp = keyboard();
  if (temp != "") {
    username = temp;
    int len = username.length();
    EEPROM.write(Username_ADDR, len);
    for (int i = 0; i < len; i++) {
      EEPROM.put(Username_ADDR + 1 + i, username[i]);
    }
    EEPROM.commit();
    tft.setTextColor(TFT_BLACK);
    tft.setCursor(0, 0, 2);
    tft.setTextSize(2);
    Settings();
  } else {
    SettingsFuncs();
  }
}

// Изменение пароля
void ChangePassword() {
  temp = keyboard();
  if (temp != "") {
    password = temp;
    int len = password.length();
    EEPROM.write(Password_ADDR, len);
    for (int i = 0; i < len; i++) {
      EEPROM.put(Password_ADDR + 1 + i, password[i]);
    }
    EEPROM.commit();
    tft.setTextColor(TFT_BLACK);
    tft.setCursor(0, 0, 2);
    tft.setTextSize(2);
    Settings();
  } else {
    SettingsFuncs();
  }
}

// Изменение имени устройства
void ChangeDeviceName() {
  temp = keyboard();
  if (temp != "") {
    deviceName = temp;
    int len = deviceName.length();
    EEPROM.write(DeviceName_ADDR, len);
    for (int i = 0; i < len; i++) {
      EEPROM.put(DeviceName_ADDR + 1 + i, deviceName[i]);
    }
    EEPROM.commit();
    tft.setTextColor(TFT_BLACK);
    tft.setCursor(0, 0, 2);
    tft.setTextSize(2);
    Settings();
  } else {
    SettingsFuncs();
  }
}
