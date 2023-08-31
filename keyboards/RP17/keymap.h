
#pragma once

#include "src/CH55x_QMK_Lite.h"
#include "config.h"

extern const __code uint16_t keymaps[MATRIX_ROWS][MATRIX_COLS];

// clang-format off
#define LAYOUT_all(         \
	K00, K01, K02, K03,     \
	K10, K11, K12, K13,     \
	K20, K21, K22,          \
	K30, K31, K32,          \
	K40, K41, K42           \
)                           \
    {K00, K01, K02, K03},   \
	{K10, K11, K12, K13},   \
	{K20, K21, K22,KC_NO},  \
	{K30, K31, K32,KC_NO},  \
	{K40, K41, K42,KC_NO}   \
// clang-format on
