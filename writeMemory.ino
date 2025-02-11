unsigned int time = millis();
int prev = 0;
short initial;

void writeMemory() {
  delay(5000);
  initial = EEPROM.read(0);
  short coords[3];
  for (int j = 0; j < 3; j++) {
    coords[j] = static_cast<short>(smooth[j]);
  }
  if (initial != 0) {
    int i = 6;
    EEPROM.put(0, static_cast<short>(0));
    while (time < 240000) {
      if (((time - prev) >= 500)) {
        EEPROM.put(i, coords);
        i += 6;
      }
    }
  }
}

void readMemory() {
  for (int i = 0; i < 512; i++) {

  }
}