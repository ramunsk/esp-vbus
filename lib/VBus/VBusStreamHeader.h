#ifndef _VBUSSTREAMHEADER_H_
#define _VBUSSTREAMHEADER_H_

#include <stdint.h>
#include <stdio.h>
#include <Print.h>

typedef struct {
    uint16_t srcAddress = 0;
    uint16_t destAddress = 0;
    uint8_t version = 0;
} VBusStreamHeader;

#endif
