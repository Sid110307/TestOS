#include "include/interrupts.h"

void IDTEntry::setOffset(size_t offset)
{
    offsetLow = static_cast<unsigned short>(offset & 0x000000000000FFFF);
    offsetMid = static_cast<unsigned short>((offset & 0x00000000FFFF0000) >> 16);
    offsetHigh = static_cast<unsigned int>((offset & 0xFFFFFFFF00000000) >> 32);
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
