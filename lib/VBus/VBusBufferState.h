#ifndef _VBUSBUFFERSTATE_H_
#define _VBUSBUFFERSTATE_H_

#include <stdint.h>

enum VBusBufferState: uint8_t {
    AWAITING,           // Buffer is waiting for a SYNC byte (0xAA)
    RECEIVING_HEADER,   // SYNC byte received, reading header
    RECEIVING_FRAMES    // Header is received and suitable, reading data packets
};

#endif
