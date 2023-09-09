#include <stdint.h>
#include <stdbool.h>
#include "include/ch5xx.h"
#include "include/ch5xx_usb.h"
#include "USBconstant.h"
#include "USBhandler.h"
#include "USBHID.h"
#include "../../keyboards/config_common.h"
#include "../qmk/tmk_core/protocol/host.h"
#include "../qmk/tmk_core/protocol/report.h"
#include <Arduino.h>

volatile __xdata uint8_t UpPoint1_Busy  = 0;   //Flag of whether upload pointer is busy
volatile __xdata uint8_t UpPoint2_Busy = 0;
volatile __data uint8_t raw_hid_receive_flag = false;

static __data uint8_t statusLED = 0;

typedef void (*pTaskFn)(void);

void USB_EP1_IN(){
    UEP1_T_LEN = 0;
    UEP1_CTRL = UEP1_CTRL & ~MASK_UEP_T_RES | UEP_T_RES_NAK;            // Default NAK
    UpPoint1_Busy = 0;                                                  //Clear busy flag
}

void USB_EP1_OUT(){
    if ( U_TOG_OK )                                                     // Discard unsynchronized packets
    {
        switch(Ep1Buffer[0]) {
            case 1:
                statusLED = Ep1Buffer[1];
                break;
            default:
                break;
        }
    }
}

uint8_t USB_EP1_send(const uint8_t *data, uint8_t data_len) {
    uint16_t waitWriteCount = 0;

    while (UpPoint1_Busy) { // wait for 250ms or give up
        waitWriteCount++;
        delayMicroseconds(5);
        if (waitWriteCount >= 50000) { return 0; }
    }

    memcpy(&Ep1Buffer[64], data, data_len);
    UEP1_T_LEN = data_len;

    UpPoint1_Busy = 1;
    UEP1_CTRL = UEP1_CTRL & ~MASK_UEP_T_RES | UEP_T_RES_ACK; // upload data and respond ACK

    return 1;
}

void USB_EP2_IN() {
    UEP2_T_LEN = 0;
    UEP2_CTRL = UEP2_CTRL & ~MASK_UEP_T_RES | UEP_T_RES_NAK; // Default NAK
    UpPoint2_Busy = 0;                                       // Clear busy flag
}

void USB_EP2_OUT() {
    if (U_TOG_OK) // Discard unsynchronized packets
    {
        raw_hid_receive_flag = true;   
    }
}

void raw_hid_task() {
    if (raw_hid_receive_flag)
    {
        raw_hid_receive(Ep2Buffer, EP2_SIZE);
        raw_hid_receive_flag = false;
    }
}

uint8_t USB_EP2_send(const uint8_t *data, uint8_t data_len) {
    uint16_t waitWriteCount = 0;

    while (UpPoint2_Busy) { // wait for 250ms or give up
        waitWriteCount++;
        delayMicroseconds(5);
        if (waitWriteCount >= 50000) { return 0; }
    }

    memcpy(&Ep2Buffer[64], data, data_len);
    UEP2_T_LEN = data_len;

    UpPoint2_Busy = 1;
    UEP2_CTRL = UEP2_CTRL & ~MASK_UEP_T_RES | UEP_T_RES_ACK; // upload data and respond ACK

    return 1;
}

uint8_t keyboard_leds(void) {
    return statusLED;
}

void USBInit(){
    USBDeviceCfg();                                                       //Device mode configuration
    USBDeviceEndPointCfg();                                               //Endpoint configuration   
    USBDeviceIntCfg();                                                    //Interrupt configuration    
    UEP0_T_LEN = 0;
    UEP1_T_LEN = 0;                                                       //Pre-use send length must be cleared	  
    UEP2_T_LEN = 0;                                                
}
