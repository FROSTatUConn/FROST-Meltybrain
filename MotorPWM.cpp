#include "MotorPWM.h"

//  PWM Implementation of Motor Control for ReadyToSky BLHELI32 ESC

//  Arduino Nano PWM Period is 500Hz

//  Full reverse for ESC is 1020us
//  Netral for ESC is 1500 us, 5% deadband
//  Full forward for ESC is 1980us

//  Period is ON time for PWM Signal between 1020us-1980us inclusive
void setMotor(motor_t* motor, int period) {
    if (period == NEUTRAL) {
        motor->direction = 0;
        motor->pwm_period = period;
        motor->speed_percentage = 0;
    } else if (period > NEUTRAL) {
        motor->pwm_period = period;
        motor->direction = 1;
        motor->speed_percentage = (100*(period - NEUTRAL)) / (MAX_FORWARD - NEUTRAL);
    } else if (period < NEUTRAL) {
        motor->pwm_period = period;
        motor->direction = -1;
        motor->speed_percentage = (100*(NEUTRAL - period)) / (NEUTRAL - MAX_BACKWARD);
    }
}


//  Safety function, functionally a killswitch
void stopMotors(motor_t* motor1, motor_t* motor2) {
    setMotor(motor1, NEUTRAL);
    // driveMotor
    setMotor(motor2, NEUTRAL);
    // driveMotor

}

//  Maps from 960 to 256 for analogWrite
int mapTimeToAnalogWrite(int percent_speed) {
    return;
}
