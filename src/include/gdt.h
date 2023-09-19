#pragma once

#include "defs.h"

struct GDTDescriptor
{
    unsigned short size;
    unsigned long offset;
} __attribute__((packed));

struct GDTEntry
{
    unsigned short limit, baseLow;
    unsigned char baseMiddle, access, granularity, baseHigh;
} __attribute__((packed));

struct GDT
{
    GDTEntry nullEntry, kernelCodeSegment, kernelDataSegment, userCodeSegment, userDataSegment, userNullSegment;
} __attribute__((packed)) __attribute__((aligned(PAGE_SIZE)));

extern "C" void loadGDT(GDTDescriptor *gdtDescriptor);
static GDT globalGDT = {
        {0,      0, 0, 0,          0,          0},
        {0xFFFF, 0, 0, 0b10011010, 0b00100000, 0},
        {0xFFFF, 0, 0, 0b10010010, 0b00000000, 0},
        {0xFFFF, 0, 0, 0b11111010, 0b00100000, 0},
        {0xFFFF, 0, 0, 0b11110010, 0b00000000, 0},
        {0,      0, 0, 0,          0,          0}
};
