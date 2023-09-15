
#include "../color.h"
#include "rgb_matrix.h"
#include "rgb_matrix_animations.h"

#ifdef RGB_MATRIX_ENABLE
#   ifdef RGB_EFFECTS_PLUS

static uint8_t scale8(uint8_t i, uint8_t scale)
{
    return (((uint16_t)i) * (1+(uint16_t)(scale))) >> 8;
}

static uint16_t scale16by8(uint16_t i, uint8_t scale)
{
    return (i * (1+((uint16_t)scale))) >> 8;
}

static int8_t abs8(int8_t i)
{
    if( i < 0) i = -i;
    return i;
}

static uint8_t qadd8(uint8_t i, uint8_t j)
{
    uint16_t t = i + j;
    if (t > 255) t = 255;
    return t;
}

bool rgb_matrix_effect_plus(void) {

    RGB rgb;
    __data HSV hsv;
    hsv.s = rgb_matrix_config.hsv.s;
    hsv.v = rgb_matrix_config.hsv.v;

    uint8_t time = scale16by8(g_rgb_timer, qadd8(rgb_matrix_config.speed >> 2, 1));
    for (uint8_t i = 0; i < RGB_MATRIX_LED_COUNT; i++) {
        switch (rgb_matrix_config.mode) {
            case RGB_MATRIX_SOLID_COLOR:
                hsv.h = rgb_matrix_config.hsv.h;
                break;
            case RGB_MATRIX_CYCLE_ALL:
                hsv.h = time;
                break;
            case RGB_MATRIX_CYCLE_LEFT_RIGHT:
                hsv.h = g_led_config.point[i].x - time;
                break;
            case RGB_MATRIX_CYCLE_UP_DOWN:
                hsv.h = g_led_config.point[i].y - time;
                break;
            case RGB_MATRIX_RAINBOW_MOVING_CHEVRON:
                hsv.h = rgb_matrix_config.hsv.h + abs8(g_led_config.point[i].y - k_rgb_matrix_center.y) + (g_led_config.point[i].x - time);
                break;
            case RGB_MATRIX_HUE_WAVE:
                hsv.h = rgb_matrix_config.hsv.h + scale8(abs8(g_led_config.point[i].x - time), 48);
                break;
        }
        rgb = hsv_to_rgb(hsv);
        ws2812_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    return false;
}

#   endif // RGB_EFFECTS_PLUS
#endif // RGB_MATRIX_ENABLE