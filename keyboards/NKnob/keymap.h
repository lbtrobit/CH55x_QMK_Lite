
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

// clang-format on
