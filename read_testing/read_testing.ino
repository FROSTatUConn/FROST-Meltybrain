#include <EEPROM.h>

short output[3];

void readMemory() {
  for (int i = 0; i < 512; i+=6) {
    EEPROM.get(i, output);
    for (int j = 0; i < 3; i++) {
      Serial.print(output[j]);
    }
    Serial.print('\n');
  }
}

void setup() {
  Serial.begin(115200);
  delay(5000);
  readMemory();
}

void loop() {

}