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
#ifdef ENCODER_ENABLE
#    include "encoder.h"
#endif

#define TOTAL_EEPROM_BYTE_COUNT             128

#define DYNAMIC_KEYMAP_MACRO_COUNT          5
#define DYNAMIC_KEYMAP_EEPROM_START         (VIA_EEPROM_CONFIG_END)
#define DYNAMIC_KEYMAP_EEPROM_MAX_ADDR      (TOTAL_EEPROM_BYTE_COUNT - 1)
#define DYNAMIC_KEYMAP_EEPROM_ADDR          DYNAMIC_KEYMAP_EEPROM_START
#define DYNAMIC_KEYMAP_ENCODER_EEPROM_ADDR  (DYNAMIC_KEYMAP_EEPROM_ADDR + (MATRIX_ROWS * MATRIX_COLS * 2))

#ifdef ENCODER_ENABLE
#   define DYNAMIC_KEYMAP_MACRO_EEPROM_ADDR (DYNAMIC_KEYMAP_ENCODER_EEPROM_ADDR + (NUM_ENCODERS * 2 * 2))
#else
#   define DYNAMIC_KEYMAP_MACRO_EEPROM_ADDR DYNAMIC_KEYMAP_ENCODER_EEPROM_ADDR
#endif // ENCODER_ENABLE

#define DYNAMIC_KEYMAP_MACRO_EEPROM_SIZE    (DYNAMIC_KEYMAP_EEPROM_MAX_ADDR - DYNAMIC_KEYMAP_MACRO_EEPROM_ADDR + 1)
#define DYNAMIC_KEYMAP_MACRO_DELAY          TAP_CODE_DELAY

#define MACRO_ID_NULL  0xFF

typedef struct
{
    uint8_t macro_id;   // Current macro ID
    uint8_t dataPtr;    // Current position in the macro data
    uint8_t startPtr;   // Start position in the macro data
    uint8_t loopCount;  // Number of times to loop
    uint16_t delayms;   // delay code
} dynamic_macro_t;

typedef enum{
    MACRO_STATE_ABORTED,
    MACRO_STATE_RUNNING,
    MACRO_STATE_DONE
} macro_state_e;

uint16_t dynamic_keymap_get_keycode(uint8_t row, uint8_t column);
void     dynamic_keymap_set_keycode(uint8_t row, uint8_t column, uint16_t keycode);
#ifdef ENCODER_ENABLE
uint16_t dynamic_keymap_get_encoder(uint8_t encoder_id, bool clockwise);
void     dynamic_keymap_set_encoder(uint8_t encoder_id, bool clockwise, uint16_t keycode);
#endif // ENCODER_ENABLE
void dynamic_keymap_reset(void);
void dynamic_macro_reset(void);
void dynamic_macro_pressed(uint8_t id);
void dynamic_macro_task(void);
