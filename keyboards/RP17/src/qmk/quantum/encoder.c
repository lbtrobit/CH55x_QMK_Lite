/*
 * Copyright 2018 Jack Humbert <jack.humb@gmail.com>
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

#include "encoder.h"
#include <string.h>
#include "action.h"

#ifdef ENCODER_ENABLE

#if !defined(ENCODER_RESOLUTIONS) && !defined(ENCODER_RESOLUTION)
#    define ENCODER_RESOLUTION 4
#endif

static __xdata pin_t encoders_pad_a[NUM_ENCODERS] = ENCODERS_PAD_A;
static __xdata pin_t encoders_pad_b[NUM_ENCODERS] = ENCODERS_PAD_B;

#ifdef ENCODER_RESOLUTIONS
static __xdata uint8_t encoder_resolutions[NUM_ENCODERS] = ENCODER_RESOLUTIONS;
#endif

#ifndef ENCODER_DIRECTION_FLIP
#    define ENCODER_CLOCKWISE           true
#    define ENCODER_COUNTER_CLOCKWISE   false
#else
#    define ENCODER_CLOCKWISE           false
#    define ENCODER_COUNTER_CLOCKWISE   true
#endif

static __xdata int8_t encoder_LUT[] = {0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0};
static __xdata uint8_t encoder_state[NUM_ENCODERS]  = {0};
static __xdata int8_t  encoder_pulses[NUM_ENCODERS] = {0};

// encoder counts
static __xdata uint8_t encoder_value[NUM_ENCODERS] = {0};

void encoder_init(void) {
    for (uint8_t i = 0; i < NUM_ENCODERS; i++) {
        setPinInputHigh(encoders_pad_a[i]);
        setPinInputHigh(encoders_pad_b[i]);
    }
    wait_us(100);
    for (uint8_t i = 0; i < NUM_ENCODERS; i++) {
        encoder_state[i] = (readPin(encoders_pad_a[i]) << 0) | (readPin(encoders_pad_b[i]) << 1);
    }
}

static void encoder_exec_mapping(uint8_t index, bool clockwise) {
    // The delays below cater for Windows and its wonderful requirements.
    keyevent_t event_tmp1 = {.key = {.row = KEYLOC_ENCODER_CW, .col = index}, .pressed = true, .time = timer_read(), .type = ENCODER_CW_EVENT};
    keyevent_t event_tmp2 = {.key = {.row = KEYLOC_ENCODER_CCW, .col = index}, .pressed = true, .time = timer_read(), .type = ENCODER_CCW_EVENT};
    action_exec(clockwise ? event_tmp1 : event_tmp2);
    wait_ms(10);
    event_tmp1.pressed = false;
    event_tmp2.pressed = false;
    action_exec(clockwise ? event_tmp1 : event_tmp2);
    wait_ms(10);
}

static bool encoder_update(uint8_t index, uint8_t state) {
    bool    changed = false;
    uint8_t i       = index;

#ifdef ENCODER_RESOLUTIONS
    const uint8_t resolution = encoder_resolutions[i];
#else
    const uint8_t resolution = ENCODER_RESOLUTION;
#endif

    encoder_pulses[i] += encoder_LUT[state & 0xF];

#ifdef ENCODER_DEFAULT_POS
    if ((encoder_pulses[i] >= resolution) || (encoder_pulses[i] <= -resolution) || ((state & 0x3) == ENCODER_DEFAULT_POS)) {
        if (encoder_pulses[i] >= 1) {
#else
    if (encoder_pulses[i] >= (int8_t)resolution) {
#endif
            encoder_value[index]++;
            changed = true;
            encoder_exec_mapping(index, ENCODER_COUNTER_CLOCKWISE);
        }

#ifdef ENCODER_DEFAULT_POS
    if (encoder_pulses[i] <= -1) {
#else
    if (encoder_pulses[i] <= -resolution) { // direction is arbitrary here, but this clockwise
#endif
            encoder_value[index]--;
            changed = true;
            encoder_exec_mapping(index, ENCODER_CLOCKWISE);
        }
        encoder_pulses[i] %= resolution;
#ifdef ENCODER_DEFAULT_POS
        encoder_pulses[i] = 0;
    }
#endif
    return changed;
}

bool encoder_read(void) {
    bool changed = false;
    for (uint8_t i = 0; i < NUM_ENCODERS; i++) {
        uint8_t new_status = (readPin(encoders_pad_a[i]) << 0) | (readPin(encoders_pad_b[i]) << 1);
        if ((encoder_state[i] & 0x3) != new_status) {
            encoder_state[i] <<= 2;
            encoder_state[i] |= new_status;
            changed |= encoder_update(i, encoder_state[i]);
        }
    }
    return changed;
}

#endif //ENCODER_ENABLE
