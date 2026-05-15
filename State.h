#ifndef Tank_h
#include "Tank.h"
#endif

#ifndef Communication_h
#include "Communication.h"
#endif

#ifndef State_h
#define State_h

//  Definitions and Prototypes for Brainfreeze FSM

#define STATUS_LED_PIN 80085

typedef enum {
    BOOT = 0x00000, IDLE = 0x00001, TANK = 0x00010, SPIN = 0x00100, FAILSAFE = 0x01000, CALIBRATION = 0x10000
} state_t;

typedef struct state_manager {
    //  TODO: Figure out how to sample battery voltage!
    float bat_voltage;
    state_t prev_state;
    state_t (*current_state)(state_manager_t* state_manager);
    int tangential_accel;
    communication_data_t* comm_data;
    int safety = channel_data(comm_data)[SAFETY];
} state_manager_t;

state_t current_state(state_manager_t* state_manager);
void blink_led(unsigned int blink_len_ms, unsigned int delay_ms, unsigned int num_blinks);



#endif