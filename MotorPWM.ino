#include "MotorPWM.h"
#include "Communication.ino"

//  PWM Implementation of Motor Control for ReadyToSky BLHELI32 ESC

//  Arduino Nano PWM Period is 500Hz

//  Full reverse for ESC is 1020us
//  Netral for ESC is 1500 us, 5% deadband
//  Full forward for ESC is 1980us


//  Safety function, functionally a killswitch
void killMotor(motor_t* motor) {
    


}

//  Maps from 960 to 256 for analogWrite
void mapPeriodTo256(motor_t* motor) {
    
}

void driveMotor(motor_t* motor) {

}