
#include "keymap.h"
#include "config.h"

const __code uint16_t keymaps[MATRIX_ROWS][MATRIX_COLS] = {
    LAYOUT_all(
                    KC_NUM, KC_PSLS,    KC_PAST,    KC_PMNS,
                    KC_P7,  KC_P8,      KC_P9,      KC_PPLS,
                    KC_P4,  KC_P5,      KC_P6,
                    KC_P1,  KC_P2,      KC_P3,
                    KC_P0,  KC_PDOT,    KC_PENT       )
};

#ifdef ENCODER_ENABLE
const __code uint16_t encoder_map[NUM_ENCODERS][NUM_DIRECTIONS] = {
    ENCODER_CCW_CW(KC_MS_WH_UP, KC_MS_WH_DOWN)
};
#endif