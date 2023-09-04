/* Copyright 2021
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

/**
 * \file
 *
 * \defgroup send_string Send String API
 *
 * \brief These functions allow you to create macros by typing out sequences of keystrokes.
 * \{
 */

#include <stdint.h>

#include "../../platforms/progmem.h"
#include "send_string_keycodes.h"

// Look-Up Tables (LUTs) to convert ASCII character to keycode sequence.
extern const uint8_t ascii_to_shift_lut[16];
extern const uint8_t ascii_to_altgr_lut[16];
extern const uint8_t ascii_to_dead_lut[16];
extern const uint8_t ascii_to_keycode_lut[128];

// clang-format off
#define KCLUT_ENTRY(a, b, c, d, e, f, g, h) \
    ( ((a) ? 1 : 0) << 0 \
    | ((b) ? 1 : 0) << 1 \
    | ((c) ? 1 : 0) << 2 \
    | ((d) ? 1 : 0) << 3 \
    | ((e) ? 1 : 0) << 4 \
    | ((f) ? 1 : 0) << 5 \
    | ((g) ? 1 : 0) << 6 \
    | ((h) ? 1 : 0) << 7 )
// clang-format on

/**
 * \brief Type out an ASCII character.
 *
 * \param ascii_code The character to type.
 */
void send_char(char ascii_code);

/** \} */
