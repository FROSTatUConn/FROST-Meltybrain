#include "SparkFun_LIS331.h"
#include <Wire.h>


#include <EEPROM.h>

int address = 0;


double RADIUS = 0.021;


// For old filter that will never be used again
// void funnyMeanFilter(int16_t x, int16_t y, int16_t z, double &sx, double &sy, double &sz);

LIS331 xl;

// int16_t curr_x[] = {0, 0, 0, 0, 0};
// int16_t curr_y[] = {0, 0, 0, 0, 0};
// int16_t curr_z[] = {0, 0, 0, 0, 0};

bool setUp = true;

double gPercent;
double ang_vel;
int rpm;
double rpm_d;
double w;

unsigned long t = 0;
unsigned long t1;
unsigned long t2;

long w1 = 0;
//long w2 = 0;

long theta = 0;


double velocity;
double angularVelocity;
double currAngle = 0;
  
double max[3] = {0, 0, 0};
double min[3] = {0, 0, 0};

double smooth[3] = {0, 0, 0};

// Simple Low Pass Filter to smooth out data
float LPF_Beta = 0.2; //0.025;
void lowPassFilter(int16_t x, int16_t y, int16_t z) {
  smooth[0] = smooth[0] - (LPF_Beta * (smooth[0] - x));
  smooth[1] = smooth[1] - (LPF_Beta * (smooth[1] - y));
  smooth[2] = smooth[2] - (LPF_Beta * (smooth[2] - z));
}

double adjust(int8_t index, bool setUp) {
  // This if statement is used to scale the values due to gravity by finding gravity
  if (setUp) {
    max[index] = max(max[index], smooth[index]*0.9);      // gets the current max value that index has felt
    min[index] = min(min[index], smooth[index]*0.9);      // gets the current max value that index has felt
  }
  
  // this line returns an adjusted version of the value to be centered and to 
  return 10.3005*(2*smooth[index]-max[index]-min[index]) / (max[index]-min[index]);   // 1.05 * g * (2*curr - mid point) / max value
  // (2*curr - mid point) / max value     This gets the data in -1<=x<=1 format
  // g is gravity (9.81 m/s/s)
  // 1.05 is included to make sure that it is going to 9.81 when facing upward
}

/* Temporary filter for basic testing
void funnyMeanFilter(int16_t x, int16_t y, int16_t z, double &sx, double &sy, double &sz) {
  // Very very very simple mean shift filter, it just has more bias towards the more recent values
  sx = 3*x;
  sy = 3*y;
  sz = 3*z;
  for (int i = 0; i<4; i++){
    curr_x[i] = curr_x[i+1];
    sx += curr_x[i+1]*(i/2+1);
    curr_y[i] = curr_y[i+1];
    sy += curr_y[i+1]*(i/2+1);
    curr_z[i] = curr_z[i+1];
    sz += curr_z[i+1]*(i/2+1);
  }
  curr_x[4] = x;
  sx /= 18;
  //sx = max(-9.81, min(sx*1.05, 9.81));
  curr_y[4] = y;
  sy /= 18;
  //sy = max(-9.81, min(sy*1.05, 9.81));
  curr_z[4] = z;
  sz /= 18;
  //sz = max(-9.81, min(sz*1.05, 9.81));
}
*/

void accelSetup() 
{
  // put your setup code here, to run once:
  pinMode(9,INPUT);       // Interrupt pin input
  Wire.begin();
  xl.setI2CAddr(0x19);    // This MUST be called BEFORE .begin() so 
                          //  .begin() can communicate with the chip
  xl.begin(LIS331::USE_I2C); // Selects the bus to be used and sets
                          //  the power up bit on the accelerometer.
                          //  Also zeroes out all accelerometer
                          //  registers that are user writable.


  // This next section configures an interrupt. It will cause pin
  //  INT1 on the accelerometer to go high when the absolute value
  //  of the reading on the Z-axis exceeds a certain level for a
  //  certain number of samples.
  xl.intSrcConfig(LIS331::INT_SRC, 1); // Select the source of the
                          //  signal which appears on pin INT1. In
                          //  this case, we want the corresponding
                          //  interrupt's status to appear. 
  xl.setIntDuration(50, 1); // Number of samples a value must meet
                          //  the interrupt condition before an
                          //  interrupt signal is issued. At the
                          //  default rate of 50Hz, this is one sec.
  xl.setIntThreshold(2, 1); // Threshold for an interrupt. This is
                          //  not actual counts, but rather, actual
                          //  counts divided by 16.
  xl.enableInterrupt(LIS331::Z_AXIS, LIS331::TRIG_ON_HIGH, 1, true);
                          // Enable the interrupt. Parameters indicate
                          //  which axis to sample, when to trigger
                          //  (in this case, when the absolute mag
                          //  of the signal exceeds the threshold),
                          //  which interrupt source we're configuring,
                          //  and whether to enable (true) or disable
                          //  (false) the interrupt.
  
  // set up Serial Library if this is main file
  // Serial.begin(115200);

  //xl.setFullScale(LIS331::HIGH_RANGE);
}

float loop_timer = 0;
float prev_loop_time = 0;
void accelLoop() 
{
  if (loop_timer >= 30000) setUp = false;
  int16_t x, y, z;
  //double smooth_x, smooth_y, smooth_z;
  if (millis() - loop_timer > 50)
  {
    //prev_loop_time = loop_timer;
    loop_timer = millis();
    xl.readAxes(x, y, z);  // The readAxes() function transfers the
                           //  current axis readings into the three
                           //  parameter variables passed to it.
  


    // Early Testing Stuffs that will NOT be used
    //smoothX = xl.convertToG(100,x);
    //funnyMeanFilter(x, y, z, smooth_x, smooth_y, smooth_z);  // very basic mean filter


    // Use the low pass filter on the raw data
    lowPassFilter(x, y, z);   // low pass filter, places outputs in smooth_


    // Scale and center the data to be in terms of m/s/s
    // Also include cutoff values to reduce random data
    double newZ = adjust(2, setUp);
    newZ = min(max(newZ, -9.81), 9.81);
    if (abs(newZ) < 2.75) newZ = 0;

    double newX = adjust(0, setUp);
    if (abs(newX) < 2.75) newX = 0;

    double newY = adjust(1, setUp); // acceleration (m/s/s)
    if (abs(newY) < 2.75) newY = 0;


    // Make it so that the angle is not recorded during set up
    if (!setUp) {

    

      // Experimental:
      // Tries to cancel out gravity when it is not facing up
      // This is to try and remove the slight acceleration picked up from gravity
      gPercent = 2*abs(acos(1-(abs(newZ)/9.81)))/PI;
      newX *= gPercent;
      newY *= gPercent;



      // Two ways of getting the ang_vel (idk if either work yet)
      ang_vel = sqrt(abs(newY) / RADIUS); // angular velocity (rad/s)
      //double ang_vel2 = abs(newX) * (millis()-t1);

      // RPM
      rpm = (int)(ang_vel / (TWO_PI)); // lol this actually rps
      rpm_d = ang_vel / TWO_PI * 60; // rpm but double


      // idk what I was on when I wrote this please shame me
      // its just trying to see if it is pos or neg lmao
      // int sign = 2*(newY/abs(newY) >= 0)-1;
      // int8_t sign = newY/abs(newY);

      // Properly gives the ang_vel a direction
      w = abs(ang_vel) * copysign(1.0, smooth[0]);





      // A Better Time
      t2 = t1;
      // Previous Time
      t1 = t;
      // Current Time
      t = millis();


      // Very simple way to get angle
      // Gets the current angle by multiplying the angular velocity by the time elapsed
      currAngle += (w * (t-t1)/1000) * (180 / PI);
      //currAngle = fmod(w*(t-t1)/1000+currAngle, 360);


      // Prediction angle algorithm used in Spencer's Hardware Blog
      // Predict what the current w is
      double w_predicted = w1 + (t-t1)/(t2-t1) * (w-w1);
      // Save the w as the "previous w"
      w1 = w;
      // Predict the current angle
      double theta_predicted = (w_predicted + w)/2 * (t-t2) + theta;
      // Save the angle
      theta = theta_predicted;


      if (loop_timer+address*1875/32 >= 60000 && address < EEPROM.length()/4) {
        EEPROM.put(address*4, float(w));
        address++;
        // address++;
        // if (address == EEPROM.length()) {
        //   address = 0;
        // }
      }
    }

    Serial.print("x:");
    Serial.print(x);
    Serial.print(",");
    Serial.print("newx:");
    Serial.print(newX);
    Serial.print(",");
    Serial.print("newy:");
    Serial.print(newY);
    Serial.print(",");
    Serial.print("newz:");
    Serial.print(newZ);
    Serial.print(",");
    //Serial.print("gPercent:");
    //Serial.print(gPercent);
    //Serial.print(",");
    Serial.print("ang_vel1:");
    Serial.print(ang_vel);
    Serial.print(",");
    //Serial.print("ang_vel2:");
    //Serial.print(ang_vel2);
    //Serial.print(",");
    // Serial.print("w:");
    //Serial.print(w);
    // Serial.print(",");
    Serial.print("angle:");
    //Serial.println(w*(t-t1));
    Serial.print(currAngle);
    //Serial.println(x);
    //Serial.println(y);
    //Serial.println(z);
    //Serial.println(xl.convertToG(100,x)); // The convertToG() function
    //Serial.println(xl.convertToG(100,y)); // accepts as parameters the
    //Serial.println(xl.convertToG(100,z)); // raw value and the current
    //Serial.println(" ");                // maximum g-rating.

    Serial.print(",rpm:");
    Serial.println(rpm_d);
  }
  else {
    //dt = micros() - loopTimer;
  }

  if (digitalRead(9) == HIGH)
  {
    Serial.println("Interrupt");
  }
}

