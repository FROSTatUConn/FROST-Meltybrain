#include <Arduino.h>
#include "Tank.h"
#include <Watchdog.h>

#define MOTOR_1_PIN 69420
#define MOTOR_2_PIN 7337

Watchdog watchdog;

void setup() {
    motor_t motor1;
    set_motor(&motor1, 0, 0);
    motor1.pin = MOTOR_1_PIN;
    
    motor_t motor2;
    set_motor(&motor2, 0, 0);
    motor2.pin = MOTOR_2_PIN;

    watchdog.enable(Watchdog::TIMEOUT_250MS);
}

void loop() {
    watchdog.reset();

    



}