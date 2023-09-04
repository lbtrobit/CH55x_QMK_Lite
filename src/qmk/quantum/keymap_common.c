/*
Copyright 2012-2017 Jun Wako <wakojun@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "keymap_common.h"
#include "../tmk_core/protocol/report.h"
#include "keycode.h"
#include "action.h"
#include "quantum.h"

// #include <inttypes.h>

/* converts key to action */
action_t action_for_key(keypos_t key) {
    // 16bit keycodes - important
    uint16_t keycode = keymap_key_to_keycode(key);
    action_t action;

    if (keycode >= KC_A && keycode <= KC_EXSEL || keycode >= KC_LEFT_CTRL && keycode <= KC_RIGHT_GUI) {
      action.code = ACTION_KEY(keycode);
    }
#ifdef EXTRAKEY_ENABLE
    else if (keycode >= KC_SYSTEM_POWER && keycode <= KC_SYSTEM_WAKE) {
      action.code = ACTION_USAGE_SYSTEM(report_keycode_to_system(keycode));
    } else if (keycode >= KC_AUDIO_MUTE && keycode <= KC_LAUNCHPAD) {
      action.code = ACTION_USAGE_CONSUMER(report_keycode_to_consumer(keycode));
    }
#endif
#ifdef MOUSE_ENABLE
    else if (keycode >= KC_MS_UP && keycode <= KC_MS_ACCEL2) {
      action.code = ACTION_MOUSEKEY(keycode);
    }
#endif // MOUSE_ENABLE
    else if (keycode == KC_TRANSPARENT) {
      action.code = ACTION_TRANSPARENT;
    } else if (keycode >= QK_MODS && keycode <= QK_MODS_MAX) {
      action.code = ACTION_MODS_KEY(QK_MODS_GET_MODS(keycode), QK_MODS_GET_BASIC_KEYCODE(keycode));
    } else {
      action.code = ACTION_NO;
    }

    return action;
}

// translates key to keycode
uint16_t keymap_key_to_keycode(keypos_t key) {
    if (key.row < MATRIX_ROWS && key.col < MATRIX_COLS) {
        return dynamic_keymap_get_keycode(key.row, key.col);
    }
#ifdef ENCODER_ENABLE
    else if (key.row == KEYLOC_ENCODER_CW && key.col < NUM_ENCODERS) {
        return dynamic_keymap_get_encoder(key.col, true);
    } else if (key.row == KEYLOC_ENCODER_CCW && key.col < NUM_ENCODERS) {
        return dynamic_keymap_get_encoder(key.col, false);
    }
#endif // ENCODER_ENABLE
    return KC_NO;
}
