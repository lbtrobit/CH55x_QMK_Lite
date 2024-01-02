
#pragma once

// clang-format off

#define KEY_MAPS {								\
    {{LCTL(KC_C), LCTL(KC_V)},}                 \
};												\

#define RGB_CONFIG { {                          \
    {0,    1},                                  \
}, {                                            \
    { 0,  0 }, { 224,  0 },                     \
}};                                             \

#define RGB_MODE_RESET {                        \
    .mode = RGB_MATRIX_CYCLE_LEFT_RIGHT,        \
    .hsv.h = 120, .hsv.s = 255, .hsv.v = 100,   \
    .speed = 50,                                \
};                                              \

// clang-format on
