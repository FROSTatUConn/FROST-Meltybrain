#ifndef Tank_h
#include "Tank.h"
#endif

#ifndef Communication_h
#include "Communication.h"
#endif

#ifndef State_h
#define State_h

//  Definitions and Prototypes for Brainfreeze FSM

#define STATE_STATUS_LED_PIN 80085

typedef enum {
    BOOT = 0x00000, IDLE = 0x00001, TANK = 0x00010, SPIN = 0x00100, FAILSAFE = 0x01000, CALIBRATION = 0x10000
} state_t;

typedef struct led_manager {
    unsigned long blink_len_ms;
    unsigned long delay_ms;
    unsigned long blinks_remaining;
    unsigned long prev_blink_time_ms;
    unsigned long curr_blink_time_ms;
    unsigned long blink_buf[3] = {0, 0, 0};
    unsigned short led_state = LOW;
} led_manager_t;

typedef struct state_manager {
    //  TODO: Figure out how to sample battery voltage!
    float bat_voltage;
    state_t prev_state;
    state_t current_state;
    int tangential_accel;
    communication_data_t* comm_data;
    int safety = channel_data(comm_data)[COMM_SAFETY];
    unsigned long prev_time_ms;
    unsigned long curr_time_ms;
    led_manager_t* led_manager;
} state_manager_t;

int check_led_buffer(led_manager_t* led_manager);  //  Returns 1 if buffer is empty, returns 0 if it has non-zero contents
void current_state(state_manager_t* state_manager);
void update_time(state_manager_t* state_manager);
void update_led(led_manager_t* led_manager, unsigned long blink_len_ms = 0, unsigned long delay_ms = 0, unsigned int num_blinks = 0);



#endif