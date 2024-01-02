
#pragma once

// clang-format off

#define KEY_MAPS {								\
    {{LWIN(KC_D)},}                             \
};												\

#define RGB_CONFIG { {                          \
    {0},                                        \
}, {                                            \
    { 0,  0 },                                  \
}};                                             \

#define RGB_MODE_RESET {                        \
    .mode = RGB_MATRIX_NONE,                    \
    .hsv.h = 10, .hsv.s = 255, .hsv.v = 100,    \
    .speed = 50,                                \
};                                              \

// clang-format on
