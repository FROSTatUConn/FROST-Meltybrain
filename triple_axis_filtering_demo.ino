#include "SparkFun_LIS331.h"
#include <Wire.h>


LIS331 xl;

typedef struct {
  // exponential smoothing filter with optional
  // auto adjusting baseline
  float alpha;    // exp smoothing parameter
  int prev;       // previous value
  int value;      // current value to return

  int avg;        // cumulative avg of sensor
  int count;      // count for cumulative avg
  bool autocb;    // auto calibrate accelerometer
                  // (auto zero baseline)
} Filter;

void filterInit(Filter* f, float alpha, bool autocb=false) {  
  // intializes values to user specified values or default. 
  // call this function in setup()
  f->alpha = alpha;
  f->prev  = 0;
  f->value = 0;

  f->avg = 0;
  f->count = 1;
  f->autocb = autocb;
}

int filterUpdate(Filter* f, int16_t current) {
  // exponential smoothing implementation for low pass filtering
  // adjustable alpha parameter
  // could improve speed by using fixed point math, not too bad now
  f->value = f->alpha * current + (1 - f->alpha) * f->prev;
  f->prev = f->value;

  // get the cumulative avg
  if (f->count >= 200) f->count = 1;
  f->avg += (f->value - f->avg) / (f->count++);

  // subtract the current value by the cumulative avg
  if (f->autocb) return (f->value - f->avg);
  else return f->value;
}

// Filter stuff
Filter x_filter;


void setup() {
  // put your setup code here, to run once:
  pinMode(3, INPUT);  // Interrupt pin input
  Wire.begin();
  xl.setI2CAddr(0x19);        
  xl.begin(LIS331::USE_I2C);  
  xl.intSrcConfig(LIS331::INT_SRC, 1);  
  xl.setIntDuration(50, 1);             
  xl.setIntThreshold(2, 1);            
  xl.enableInterrupt(LIS331::Z_AXIS, LIS331::TRIG_ON_HIGH, 1, true);
  Serial.begin(9600);

  // initialize the filter with alpha value
  filterInit(&x_filter, 0.37, true); // corresponds to 63%, (e-1)/e
}

void loop() {
  static long loopTimer = 0;
  int16_t x, y, z;
  if (millis() - loopTimer > 5) {
    loopTimer = millis();
    xl.readAxes(x, y, z);

    // Serial.println(xl.convertToG(6,x)); // The convertToG() function
    // Serial.println(xl.convertToG(6,y)); // accepts as parameters the
    // Serial.println(xl.convertToG(6,z)); // raw value and the current
    // Serial.println(" ");                // maximum g-rating.
    
    Serial.print(filterUpdate(&x_filter, x));
    Serial.print(",25,-25,");
    Serial.print(x);
    Serial.print("\n");
  }
  if (digitalRead(3) == HIGH) {
    Serial.println("Interrupt");
  }
}
