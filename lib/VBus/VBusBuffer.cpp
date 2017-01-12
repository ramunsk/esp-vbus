#include <dbg.h>
#include <Arduino.h>    // have no idea where memcmp/memcpy are defined
#include "VBusBuffer.h"


const uint8_t VBusBuffer::_headerMatch[] = {
    0xAA,       // SYNC byte
    0x10, 0x00, // destination address 0x0010 (DFA)
    0x21, 0x77, // source address 0x7721 (DeltaSol E [Controller])
    0x10,       // protocol version (v1)
    0x00, 0x01, // command 0x0100 ()
    0x11,       // frame count 0x11 (17)
    0x35        // checksum
};


// === PUBLIC ==================================================================
VBusBuffer::VBusBuffer():
    _pos(0),
    _frameIndex(0),
    _state(AWAITING),
    _onFrameReceived(nullptr)
    {
        reset();
    }

void VBusBuffer::onFrameReceived(onVBusDataFrameReceived onFrameReceived){
    _onFrameReceived = onFrameReceived;
}

void VBusBuffer::push(const uint8_t data) {
    // From docs: (p.8)
    // The reception of a SYNC-Byte starts a new data stream. If the current reception was incomplete it will be
    // cancelled and discarded. This happens as well whenever a byte is received that has its MSB (Most
    // Significant Bit) set (its value is greater than 0x7F or 127 decimal). All received bytes up to the next SYNCByte
    // are ignored.
    if (data > 0x7F && data != 0xAA){
        DEBUG_PRINT("VBUSBUFFER: Invalid byte received [0x%02X]", data);
        reset();
        return;
    }

    if (data == 0xAA){
        // sync byte received
        // drop everything read up to this point
        // and start reading header
        reset();
        _state = RECEIVING_HEADER;
        DEBUG_PRINT("VBUSBUFFER: SYNC byte received, reading header");
    }


    if (_state == RECEIVING_HEADER){
        readHeader(data);
    } else if (_state == RECEIVING_FRAMES){
        readFrame(data);
    }
};


// === PRIVATE =================================================================
void VBusBuffer::reset(){
    _state = AWAITING;
    _frameIndex = 0;
    memset(_payload, 0, VBUS_PAYLOAD_SIZE);
    resetBuffer();
}

void VBusBuffer::resetBuffer() {
    _pos = 0;
    memset(_buffer, 0, VBUS_BUFFER_SIZE);
    memset(_payload, 0, VBUS_PAYLOAD_SIZE);
}

void VBusBuffer::writeBuffer(const uint8_t data){
    _buffer[_pos++] = data;
}

void VBusBuffer::readHeader(const uint8_t data){
    writeBuffer(data);
    if (_pos != VBUS_HEADER_SIZE){
        return;
    }

    if (memcmp(_buffer, _headerMatch, VBUS_HEADER_SIZE) == 0){
        DEBUG_PRINT("VBUSBUFFER: matching stream incoming");
        resetBuffer();
        _state = RECEIVING_FRAMES;
    }
}

void VBusBuffer::readFrame(const uint8_t data){
    writeBuffer(data);
    if (_pos != VBUS_FRAME_SIZE){
        return;
    }

    // TODO: for the future: calculate crc
    // e.g.
    // uint8_t crc = calcFrameCRC();
    // if (crc != _buffer[VBUS_BUFFER_SIZE - 1]){
    //     // call some callback or something
    // }

    parseFrame();
    DEBUG_PRINT("VBUSBUFFER: frame received [0x%02X 0x%02X 0x%02X 0x%02X]", _payload[0], _payload[1], _payload[2], _payload[3]);

    if (_onFrameReceived != nullptr){
        _onFrameReceived(_frameIndex, _payload);
    }

    resetBuffer();
    _frameIndex++;
}

void VBusBuffer::parseFrame(){
    uint8_t septet = _buffer[VBUS_FRAME_SIZE - 2];
    for (uint8_t i = 0; i < VBUS_FRAME_SIZE - 2; i++) {
        if (septet & (1 << i)) {
            _payload[i] = _buffer[i] | 0x80;
        } else {
            _payload[i] = _buffer[i];
        }
    }
}


// for the future
// uint8_t VBusBuffer::calcFrameCRC()
// {
//     uint8_t crc = 0x7F;
//     for (size_t i = 0; i < VBUS_FRAME_SIZE; i++) {
//         crc = (crc - _buffer [i]) & 0x7F;
//     }
//     return crc;
// }
