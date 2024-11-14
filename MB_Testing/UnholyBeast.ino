// Bulk of code from SWallen Hardware

#include <IBusBM.h>
#include <stdint.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>

// Miscellaneous definitions

#define RECEIVER_VAL_CEILING 2000
#define DEADZONE_CONST 75
#define LED LED_BUILTIN

// Mode definitions

#define IDLE_MODE 0
#define TANK_MODE 1
#define SPIN_MODE 2

// Motor A connections
#define in1 11
#define in2 10
// Motor B connections
#define in3 6
#define in4 5

int val1;      // Channel 0 Right Stick - Left/Right (all values 1000->2000)
int val2;      // Channel 1 Right Stick - Up/Down
int val3;      // Channel 2 Left Stick - Up/Down (throttle for planes)
int val4;      // Channel 3 Left Stick - Left/Right
int val5 = 0;  // Unassigned channel
int val6 = 0;  // SWD Switch
int accel;
float leftWheelAccel;
float rightWheelAccel;

// The safety is on by default

byte channelAmount = 6;
int mode = IDLE_MODE;
IBusBM IBus;  // IBus object


void setup() {
  IBus.begin(Serial);    // iBUS object connected to serial0 RX pin
  Serial.begin(115200);  // set up Serial library at 115200 bps


  // Initialize an IBus object, with 6 expected channels.






  // Set all the motor control pins to outputs
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  // Turn off motors - Initial state
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);


  /* wdt_disable();
  delay(5000);
  pinMode(LED, OUTPUT); */

  //wdt_enable(WDTO_4S);
  //wdt_interrupt_enable();
  //sei();
  //Serial.println("WDT primed");
}


void loop() {

  // Serial.println("FEED ME, GARY!");

  val1 = IBus.readChannel(0) - 1000;  // get latest value for servo channel 1
  val2 = IBus.readChannel(1) - 1000;
  val3 = IBus.readChannel(2) - 1000;
  val4 = IBus.readChannel(3) - 1000;
  val5 = IBus.readChannel(4) - 1000;
  val6 = IBus.readChannel(5) - 1000;

  Serial.print("ch0:");
  Serial.println(val1);
  Serial.print("ch1:");
  Serial.println(val2);
  Serial.print("ch2:");
  Serial.println(val3);
  Serial.print("ch3:");
  Serial.println(val4);
  Serial.print("ch4:");
  Serial.println(val5);
  Serial.print("ch5:");
  Serial.println(val6);

  // if (val6 != 1000) {
  if (1) {
    // Sets mode to idle if the safety is on

    mode = IDLE_MODE;


    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
  } else {
    // If everything is in order, mode is set to TANK_MODE, which means that the safety is off

    mode = TANK_MODE;
    Serial.print("mode:");
    Serial.println(mode);
  }


  if (mode == TANK_MODE) {

    // Acceleration Calculations

    if (val4 > (500 + DEADZONE_CONST)) {
      // If the turn direction is to the right

      leftWheelAccel = ((((((float)val4) / 1000))) * 255);
      rightWheelAccel = ((((1.0 - (((float)val4) / 1000)))) * 255);

    } else if (val4 < (500 - DEADZONE_CONST)) {
      // If the turn direction is to the left

      leftWheelAccel = ((((((float)val4) / 1000))) * 255);
      rightWheelAccel = ((((1.0 - (((float)val4) / 1000)))) * 255);

    } else {
      // If there is no turn direction

      leftWheelAccel = 255;
      rightWheelAccel = 255;
    }

    if (val3 > 500 + DEADZONE_CONST) {
      // Forward Acceleration Control

      // Left Motor
      analogWrite(in1, LOW);
      analogWrite(in2, leftWheelAccel);

      // Right Motor

      analogWrite(in4, LOW);
      analogWrite(in3, rightWheelAccel);


    } else if (val3 < (500 - DEADZONE_CONST)) {
      // Backward Acceleration Control

      // Left Motor
      analogWrite(in2, LOW);
      analogWrite(in1, leftWheelAccel);

      // Right Motor
      analogWrite(in3, LOW);
      analogWrite(in4, rightWheelAccel);

    } else if (val3 < (500 + DEADZONE_CONST) && val3 > (500 - DEADZONE_CONST) && val4 > (500 + DEADZONE_CONST)) {
      // Spin in place right, calculate the proportion of the stick to the right of center

      leftWheelAccel = ((val4 - 500) / 500) * 255;
      rightWheelAccel = 0;

      analogWrite(in1, LOW);
      analogWrite(in2, leftWheelAccel);

    } else if (val3 < (500 + DEADZONE_CONST) && val3 > (500 - DEADZONE_CONST) && val4 < (500 - DEADZONE_CONST)) {
      // Spin in place to the left, calculate the proportion of the stick to the left of center

      leftWheelAccel = 0;
      rightWheelAccel = (((500 - val4) / 500) * 255);

      analogWrite(in4, LOW);
      analogWrite(in2, leftWheelAccel);
    }
  }
}


/* Serial.print("Left_motor:");
    Serial.println(leftWheelAccel);

    Serial.print("Right_motor:");
    Serial.println(rightWheelAccel); */
