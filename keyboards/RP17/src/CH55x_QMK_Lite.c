
#include "qmk/quantum/keyboard.h"
#include "ch55xduino/USBHID.h"

void ch55xQmkLite_init(void)
{
    USBInit();
    
    keyboard_init();
}

void ch55xQmkLite_task(void)
{
    keyboard_task();
}

