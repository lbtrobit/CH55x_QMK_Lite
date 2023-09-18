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
#include <stdint.h>
#include <ctype.h>

const __code uint16_t keymaps[DYNAMIC_KEYMAP_LAYER_COUNT][MATRIX_ROWS][MATRIX_COLS] = KEY_MAPS
#ifdef ENCODER_ENABLE
const __code uint16_t encoder_map[DYNAMIC_KEYMAP_LAYER_COUNT][NUM_ENCODERS][NUM_DIRECTIONS] = ENCODER_MAP
#endif // ENCODER_ENABLE

uint16_t dynamic_keymap_get_keycode(uint8_t layer, uint8_t row, uint8_t column) {
    uint8_t address = DYNAMIC_KEYMAP_EEPROM_ADDR + (layer * MATRIX_ROWS * MATRIX_COLS * 2) + (row * MATRIX_COLS * 2) + (column * 2);
    // Big endian, so we can read/write EEPROM directly from host if we want
    uint16_t keycode = (eeprom_read_byte(address) << 8) + (eeprom_read_byte(address + 1) & 0xFF);
    return keycode;
}

void dynamic_keymap_set_keycode(uint8_t layer, uint8_t row, uint8_t column, uint16_t keycode) {
    uint8_t address = DYNAMIC_KEYMAP_EEPROM_ADDR + (layer * MATRIX_ROWS * MATRIX_COLS * 2) + (row * MATRIX_COLS * 2) + (column * 2);
    // Big endian, so we can read/write EEPROM directly from host if we want
    eeprom_write_byte(address, (uint8_t)(keycode >> 8));
    eeprom_write_byte(address + 1, (uint8_t)(keycode & 0xFF));
}

#ifdef ENCODER_ENABLE
uint16_t dynamic_keymap_get_encoder(uint8_t layer, uint8_t encoder_id, bool clockwise) {
    uint8_t address = DYNAMIC_KEYMAP_ENCODER_EEPROM_ADDR + (layer * NUM_ENCODERS * 2 * 2) + (encoder_id * 2 * 2);
    // Big endian, so we can read/write EEPROM directly from host if we want
    uint16_t keycode = ((uint16_t)eeprom_read_byte(address + (clockwise ? 0 : 2))) << 8;
    keycode |= eeprom_read_byte(address + (clockwise ? 0 : 2) + 1);
    return keycode;
}

void dynamic_keymap_set_encoder(uint8_t layer, uint8_t encoder_id, bool clockwise, uint16_t keycode) {
    uint8_t address = DYNAMIC_KEYMAP_ENCODER_EEPROM_ADDR + (layer * NUM_ENCODERS * 2 * 2) + (encoder_id * 2 * 2);
    // Big endian, so we can read/write EEPROM directly from host if we want
    eeprom_write_byte(address + (clockwise ? 0 : 2), (uint8_t)(keycode >> 8));
    eeprom_write_byte(address + (clockwise ? 0 : 2) + 1, (uint8_t)(keycode & 0xFF));
}
#endif // ENCODER_ENABLE

void dynamic_keymap_reset(void) {
    // Reset the keymaps in EEPROM to what is in flash.
    for (uint8_t layer = 0; layer < DYNAMIC_KEYMAP_LAYER_COUNT; layer++) {
        for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
            for (uint8_t column = 0; column < MATRIX_COLS; column++) {
                dynamic_keymap_set_keycode(layer, row, column, pgm_read_word(&keymaps[layer][row][column]));
            }
        }
    }
#ifdef ENCODER_ENABLE
    for (uint8_t layer = 0; layer < DYNAMIC_KEYMAP_LAYER_COUNT; layer++) {
        for (uint8_t encoder = 0; encoder < NUM_ENCODERS; encoder++) {
            dynamic_keymap_set_encoder(layer, encoder, true, pgm_read_word(&encoder_map[layer][encoder][true ? 0 : 1]));
            dynamic_keymap_set_encoder(layer, encoder, false, pgm_read_word(&encoder_map[layer][encoder][false ? 0 : 1]));
        }
    }
#endif // ENCODER_ENABLE
}
