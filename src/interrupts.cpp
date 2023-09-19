#include "include/interrupts.h"

void IDTEntry::setOffset(size_t offset)
{
    offsetLow = offset & 0xFFFF;
    offsetMid = offset >> 16 & 0xFFFF;
    offsetHigh = offset >> 32 & 0xFFFFFFFF;
}

size_t IDTEntry::getOffset() const
{
    return static_cast<size_t>(offsetLow) | static_cast<size_t>(offsetMid) << 16 |
           static_cast<size_t>(offsetHigh) << 32;
}

void PortIO::outPort(unsigned short port, unsigned char data) { asm volatile("outb %0, %1" : : "a"(data), "Nd"(port)); }
unsigned char PortIO::inPort(unsigned short port)
{
    unsigned char data;
    asm volatile("inb %1, %0" : "=a"(data) : "Nd"(port));

    return data;
}
