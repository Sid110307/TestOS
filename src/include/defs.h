#pragma once

typedef unsigned long long size_t;

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
        size_t byteIndex = index / 8;
        unsigned char bitIndex = index % 8, bitIndexer = 0B10000000 >> bitIndex;

        return (buffer[byteIndex] & bitIndexer) > 0;
    }

    void set(size_t index, bool value) const
    {
        size_t byteIndex = index / 8;
        unsigned char bitIndex = index % 8, bitIndexer = 0B10000000 >> bitIndex;

        value ? buffer[byteIndex] |= bitIndexer : buffer[byteIndex] &= ~bitIndexer;
    }
};

namespace Memory
{
    static inline void *memset(void *ptr, int value, size_t size)
    {
        for (size_t i = 0; i < size; ++i) reinterpret_cast<unsigned char *>(ptr)[i] = value;
        return ptr;
    }

    static inline void *memcpy(void *destination, const void *source, size_t size)
    {
        for (size_t i = 0; i < size; ++i)
            reinterpret_cast<unsigned char *>(destination)[i] = reinterpret_cast<const unsigned char *>(source)[i];
        return destination;
    }

    static inline int memcmp(const void *a, const void *b, size_t size)
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
[[maybe_unused]] static const char *toString(size_t value, size_t base = 10)
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
