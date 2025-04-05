#include <EEPROM.h>

//float f = 0.00f;
int eeAddress = 0;

int time = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  for (int i=0; i<EEPROM.length(); i=i+4) {
    int data;
    EEPROM.get(i, data);
    //data = sqrt(data*data / RADIUS / RADIUS2);
    Serial.print("Time:");
    Serial.print(float(i)*1875/32000);
    //Serial.print(" - ");
    Serial.print(",W:");
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
