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

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "../color.h"

#ifndef RGB_MATRIX_ROWS
#define RGB_MATRIX_ROWS MATRIX_ROWS
#endif
#ifndef RGB_MATRIX_COLS
#define RGB_MATRIX_COLS MATRIX_COLS
#endif

typedef enum rgb_task_states { 
    STARTING, 
    RENDERING, 
    FLUSHING, 
    SYNCING 
} rgb_task_states;

typedef struct {
    bool    init;
} effect_params_t;

typedef struct {
    uint8_t x;
    uint8_t y;
} led_point_t;

#define HAS_FLAGS(bits, flags) ((bits & flags) == flags)
#define HAS_ANY_FLAGS(bits, flags) ((bits & flags) != 0x00)

#define LED_FLAG_ALL 0xFF
#define LED_FLAG_NONE 0x00
#define LED_FLAG_MODIFIER 0x01
#define LED_FLAG_KEYLIGHT 0x04
#define LED_FLAG_INDICATOR 0x08

#define NO_LED 255

typedef struct {
    uint8_t     matrix_co[RGB_MATRIX_ROWS][RGB_MATRIX_COLS];
    led_point_t point[RGB_MATRIX_LED_COUNT];
} led_config_t;

typedef union {
    uint64_t raw;
    struct {
        uint8_t     mode;
        HSV         hsv;
        uint8_t     speed;
    };
} rgb_config_t;
