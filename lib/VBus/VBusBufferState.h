#ifndef _VBUSBUFFERSTATE_H_
#define _VBUSBUFFERSTATE_H_

#include <stdint.h>
#include <vector>
#include <Arduino.h>

enum VBusBufferState: uint8_t {
    AWAITING, RECEIVING_HEADER, RECEIVING_FRAMES
};

#endif
