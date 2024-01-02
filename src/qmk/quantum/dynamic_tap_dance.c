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

#include "../platforms/progmem.h"
#include "quantum.h"
#include "via.h"
#include <stdint.h>

#ifdef TAP_DANCE_ENABLE

__data td_info_t td_info = { .id = TD_ID_NONE, .count = 0, .wait_release = false, .last_time = 0 };

uint16_t dynamic_get_tap_dance_keycode(uint8_t id, uint8_t index) {
    uint8_t address = DYNAMIC_TAP_DANCE_EEPROM_ADDR + (id * 2 * 3) + (index * 2);
    uint16_t keycode = (eeprom_read_byte(address) << 8) + (eeprom_read_byte(address + 1) & 0xFF);
    return keycode;
}

void dynamic_set_tap_dance_keycode(uint8_t id, uint8_t index, uint16_t keycode) {
    uint8_t address = DYNAMIC_TAP_DANCE_EEPROM_ADDR + (id * 2 * 3) + (index * 2);
    eeprom_write_byte(address, (uint8_t)(keycode >> 8));
    eeprom_write_byte(address + 1, (uint8_t)(keycode & 0xFF));
}

void dynamic_tap_dance_reset(void) {
    for (uint8_t i = 0; i < DYNAMIC_TAP_DANCE_EEPROM_SIZE; i++) {
        eeprom_write_byte(DYNAMIC_TAP_DANCE_EEPROM_ADDR + i, 0);
    }
}

void dynamic_tap_dance_key_event(uint8_t id, uint8_t action, bool pressed) {
    keyevent_t event_tmp;

    event_tmp.key.row = KEYLOC_TAP_DANCE_MIN + id;
    event_tmp.key.col = action - TD_TAP;
    event_tmp.pressed = pressed;
    event_tmp.time = timer_read();
    event_tmp.type = KEY_EVENT;
    action_exec(event_tmp);
}

void tap_dance_info_reset(void) {
    td_info.id = TD_ID_NONE;
    td_info.count = 0;
    td_info.wait_release = false;
}

void tap_dance_task(void) {
    if (td_info.id != TD_ID_NONE) {
        if (timer_elapsed(td_info.last_time) > TAPPING_TERM) {
            if (td_info.count == 0) {
                if (td_info.wait_release == false) {
                    dynamic_tap_dance_key_event(td_info.id, TD_HOLD, true);
                    td_info.wait_release = true;
                }
            } else {
                if (td_info.count <= 2) {
                    dynamic_tap_dance_key_event(td_info.id, td_info.count, true);
                    wait_ms(10);
                    dynamic_tap_dance_key_event(td_info.id, td_info.count, false);
                }
                tap_dance_info_reset();
            }
        }
    }
}

bool process_tap_dance(uint16_t keycode, keyrecord_t *record) {
    if (keycode >= KC_TD_0 && keycode <= KC_TD_4) {
        uint8_t id = keycode - KC_TD_0;
        if (td_info.wait_release == true) {
            dynamic_tap_dance_key_event(td_info.id, TD_HOLD, false);
            tap_dance_info_reset();
        } else {
            if (record->event.pressed) {
                td_info.id = id;
            } else {
                if (td_info.id == id) {
                    td_info.count++;
                }
            }
        } 
        td_info.last_time = timer_read();
        return false;
    }

    return true;
}

#endif
