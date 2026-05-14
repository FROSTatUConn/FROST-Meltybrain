#include "MotorPWM.h"

//  PWM Implementation of Motor Control for ReadyToSky BLHELI32 ESC

//  Arduino Nano PWM Period is 500Hz

//  Full reverse for ESC is 1020us
//  Netral for ESC is 1500 us, 5% deadband
//  Full forward for ESC is 1980us

//  Maps a given speed percentage and direction to appropriate 
//  duty cycle for ESC and updates motor struct
void set_motor(motor_t* motor, int spd_percent, int dir) {
    int duty_cycle = 75;
    if (dir == 0) {
        motor->duty_cycle = duty_cycle;
        motor->direction = 0;
    } else if (dir < 0) {
        //  Map percentage to the range betweeen 51% and 75%
        duty_cycle = (int)(75-((24.)*(((double)spd_percent)/100.)));
        motor->duty_cycle = duty_cycle;
    } else if (dir > 0) {
        //  Map percentage to the range between 75% and 99%
        duty_cycle = (int)((((24.)*(((double)spd_percent)/100.)))+75.);
        motor->duty_cycle = duty_cycle;
    }
}

//  Safety function, functionally a killswitch
void stop_motors(motor_t* motor1, motor_t* motor2) {
    //  TODO: Have a global flag that sets state to IDLE?
    set_motor(motor1, 0, 0);
    analogWrite(motor1->pin, motor1->duty_cycle);
    set_motor(motor2, 0, 0);
    analogWrite(motor2->pin, motor2->duty_cycle);
}

//  Safety check and calls analogWrite()
void drive_motor(motor_t* motor) {
    //  TODO: Add safety check to here, call stop_motors() if failed
    if (motor->direction != 0) {
        analogWrite(motor->pin, (int)((((double)motor->duty_cycle/100.))*ANALOG_RESOLUTION));
    } else {
        motor->duty_cycle = 75;
        analogWrite(motor->pin, motor->duty_cycle);
    }
}

