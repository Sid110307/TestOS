#pragma once

#include "defs.h"

#define INDENT 4

typedef struct
{
    unsigned char magic[2], mode, charSize;
} PSF1Header;

typedef struct
{
    PSF1Header *fontHeader;
    void *glyphBuffer;
} PSF1Font;

typedef struct
{
    unsigned int *baseAddress;
    unsigned int bufferSize;
    unsigned int width;
    unsigned int height;
    unsigned int pixelsPerScanLine;
} Framebuffer;

class Renderer
{
public:
    Renderer(Framebuffer *framebuffer, PSF1Font *font);

    void putPixel(unsigned int x, unsigned int y, unsigned int color);
    void putChar(char c, unsigned int color);
    void putString(const char *str, unsigned int color);
    void putNumber(size_t n, unsigned int color);

    void clear(unsigned int color);
    static void delay(unsigned int ms);

    void drawRect(unsigned int width, unsigned int height, unsigned int color);
    void fillRect(unsigned int width, unsigned int height, unsigned int color);

    void drawLine(unsigned int destX, unsigned int destY, unsigned int color);

    void drawCircle(unsigned int radius, unsigned int color);
    void fillCircle(unsigned int radius, unsigned int color);

    struct CursorPosition
    {
        unsigned int x, y;
    } cursorPosition;

private:
    Framebuffer *framebuffer;
    PSF1Font *font;
};
