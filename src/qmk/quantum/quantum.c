/* Copyright 2016-2017 Jack Humbert
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

#include "quantum.h"

/* Core keycode function, hands off handling to other functions,
    then processes internal quantum keycodes, and then processes
    ACTIONs.                                                      */
bool process_record_quantum(keyrecord_t *record) {
    uint16_t keycode = keymap_key_to_keycode(record->event.key);

    if (process_record_via(keycode, record) == false) {
        return false;
    }
    
    return true;
}

// void usb_send_for_debug(uint8_t v1, uint8_t v2)
// {
//     report_keyboard_t report1;
//     report1.keys[0] = 0x1E + v1;
//     report1.keys[1] = v2;
//     host_keyboard_send(&report1);
//     report1.keys[0] = 0x00;
//     report1.keys[1] = 0x00;
//     host_keyboard_send(&report1);
// }