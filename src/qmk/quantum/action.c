/*
Copyright 2012,2013 Jun Wako <wakojun@gmail.com>

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
#include "keycode.h"
#include "keyboard.h"
#include "led.h"
#include "action_util.h"
#include "action.h"
#include "../platforms/wait.h"
#include "quantum.h"

/** \brief Called to execute an action.
 *
 * FIXME: Needs documentation.
 */
void action_exec(keyevent_t event) {
    if (event.pressed) {
        // clear the potential weak mods left by previously pressed keys
        clear_weak_mods();
    }

    keyrecord_t record;
    record.event = event;

    if (record.event.type == TICK_EVENT) {
        dynamic_macro_task();
        return;
    }

    if (process_record_quantum(&record) == false) {
        return;
    }

    action_t action_tmp;
    action_tmp = action_for_key(record.event.key);
    process_action(record, action_tmp);
}

/** \brief Take an action and processes it.
 *
 * FIXME: Needs documentation.
 */
void process_action(keyrecord_t record, action_t action) {

    switch (action.kind.id) {
        /* Key and Mods */
        case ACT_LMODS:
        case ACT_RMODS: {
            uint8_t mods = (action.kind.id == ACT_LMODS) ? action.key.mods : action.key.mods << 4;
            if (record.event.pressed) {
                if (mods) {
                    if (IS_MODIFIER_KEYCODE(action.key.code) || action.key.code == KC_NO) {
                        // e.g. LSFT(KC_LEFT_GUI): we don't want the LSFT to be weak as it would make it useless.
                        // This also makes LSFT(KC_LEFT_GUI) behave exactly the same as LGUI(KC_LEFT_SHIFT).
                        // Same applies for some keys like KC_MEH which are declared as MEH(KC_NO).
                        add_mods(mods);
                    } else {
                        add_weak_mods(mods);
                    }
                    send_keyboard_report();
                }
                register_code(action.key.code);
            } else {
                unregister_code(action.key.code);
                if (mods) {
                    if (IS_MODIFIER_KEYCODE(action.key.code) || action.key.code == KC_NO) {
                        del_mods(mods);
                    } else {
                        del_weak_mods(mods);
                    }
                    send_keyboard_report();
                }
            }
        } break;
#ifdef EXTRAKEY_ENABLE
        /* other HID usage */
        case ACT_USAGE:
            switch (action.usage.page) {
                case PAGE_SYSTEM:
                    host_system_send(record.event.pressed ? action.usage.code : 0);
                    break;
                case PAGE_CONSUMER:
                    host_consumer_send(record.event.pressed ? action.usage.code : 0);
                    break;
            }
            break;
#endif // EXTRAKEY_ENABLE
#ifdef MOUSE_ENABLE
        /* Mouse */
        case ACT_MOUSEKEY:
            if (record.event.pressed) {
                add_mouse_key(action.key.code);
            } else {
                del_mouse_key(action.key.code);
            }
            send_mouse_report();
            break;
#endif // MOUSE_ENABLE
        default:
            break;
    }
}

/** \brief Utilities for actions. (FIXME: Needs better description)
 *
 * FIXME: Needs documentation.
 */
void register_code(uint8_t code) {
    if (code == KC_NO) {
        return;
    } else if (IS_BASIC_KEYCODE(code)) {
        // Force a new key press if the key is already pressed
        // without this, keys with the same keycode, but different
        // modifiers will be reported incorrectly, see issue #1708
        if (is_key_pressed(&keyboard_report, code)) {
            del_key(code);
            send_keyboard_report();
        }
        add_key(code);
        send_keyboard_report();
    } else if (IS_MODIFIER_KEYCODE(code)) {
        add_mods(MOD_BIT(code));
        send_keyboard_report();
    }
#ifdef EXTRAKEY_ENABLE
    else if (IS_SYSTEM_KEYCODE(code)) {
        host_system_send(report_keycode_to_system(code));
    } else if (IS_CONSUMER_KEYCODE(code)) {
        host_consumer_send(report_keycode_to_consumer(code));
    }
#endif
#ifdef MOUSE_ENABLE
    else if (IS_MOUSE_KEYCODE(code)) {
        add_mouse_key(code);
        send_mouse_report();
    }
#endif
}

/** \brief Utilities for actions. (FIXME: Needs better description)
 *
 * FIXME: Needs documentation.
 */
 void unregister_code(uint8_t code) {
    if (code == KC_NO) {
        return;
    } else if (IS_BASIC_KEYCODE(code)) {
        del_key(code);
        send_keyboard_report();
    } else if (IS_MODIFIER_KEYCODE(code)) {
        del_mods(MOD_BIT(code));
        send_keyboard_report();
    }
#ifdef EXTRAKEY_ENABLE
    else if (IS_SYSTEM_KEYCODE(code)) {
        host_system_send(0);
    } else if (IS_CONSUMER_KEYCODE(code)) {
        host_consumer_send(0);
    }
#endif
#ifdef MOUSE_ENABLE
    else if (IS_MOUSE_KEYCODE(code)) {
        del_mouse_key(code);
        send_mouse_report();
    }
#endif
}

/** \brief Tap a keycode with the default delay.
 *
 * \param code The basic keycode to tap. If `code` is `KC_CAPS_LOCK`, the delay will be `TAP_HOLD_CAPS_DELAY`, otherwise `TAP_CODE_DELAY`, if defined.
 */
void tap_code(uint8_t code) {
    register_code(code);
    if (code == KC_CAPS_LOCK) {
        wait_ms(TAP_HOLD_CAPS_DELAY);
    } else {
        wait_ms(TAP_CODE_DELAY);
    }
    unregister_code(code);
}
