#ifndef _VBUSBUFFER_H_
#define _VBUSBUFFER_H_

#include <Arduino.h>
#include <PrintEx.h>
#include <VBusBufferState.h>


#define VBUS_BUFFER_SIZE 200
#define VBUS_HEADER_SIZE 10
#define VBUS_PAYLOAD_SIZE 68
#define VBUS_FRAME_COUNT 17

typedef void(* onVBusDataFrameReceived)(const uint8_t frameIndex, const uint8_t data[]);

class VBusBuffer {
public:
    VBusBuffer(onVBusDataFrameReceived);
    void push(const uint8_t data);

private:
    uint8_t _buffer[VBUS_BUFFER_SIZE];
    uint8_t _payload[VBUS_PAYLOAD_SIZE];
    uint8_t _pos;
    PrintEx _printex;
    enum VBusBufferState _state;
    static const uint8_t _headerMatch[VBUS_HEADER_SIZE];
    uint8_t _frameIndex;

    void writeBuffer(const uint8_t data);
    void readHeader(const uint8_t data);
    void readFrame(const uint8_t data);
    void reset();
    void resetBuffer();
    uint8_t calcFrameCRC();
    void parseFrame();
    onVBusDataFrameReceived _onFrameReceived;
};


#endif
