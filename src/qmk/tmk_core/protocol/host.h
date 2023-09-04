/*
Copyright 2011 Jun Wako <wakojun@gmail.com>

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
#include "report.h"

#ifdef __cplusplus
extern "C" {
#endif

/* host driver interface */
uint8_t host_keyboard_leds(void);
void    host_keyboard_send(report_keyboard_t *report);
void    host_system_send(uint16_t usage);
void    host_consumer_send(uint16_t usage);
void    host_mouse_send(report_mouse_t *report);

void    raw_hid_receive(uint8_t *data, uint8_t length);
void    raw_hid_send(uint8_t *data, uint8_t length);

#ifdef __cplusplus
}
#endif
