/* Copyright 2016-2018 Erez Zukerman, Jack Humbert, Yiancar
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
#pragma once

#include "../../../keyboards/config_common.h"
#include "../platforms/wait.h"
#include "matrix.h"
#include "keyboard.h"
#include "send_string/send_string.h"
#include "keymap_common.h"
#include "quantum_keycodes.h"
#include "../platforms/timer.h"
#include "../platforms/gpio.h"
#include "../tmk_core/protocol/host.h"
#include "led.h"
#include "action_util.h"
#include <stddef.h>
#include <stdlib.h>
#include <Arduino.h>
#include "send_string/send_string.h"
#include "dynamic_keymap.h"
#include "via.h"

#ifdef ENCODER_ENABLE
#    include "encoder.h"
#endif

void usb_send_for_debug(uint8_t v1, uint8_t v2);
