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

#include <stdint.h>
#include "../../quantum/keyboard.h"
#include "../../quantum/keycode.h"
#include "host.h"
#include "../../../ch55xduino/USBHID.h"

#ifdef EXTRAKEY_ENABLE
static __xdata uint16_t last_system_usage   = 0;
static __xdata uint16_t last_consumer_usage = 0;

void host_system_send(uint16_t usage) {
    if (usage == last_system_usage) return;
    last_system_usage = usage;

    report_extra_t report = {
        .report_id = REPORT_ID_SYSTEM,
        .usage     = usage,
    };
    USB_EP1_send((uint8_t *)&report, 4);
}

void host_consumer_send(uint16_t usage) {
    if (usage == last_consumer_usage) return;
    last_consumer_usage = usage;

    report_extra_t report = {
        .report_id = REPORT_ID_CONSUMER,
        .usage     = usage,
    };
    USB_EP1_send((uint8_t *)&report, 4);
}
#endif

uint8_t host_keyboard_leds(void) {
    return keyboard_leds();
}

/* send report */
void host_keyboard_send(report_keyboard_t *report) {
    report->report_id = REPORT_ID_KEYBOARD;
    USB_EP1_send((uint8_t *)report, 9);
}

void raw_hid_send(uint8_t *data, uint8_t length){
    USB_EP2_send((uint8_t *)data,length);
}

#ifdef MOUSE_ENABLE
void host_mouse_send(report_mouse_t *report) {
    report->report_id = REPORT_ID_MOUSE;
    USB_EP1_send((uint8_t *)report, 5);
}
#endif
