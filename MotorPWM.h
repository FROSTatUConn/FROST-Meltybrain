#ifndef Arduino_h
#include <Arduino.h>
#endif

#ifndef Motor_PWM_h
#define Motor_PWM_h

#define ANALOG_RESOLUTION 255

//  Header for PWM Implementation of Motor Control for ReadyToSky BLHELI32 ESC

//  Arduino Nano PWM Frequency is 500Hz

//  Full reverse for ESC is 1020us
//  Netral for ESC is 1500 us, 5% deadband
//  Full forward for ESC is 1980us

//  Alternatively, as inputs to analogWrite/PWM duty cycles:
//  51% is full reverse
//  75% is neutral
//  99% is full forward

//  Do not make direct calls to analogWrite()! Instead, use set_motor and drive_motor
//  as wrappers

typedef struct Motor {
    int duty_cycle = 75;  //  51-99
    int direction = 0;  //  1 for forward, -1 for reverse, 0 for neutral, 0 is default
    int pin;
} motor_t;

void stop_motors(motor_t* motor1, motor_t* motor2);
void set_motor(motor_t* motor, int speed_percentage, int direction);
void drive_motor(motor_t* motor);

#endif

