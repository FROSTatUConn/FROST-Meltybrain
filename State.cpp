#include "State.h"

void blink_led(unsigned int blink_len_ms, unsigned int delay_ms, unsigned int num_blinks) {
    for (int i = 0; i < num_blinks; i++) {
        digitalWrite(STATUS_LED_PIN, HIGH);
        delay(blink_len_ms);
        digitalWrite(STATUS_LED_PIN, LOW);
        delay(delay_ms);
    }
}

state_t current_state(state_manager_t* state_manager) {
    //  Start-up
    if (state_manager->prev_state == NULL) {
        state_manager->prev_state = BOOT;
        blink_led(500, 100, 5);
        return IDLE;
    }

    if (state_manager->prev_state == BOOT) {
        state_manager->prev_state = IDLE;
        //  TODO: Once battery voltage sampling is solved, add that to this condition
        if (state_manager->safety > 1750) {
            blink_led(200, 50, 2);
            return TANK;
        }
    }
}

