/* Copyright 2023 lbtrobit
 * Copyright 2017 Jason Williams (Wilba)
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
#include "dynamic_keymap.h"
#include "via.h"
#include "dynamic_macro.h"
#include <ctype.h>

__idata dynamic_macro_t macro = {MACRO_ID_NULL, 0, 0, 0};

void dynamic_macro_reset(void) {
    for (uint8_t i = 0; i < DYNAMIC_KEYMAP_MACRO_EEPROM_SIZE; i++) {
        eeprom_write_byte(DYNAMIC_KEYMAP_MACRO_EEPROM_ADDR + i, 0);
    }
}

void dynamic_macro_loop_reset(void) {
    for (uint8_t i = 0; i < VIA_EEPROM_CUSTOM_MACRO_LOOP_SIZE; i++) {
        eeprom_write_byte(VIA_EEPROM_CUSTOM_MACRO_LOOP_ADDR + i, 0);
    }
}

void dynamic_macro_clear_keys(void) {
    clear_keys();
    send_keyboard_report();
#ifdef MOUSE_ENABLE
    clear_mouse();
    send_mouse_report();
#endif // MOUSE_ENABLE
}

#ifdef MOUSE_ENABLE
#   ifdef FCODE_TO_MOUSE_ENABLE
typedef struct FCodeToMouse {
    uint8_t FCode;
    uint8_t MouseCode;
} FCodeToMouse;

const FCodeToMouse FCodeToMousemap[] = {
    {KC_F13, KC_MS_BTN1},
    {KC_F14, KC_MS_BTN2},
    {KC_F15, KC_MS_BTN3},
    {KC_F16, KC_MS_UP},
    {KC_F17, KC_MS_DOWN},
    {KC_F18, KC_MS_LEFT},
    {KC_F19, KC_MS_RIGHT},
    {KC_F20, KC_MS_WH_UP},
    {KC_F21, KC_MS_WH_DOWN},
};

uint8_t fcode_to_mouse(uint8_t code) {
    for (uint8_t i = 0; i < sizeof(FCodeToMousemap) / sizeof(FCodeToMousemap[0]); i++) {
        if (FCodeToMousemap[i].FCode == code) {
            return FCodeToMousemap[i].MouseCode;
        }
    }
    return code;
}
#   endif // FCODE_TO_MOUSE_ENABLE
#endif // MOUSE_ENABLE

void dynamic_macro_pressed(uint8_t id) {

    // send null report to clear the keyboard state
    dynamic_macro_clear_keys();

    // If the macro is already running, then stop it.
    if (macro.macro_id == id) {
        macro.macro_id = MACRO_ID_NULL;
        return;
    }

    // Check the last byte of the buffer.
    // If it's not zero, then we are in the middle
    // of buffer writing, possibly an aborted buffer
    // write. So do nothing.
    macro.dataPtr = DYNAMIC_KEYMAP_MACRO_EEPROM_ADDR + DYNAMIC_KEYMAP_MACRO_EEPROM_SIZE - 1;
    if (eeprom_read_byte(macro.dataPtr) != 0) {
        return;
    }

    // start new macro
    macro.macro_id = id;
    macro.delayms = 0;
    macro.loopCount = eeprom_read_byte(VIA_EEPROM_CUSTOM_MACRO_LOOP_ADDR + macro.macro_id);

    // Skip N null characters
    // p will then point to the Nth macro
    macro.dataPtr = DYNAMIC_KEYMAP_MACRO_EEPROM_ADDR;
    while (id > 0) {
        // If we are past the end of the buffer, then there is
        // no Nth macro in the buffer.
        if (macro.dataPtr == (DYNAMIC_KEYMAP_MACRO_EEPROM_ADDR + DYNAMIC_KEYMAP_MACRO_EEPROM_SIZE)) {
            macro.macro_id = MACRO_ID_NULL;
            return;
        }
        if (eeprom_read_byte(macro.dataPtr) == 0) {
            --id;
        }
        ++macro.dataPtr;
    }
    macro.startPtr = macro.dataPtr;  
}

uint8_t dynamic_macro_send(void) {

    uint8_t macro_state = MACRO_STATE_ABORTED;

    // Send the macro string by making a temporary string.
    __data char data[9] = {0};

    data[0] = eeprom_read_byte(macro.dataPtr++);
    // Stop at the null terminator of this macro string
    if (data[0] == 0) {
        macro_state = MACRO_STATE_DONE;
        return macro_state;
    } else if (data[0] == SS_QMK_PREFIX) {
        // Get the code
        data[1] = eeprom_read_byte(macro.dataPtr++);
        // Unexpected null, abort.
        if (data[1] == 0) {
            return macro_state;
        } else if (data[1] == SS_TAP_CODE || data[1] == SS_DOWN_CODE || data[1] == SS_UP_CODE) {
            // Get the keycode
            data[2] = eeprom_read_byte(macro.dataPtr++);
            // Unexpected null, abort.
            if (data[2] == 0) {
                return macro_state;
            }
#ifdef MOUSE_ENABLE
#  ifdef FCODE_TO_MOUSE_ENABLE
            // Convert F13-F21 to mouse codes
            data[2] = fcode_to_mouse(data[2]);
#  endif // FCODE_TO_MOUSE_ENABLE
#endif // MOUSE_ENABLE
            switch (data[1]) {
                case SS_TAP_CODE:
                    // tap
                    tap_code(data[2]);
                    break;
                case SS_DOWN_CODE:
                    // down
                    register_code(data[2]);
                    break;
                case SS_UP_CODE:
                    // up
                    unregister_code(data[2]);
                    break;
            }
        } else if (data[1] == SS_DELAY_CODE) {
            // Get the number and '|'
            // At most this is 4 digits plus '|'
            uint8_t i = 2;
            while (1) {
                data[i] = eeprom_read_byte(macro.dataPtr++);
                // Unexpected null, abort
                if (data[i] == 0) {
                    return macro_state;
                }
                // Found '|', send it
                if (data[i] == '|') {
                    break;
                }
                // If haven't found '|' by i==7 then
                // number too big, abort
                if (i == 7) {
                    return macro_state;
                }
                ++i;
            }
            // delay
            uint16_t ms = 0;
            i = 2;
            uint8_t keycode = data[i];
            while (isdigit(keycode)) {
                ms = ms * 10 + (keycode - '0');
                keycode = data[++i];
            }
            macro.delayms = ms;
        }
    } else {
        send_char(data[0]);
    }

    macro_state = MACRO_STATE_RUNNING;
    return macro_state;
}

void dynamic_macro_task(void) {

    // If the macro is not running, then do nothing.
    if (macro.macro_id == MACRO_ID_NULL) {
        return;
    } 
    // If the macro delay is not zero, then decrement it.
    if (macro.delayms > 0) {
        --macro.delayms;
        return;
    }

    // If the macro delay is zero, then send the next character.
    uint8_t macro_state = dynamic_macro_send();
    // If the macro is done, then stop it.
    if (macro_state == MACRO_STATE_DONE) {
        // If the macro is looping and not infinite, then decrement the loop count.
        if (macro.loopCount > 0) {
            if (macro.loopCount != 255)
            {
                --macro.loopCount;
            }
            macro.dataPtr = macro.startPtr;
        } else if (macro.loopCount == 0) {
            macro.macro_id = MACRO_ID_NULL;
        }
    } else if (macro_state == MACRO_STATE_ABORTED) {
        // If the macro is aborted, then stop it.
        macro.macro_id = MACRO_ID_NULL;
        dynamic_macro_clear_keys();
    }
}
