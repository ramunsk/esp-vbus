#include <Arduino.h>

#include "VBusBuffer.h"

#define VBUS_HEADER_MATCH {0xAA, 0x10, 0x00, 0x21, 0x77, 0x10, 0x00, 0x01, 0x11, 0x35}
#define VBUS_FRAME_SIZE 6


VBusBuffer::VBusBuffer(onVBusDataFrameReceived onFrameReceived):
    _pos(0),
    _frameIndex(0),
    _state(AWAITING),
    _onFrameReceived(onFrameReceived)
    {
        reset();
    }

const uint8_t VBusBuffer::_headerMatch[] = VBUS_HEADER_MATCH;

void VBusBuffer::push(const uint8_t data) {
    // From docs: (p.8)
    // The reception of a SYNC-Byte starts a new data stream. If the current reception was incomplete it will be
    // cancelled and discarded. This happens as well whenever a byte is received that has its MSB (Most
    // Significant Bit) set (its value is greater than 0x7F or 127 decimal). All received bytes up to the next SYNCByte
    // are ignored.
    if (data > 0x7F && data != 0xAA){
        // invalid byte received
        // switch to waiting for sync byte
        // _printex
        //     .newln()
        //     .print("Received invalid byte [0x").printHEX(data).println("]. Resetting...");
        reset();
        return;
    }

    if (data == 0xAA){
        // sync byte received
        // drop everything read up to this point
        // and start reading header
        reset();
        _state = RECEIVING_HEADER;
        //_printex.newln().println("SYNCByte received. Receiving data...");
    }


    if (_state == RECEIVING_HEADER){
        readHeader(data);
    } else if (_state == RECEIVING_FRAMES){
        readFrame(data);
    }
}

void VBusBuffer::readHeader(const uint8_t data){
    writeBuffer(data);
    if (_pos != VBUS_HEADER_SIZE){
        return;
    }

    if (memcmp(_buffer, _headerMatch, 10) == 0){
        //_printex.newln().println("Found a matching packet...");
        resetBuffer();
        _state = RECEIVING_FRAMES;
    }
}

void VBusBuffer::readFrame(const uint8_t data){
    uint8_t frameData[4];
    writeBuffer(data);
    if (_pos != VBUS_FRAME_SIZE){
        return;
    }

    uint8_t crc = calcFrameCRC();
    if (crc != _buffer[VBUS_BUFFER_SIZE - 1]){
        // TODO: handle error;
    }

    parseFrame();
    memcpy(&_payload[_frameIndex * 4], _buffer, 4);
    memcpy(frameData, _buffer, 4);

    if (_onFrameReceived != nullptr){
        _onFrameReceived(_frameIndex, frameData);
    }

    //_printex.print("Frame: ").print(_frameIndex).print(", value: ").println(value);

    resetBuffer();
    _frameIndex++;

    // if (_frameIndex == VBUS_FRAME_COUNT){
    //     _printex.newln().print("Payload: ");
    //     for (size_t i = 0; i < VBUS_PAYLOAD_SIZE; i++) {
    //         _printex.print(" ").printHEX(_payload[i]);
    //     }
    //     _printex.newln();
    // }
}

uint8_t VBusBuffer::calcFrameCRC()
{
    uint8_t crc = 0x7F;
    for (size_t i = 0; i < VBUS_FRAME_SIZE; i++) {
        crc = (crc - _buffer [i]) & 0x7F;
    }
    return crc;
}

void VBusBuffer::parseFrame(){
    uint8_t septet = _buffer[VBUS_FRAME_SIZE - 2];
    for (size_t i = 0; i < VBUS_FRAME_SIZE - 2; i++) {
        if (septet & (1 << i)) {
            _buffer[i] |= 0x80;
        }
    }

}


void VBusBuffer::writeBuffer(const uint8_t data){
    _buffer[_pos++] = data;
}

void VBusBuffer::reset(){
    _state = AWAITING;
    _frameIndex = 0;
    memset(_payload, 0, VBUS_PAYLOAD_SIZE);
    resetBuffer();
}

void VBusBuffer::resetBuffer() {
    _pos = 0;
    memset(_buffer, 0, VBUS_BUFFER_SIZE);
}
