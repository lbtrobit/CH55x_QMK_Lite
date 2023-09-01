/*
Copyright 2012-2018 Jun Wako, Jack Humbert, Yiancar

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
#include <stdbool.h>
#include <string.h>
#include "matrix.h"
#include "debounce.h"
#include "quantum.h"
#include "../platforms/timer.h"

#define ROWS_PER_HAND   (MATRIX_ROWS)

#ifndef MATRIX_INPUT_PRESSED_STATE
#    define MATRIX_INPUT_PRESSED_STATE 0
#endif

#ifdef DIRECT_PINS
static const pin_t direct_pins[ROWS_PER_HAND][MATRIX_COLS] = DIRECT_PINS;
#elif (DIODE_DIRECTION == ROW2COL) || (DIODE_DIRECTION == COL2ROW)
#    ifdef MATRIX_ROW_PINS
static const __code pin_t row_pins[ROWS_PER_HAND] = MATRIX_ROW_PINS;
#    endif // MATRIX_ROW_PINS
#    ifdef MATRIX_COL_PINS
static const __code pin_t col_pins[MATRIX_COLS]   = MATRIX_COL_PINS;
#    endif // MATRIX_COL_PINS
#endif

/* matrix state(1:on, 0:off) */
__xdata matrix_row_t raw_matrix[MATRIX_ROWS]; // raw values
__xdata matrix_row_t matrix[MATRIX_ROWS];     // debounced values

void matrix_init_pins(void);
void matrix_read_cols_on_row(matrix_row_t current_matrix[], uint8_t current_row);
void matrix_read_rows_on_col(matrix_row_t current_matrix[], uint8_t current_col, matrix_row_t row_shifter);

static void setPinOutput_writeLow(uint8_t pin) {
        setPinOutput(pin);
        writePinLow(pin);
}

static void setPinInputHigh_atomic(uint8_t pin) {
        writePinHigh(pin);
        setPinInputHigh(pin);
}

static uint8_t readMatrixPin(uint8_t pin) {
    return (readPin(pin) == MATRIX_INPUT_PRESSED_STATE) ? 0 : 1;
}

// matrix code

#ifdef DIRECT_PINS

void matrix_init_pins(void) {
    for (uint8_t row = 0; row < ROWS_PER_HAND; row++) {
        for (uint8_t col = 0; col < MATRIX_COLS; col++) {
            setPinInputHigh(direct_pins[row][col]);
        }
    }
}

void matrix_read_cols_on_row(matrix_row_t current_matrix[], uint8_t current_row) {
    // Start with a clear matrix row
    matrix_row_t current_row_value = 0;

    matrix_row_t row_shifter = MATRIX_ROW_SHIFTER;
    for (uint8_t col_index = 0; col_index < MATRIX_COLS; col_index++, row_shifter <<= 1) {
        current_row_value |= readMatrixPin(direct_pins[current_row][col_index]) ? 0 : row_shifter;
    }

    // Update the matrix
    current_matrix[current_row] = current_row_value;
}

#elif defined(DIODE_DIRECTION)
#    if defined(MATRIX_ROW_PINS) && defined(MATRIX_COL_PINS)
#        if (DIODE_DIRECTION == COL2ROW)

void matrix_init_pins(void) {
    for (uint8_t i = 0; i < ROWS_PER_HAND; i++) {
        setPinInputHigh_atomic(row_pins[i]);
    }
    for (uint8_t i = 0; i < MATRIX_COLS; i++) {
        setPinInputHigh_atomic(col_pins[i]);
    }
}

void matrix_read_cols_on_row(matrix_row_t current_matrix[], uint8_t current_row) {
    // Start with a clear matrix row
    matrix_row_t current_row_value = 0;

    setPinOutput_writeLow(row_pins[current_row]);
    waitInputPinDelay();

    // For each col...
    matrix_row_t row_shifter = MATRIX_ROW_SHIFTER;
    for (uint8_t col_index = 0; col_index < MATRIX_COLS; col_index++, row_shifter <<= 1) {
        uint8_t pin_state = readMatrixPin(col_pins[col_index]);

        // Populate the matrix row with the state of the col pin
        current_row_value |= pin_state ? 0 : row_shifter;
    }

    // Unselect row
    setPinInputHigh_atomic(row_pins[current_row]);
    wait_us(MATRIX_IO_DELAY); // wait for all Col signals to go HIGH

    // Update the matrix
    current_matrix[current_row] = current_row_value;
}

#        elif (DIODE_DIRECTION == ROW2COL)

void matrix_init_pins(void) {
    for (uint8_t i = 0; i < MATRIX_COLS; i++) {
        setPinInputHigh_atomic(col_pins[i]);
    }
    for (uint8_t i = 0; i < ROWS_PER_HAND; i++) {
        setPinInputHigh_atomic(row_pins[i]);
    }
}

void matrix_read_rows_on_col(matrix_row_t current_matrix[], uint8_t current_col, matrix_row_t row_shifter) {

    // Select col
    setPinOutput_writeLow(col_pins[current_col]);
    waitInputPinDelay();

    // For each row...
    for (uint8_t row_index = 0; row_index < ROWS_PER_HAND; row_index++) {
        // Check row pin state
        if (readMatrixPin(row_pins[row_index]) == 0) {
            // Pin LO, set col bit
            current_matrix[row_index] |= row_shifter;
        } else {
            // Pin HI, clear col bit
            current_matrix[row_index] &= ~row_shifter;
        }
    }

    // Unselect col
    setPinInputHigh_atomic(col_pins[current_col]);
    wait_us(MATRIX_IO_DELAY); // wait for all Row signals to go HIGH
}

#        else
#            error DIODE_DIRECTION must be one of COL2ROW or ROW2COL!
#        endif
#    endif // defined(MATRIX_ROW_PINS) && defined(MATRIX_COL_PINS)
#else
#    error DIODE_DIRECTION is not defined!
#endif

void matrix_init(void) {
    // initialize key pins
    matrix_init_pins();

    // initialize matrix state: all keys off
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        matrix[i] = 0;
        raw_matrix[i] = 0;
    }
}

uint8_t matrix_scan(void) {
    matrix_row_t curr_matrix[MATRIX_ROWS] = {0};

#if defined(DIRECT_PINS) || (DIODE_DIRECTION == COL2ROW)
    // Set row, read cols
    for (uint8_t current_row = 0; current_row < ROWS_PER_HAND; current_row++) {
        matrix_read_cols_on_row(curr_matrix, current_row);
    }
#elif (DIODE_DIRECTION == ROW2COL)
    // Set col, read rows
    matrix_row_t row_shifter = MATRIX_ROW_SHIFTER;
    for (uint8_t current_col = 0; current_col < MATRIX_COLS; current_col++, row_shifter <<= 1) {
        matrix_read_rows_on_col(curr_matrix, current_col, row_shifter);
    }
#endif

    bool changed = memcmp(raw_matrix, curr_matrix, sizeof(curr_matrix)) != 0;
    if (changed) memcpy(raw_matrix, curr_matrix, sizeof(curr_matrix));

    changed = debounce(raw_matrix, matrix, ROWS_PER_HAND, changed);
    return (uint8_t)changed;
}
