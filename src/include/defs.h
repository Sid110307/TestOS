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

static char buffer[sizeof(size_t) * 8 + 1];
static const char *toString(size_t value, size_t base = 10)
{
    size_t i = 0;
    while (value > 0)
    {
        size_t digit = value % base;

        buffer[i++] = static_cast<char>(digit < 10 ? '0' + digit : 'A' + digit - 10);
        value /= base;
    }

    buffer[i] = '\0';
    for (size_t j = 0; j < i / 2; ++j)
    {
        char temp = buffer[j];

        buffer[j] = buffer[i - j - 1];
        buffer[i - j - 1] = temp;
    }

    return buffer;
}
