/*
Copyright 2011,2012 Jun Wako <wakojun@gmail.com>

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

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "../../quantum/keycode.h"
#include "../../../../keyboards/config_common.h"

// clang-format off

/* HID report IDs */
enum hid_report_ids {
    REPORT_ID_KEYBOARD = 1,
    REPORT_ID_SYSTEM,
    REPORT_ID_CONSUMER,
    REPORT_ID_MOUSE,
};

/* Consumer Page (0x0C)
 *
 * See https://www.usb.org/sites/default/files/documents/hut1_12v2.pdf#page=75
 */
enum consumer_usages {
    // 15.5 Display Controls
    SNAPSHOT               = 0x065,
    BRIGHTNESS_UP          = 0x06F, // https://www.usb.org/sites/default/files/hutrr41_0.pdf
    BRIGHTNESS_DOWN        = 0x070,
    // 15.7 Transport Controls
    TRANSPORT_RECORD       = 0x0B2,
    TRANSPORT_FAST_FORWARD = 0x0B3,
    TRANSPORT_REWIND       = 0x0B4,
    TRANSPORT_NEXT_TRACK   = 0x0B5,
    TRANSPORT_PREV_TRACK   = 0x0B6,
    TRANSPORT_STOP         = 0x0B7,
    TRANSPORT_EJECT        = 0x0B8,
    TRANSPORT_RANDOM_PLAY  = 0x0B9,
    TRANSPORT_STOP_EJECT   = 0x0CC,
    TRANSPORT_PLAY_PAUSE   = 0x0CD,
    // 15.9.1 Audio Controls - Volume
    AUDIO_MUTE             = 0x0E2,
    AUDIO_VOL_UP           = 0x0E9,
    AUDIO_VOL_DOWN         = 0x0EA,
    // 15.15 Application Launch Buttons
    AL_CC_CONFIG           = 0x183,
    AL_EMAIL               = 0x18A,
    AL_CALCULATOR          = 0x192,
    AL_LOCAL_BROWSER       = 0x194,
    AL_LOCK                = 0x19E,
    AL_CONTROL_PANEL       = 0x19F,
    AL_ASSISTANT           = 0x1CB,
    AL_KEYBOARD_LAYOUT     = 0x1AE,
    // 15.16 Generic GUI Application Controls
    AC_NEW                 = 0x201,
    AC_OPEN                = 0x202,
    AC_CLOSE               = 0x203,
    AC_EXIT                = 0x204,
    AC_MAXIMIZE            = 0x205,
    AC_MINIMIZE            = 0x206,
    AC_SAVE                = 0x207,
    AC_PRINT               = 0x208,
    AC_PROPERTIES          = 0x209,
    AC_UNDO                = 0x21A,
    AC_COPY                = 0x21B,
    AC_CUT                 = 0x21C,
    AC_PASTE               = 0x21D,
    AC_SELECT_ALL          = 0x21E,
    AC_FIND                = 0x21F,
    AC_SEARCH              = 0x221,
    AC_HOME                = 0x223,
    AC_BACK                = 0x224,
    AC_FORWARD             = 0x225,
    AC_STOP                = 0x226,
    AC_REFRESH             = 0x227,
    AC_BOOKMARKS           = 0x22A,
    AC_MISSION_CONTROL     = 0x29F,
    AC_LAUNCHPAD           = 0x2A0
};

/* Generic Desktop Page (0x01)
 *
 * See https://www.usb.org/sites/default/files/documents/hut1_12v2.pdf#page=26
 */
enum desktop_usages {
    // 4.5.1 System Controls - Power Controls
    SYSTEM_POWER_DOWN             = 0x81,
    SYSTEM_SLEEP                  = 0x82,
    SYSTEM_WAKE_UP                = 0x83,
    SYSTEM_RESTART                = 0x8F,
    // 4.10 System Display Controls
    SYSTEM_DISPLAY_TOGGLE_INT_EXT = 0xB5
};

// clang-format on

#define KEYBOARD_REPORT_SIZE 9
#define KEYBOARD_REPORT_KEYS 6

#ifdef __cplusplus
extern "C" {
#endif

/*
 * keyboard report is 8-byte array retains state of 8 modifiers and 6 keys.
 *
 * byte |0       |1       |2       |3       |4       |5       |6       |7
 * -----+--------+--------+--------+--------+--------+--------+--------+--------
 * desc |mods    |reserved|keys[0] |keys[1] |keys[2] |keys[3] |keys[4] |keys[5]
 *
 * It is exended to 16 bytes to retain 120keys+8mods when NKRO mode.
 *
 * byte |0       |1       |2       |3       |4       |5       |6       |7        ... |15
 * -----+--------+--------+--------+--------+--------+--------+--------+--------     +--------
 * desc |mods    |bits[0] |bits[1] |bits[2] |bits[3] |bits[4] |bits[5] |bits[6]  ... |bit[14]
 *
 * mods retains state of 8 modifiers.
 *
 *  bit |0       |1       |2       |3       |4       |5       |6       |7
 * -----+--------+--------+--------+--------+--------+--------+--------+--------
 * desc |Lcontrol|Lshift  |Lalt    |Lgui    |Rcontrol|Rshift  |Ralt    |Rgui
 *
 */
typedef union {
    uint8_t raw[KEYBOARD_REPORT_SIZE];
    struct {
        uint8_t report_id;
        uint8_t mods;
        uint8_t reserved;
        uint8_t keys[KEYBOARD_REPORT_KEYS];
    };
} report_keyboard_t;

#ifdef EXTRAKEY_ENABLE
typedef struct {
    uint8_t  report_id;
    uint16_t usage;
} report_extra_t;

typedef struct {
    uint8_t report_id;
    uint8_t buttons;
    int8_t x;
    int8_t y;
    int8_t v;
} report_mouse_t;

uint16_t report_keycode_to_system(uint8_t key);
uint16_t report_keycode_to_consumer(uint8_t key);
#endif

bool is_key_pressed(report_keyboard_t* keyboard_report, uint8_t key);
void add_key_to_report(report_keyboard_t* keyboard_report, uint8_t key);
void del_key_from_report(report_keyboard_t* keyboard_report, uint8_t key);

void add_mousekey_to_report(report_mouse_t* mouse_report, uint8_t key);
void del_mousekey_from_report(report_mouse_t* mouse_report, uint8_t key);

#ifdef __cplusplus
}
#endif
