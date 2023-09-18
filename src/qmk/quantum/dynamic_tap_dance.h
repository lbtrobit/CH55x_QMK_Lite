/* Copyright 2023 lbtrobit
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include "action.h"
#include "quantum.h"
#include <stdint.h>

#ifndef TAP_DANCE_COUNT
#    define TAP_DANCE_COUNT 5
#endif

#ifndef TAPPING_TERM
#   define TAPPING_TERM 200
#endif

#define TD_ID_NONE 0xFF

typedef enum {
    TD_NONE = 0,
    TD_TAP,
    TD_DOUBLE_TAP,
    TD_HOLD,
} td_state_t;

typedef struct {
    uint8_t id;
    uint8_t count;
    bool wait_release;
    uint16_t last_time;
} td_info_t;

uint16_t dynamic_get_tap_dance_keycode(uint8_t id, uint8_t index);
void dynamic_set_tap_dance_keycode(uint8_t id, uint8_t index, uint16_t keycode);
void dynamic_tap_dance_reset(void);
void dynamic_tap_dance_pressed(uint8_t id);
void tap_dance_task(void);
bool process_tap_dance(uint16_t keycode, keyrecord_t *record);
