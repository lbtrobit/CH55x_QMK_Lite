/* Copyright 2018 James Laird-Wah
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
#include <WS2812.h>
#include <Arduino.h>

#ifdef RGB_MATRIX_ENABLE

// LED color buffer
__xdata LED_TYPE rgb_matrix_ws2812_array[RGB_MATRIX_LED_COUNT];

void ws2812_init(void) {
    pinMode(WS2812_DI_PIN, OUTPUT);
    digitalWrite(WS2812_DI_PIN, LOW);
}

void ws2812_flush(void) {
#if WS2812_DI_PIN == 10
    neopixel_show_P1_0(rgb_matrix_ws2812_array, RGB_MATRIX_LED_COUNT * 3);
#elif WS2812_DI_PIN == 11
    neopixel_show_P1_1(rgb_matrix_ws2812_array, RGB_MATRIX_LED_COUNT * 3);
#elif WS2812_DI_PIN == 12
    neopixel_show_P1_2(rgb_matrix_ws2812_array, RGB_MATRIX_LED_COUNT * 3);
#elif WS2812_DI_PIN == 13
    neopixel_show_P1_3(rgb_matrix_ws2812_array, RGB_MATRIX_LED_COUNT * 3);
#elif WS2812_DI_PIN == 14
    neopixel_show_P1_4(rgb_matrix_ws2812_array, RGB_MATRIX_LED_COUNT * 3);
#elif WS2812_DI_PIN == 15
    neopixel_show_P1_5(rgb_matrix_ws2812_array, RGB_MATRIX_LED_COUNT * 3);
#elif WS2812_DI_PIN == 16
    neopixel_show_P1_6(rgb_matrix_ws2812_array, RGB_MATRIX_LED_COUNT * 3);
#elif WS2812_DI_PIN == 17
    neopixel_show_P1_7(rgb_matrix_ws2812_array, RGB_MATRIX_LED_COUNT * 3);
#elif WS2812_DI_PIN == 30
    neopixel_show_P3_0(rgb_matrix_ws2812_array, RGB_MATRIX_LED_COUNT * 3);
#elif WS2812_DI_PIN == 31
    neopixel_show_P3_1(rgb_matrix_ws2812_array, RGB_MATRIX_LED_COUNT * 3);
#elif WS2812_DI_PIN == 32
    neopixel_show_P3_2(rgb_matrix_ws2812_array, RGB_MATRIX_LED_COUNT * 3);
#elif WS2812_DI_PIN == 33
    neopixel_show_P3_3(rgb_matrix_ws2812_array, RGB_MATRIX_LED_COUNT * 3);
#elif WS2812_DI_PIN == 34
    neopixel_show_P3_4(rgb_matrix_ws2812_array, RGB_MATRIX_LED_COUNT * 3);
#elif WS2812_DI_PIN == 35
    neopixel_show_P3_5(rgb_matrix_ws2812_array, RGB_MATRIX_LED_COUNT * 3);
#elif WS2812_DI_PIN == 36
    neopixel_show_P3_6(rgb_matrix_ws2812_array, RGB_MATRIX_LED_COUNT * 3);
#elif WS2812_DI_PIN == 37
    neopixel_show_P3_7(rgb_matrix_ws2812_array, RGB_MATRIX_LED_COUNT * 3);
#endif
}

// Set an led in the buffer to a color
void ws2812_set_color(__data uint8_t i, __data uint8_t r, __data uint8_t g, __data uint8_t b) {
    rgb_matrix_ws2812_array[i].r = r;
    rgb_matrix_ws2812_array[i].g = g;
    rgb_matrix_ws2812_array[i].b = b;
}

void ws2812_set_color_all(uint8_t r, uint8_t g, uint8_t b) {
    for (uint8_t i = 0; i < RGB_MATRIX_LED_COUNT; i++) {
        ws2812_set_color(i, r, g, b);
    }
}

#endif
