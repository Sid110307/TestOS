#pragma once

#include "defs.h"
#include "renderer.h"

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

struct InterruptFrame
{
    size_t instructionPointer, codeSegment;
    size_t cpuFlags, stackPointer, stackSegment;
};

class InterruptManager
{
public:
    static void initialize();
    static void registerHandler(unsigned char interrupt, void (*handler)(InterruptFrame *frame));

    static void enableInterrupts();
    static void disableInterrupts();

private:
    static IDTEntry idt[IDT_SIZE];
    static void (*handlers[IDT_SIZE])(InterruptFrame *frame);

    static void outPort(unsigned short port, unsigned char data);
    static unsigned char inPort(unsigned short port);

    static void interruptHandler(InterruptFrame *frame);
};
