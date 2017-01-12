#ifndef _VBUSBUFFER_H_
#define _VBUSBUFFER_H_

#include <VBusBufferState.h>

#define VBUS_BUFFER_SIZE 10
#define VBUS_HEADER_SIZE 10
#define VBUS_FRAME_SIZE 6
#define VBUS_PAYLOAD_SIZE 4

typedef void(* onVBusDataFrameReceived)(const uint8_t frameIndex, const uint8_t data[]);

class VBusBuffer {
public:
    VBusBuffer();
    void push(const uint8_t data);
    void onFrameReceived(onVBusDataFrameReceived);


private:
    static const uint8_t _headerMatch[VBUS_HEADER_SIZE];    // header of the stream to be parsed
    uint8_t _buffer[VBUS_BUFFER_SIZE];                      // buffer to store bytes from VBUS
    uint8_t _payload[VBUS_PAYLOAD_SIZE];                    // buffer to store data from VBus frames
    uint8_t _pos;                                           // current buffer position
    uint8_t _frameIndex;                                    // current data frame
    enum VBusBufferState _state;

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
