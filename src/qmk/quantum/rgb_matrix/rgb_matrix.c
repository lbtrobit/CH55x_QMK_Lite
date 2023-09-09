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

#include "rgb_matrix.h"
#include "../../platforms/progmem.h"
#include "rgb_matrix_animations.h"
#include <stdint.h>
#include <string.h>

#ifdef RGB_MATRIX_ENABLE

#ifdef RGB_EFFECTS_PLUS
# ifndef RGB_MATRIX_CENTER
const led_point_t k_rgb_matrix_center = {112, 32};
# else
const led_point_t k_rgb_matrix_center = RGB_MATRIX_CENTER;
# endif
const led_config_t g_led_config = RGB_CONFIG
#endif // RGB_EFFECTS_PLUS

__idata rgb_config_t rgb_matrix_config;
__idata uint32_t g_rgb_timer;

// internals
static __data uint32_t          rgb_timer_buffer;
static __data uint8_t           rgb_last_effect   = UINT8_MAX;
static __data effect_params_t   rgb_effect_params = {false};
static __data rgb_task_states   rgb_task_state = SYNCING;


static void rgb_task_start(void) {
    // update double buffers
    g_rgb_timer = rgb_timer_buffer;
    // next task
    rgb_task_state = RENDERING;
}

static void rgb_task_render(uint8_t effect) {
    bool rendering         = false;

    rgb_effect_params.init = (effect != rgb_last_effect);

    // each effect can opt to do calculations
    // and/or request PWM buffer updates.
    switch (effect) {
        case RGB_MATRIX_NONE:
            rendering = rgb_matrix_none(&rgb_effect_params);
            break;

        case RGB_MATRIX_SOLID_RGB:
            rendering = rgb_matrix_SOLID_RGB();
            break;

        case RGB_MATRIX_SIGNAL_RGB:
            rendering = rgb_matrix_SIGNAL_RGB();
            break;

        default:
 #ifdef RGB_EFFECTS_PLUS
            if (effect >= RGB_MATRIX_CYCLE_ALL && effect < RGB_MATRIX_EFFECT_MAX) {
                rendering = rgb_matrix_effect_plus();
            }
#endif // RGB_EFFECTS_PLUS
            break;
    }

    // next task
    if (!rendering) {
        rgb_task_state = FLUSHING;
        if (!rgb_effect_params.init && effect == RGB_MATRIX_NONE) {
            // We only need to flush once if we are RGB_MATRIX_NONE
            rgb_task_state = SYNCING;
        }
    }
}

void rgb_matrix_task(void) {
    rgb_timer_buffer = timer_read32();

    switch (rgb_task_state) {
        case STARTING:
            rgb_task_start();
            break;
        case RENDERING:
            rgb_task_render(rgb_matrix_config.mode);
            break;
        case FLUSHING:
            // update last trackers after the first full render so we can init over several frames
            rgb_last_effect = rgb_matrix_config.mode;
            // update pwm buffers
            ws2812_flush();
            // next task
            rgb_task_state = SYNCING;
            break;
        case SYNCING:
            if (timer_elapsed32(g_rgb_timer) >= 20) rgb_task_state = STARTING;
            break;
    }
}

void rgb_matrix_effects_init(void)
{
    rgb_effect_params.init = true;
}

void rgb_matrix_set_mode(__data uint8_t mode) {
    rgb_effect_params.init = true;
    rgb_matrix_config.mode = mode;
    eeprom_write_byte(RGB_MATRIX_EEPROM_ADDR_EFFECT, rgb_matrix_config.mode);
}

#ifdef RGB_EFFECTS_PLUS
void rgb_matrix_set_hs(__data uint8_t hue, __data uint8_t sat) {
    rgb_matrix_config.hsv.h  = hue;
    rgb_matrix_config.hsv.s  = sat;
    eeprom_write_byte(RGB_MATRIX_EEPROM_ADDR_HUE, rgb_matrix_config.hsv.h);
    eeprom_write_byte(RGB_MATRIX_EEPROM_ADDR_SAT, rgb_matrix_config.hsv.s);
}

void rgb_matrix_set_val(__data uint8_t val) {
    rgb_matrix_config.hsv.v  = val;
    eeprom_write_byte(RGB_MATRIX_EEPROM_ADDR_VAL, rgb_matrix_config.hsv.v);
}

void rgb_matrix_set_speed(__data uint8_t speed) {
    rgb_matrix_config.speed  = speed;
    eeprom_write_byte(RGB_MATRIX_EEPROM_ADDR_SPEED, rgb_matrix_config.speed);
}
#endif // RGB_EFFECTS_ENABLE

void rgb_matrix_reset(void)
{
    // reset eeprom rgb data
    eeprom_write_byte(RGB_MATRIX_EEPROM_ADDR_RED, 0x00);
    eeprom_write_byte(RGB_MATRIX_EEPROM_ADDR_GREEN, 0x80);
    eeprom_write_byte(RGB_MATRIX_EEPROM_ADDR_BLUE, 0x80);

#ifdef RGB_EFFECTS_PLUS
    rgb_matrix_set_hs(10, 255);
    rgb_matrix_set_val(150);
    rgb_matrix_set_speed(50);
    rgb_matrix_set_mode(RGB_MATRIX_CYCLE_ALL);
#else
    rgb_matrix_set_mode(RGB_MATRIX_SOLID_RGB);
#endif // RGB_EFFECTS_ENABLE
}

void rgb_matrix_init(void) {
    ws2812_init();

    rgb_matrix_config.mode   = eeprom_read_byte(RGB_MATRIX_EEPROM_ADDR_EFFECT);

#ifdef RGB_EFFECTS_PLUS
    rgb_matrix_config.hsv.h  = eeprom_read_byte(RGB_MATRIX_EEPROM_ADDR_HUE);
    rgb_matrix_config.hsv.s  = eeprom_read_byte(RGB_MATRIX_EEPROM_ADDR_SAT);
    rgb_matrix_config.hsv.v  = eeprom_read_byte(RGB_MATRIX_EEPROM_ADDR_VAL);
    rgb_matrix_config.speed  = eeprom_read_byte(RGB_MATRIX_EEPROM_ADDR_SPEED);
#endif // RGB_EFFECTS_PLUS
}

bool rgb_matrix_none(effect_params_t *params) {
    if (!params->init) {
        return false;
    }

    ws2812_set_color_all(0, 0, 0);
    return false;
}

bool rgb_matrix_SOLID_RGB(void)
{
    LED_TYPE color;
    color.r = eeprom_read_byte(RGB_MATRIX_EEPROM_ADDR_RED);
    color.g = eeprom_read_byte(RGB_MATRIX_EEPROM_ADDR_GREEN);
    color.b = eeprom_read_byte(RGB_MATRIX_EEPROM_ADDR_BLUE);
    ws2812_set_color_all(color.r, color.g, color.b);
    return false;
}

bool rgb_matrix_SIGNAL_RGB(void) {
    return false;
}

void rgb_matrix_signalrgb_set_leds(uint8_t *data) {
    uint8_t first_led_index = data[1];
    uint8_t leds_num        = data[2];

    if (leds_num >= 10) {
        data[1] = DEVICE_ERROR_LEDS;
        return;
    }
    uint8_t led_index   = first_led_index;
    uint8_t color_index = 3;
    for (uint8_t i = 0; i < leds_num; ++i, ++led_index, color_index += 3) {
        rgb_matrix_ws2812_array[led_index].r = data[color_index];
        rgb_matrix_ws2812_array[led_index].g = data[color_index + 1];
        rgb_matrix_ws2812_array[led_index].b = data[color_index + 2];
    }
}

#endif
