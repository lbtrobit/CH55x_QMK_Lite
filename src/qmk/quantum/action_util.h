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

#pragma once

#include <stdint.h>
#include "../tmk_core/protocol/report.h"
#include "modifiers.h"

#ifdef __cplusplus
extern "C" {
#endif

extern __xdata report_keyboard_t keyboard_report;
extern __xdata report_mouse_t mouse_report;

void send_keyboard_report(void);
void send_mouse_report(void);

extern void add_key(uint8_t key);
extern void del_key(uint8_t key);
extern void clear_keys(void);

void add_mouse_key(uint8_t key);
void del_mouse_key(uint8_t key);
void clear_mouse(void);

/* modifier */
void    add_mods(uint8_t mods);
void    del_mods(uint8_t mods);

void    add_weak_mods(uint8_t mods);
void    del_weak_mods(uint8_t mods);
void    clear_weak_mods(void);

#ifdef __cplusplus
}
#endif
