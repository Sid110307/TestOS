#pragma once

#include "defs.h"
#include "renderer.h"
#include "memory.h"

struct IDTDescriptor
{
    unsigned short limit;
    size_t offset;
} __attribute__((packed));

struct IDTEntry
{
    unsigned short offsetLow, offsetMid, selector;
    unsigned char ist, typeAttr;
    unsigned int offsetHigh, zero;

    void setOffset(size_t offset);
    size_t getOffset() const;
};

namespace PortIO
{
    void outPort(unsigned short port, unsigned char data);
    unsigned char inPort(unsigned short port);
};
