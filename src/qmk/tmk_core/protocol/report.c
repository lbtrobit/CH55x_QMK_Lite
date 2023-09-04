/* Copyright 2017 Fred Sundvik
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */



#include <string.h>
#include "report.h"
#include "host.h"

/** \brief Checks if a key is pressed in the report
 *
 * Returns true if the keyboard_report reports that the key is pressed, otherwise false
 * Note: The function doesn't support modifers currently, and it returns false for KC_NO
 */
bool is_key_pressed(report_keyboard_t* keyboard_report, uint8_t key) {
    for (uint8_t i = 0; i < KEYBOARD_REPORT_KEYS; i++) {
        if (keyboard_report->keys[i] == key) {
            return true;
        }
    }
    return false;
}

/** \brief add key to report
 *
 * FIXME: Needs doc
 */
void add_key_to_report(report_keyboard_t* keyboard_report, uint8_t key) {
    for (uint8_t i = 0; i < KEYBOARD_REPORT_KEYS; i++) {
        if (keyboard_report->keys[i] == key) {
            return;
        }
        if (keyboard_report->keys[i] == 0) {
            keyboard_report->keys[i] = key;
            return;
        }
    }
}

/** \brief del key from report
 *
 * FIXME: Needs doc
 */
void del_key_from_report(report_keyboard_t* keyboard_report, uint8_t key) {
    for (uint8_t i = 0; i < KEYBOARD_REPORT_KEYS; i++) {
        if (keyboard_report->keys[i] == key) {
            keyboard_report->keys[i] = 0;
        }
    }
}

#ifdef EXTRAKEY_ENABLE
/* keycode to system usage */
uint16_t report_keycode_to_system(uint8_t key) {
    switch (key) {
        case KC_SYSTEM_POWER:
            return SYSTEM_POWER_DOWN;
        case KC_SYSTEM_SLEEP:
            return SYSTEM_SLEEP;
        case KC_SYSTEM_WAKE:
            return SYSTEM_WAKE_UP;
        default:
            return 0;
    }
}

typedef struct KeycodeToConsumer {
    uint8_t keycode;
    uint16_t consumer;
} KeycodeToConsumer_t;

const KeycodeToConsumer_t usageKeymap[] = {
    {KC_AUDIO_MUTE, AUDIO_MUTE},
    {KC_AUDIO_VOL_UP, AUDIO_VOL_UP},
    {KC_AUDIO_VOL_DOWN, AUDIO_VOL_DOWN},
    {KC_MEDIA_NEXT_TRACK, TRANSPORT_NEXT_TRACK},
    {KC_MEDIA_PREV_TRACK, TRANSPORT_PREV_TRACK},
    {KC_MEDIA_FAST_FORWARD, TRANSPORT_FAST_FORWARD},
    {KC_MEDIA_REWIND, TRANSPORT_REWIND},
    {KC_MEDIA_STOP, TRANSPORT_STOP},
    {KC_MEDIA_EJECT, TRANSPORT_STOP_EJECT},
    {KC_MEDIA_PLAY_PAUSE, TRANSPORT_PLAY_PAUSE},
    {KC_MEDIA_SELECT, AL_CC_CONFIG},
    {KC_MAIL, AL_EMAIL},
    {KC_CALCULATOR, AL_CALCULATOR},
    {KC_MY_COMPUTER, AL_LOCAL_BROWSER},
    {KC_CONTROL_PANEL, AL_CONTROL_PANEL},
    {KC_ASSISTANT, AL_ASSISTANT},
    {KC_WWW_SEARCH, AC_SEARCH},
    {KC_WWW_HOME, AC_HOME},
    {KC_WWW_BACK, AC_BACK},
    {KC_WWW_FORWARD, AC_FORWARD},
    {KC_WWW_STOP, AC_STOP},
    {KC_WWW_REFRESH, AC_REFRESH},
    {KC_BRIGHTNESS_UP, BRIGHTNESS_UP},
    {KC_BRIGHTNESS_DOWN, BRIGHTNESS_DOWN},
    {KC_WWW_FAVORITES, AC_BOOKMARKS},
    {KC_MISSION_CONTROL, AC_MISSION_CONTROL},
    {KC_LAUNCHPAD, AC_LAUNCHPAD},
};

uint16_t report_keycode_to_consumer(uint8_t key) {
    for (uint8_t i = 0; i < sizeof(usageKeymap) / sizeof(KeycodeToConsumer_t); i++) {
        if (key == usageKeymap[i].keycode) {
            return usageKeymap[i].consumer;
        }
    }
    return 0;
}
#endif

#ifdef MOUSE_ENABLE
void add_mousekey_to_report(report_mouse_t* mouse_report, uint8_t code) {
    if (code == KC_MS_UP) {
        mouse_report->y = -1;
    } else if (code == KC_MS_DOWN) {
        mouse_report->y = 1;
    } else if (code == KC_MS_LEFT) {
        mouse_report->x = -1;
    } else if (code == KC_MS_RIGHT) {
        mouse_report->x = 1;
    } else if (code == KC_MS_WH_UP) {
        mouse_report->v = 1;
    } else if (code == KC_MS_WH_DOWN) {
        mouse_report->v = -1;
    } else {
        if (IS_MOUSEKEY_BUTTON(code)) {
            mouse_report->buttons |= 1 << (code - KC_MS_BTN1);
        }
    }
}

void del_mousekey_from_report(report_mouse_t* mouse_report, uint8_t code) {
    if (code == KC_MS_UP || code == KC_MS_DOWN) {
        mouse_report->y = 0;
    } else if (code == KC_MS_LEFT || code == KC_MS_RIGHT) {
        mouse_report->x = 0;
    } else if (code == KC_MS_WH_UP || code == KC_MS_WH_DOWN) {
        mouse_report->v = 0;
    } else {
        if (IS_MOUSEKEY_BUTTON(code)) {
            mouse_report->buttons &= ~(1 << (code - KC_MS_BTN1));
        }
    }
}
#endif
