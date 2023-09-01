/* Copyright 2020 zvecr<git@zvecr.com>
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
#include "led.h"
#include "../tmk_core/protocol/host.h"
#include "../platforms/gpio.h"
#include "../../../keyboards/config_common.h"

#ifndef LED_PIN_ON_STATE
#    define LED_PIN_ON_STATE 1
#endif

#ifdef LED_ENABLE
/** \brief Write LED state to hardware
 */
void led_update_ports(uint8_t led_state) {
#if LED_PIN_ON_STATE == 0
    // invert the whole thing to avoid having to conditionally !led_state.x later
    led_state.raw = ~led_state.raw;
#endif

#ifdef LED_NUM_LOCK_PIN
    writePin(LED_NUM_LOCK_PIN, led_state.num_lock);
#endif
#ifdef LED_CAPS_LOCK_PIN
    writePin(LED_CAPS_LOCK_PIN, led_state.caps_lock);
#endif
#ifdef LED_SCROLL_LOCK_PIN
    writePin(LED_SCROLL_LOCK_PIN, led_state.scroll_lock);
#endif
#ifdef LED_COMPOSE_PIN
    writePin(LED_COMPOSE_PIN, led_state.compose);
#endif
#ifdef LED_KANA_PIN
    writePin(LED_KANA_PIN, led_state.kana);
#endif
}

/** \brief Initialise any LED related hardware and/or state
 */
 void led_init_ports(void) {
#ifdef LED_NUM_LOCK_PIN
    setPinOutput(LED_NUM_LOCK_PIN);
    writePin(LED_NUM_LOCK_PIN, !LED_PIN_ON_STATE);
#endif
#ifdef LED_CAPS_LOCK_PIN
    setPinOutput(LED_CAPS_LOCK_PIN);
    writePin(LED_CAPS_LOCK_PIN, !LED_PIN_ON_STATE);
#endif
#ifdef LED_SCROLL_LOCK_PIN
    setPinOutput(LED_SCROLL_LOCK_PIN);
    writePin(LED_SCROLL_LOCK_PIN, !LED_PIN_ON_STATE);
#endif
#ifdef LED_COMPOSE_PIN
    setPinOutput(LED_COMPOSE_PIN);
    writePin(LED_COMPOSE_PIN, !LED_PIN_ON_STATE);
#endif
#ifdef LED_KANA_PIN
    setPinOutput(LED_KANA_PIN);
    writePin(LED_KANA_PIN, !LED_PIN_ON_STATE);
#endif
}

/** \brief Entrypoint for protocol to LED binding
 */
void led_set(uint8_t usb_led) {
    led_update_ports(usb_led);
}

/** \brief set host led state
 *
 * Only sets state if change detected
 */
void led_task(void) {
    static __xdata uint8_t last_led_status = 0;

    // update LED
    uint8_t led_status = host_keyboard_leds();

    if (last_led_status != led_status) {
        last_led_status = led_status;
        led_set(led_status);
    }
}
#endif
