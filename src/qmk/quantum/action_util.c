/*
Copyright 2013 Jun Wako <wakojun@gmail.com>

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
#include "../tmk_core/protocol/host.h"
#include "../tmk_core/protocol/report.h"
#include "action_util.h"
#include "../platforms/timer.h"
#include <string.h>

static __xdata uint8_t real_mods = 0;
static __xdata uint8_t weak_mods = 0;

__xdata report_keyboard_t keyboard_report;
__xdata report_mouse_t mouse_report;

/* key */
void add_key(uint8_t key) {
    add_key_to_report(&keyboard_report, key);
}

void del_key(uint8_t key) {
    del_key_from_report(&keyboard_report, key);
}

void clear_keys(void) {
    for (uint8_t i = 0; i < KEYBOARD_REPORT_SIZE; i++) {
        keyboard_report.raw[i] = 0;
    }
    real_mods = 0;
}

#ifdef MOUSE_ENABLE
/* mouse */
void add_mouse_key(uint8_t key) {
    add_mousekey_to_report(&mouse_report, key);
}

void del_mouse_key(uint8_t key) {
    del_mousekey_from_report(&mouse_report, key);
}

void clear_mouse(void) {
    mouse_report.x = 0;
    mouse_report.y = 0;
    mouse_report.v = 0;
    mouse_report.buttons = 0;
}

/** \brief Send mouse report
 *
 * FIXME: needs doc
 */
void send_mouse_report(void) {
    static __xdata report_mouse_t last_report;

    /* Only send the report if there are changes to propagate to the host. */
    if (memcmp(&mouse_report, &last_report, sizeof(report_mouse_t)) != 0) {
        memcpy(&last_report, &mouse_report, sizeof(report_mouse_t));
        host_mouse_send(&mouse_report);
    }
}
#endif

/** \brief Send keyboard report
 *
 * FIXME: needs doc
 */
void send_keyboard_report(void) {
    keyboard_report.mods = real_mods;
    keyboard_report.mods |= weak_mods;

    static __xdata report_keyboard_t last_report;

    /* Only send the report if there are changes to propagate to the host. */
    if (memcmp(&keyboard_report, &last_report, sizeof(report_keyboard_t)) != 0) {
        memcpy(&last_report, &keyboard_report, sizeof(report_keyboard_t));
        host_keyboard_send(&keyboard_report);
    }
}
/** \brief add mods
 *
 * FIXME: needs doc
 */
void add_mods(uint8_t mods) {
    real_mods |= mods;
}
/** \brief del mods
 *
 * FIXME: needs doc
 */
void del_mods(uint8_t mods) {
    real_mods &= ~mods;
}
/** \brief add weak mods
 *
 * FIXME: needs doc
 */
void add_weak_mods(uint8_t mods) {
    weak_mods |= mods;
}
/** \brief del weak mods
 *
 * FIXME: needs doc
 */
void del_weak_mods(uint8_t mods) {
    weak_mods &= ~mods;
}
/** \brief clear weak mods
 *
 * FIXME: needs doc
 */
void clear_weak_mods(void) {
    weak_mods = 0;
}
