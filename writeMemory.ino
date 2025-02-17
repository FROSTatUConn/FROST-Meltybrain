unsigned int time = millis();
int prev = 0;
short initial;

void writeMemory() {
  // Delays ONE MINUTE!!!
  delay(60000);

  // Reads the first byte
  initial = EEPROM.read(0);

  // This array of size three stores the current filtered coordinates
  short coords[3];

  // 
  if (initial != 0) {
    int i = 6;
    EEPROM.put(0, static_cast<short>(0));
    while (time < 240000) {
      unsigned int current_time = time;
      if (((current_time - prev) >= 500)) {
        EEPROM.put(i, coords);
        i += 6;
      }
      // This loop records the current three coordinates
      for (int j = 0; j < 3; j++) {
        coords[j] = static_cast<short>(smooth[j]);
      }


    }
  }
}

