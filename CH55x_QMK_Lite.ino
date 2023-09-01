/*
  RP17 keyboard

  created 2023
  by Robit for use with CH55xduino、QMK

  This example code is in the public domain.

*/

//For windows user, if you ever played with other HID device with the same PID C55D
//You may need to uninstall the previous driver completely


#ifndef USER_USB_RAM
#error "This example needs to be compiled with a USER USB setting"
#endif

#include "src/CH55x_QMK_Lite.h"

void setup() {
  ch55xQmkLite_init();
}

void loop() {
  ch55xQmkLite_task();
}
