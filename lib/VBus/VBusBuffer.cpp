#include <string.h>

#include "VBusBuffer.h"

VBusBuffer::VBusBuffer():
    _pos(0)
    {
        reset();
    }

void VBusBuffer::push(const uint8_t data) {
    _buffer[_pos] = data;
    _pos++;
}

void VBusBuffer::reset(){
    _pos = 0;
    memset(_buffer, 0, VBUS_BUFFER_SIZE);
}
