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
#include "../../../keymap.h"
#include <stdint.h>

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

void dynamic_keymap_macro_reset(void) {
    for (uint8_t i = 0; i < DYNAMIC_KEYMAP_MACRO_EEPROM_SIZE; i++) {
        eeprom_write_byte(DYNAMIC_KEYMAP_MACRO_EEPROM_ADDR + i, 0);
    }
}

void dynamic_keymap_macro_send(uint8_t id) {
    // Check the last byte of the buffer.
    // If it's not zero, then we are in the middle
    // of buffer writing, possibly an aborted buffer
    // write. So do nothing.
    uint8_t p = DYNAMIC_KEYMAP_MACRO_EEPROM_ADDR + DYNAMIC_KEYMAP_MACRO_EEPROM_SIZE - 1;
    if (eeprom_read_byte(p) != 0) {
        return;
    }

    // Skip N null characters
    // p will then point to the Nth macro
    p         = DYNAMIC_KEYMAP_MACRO_EEPROM_ADDR;
    while (id > 0) {
        // If we are past the end of the buffer, then there is
        // no Nth macro in the buffer.
        if (p == (DYNAMIC_KEYMAP_MACRO_EEPROM_ADDR + DYNAMIC_KEYMAP_MACRO_EEPROM_SIZE)) {
            return;
        }
        if (eeprom_read_byte(p) == 0) {
            --id;
        }
        ++p;
    }

    // Send the macro string by making a temporary string.
    char data[8] = {0};
    // We already checked there was a null at the end of
    // the buffer, so this cannot go past the end
    while (1) {
        data[0] = eeprom_read_byte(p++);
        data[1] = 0;
        // Stop at the null terminator of this macro string
        if (data[0] == 0) {
            break;
        }
        if (data[0] == SS_QMK_PREFIX) {
            // Get the code
            data[1] = eeprom_read_byte(p++);
            // Unexpected null, abort.
            if (data[1] == 0) {
                return;
            }
            if (data[1] == SS_TAP_CODE || data[1] == SS_DOWN_CODE || data[1] == SS_UP_CODE) {
                // Get the keycode
                data[2] = eeprom_read_byte(p++);
                // Unexpected null, abort.
                if (data[2] == 0) {
                    return;
                }
                // Null terminate
                data[3] = 0;
            } else if (data[1] == SS_DELAY_CODE) {
                // Get the number and '|'
                // At most this is 4 digits plus '|'
                uint8_t i = 2;
                while (1) {
                    data[i] = eeprom_read_byte(p++);
                    // Unexpected null, abort
                    if (data[i] == 0) {
                        return;
                    }
                    // Found '|', send it
                    if (data[i] == '|') {
                        data[i + 1] = 0;
                        break;
                    }
                    // If haven't found '|' by i==6 then
                    // number too big, abort
                    if (i == 6) {
                        return;
                    }
                    ++i;
                }
            }
        }
        send_string_with_delay(data, DYNAMIC_KEYMAP_MACRO_DELAY);
    }
}
