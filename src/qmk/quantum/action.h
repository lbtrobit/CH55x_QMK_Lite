/*
Copyright 2012,2013 Jun Wako <wakojun@gmail.com>

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

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "../platforms/progmem.h"
#include "keyboard.h"
#include "keycode.h"
#include "action_code.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef TAP_CODE_DELAY
#    define TAP_CODE_DELAY 0
#endif
#ifndef TAP_HOLD_CAPS_DELAY
#    define TAP_HOLD_CAPS_DELAY 80
#endif

/* Key event container for recording */
typedef struct {
    keyevent_t event;
} keyrecord_t;

/* Execute action per keyevent */
void action_exec(keyevent_t event);

/* action for key */
action_t action_for_key(keypos_t key);

/* keyboard-specific key event (pre)processing */
bool process_record_quantum(keyrecord_t *record);

void process_action(keyrecord_t record, action_t action);
void register_code(uint8_t code);
void unregister_code(uint8_t code);
void tap_code(uint8_t code);

#ifdef __cplusplus
}
#endif
