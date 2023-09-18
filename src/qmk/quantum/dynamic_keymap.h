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
#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "quantum.h"
#include "dynamic_macro.h"
#include "dynamic_tap_dance.h"
#ifdef ENCODER_ENABLE
#    include "encoder.h"
#endif

#ifdef TWO_LAYER_ENABLE
#define DYNAMIC_KEYMAP_LAYER_COUNT          2
#else
#define DYNAMIC_KEYMAP_LAYER_COUNT          1
#endif

#define TOTAL_EEPROM_BYTE_COUNT             128

#define DYNAMIC_KEYMAP_MACRO_COUNT          5
#define DYNAMIC_KEYMAP_EEPROM_START         (VIA_EEPROM_CONFIG_END)
#define DYNAMIC_KEYMAP_EEPROM_MAX_ADDR      (TOTAL_EEPROM_BYTE_COUNT - 1)
#define DYNAMIC_KEYMAP_EEPROM_ADDR          DYNAMIC_KEYMAP_EEPROM_START
#define DYNAMIC_KEYMAP_ENCODER_EEPROM_ADDR  (DYNAMIC_KEYMAP_EEPROM_ADDR + (DYNAMIC_KEYMAP_LAYER_COUNT * MATRIX_ROWS * MATRIX_COLS * 2))

#ifdef ENCODER_ENABLE
#   define DYNAMIC_TAP_DANCE_EEPROM_ADDR    (DYNAMIC_KEYMAP_ENCODER_EEPROM_ADDR + (DYNAMIC_KEYMAP_LAYER_COUNT * NUM_ENCODERS * 2 * 2))
#else
#   define DYNAMIC_TAP_DANCE_EEPROM_ADDR    DYNAMIC_KEYMAP_ENCODER_EEPROM_ADDR
#endif // ENCODER_ENABLE

#ifdef TAP_DANCE_ENABLE
#   define DYNAMIC_TAP_DANCE_EEPROM_SIZE    (TAP_DANCE_COUNT * 2 * 3)  
#   define DYNAMIC_KEYMAP_MACRO_EEPROM_ADDR (DYNAMIC_TAP_DANCE_EEPROM_ADDR + DYNAMIC_TAP_DANCE_EEPROM_SIZE)
#else
#   define DYNAMIC_KEYMAP_MACRO_EEPROM_ADDR DYNAMIC_TAP_DANCE_EEPROM_ADDR
#endif // TAP_DANCE_ENABLE

#define DYNAMIC_KEYMAP_MACRO_EEPROM_SIZE    (DYNAMIC_KEYMAP_EEPROM_MAX_ADDR - DYNAMIC_KEYMAP_MACRO_EEPROM_ADDR + 1)

uint16_t dynamic_keymap_get_keycode(uint8_t layer, uint8_t row, uint8_t column);
void     dynamic_keymap_set_keycode(uint8_t layer, uint8_t row, uint8_t column, uint16_t keycode);
#ifdef ENCODER_ENABLE
uint16_t dynamic_keymap_get_encoder(uint8_t layer, uint8_t encoder_id, bool clockwise);
void     dynamic_keymap_set_encoder(uint8_t layer, uint8_t encoder_id, bool clockwise, uint16_t keycode);
#endif // ENCODER_ENABLE
void dynamic_keymap_reset(void);
