#include <IBusBM.h>

// Motor A connections
#define in1 11
#define in2 10
// Motor B connections
#define in3 6
#define in4 5

// Miscellaneous definitions

#define RECEIVER_VAL_CEILING 2000
#define DEADZONE_CONST 75
#define LED LED_BUILTIN

// Mode definitions

#define IDLE_MODE 0
#define TANK_MODE 1
#define SPIN_MODE 2

int mode = IDLE_MODE;

IBusBM IBus;

void setup() {
  IBus.begin(Serial);
  Serial.begin(115200);           // set up Serial library at 9600 bps

  // Set all the motor control pins to outputs
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  // Turns off all motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void loop() {


  if (getChannelData(5) != 1000) {
    mode = IDLE_MODE;
    // Next line will be changed to an elif statement considering if angular velocity is greater than an arbitrary value we set
  } else {
    mode = TANK_MODE;
  }

  if (mode == TANK_MODE) {
    tankMode();
  }
}


