#pragma once

typedef unsigned long long size_t;

constexpr size_t FRAMEBUFFER_INDENT = 4;
constexpr size_t PAGE_SIZE = 4096, PAGE_TABLE_SIZE = 512;
constexpr size_t IDT_INTERRUPT_GATE = 0x8E, IDT_CALL_GATE = 0x8C, IDT_TRAP_GATE = 0x8F;
constexpr size_t PIC1_COMMAND = 0x20, PIC1_DATA = 0x21, PIC2_COMMAND = 0xA0, PIC2_DATA = 0xA1, PIC_EOI = 0x20;
constexpr size_t ICW1_ICW4 = 0x01, ICW1_INIT = 0x10, ICW4_8086 = 0x01;
constexpr size_t IDT_SIZE = 256;
constexpr size_t KERNEL_CS = 0x08;

enum Colors
{
    Black = 0x000000,
    White = 0xFFFFFF,
    Red = 0xFF0000,
    Green = 0x00FF00,
    Blue = 0x0000FF,
    Yellow = 0xFFFF00,
    Cyan = 0x00FFFF,
    Magenta = 0xFF00FF,
    Gray = 0x808080,
    DarkGray = 0x404040,
    LightGray = 0xC0C0C0,
    DarkRed = 0x800000,
    DarkGreen = 0x008000,
    DarkBlue = 0x000080,
    DarkYellow = 0x808000,
    DarkCyan = 0x008080,
    DarkMagenta = 0x800080,
    Transparent = 0x00000000
};

struct Bitmap
{
    unsigned char *buffer;
    size_t size;

    bool operator[](size_t index) const
    {
        if (index > size * 8) return false;

        size_t byteIndex = index / 8;
        unsigned char bitIndex = index % 8, bitIndexer = 0B10000000 >> bitIndex;

        return (buffer[byteIndex] & bitIndexer) > 0;
    }

    bool set(size_t index, bool value) const
    {
        if (index > size * 8) return false;

        size_t byteIndex = index / 8;
        unsigned char bitIndex = index % 8, bitIndexer = 0B10000000 >> bitIndex;

        if (value) buffer[byteIndex] |= bitIndexer;
        else buffer[byteIndex] &= ~bitIndexer;

        return value;
    };
};

namespace Memory
{
    [[maybe_unused]] static inline void *memset(void *ptr, int value, size_t size)
    {
        for (size_t i = 0; i < size; ++i) reinterpret_cast<unsigned char *>(ptr)[i] = value;
        return ptr;
    }

    [[maybe_unused]] static inline void *memcpy(void *destination, const void *source, size_t size)
    {
        for (size_t i = 0; i < size; ++i)
            reinterpret_cast<unsigned char *>(destination)[i] = reinterpret_cast<const unsigned char *>(source)[i];
        return destination;
    }

    [[maybe_unused]] static inline int memcmp(const void *a, const void *b, size_t size)
    {
        for (size_t i = 0; i < size; ++i)
        {
            if (reinterpret_cast<const unsigned char *>(a)[i] > reinterpret_cast<const unsigned char *>(b)[i]) return 1;
            else if (reinterpret_cast<const unsigned char *>(a)[i] < reinterpret_cast<const unsigned char *>(b)[i])
                return -1;
        }

        return 0;
    }
}

static char buffer[sizeof(size_t) * 8 + 1];
[[maybe_unused]] __attribute__((no_caller_saved_registers)) static const char *toString(size_t value, size_t base = 10)
{
    char *ptr = &buffer[sizeof(buffer) - 1];
    *ptr = '\0';

    do
    {
        *--ptr = "0123456789ABCDEF"[value % base];
        value /= base;
    } while (value != 0);

    return ptr;
}
