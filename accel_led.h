#ifndef Arduino_h
#include <Arduino.h>
#endif

#ifndef TwoWire_h
#include <Wire.h>
#endif

#ifndef __sparkfun_lis331_h__
#include "SparkFun_LIS331.h"
#endif


#ifndef _ACCEL_LED_
#define _ACCEL_LED_

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


void filterInit(Filter* f, float alpha, bool autocb=false);
int filterUpdate(Filter* f, int16_t current);
void runAccelerometer(void);
int16_t getAngle(void);




#endif