#include "SparkFun_LIS331.h"
#include <Wire.h>


#include <EEPROM.h>

int address = 0;


//double RADIUS = 0.021;
//double RADIUS = 0.0188;
//#define RADIUS 663  // mm*10
#define RADIUS 239 // mm*10


LIS331 xl;

unsigned long loop_timer = 0;


bool setUp = false;
// unsigned int cutOff = 2750; // mm/s/s
unsigned int cutOff = 3500;

unsigned long dt;

unsigned long t = 0;
unsigned long prev_newY = 0;
unsigned long prev_inv_w = 0;

unsigned short drift = 100;
// 85 : 13000
// 100 : 11000 or 15000


unsigned long currAngle = 0;

long smooth[3] = { 0, 0, 0 };

long max[3] = { 0, 0, 0 };
long min[3] = { 0, 0, 0 };

// Simple Low Pass Filter to smooth out data
//float LPF_Beta = 0.2;
void lowPassFilter(int16_t x, int16_t y, int16_t z) {
  // Also converts m/s/s to mm/s/s
  smooth[0] = 4 * smooth[0] / 5 + x * 200;
  smooth[1] = 4 * smooth[1] / 5 + y * 200;
  smooth[2] = 4 * smooth[2] / 5 + z * 200;
}

long adjust(int8_t index) {
//long adjust(int8_t index, bool setUp) {
  // Converts to mm/s/s
  
  // this line returns an adjusted version of the value to be centered and to
  return 20601 * (2*smooth[index] - max[index] - min[index]) / (2*(max[index] - min[index]));  // 1.05 * g * (2*curr - mid point) / max value
  //return ((2 * smooth[index] - max[index] - min[index]) * 20601L) / (2 * (max[index] - min[index]));
  // (2*curr - mid point) / max value     This gets the data in -1<=x<=1 format
  // g is gravity (9.81 m/s/s or 98100 mm/s/s)
  // 1.05 is included to make sure that it is going to 9.81 when facing upward
}

void setUpMaxMin() {
  int16_t x, y, z;

  int8_t temp_address = 0;

  unsigned long timer_temp = micros();
  int8_t value = 0;
  int8_t count = 0;
  long average = 0;
  char* type[] = {"Max X", "Min X", "Max Y", "Min Y", "Max Z", "Min Z", "Done!"};
  Serial.println("Setting Up Max and Mins, start with Max X in 5 sec");
  delay(5000);
  Serial.println(type[value]);
  while(value != 6) {
    xl.readAxes(x, y, z);
    lowPassFilter(x, y, z);

    average = (average*count + smooth[value/2])/(++count);
    if (micros()-timer_temp > 10000000) {
      Serial.println(average);

      EEPROM.put(temp_address, average);
      temp_address += 4;

      value++;
      count = 0;
      average = 0;

      Serial.print("Switching to ");
      Serial.print(type[value]);
      Serial.println(" in 5 sec");
      delay(5000);
      timer_temp = micros();
      Serial.println(type[value]);
    }
  }
  setUp = false;
}

unsigned long isqrt(unsigned long num) {
  unsigned long res = 0;
  unsigned long bit = 1UL << 30;

  while (bit > num) bit >>= 2;

  while (bit != 0) {
    if (num >= res + bit) {
      num -= res + bit;
      res = (res >> 1) + bit;
    } else {
      res >>= 1;
    }
    bit >>= 2;
  }
  return res;
}






void accelSetup() {
  // put your setup code here, to run once:
  pinMode(9, INPUT);  // Interrupt pin input
  Wire.begin();
  xl.setI2CAddr(0x19);
  xl.begin(LIS331::USE_I2C);

  xl.intSrcConfig(LIS331::INT_SRC, 1);
  xl.setIntDuration(50, 1);
  xl.setIntThreshold(2, 1);
  xl.enableInterrupt(LIS331::Z_AXIS, LIS331::TRIG_ON_HIGH, 1, true);

  pinMode(12, OUTPUT);


  // Get max and min values form eeprom (also create these values if setup is true)

  if (setUp) setUpMaxMin();
  EEPROM.get(0, max[0]);
  EEPROM.get(4, min[0]);
  EEPROM.get(8, max[1]);
  EEPROM.get(12, min[1]);
  EEPROM.get(16, max[2]);
  EEPROM.get(20, min[2]);
  //EEPROM.get(0, max);
  //EEPROM.get(12, min);

  Serial.println(max[0]);
  Serial.println(max[1]);
  Serial.println(max[2]);
  Serial.println(min[0]);
  Serial.println(min[1]);
  Serial.println(min[2]);

}

void accelLoop() {
  // if (loop_timer >= 90000000) {
  //   drift = 150;
  // } else if (loop_timer >= 85000000) {
  //   drift = 140;
  // } else if (loop_timer >= 80000000) {
  //   drift = 130;
  // } else if (loop_timer >= 75000000) {
  //   drift = 120;
  // } else if (loop_timer >= 70000000) {
  //   drift = 110;
  // } else if (loop_timer >= 65000000) {
  //   drift = 100;
  // } else if (loop_timer >= 60000000) {
  //   drift = 90;
  // } else if (loop_timer >= 55000000) {
  //   drift = 80;
  // } else if (loop_timer >= 50000000) {
  //   drift = 70;
  // } else if (loop_timer >= 45000000) {
  //   drift = 60;
  // } else if (loop_timer >= 40000000) {
  //   drift = 50;
  // }

  int16_t x, y, z;

  unsigned long timer_temp = micros();
  if (timer_temp - loop_timer > 5000) { //100000
    loop_timer = timer_temp;

    xl.readAxes(x, y, z);

    // Use the low pass filter on the raw data
    lowPassFilter(x, y, z);  // low pass filter, places outputs in smooth_

    // Lower the cutOff if the angular velocity is greater than half a rotation per second
    //cutOff = w >= PI ? 27500 : 5000;

    long newZ = adjust(2);
    newZ = constrain(newZ, -9810, 9810);
    if (abs(newZ) < cutOff) newZ = 0;

    long newX = adjust(0);
    int sign = newX > 0 ? 1 : -1;
    if (abs(newX) < cutOff) newX = 0;

    long newY = adjust(1);  // acceleration (mm/s/s)
    if (abs(newY) < cutOff) newY = 0;

    //test
    //newY = 8000; // 8 m/s/s
    //newY = 500000; // 500 m/s/s





    /*
    // Make it so that the angle is not recorded during set up
    if (!setUp) {
      // gPercent = 2 * abs(acos(1 - (abs(newZ) / 9810))) / PI;
      // newX *= gPercent;
      // newY *= gPercent;


      // newX = abs(newZ) * newX / 9810;
      // newY = abs(newZ * newY / 9810);



      // ang_vel = sqrt(abs(newY) / RADIUS/100)*10;  // angular velocity (rad/s)
      //unsigned long inverse_w = 2 * 10000 * 10 / (unsigned long)(sqrt(newY*10*100 / RADIUS));  // inverse w (microsec / rad)
      
      //unsigned long inverse_w = (newY == 0) ? 0 : 200000 / (unsigned long)(sqrt((unsigned long)abs(newY)*1000 / RADIUS));
      //unsigned long inverse_w = (newY) ? 200000 / (unsigned long)sqrt((unsigned long)abs(newY)*1000 / RADIUS) : 0;
      unsigned long inverse_w = (newY) ? 200000 / isqrt((unsigned long)abs(newY)*1000 / RADIUS) : 0;

      // unsigned long temp = micros();
      // dt = (temp - t); // times 1000000 to convert rad to micro rad
      // t = temp;
      dt = micros() - t;
      t += dt;

      // testing drift
      // y(13000) = 85
      // y(15000) = 100


      // (100-85)/(11000-13000)*y + b
      //drift = -3*abs(newY)/400 + 365/2;
      // (100-85)/(15000-13000)*y + b
      //drift = 3*abs(newY)/400 + 25/2;


      // rad/s * (micros / 1000000)
      // 1000 * microsec / (microsec / microrad)
      // 1000 * microrad = nanorad
      unsigned long scaled_dt = dt*10000; // dt*10000*10;  why 10? idk // no more 10 :( it because of overflow


      // unsigned long predict_f = (newY == 0) ? 0 : scaled_dt / inverse_w;
      // unsigned long predict_i = (prev_newY == 0) ? 0 : scaled_dt / prev_inv_w;
      // currAngle += 1000*(predict_f + predict_i);
      currAngle += ((newY == 0 ? 0 : scaled_dt / inverse_w) + (prev_newY == 0 ? 0 : scaled_dt / prev_inv_w));

      currAngle %= 6283185;


      prev_newY = newY;
      prev_inv_w = inverse_w;

      // testing *3 to make it 1/8 of the circle
      if (currAngle < 261799*3) digitalWrite(12, HIGH); // 261799 original value (15 degrees)
      else digitalWrite(12, LOW);
    } else {
      t = micros();
    }
    */

    //unsigned long inverse_w = (newY) ? 200000 / isqrt((unsigned long)abs(newY)*10000 / RADIUS) : 0;
    //unsigned long inverse_w = (newY) ? 200000 / isqrt((unsigned long)abs(newY)*10 / RADIUS) : 0; // s/rad * 2 * 100000
    unsigned long inverse_w = (newY) ? 2000000 / isqrt((unsigned long)abs(newY)*1000 / RADIUS) : 0; // s/rad * 2 * 100000
    // Times 2 to cancel out /2 in angle calculation
    // Times 10 to cancel out the mm*10 in RADIUS

    dt = micros() - t;
    t += dt;

    //unsigned long scaled_dt = dt*100000;
    unsigned long scaled_dt = dt*1000; // nano seconds

    // 100*(2 * ns / (s/rad * 2 * 100000)) = 100 * rad/s * (s * 1000000000) / 100000 = rad * 100 * 1000000000 / 100000 = rad*1000000 = micro rad
    currAngle += 100*(((newY == 0 ? 0 : scaled_dt / inverse_w) + (prev_newY == 0 ? 0 : scaled_dt / prev_inv_w)));

    currAngle %= 6283185;


    prev_newY = newY;
    prev_inv_w = inverse_w;

    // testing *3 to make it 1/8 of the circle
    if (currAngle < 261799*3) digitalWrite(12, HIGH); // 261799 original value (15 degrees)
    else digitalWrite(12, LOW);

    
    Serial.print("newY:");
    Serial.print(newY);
    Serial.print(",inverse_w:");
    Serial.print(prev_inv_w);
    Serial.print(",dt:");
    Serial.print(dt);
    Serial.print(",angle:");
    Serial.println(currAngle);
  }


  if (digitalRead(9) == HIGH) {
    Serial.println("Interrupt");
  }


  // if (loop_timer >= 40000000 + address * 1875 / 32 && address < EEPROM.length() && loop_timer <= 100000000) {
  //   // Puts the angular velocity into the EEPROM
  //   //EEPROM.put(address, int(prev_newY));
  //   //EEPROM.put(address, float(temp));
  //   EEPROM.put(address, int(prev_inv_w));
  //   // Increments the EEPROM
  //   address += 4;
  // }
}