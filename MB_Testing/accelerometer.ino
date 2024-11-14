#include "accelerometer.h"
#include <Wire.h>
#include <SparkFun_LIS331.h>

LIS331 xl;
int16_t raw_xyz[3];
#define Z_ACCEL raw_xyz[2];

// accelerometer access times
unsigned long acc_access_times[2];
// 
unsigned long ptime = 0;
// what is this used for?
uint16_t robot_period[2];

// the angle using only acceleration
int16_t acc_angle = 0;
uint16_t prev_angle = 0;

void initAccelerometer(void) {
  Wire.begin();
  xl.setI2CAddr(ACC_ADDR);
  xl.begin(ACC_MODE);
}

void runAccelerometer(void) {
  // check the accelerometer periodically
  // if it is time to check the accelerometer
  // directly calculate the angle heading
  if (micros() - ptime > ACC_MEASUREMENT_PERIOD) {

    ptime = micros();
    xl.readAxes(raw_xyz[0], raw_xyz[1], raw_xyz[2]);

    acc_access_times[1] = acc_access_times[0];
    acc_access_times[0] = micros();
    // get acceleration data

    if (0) { // TODO: add error handling for if accelerometer not working 
      ptime -= ACC_MEASUREMENT_PERIOD;
    } else {

      // if the acceleration is too little, exit early
      if (raw_xyz[2] < MB_MIN_ACC) return;

      robot_period[1] = robot_period[0];
      robot_period[0] = raw_xyz[2];

      unsigned long dt = acc_access_times[0] - acc_access_times[1];
      prev_angle = (prev_angle + (dt / robot_period[0] + dt / robot_period[1]) / 2) % 360;
      acc_angle = prev_angle;
    }
  // otherwise, if it is not time to check the accelerometer
  // extrapolate the angle heading
  } else {
    unsigned long dt = micros();
    unsigned long period_predicted =  (robot_period[1] + (dt - acc_access_times[1]) * 
                                      (robot_period[0] - robot_period[1]) / 
                                      (acc_access_times[0] - acc_access_times[1]));
    dt -= acc_access_times[0];
    acc_angle = (prev_angle + (dt / period_predicted + dt / robot_period[0]) / 2) % 360;
  }
}

int16_t getAngle() {
  return acc_angle;
}
