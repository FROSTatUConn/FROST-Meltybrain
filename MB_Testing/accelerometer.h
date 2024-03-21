#include <Wire.h>
#include "SparkFun_LI331.h"

#ifndef __accelerometer_h__
#define __accelerometer_h__ 

// parameters for communicating with accelerometer
#define ACC_ADDR 0x19
#define ACC_MODE LIS331::USE_I2C

// parameters for accelerometer heading control
// accelerometer measurement period
#define ACC_MEASUREMENT_PERIOD 10
// meltybrain minimum acceleration to engage spin mode
#define MB_MIN_ACC 400









#endif