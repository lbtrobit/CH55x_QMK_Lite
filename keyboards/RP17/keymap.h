
#pragma once

// clang-format off

#define KEY_MAPS {								\
    {KC_NUM, KC_PSLS,    KC_PAST,    KC_PMNS},	\
    {KC_P7,  KC_P8,      KC_P9,      KC_PPLS},	\
    {KC_P4,  KC_P5,      KC_P6,		KC_NO},		\
    {KC_P1,  KC_P2,      KC_P3,		KC_NO},		\
    {KC_P0,  KC_PDOT,    KC_PENT,	KC_NO},		\
};												\

#define ENCODER_MAP {							\
    ENCODER_CCW_CW(KC_MS_WH_UP, KC_MS_WH_DOWN)	\
};												\

#define RGB_CONFIG { {                          \
    {16,    15,     14,     13},                \
    {12,    11,     10,     9},                 \
    {8,     7,      6,      NO_LED},            \
    {5,     4,      3,      NO_LED},            \
    {2,     1,      0,      NO_LED},            \
}, {                                            \
    { 224,  56 }, { 150,  64 }, { 37,  64 },  { 150,  48 },     \
    {  75,  48 }, {  0,  48 },  { 150,  32 }, { 75,  32 },      \
    {  0,  32 },  {  224,  24 },{ 150,  16 },                   \
    {  75,  16 }, {  0,  16 },  { 224,  0 },                    \
    { 150,  0 },  {  75,  0 },  { 0,  0 },                      \
}, {                                                            \
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,          \
}};                                                             \

// clang-format on
