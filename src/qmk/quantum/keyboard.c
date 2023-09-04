/*
Copyright 2011, 2012, 2013 Jun Wako <wakojun@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdint.h>
#include "quantum.h"
#include "keyboard.h"
#include "matrix.h"
#include "../tmk_core/protocol/host.h"
#include "led.h"
#include "keycode.h"
#include "../platforms/timer.h"
#include "via.h"

#ifdef ENCODER_ENABLE
#    include "encoder.h"
#endif

/** \brief keyboard_init
 *
 * FIXME: needs doc
 */
void keyboard_init(void) {
    timer_init();
    matrix_init();
#ifdef LED_ENABLE
    led_init_ports();
#endif
    via_init();
#ifdef ENCODER_ENABLE
    encoder_init();
#endif   
}

/**
 * @brief Generates a tick event at a maximum rate of 1KHz that drives the
 * internal QMK state machine.
 */
static void generate_tick_event(void) {
    static __xdata uint16_t last_tick = 0;
    const uint16_t  now = timer_read();

    if (TIMER_DIFF_16(now, last_tick) != 0) {
        keyevent_t event_tmp = {.key = {.row = 0, .col = 0}, .pressed = false, .time = timer_read(), .type = TICK_EVENT};
        action_exec(event_tmp);
        last_tick = now;
    }
}

/**
 * @brief This task scans the keyboards matrix and processes any key presses
 * that occur.
 *
 * @return true Matrix did change
 * @return false Matrix didn't change
 */
static void matrix_task(void) {

    static __xdata matrix_row_t matrix_previous[MATRIX_ROWS];

    if(!matrix_scan()) {
        generate_tick_event();
        return;
    }

    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        const matrix_row_t current_row = matrix[row];
        const matrix_row_t row_changes = current_row ^ matrix_previous[row];

        if (!row_changes) {
            continue;
        }

        matrix_row_t col_mask = 1;
        keyevent_t event_tmp;
        for (uint8_t col = 0; col < MATRIX_COLS; col++, col_mask <<= 1) {
            if (row_changes & col_mask) {
                const bool key_pressed = current_row & col_mask;

                event_tmp.key.row = row;
                event_tmp.key.col = col;
                event_tmp.pressed = key_pressed;
                event_tmp.time = timer_read();
                event_tmp.type = KEY_EVENT;
                action_exec(event_tmp);
            }
        }

        matrix_previous[row] = current_row;
    }
}

/** \brief Main task that is repeatedly called as fast as possible. */
void keyboard_task(void) {

    matrix_task();

#ifdef ENCODER_ENABLE
    encoder_read();
#endif

#ifdef LED_ENABLE
    led_task();
#endif
}
