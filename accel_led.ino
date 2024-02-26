#include "accel_led.h"

#define ACC_MEAS_PER 1  // accelerometer measurement period in ms
#define INT_PIN 1       // interrupt pin
#define RADIUS 3        // meters, check the units

LIS331 xl;

int16_t prevAngle = 0;
int16_t currAngle = 0;
int16_t raw_xyz[3];
Filter* filter_xyz[3];

// accelerometer access times in queue
unsigned long accTime[2] = {0, 0};


void accelerometerInit() {
  Wire.begin();
  xl.setI2CAddr(0x19);        
  xl.begin(LIS331::USE_I2C);  
  
  // disabling interrupt functionality for now
  /*
  pinMode(INT_PIN, INPUT);  // Interrupt pin input
  xl.intSrcConfig(LIS331::INT_SRC, INT_PIN);  // interrupt source configuration
  xl.setIntDuration(50, INT_PIN);             // set interrupt duration  
  xl.setIntThreshold(2, INT_PIN);             // set interrupt threshold
  xl.enableInterrupt(LIS331::Z_AXIS, LIS331::TRIG_ON_HIGH, INT_PIN, true);
  */

  for (int i = 0; i < 3; i++) {
    filterInit(filter_xyz[i], 0.37, true);
  }
}

void runAccelerometer(void) {
  // naive approach only using centripetal acceleration
  // to angular velocity
  unsigned long dt;
  if (micros() - accTime[1] > ACC_MEAS_PER) {
    // if it is time to pull from acc, overwrite values
    accTime[0] = accTime[1];
    accTime[1] = micros();
    xl.readAxes(raw_xyz[0], raw_xyz[1], raw_xyz[2]);
    for (int i = 0; i < 3; i++) {
      filterUpdate(filter_xyz[i], raw_xyz[i]);
    }
    // assume that it works
    // centripetal acceleration a_c is the acceleration in the x axis
    dt = accTime[1] - accTime[0];
  } else {
    dt = micros() - accTime[1];
  }
  prevAngle = currAngle;
  currAngle = (int16_t)(dt * sqrt(filter_xyz[0]->value / RADIUS)) + prevAngle;
}

int16_t getAngle(void) {
  return currAngle;
}

/*  
  Initialize exponential smoothing filter with filter object, 
  exp smoothing param, auto calibration.
*/
void filterInit(Filter* f, float alpha, bool autocb) {  
  // intializes values to user specified values or default. 
  // call this function in setup()
  f->alpha = alpha;
  f->prev  = 0;
  f->value = 0;

  f->avg = 0;
  f->count = 1;
  f->autocb = autocb;
}

/*
  Call while running to get the updated filter output
  using current value as input.
*/
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

// NOTE: the heading angle is calculated using discrete integration
// NOTE: default arguments are provided in the header file
// NOTE: accelerometer has a set clock speed, don't want to sample too quickly
//       if it is tme to pull from accelerometer, overwrite values
//       else predict heading

