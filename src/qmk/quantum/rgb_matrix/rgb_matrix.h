/* Copyright 2017 Jason Williams
 * Copyright 2017 Jack Humbert
 * Copyright 2018 Yiancar
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
#include "rgb_matrix_types.h"
#include "../color.h"
#include "../quantum.h"

enum rgb_matrix_effects {
    RGB_MATRIX_NONE = 0,
    RGB_MATRIX_SOLID_RGB, // via set Solid RGB 
#ifdef RGB_EFFECTS_PLUS
    RGB_MATRIX_CYCLE_ALL,
    RGB_MATRIX_CYCLE_LEFT_RIGHT,
    RGB_MATRIX_CYCLE_UP_DOWN,
    RGB_MATRIX_RAINBOW_MOVING_CHEVRON,
    RGB_MATRIX_HUE_WAVE,
#endif
    RGB_MATRIX_SIGNAL_RGB,
    RGB_MATRIX_EFFECT_MAX
};

#ifdef RGB_MATRIX_ENABLE
//epprom地址的偏移
enum rgb_matrix_eeprom_offset {
    RGB_MATRIX_EEPROM_ADDR_RED = VIA_EEPROM_CUSTOM_RGB_MATRIX_ADDR,
    RGB_MATRIX_EEPROM_ADDR_GREEN,
    RGB_MATRIX_EEPROM_ADDR_BLUE,
    RGB_MATRIX_EEPROM_ADDR_EFFECT,
    RGB_MATRIX_EEPROM_ADDR_HUE,
    RGB_MATRIX_EEPROM_ADDR_SAT,
    RGB_MATRIX_EEPROM_ADDR_VAL,
    RGB_MATRIX_EEPROM_ADDR_SPEED,
};
#endif

bool rgb_matrix_none(effect_params_t *params);
bool rgb_matrix_SOLID_RGB(void);
bool rgb_matrix_SIGNAL_RGB(void);

void rgb_matrix_init(void);
void rgb_matrix_task(void);
void rgb_matrix_reset(void);
void rgb_matrix_signalrgb_set_leds(uint8_t *data);
void rgb_matrix_set_mode(__data uint8_t mode);
void rgb_matrix_set_mode_noeeprom(__data uint8_t mode);
void rgb_matrix_reload_mode(void);
void rgb_matrix_set_hs(__data uint8_t hue, __data uint8_t sat);
void rgb_matrix_set_val(__data uint8_t val);
void rgb_matrix_set_speed(__data uint8_t speed);

void ws2812_init(void);
void ws2812_set_color(__data uint8_t index, __data uint8_t red, __data uint8_t green, __data uint8_t blue);
void ws2812_set_color_all(__data uint8_t red, __data uint8_t green, __data uint8_t blue);
void ws2812_flush(void);

// LED color buffer
extern __xdata LED_TYPE rgb_matrix_ws2812_array[RGB_MATRIX_LED_COUNT];
extern const led_point_t k_rgb_matrix_center;
extern const led_config_t g_led_config;
extern __idata rgb_config_t rgb_matrix_config;
extern __idata uint32_t g_rgb_timer;

