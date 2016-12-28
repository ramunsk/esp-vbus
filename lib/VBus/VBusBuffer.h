#ifndef _VBUSBUFFER_H_
#define _VBUSBUFFER_H_

#include <stdint.h>

#define VBUS_BUFFER_SIZE 200

class VBusBuffer {
public:
    VBusBuffer();
    void push(const uint8_t data);

private:
    uint8_t _buffer[VBUS_BUFFER_SIZE];
    uint16_t _pos;
    void reset();
};


#endif
