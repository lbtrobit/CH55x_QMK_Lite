/* Copyright 2017 Jason Williams (Wilba)
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
#include <stdint.h>
#include <ctype.h>

const __code uint16_t keymaps[MATRIX_ROWS][MATRIX_COLS] = KEY_MAPS
#ifdef ENCODER_ENABLE
const __code uint16_t encoder_map[NUM_ENCODERS][NUM_DIRECTIONS] = ENCODER_MAP
#endif // ENCODER_ENABLE

__xdata dynamic_macro_t macro = {MACRO_ID_NULL, 0, 0, 0};

uint16_t dynamic_keymap_get_keycode(uint8_t row, uint8_t column) {
    uint8_t address = DYNAMIC_KEYMAP_EEPROM_ADDR + (row * MATRIX_COLS * 2) + (column * 2);
    // Big endian, so we can read/write EEPROM directly from host if we want
    uint16_t keycode = (eeprom_read_byte(address) << 8) + (eeprom_read_byte(address + 1) & 0xFF);
    return keycode;
}

void dynamic_keymap_set_keycode(uint8_t row, uint8_t column, uint16_t keycode) {
    uint8_t address = DYNAMIC_KEYMAP_EEPROM_ADDR + (row * MATRIX_COLS * 2) + (column * 2);
    // Big endian, so we can read/write EEPROM directly from host if we want
    eeprom_write_byte(address, (uint8_t)(keycode >> 8));
    eeprom_write_byte(address + 1, (uint8_t)(keycode & 0xFF));
}

#ifdef ENCODER_ENABLE
uint16_t dynamic_keymap_get_encoder(uint8_t encoder_id, bool clockwise) {
    uint8_t address = DYNAMIC_KEYMAP_ENCODER_EEPROM_ADDR + (NUM_ENCODERS * 2 * 2) + (encoder_id * 2 * 2);
    // Big endian, so we can read/write EEPROM directly from host if we want
    uint16_t keycode = ((uint16_t)eeprom_read_byte(address + (clockwise ? 0 : 2))) << 8;
    keycode |= eeprom_read_byte(address + (clockwise ? 0 : 2) + 1);
    return keycode;
}

void dynamic_keymap_set_encoder(uint8_t encoder_id, bool clockwise, uint16_t keycode) {
    uint8_t address = DYNAMIC_KEYMAP_ENCODER_EEPROM_ADDR + (NUM_ENCODERS * 2 * 2) + (encoder_id * 2 * 2);
    // Big endian, so we can read/write EEPROM directly from host if we want
    eeprom_write_byte(address + (clockwise ? 0 : 2), (uint8_t)(keycode >> 8));
    eeprom_write_byte(address + (clockwise ? 0 : 2) + 1, (uint8_t)(keycode & 0xFF));
}
#endif // ENCODER_ENABLE

void dynamic_keymap_reset(void) {
    // Reset the keymaps in EEPROM to what is in flash.
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        for (uint8_t column = 0; column < MATRIX_COLS; column++) {
          dynamic_keymap_set_keycode(row, column, pgm_read_word(&keymaps[row][column]));
        }
    }
#ifdef ENCODER_ENABLE
    for (uint8_t encoder = 0; encoder < NUM_ENCODERS; encoder++) {
          dynamic_keymap_set_encoder(encoder, true, pgm_read_word(&encoder_map[encoder][true ? 0 : 1]));
          dynamic_keymap_set_encoder(encoder, false, pgm_read_word(&encoder_map[encoder][false ? 0 : 1]));
    }
#endif // ENCODER_ENABLE
}

void dynamic_macro_reset(void) {
    for (uint8_t i = 0; i < DYNAMIC_KEYMAP_MACRO_EEPROM_SIZE; i++) {
        eeprom_write_byte(DYNAMIC_KEYMAP_MACRO_EEPROM_ADDR + i, 0);
    }
    // reset macro loop count
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
    char data[9] = {0};

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
