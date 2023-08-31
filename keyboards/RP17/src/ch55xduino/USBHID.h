#ifndef __USB_HID_KBD_H__
#define __USB_HID_KBD_H__

#include <stdint.h>
#include "include/ch5xx.h"
#include "include/ch5xx_usb.h"

#ifdef __cplusplus
extern "C" {
#endif

void USBInit(void);
uint8_t USB_EP1_send(const uint8_t *data, uint8_t data_len);
uint8_t USB_EP2_send(const uint8_t *data, uint8_t data_len);
uint8_t keyboard_leds(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif

