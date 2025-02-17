#include <EEPROM.h>

short output[3];

void readMemory() {

/* Iterates through the maximum number of arrays of three shorts that
can be iterated through in the Arduino Nano's 1024 byte EEPROM */

  for (int i = 0; i < 341; i+=6) {
    EEPROM.get(i, output);
    for (int j = 0; j < 3; j++) {
      Serial.print(output[j] + " ");
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