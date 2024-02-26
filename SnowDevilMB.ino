#include <stdint.h>
#include <avr/wdt.h>
#include "accel_led.h"
#include "state_machine.h"

#define LED LED_BUILTIN

// IMPORTANT: MAKE SURE THERE IS ENOUGH DELAY BEFORE WATCHDOG STARTUP
// OTHERWISE, MICROCONTROLLER WILL RESET AND DAMAGE PROCESSOR

// wdt_enable(WDTO_4S) to start the watchdog timer 
// wdt_disable() to disable
// wdt_reset() to feed the watchdog timer 

state_t state = IDLE;


void setup() {

  // initialize the accelerometer

  // initialize the filter

  // initialize serial communication
  Serial.begin(9600);
}

void loop() {
}
