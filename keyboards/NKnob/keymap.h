
#pragma once

// clang-format off

#define KEY_MAPS {								\
    {{KC_MUTE},}                                \
};												\

#define ENCODER_MAP {							    \
    {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},             \
};												    \

#define RGB_CONFIG { {                          \
    {2,    3},                                  \
    {1,    0},                                  \
}, {                                            \
    { 224,  64 }, { 0,  64 },                   \
    { 0,  0 }, { 224,  0 },                     \
}};                                             \

#define RGB_MODE_RESET {                        \
    .mode = RGB_MATRIX_NONE,                    \
    .rgb.r = 0, .rgb.g = 125, .rgb.b = 125,     \
};                                              \

// clang-format on
