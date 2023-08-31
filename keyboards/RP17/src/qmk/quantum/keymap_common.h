// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <stdint.h>
#include "keyboard.h"

// translates key to keycode
uint16_t keymap_key_to_keycode(keypos_t key);
