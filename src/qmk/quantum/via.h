/* Copyright 2019 Jason Williams (Wilba)
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

#include "action.h"
#include "quantum.h"

#define VIA_MAGIC_CODE                      0x5A
#define VIA_EEPROM_MAGIC_ADDR               0

#define VIA_EEPROM_SAVE_INTERVAL            500

// The end of the EEPROM memory used by VIA
// By default, dynamic keymaps will start at this if there is no
// custom config
#define VIA_EEPROM_CUSTOM_CONFIG_ADDR       (VIA_EEPROM_MAGIC_ADDR + 1)

#define VIA_EEPROM_CUSTOM_MACRO_LOOP_ADDR   VIA_EEPROM_CUSTOM_CONFIG_ADDR
#define VIA_EEPROM_CUSTOM_MACRO_LOOP_SIZE   5

#ifdef RGB_MATRIX_ENABLE
#define VIA_EEPROM_CUSTOM_RGB_MATRIX_ADDR   (VIA_EEPROM_CUSTOM_MACRO_LOOP_ADDR + VIA_EEPROM_CUSTOM_MACRO_LOOP_SIZE)
# ifdef RGB_EFFECTS_ENABLE
# define VIA_EEPROM_CUSTOM_RGB_MATRIX_SIZE   12
# else  
# define VIA_EEPROM_CUSTOM_RGB_MATRIX_SIZE   4
# endif // RGB_EFFECTS_ENABLE
#define VIA_EEPROM_CUSTOM_CONFIG_SIZE       VIA_EEPROM_CUSTOM_MACRO_LOOP_SIZE + VIA_EEPROM_CUSTOM_RGB_MATRIX_SIZE
#else
#define VIA_EEPROM_CUSTOM_CONFIG_SIZE       VIA_EEPROM_CUSTOM_MACRO_LOOP_SIZE
#endif // RGB_MATRIX_ENABLE

#define VIA_EEPROM_CONFIG_END               (VIA_EEPROM_CUSTOM_CONFIG_ADDR + VIA_EEPROM_CUSTOM_CONFIG_SIZE)

// This is changed only when the command IDs change,
// so VIA Configurator can detect compatible firmware.
#define VIA_PROTOCOL_VERSION                0x000C

enum via_command_id {
    id_get_protocol_version                 = 0x01, // always 0x01
    id_get_keyboard_value                   = 0x02,
    id_set_keyboard_value                   = 0x03,
    id_dynamic_keymap_get_keycode           = 0x04,
    id_dynamic_keymap_set_keycode           = 0x05,
    id_dynamic_keymap_reset                 = 0x06,
    id_custom_set_value                     = 0x07,
    id_custom_get_value                     = 0x08,
    id_custom_save                          = 0x09,
    id_eeprom_reset                         = 0x0A,
    id_bootloader_jump                      = 0x0B,
    id_dynamic_keymap_macro_get_count       = 0x0C,
    id_dynamic_keymap_macro_get_buffer_size = 0x0D,
    id_dynamic_keymap_macro_get_buffer      = 0x0E,
    id_dynamic_keymap_macro_set_buffer      = 0x0F,
    id_dynamic_keymap_macro_reset           = 0x10,
    id_dynamic_keymap_get_layer_count       = 0x11,
    id_dynamic_keymap_get_buffer            = 0x12,
    id_dynamic_keymap_set_buffer            = 0x13,
    id_dynamic_keymap_get_encoder           = 0x14,
    id_dynamic_keymap_set_encoder           = 0x15,
    id_unhandled                            = 0xFF,

    id_signalrgb_qmk_version                = 0x21,
    id_signalrgb_protocol_version           = 0x22,
    id_signalrgb_unique_identifier          = 0x23,
    id_signalrgb_stream_leds                = 0x24,
    id_signalrgb_effect_enable              = 0x25,
    id_signalrgb_effect_disable             = 0x26,
    id_signalrgb_get_total_leds             = 0x27,
    id_signalrgb_get_firmware_type          = 0x28,
};

enum signalrgb_responses {
    PROTOCOL_VERSION_BYTE_1 = 1,
    PROTOCOL_VERSION_BYTE_2 = 0,
    PROTOCOL_VERSION_BYTE_3 = 4,
    QMK_VERSION_BYTE_1 = 0,
    QMK_VERSION_BYTE_2 = 21,
    QMK_VERSION_BYTE_3 = 6,
    DEVICE_UNIQUE_IDENTIFIER_BYTE_1 = 0,
    DEVICE_UNIQUE_IDENTIFIER_BYTE_2 = 0,
    DEVICE_UNIQUE_IDENTIFIER_BYTE_3 = 0,
    FIRMWARE_TYPE_BYTE = 2, 
    DEVICE_ERROR_LEDS = 254,
};

enum via_keyboard_value_id {
    id_uptime              = 0x01,
    id_layout_options      = 0x02,
    id_switch_matrix_state = 0x03,
    id_firmware_version    = 0x04,
    id_device_indication   = 0x05,
};

enum via_channel_id {
    id_custom_channel         = 0,
    id_qmk_backlight_channel  = 1,
    id_qmk_rgblight_channel   = 2,
    id_qmk_rgb_matrix_channel = 3,
    id_qmk_audio_channel      = 4,

    id_qmk_macro_loop_channel = 5,
    id_qmk_macro_system       = 6,
};

enum via_qmk_rgb_matrix_value {
    id_qmk_rgb_matrix_brightness   = 1,
    id_qmk_rgb_matrix_effect       = 2,
    id_qmk_rgb_matrix_effect_speed = 3,
    id_qmk_rgb_matrix_color        = 4,

    id_qmk_rgb_matrix_color_red    = 5,
    id_qmk_rgb_matrix_color_green  = 6,
    id_qmk_rgb_matrix_color_blue   = 7,
};

enum via_qmk_macro_loop_value {
    id_qmk_macro_loop_m0    = 1,
    id_qmk_macro_loop_m1    = 2,
    id_qmk_macro_loop_m2    = 3,
    id_qmk_macro_loop_m3    = 4,
    id_qmk_macro_loop_m4    = 5,
};

enum id_qmk_macro_system_value {
    id_system_eeprom_reset    = 1,
    id_system_bootloader_jump = 2,
};

// Called by QMK core to initialize dynamic keymaps etc.
void eeconfig_init_via(void);
void via_init(void);

// Called by QMK core to process VIA-specific keycodes.
bool process_record_via(uint16_t keycode, keyrecord_t *record);