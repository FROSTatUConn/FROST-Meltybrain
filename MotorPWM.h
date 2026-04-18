#ifndef Arduino_h
#include <Arduino.h>
#endif

#ifndef MOTOR_PWM
#define MOTOR_PWM

#define PERIOD_RANGE 960
#define PERIOD_RESOLUTION 256

typedef struct Motor {
    int speed_percentage = 0;  //  0-100
    int direction = 0;  //  1 for forward, -1 for reverse, 0 for off, 0 is default
    int pwm_period;  //  1020-1980us
} motor_t;

typedef enum {
    MAX_FORWARD=1980, MAX_BACKWARD=1020, NEUTRAL=1500
} std_max_period;

void stopMotors(motor_t* motor1, motor_t* motor2);
int mapTimeToAnalogWrite(int u_timing);
void setMotor(motor_t* motor, int period);

#endif

