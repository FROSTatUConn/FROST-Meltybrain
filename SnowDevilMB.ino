#include <stdint.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include "state_machine.h"

#define LED LED_BUILTIN

// IMPORTANT: MAKE SURE THERE IS ENOUGH DELAY BEFORE WATCHDOG STARTUP
// OTHERWISE, MICROCONTROLLER WILL RESET AND DAMAGE PROCESSOR

state_t state = IDLE;

ISR(WDT_vect) {
  Serial.println("WDT countdown ended");
  while (1) 0;
}

void setup() {
  Serial.begin(9600);
  wdt_disable();
  delay(5000);
  pinMode(LED, OUTPUT);

  wdt_enable(WDTO_4S);
  //wdt_interrupt_enable();
  sei();
  Serial.println("WDT primed");
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED, HIGH);
  delay(100);
  digitalWrite(LED, LOW);
  delay(500);
  wdt_reset();
}
