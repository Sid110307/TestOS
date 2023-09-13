#pragma once

#include "colors.h"

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

    void putPixel(unsigned int x, unsigned int y, Colors color);
    void putChar(unsigned int x, unsigned int y, Colors color, char c);
    void putString(unsigned int x, unsigned int y, Colors color, const char *str);
    void putNumber(unsigned int x, unsigned int y, Colors color, unsigned long long n);

    void clear(Colors color);
    static void delay(unsigned int ms);

    void drawRect(unsigned int x, unsigned int y, unsigned int width, unsigned int height, Colors color);
    void fillRect(unsigned int x, unsigned int y, unsigned int width, unsigned int height, Colors color);

    void drawLine(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, Colors color);

    void drawCircle(unsigned int x, unsigned int y, unsigned int radius, Colors color);
    void fillCircle(unsigned int x, unsigned int y, unsigned int radius, Colors color);

private:
    Framebuffer *framebuffer;
    PSF1Font *font;
};
