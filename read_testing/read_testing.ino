#include <EEPROM.h>

short output;

void readMemory() {
  for (int i = 0; i < 512; i+=2) {
    EEPROM.get(i, output);
  }
}

void setup() {
  Serial.begin(115200);
}

void loop() {

}