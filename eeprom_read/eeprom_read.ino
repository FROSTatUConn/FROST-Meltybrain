#include <EEPROM.h>

//float f = 0.00f;
int eeAddress = 0;

int time = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  //EEPROM.write(eeAddress, 17);
  float a = 1001.001;

  // Tests
  // EEPROM.put(0, a);
  // EEPROM.put(4, a);
  // EEPROM.put(8, a);
  // EEPROM.put(12, a);

  // EEPROM.get(0, a);
  // Serial.println(a);
  // EEPROM.get(4, a);
  // Serial.println(a);
  // EEPROM.get(8, a);
  // Serial.println(a);
  // EEPROM.get(12, a);
  // Serial.println(a);


  for (int i=0; i<EEPROM.length(); i=i+4) {
    float data;
    EEPROM.get(i, data);
    Serial.print(i);
    Serial.print(" - ");
    Serial.println(data);
    
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  // if (millis() - time >= 200) {
  //   time = millis();
    
  //   if (eeAddress+1 == EEPROM.length()) {
  //     eeAddress = 0;
  //    } else {
  //     eeAddress++;
  //   }
  // }
  
}
