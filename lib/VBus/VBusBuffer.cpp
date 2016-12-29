#include <Arduino.h>

#include "VBusBuffer.h"

VBusBuffer::VBusBuffer():
    _pos(0),
    _streamHeader(VBusStreamHeader()),
    _printex(PrintEx(&Serial)),
    _state(AWAITING)
    {
        reset();
    }

void VBusBuffer::push(const uint8_t data) {
    // From docs: (p.8)
    // The reception of a SYNC-Byte starts a new data stream. If the current reception was incomplete it will be
    // cancelled and discarded. This happens as well whenever a byte is received that has its MSB (Most
    // Significant Bit) set (its value is greater than 0x7F or 127 decimal). All received bytes up to the next SYNCByte
    // are ignored.
    if (data > 0x7F && data != 0xAA){
        _printex
            .newln()
            .print("Received [").printHEX(data).print("]. Resetting...").newln();
        reset();
        return;
    }

    if (data == 0xAA){
        reset();
        _buffer[_pos] = data;
        _pos++;
        _state = RECEIVING;
        _printex.newln().println("SYNCByte received. Receiving data...");
    }

    if (_state != RECEIVING){
        return;
    }

    _buffer[_pos] = data;
    _pos++;


}

void VBusBuffer::reset(){
    _pos = 0;
    memset(_buffer, 0, VBUS_BUFFER_SIZE);
    _streamHeader = VBusStreamHeader();
    _state = AWAITING;
}

void VBusBuffer::parseHeader() {
    _streamHeader.srcAddress = _buffer[2] << 8 | _buffer[1];
    _streamHeader.destAddress = _buffer[4] << 8 | _buffer[3];
    _streamHeader.version = _buffer[5];
    printDebugInfo();
}

void VBusBuffer::printDebugInfo(){
    _printex.println("");
    _printex.print("Source address:      0x").printHEXln(_streamHeader.srcAddress);
    _printex.print("Destination address: 0x").printHEXln(_streamHeader.destAddress);
    _printex.print("Version:             ").println(_streamHeader.version);
}
