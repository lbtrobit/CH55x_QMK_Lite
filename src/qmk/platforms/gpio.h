/* Copyright 2021 QMK
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
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

#include <stdint.h>
#include <Arduino.h>

typedef uint8_t pin_t;

#define NO_PIN 0x00

#define setPinInput(pin)        pinMode(pin, INPUT)
#define setPinInputHigh(pin)    pinMode(pin, INPUT_PULLUP)
#define setPinOutput(pin)       pinMode(pin, OUTPUT)

#define writePinHigh(pin)       digitalWrite(pin, HIGH)
#define writePinLow(pin)        digitalWrite(pin, LOW)
#define writePin(pin, level)    ((level) ? writePinHigh(pin) : writePinLow(pin))

#define readPin(pin)            digitalRead(pin)



