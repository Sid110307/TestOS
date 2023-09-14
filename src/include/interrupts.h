#pragma once

#include "defs.h"

#define IDT_INTERRUPT_GATE 0x8E
#define IDT_CALL_GATE 0x8C
#define IDT_TRAP_GATE 0x8F

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1
#define PIC_EOI 0x20

#define IRQ0 0x20
#define IRQ1 0x21
#define IRQ12 0x2C

#define IDT_SIZE 256
#define KERNEL_CS 0x08

struct IDTDescriptor
{
    unsigned short limit;
    size_t offset;
};

struct IDTEntry
{
    unsigned short offsetLow, offsetMid, selector;
    unsigned char ist, typeAttr;
    unsigned int offsetHigh, zero;

    void setOffset(size_t offset);
    size_t getOffset();
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
    static void irq0Handler(InterruptFrame *frame);
    static void irq1Handler(InterruptFrame *frame);
    static void irq12Handler(InterruptFrame *frame);
};
