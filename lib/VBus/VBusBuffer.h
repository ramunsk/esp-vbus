#ifndef _VBUSBUFFER_H_
#define _VBUSBUFFER_H_

#include <Arduino.h>
#include <VBusStreamHeader.h>
#include <PrintEx.h>
#include <VBusBufferState.h>


#define VBUS_BUFFER_SIZE 200

class VBusBuffer {
public:
    VBusBuffer();
    void push(const uint8_t data);

private:
    uint8_t _buffer[VBUS_BUFFER_SIZE];
    uint8_t _pos;
    VBusStreamHeader _streamHeader;
    PrintEx _printex;
    enum VBusBufferState _state;
    void reset();

    void parseHeader();

    void printDebugInfo();
};


#endif
