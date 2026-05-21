#include "State.h"

//  TODO: Figure out if this function will be needed in practice
void update_time(state_manager_t* state_manager) {
    state_manager->prev_time_ms = state_manager->curr_time_ms;
    state_manager->curr_time_ms = millis();
}

int check_led_buffer(led_manager_t* led_manager) {

}

//  TODO: Decide if update_time() should be bundled into update_led() or not
void update_led(led_manager_t* led_manager, unsigned long blink_len_ms, unsigned long delay_ms, unsigned int num_blinks) {
    if (blink_len_ms || delay_ms || num_blinks == 0) {
        if (led_manager->led_state == HIGH) {
            if (led_manager->curr_blink_time_ms - led_manager->prev_blink_time_ms >= led_manager->blink_len_ms) {
                led_manager->led_state = LOW;
                digitalWrite(STATE_STATUS_LED_PIN, led_manager->led_state);
                //  TODO: Update led_manager struct with new time data and decrement number of blinks left
            }
        } else {

        }
    } else {
        //  TODO: Implement buffer for next LED state?
        //  Implement blocking delay if blink buffer is full and something needs to be added
        if (led_manager->blinks_remaining > 0) {

        }
    }
}

void current_state(state_manager_t* state_manager) {
    //  Start-up
    if (state_manager->prev_state == NULL) {
        state_manager->prev_state = BOOT;
        update_time(state_manager);
        update_led(state_manager->led_manager, 500, 100, 5);
        state_manager->current_state = IDLE;
        return;
    }

    update_time(state_manager);
    update_led(state_manager->led_manager);

    if (state_manager->prev_state == BOOT) {
        state_manager->prev_state = IDLE;
        //  TODO: Once battery voltage sampling is solved, add that to this condition
        if (state_manager->safety > 1750) {
            update_led(state_manager->led_manager, 200, 50, 2);
            state_manager->current_state = TANK;
        }
    }
}

