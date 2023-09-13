#include "include/renderer.h"

Renderer::Renderer(Framebuffer *framebuffer, PSF1Font *font)
{
    this->framebuffer = framebuffer;
    this->font = font;
}

void Renderer::putPixel(unsigned int x, unsigned int y, Colors color)
{
    if (x >= framebuffer->width || y >= framebuffer->height) return;

    unsigned int *pixel = framebuffer->baseAddress + y * framebuffer->pixelsPerScanLine + x;
    *pixel = color;
}

void Renderer::putChar(unsigned int x, unsigned int y, Colors color, char c)
{
    unsigned char *glyph = static_cast<unsigned char *>(font->glyphBuffer) + (c * font->fontHeader->charSize);

    for (unsigned int i = 0; i < font->fontHeader->charSize; ++i)
    {
        for (unsigned int j = 0; j < 8; ++j) if ((*glyph & 0B10000000 >> j) > 0) putPixel(x + j, y + i, color);
        glyph++;
    }
}

void Renderer::putString(unsigned int x, unsigned int y, Colors color, const char *str)
{
    unsigned int xOffset = 0;

    while (*str != '\0')
    {
        putChar(x + xOffset, y, color, *str);

        xOffset += 8;
        str++;
    }
}

void Renderer::putNumber(unsigned int x, unsigned int y, Colors color, unsigned long long n)
{
    unsigned int xOffset = 0;

    if (n == 0)
    {
        putChar(x, y, color, '0');
        return;
    }

    while (n > 0)
    {
        putChar(x + xOffset, y, color, static_cast<char>(n % 10 + '0'));

        xOffset += 8;
        n /= 10;
    }
}

void Renderer::clear(Colors color)
{
    for (unsigned int y = 0; y < framebuffer->height; ++y)
        for (unsigned int x = 0; x < framebuffer->width; ++x) putPixel(x, y, color);
}

void Renderer::delay(unsigned int ms)
{
    for (unsigned int i = 0; i < ms; ++i) for (unsigned int j = 0; j < 1000000; ++j) __asm__("nop");
}

void Renderer::drawRect(unsigned int x, unsigned int y, unsigned int width, unsigned int height, Colors color)
{
    for (unsigned int i = 0; i < width; ++i)
    {
        putPixel(x + i, y, color);
        putPixel(x + i, y + height - 1, color);
    }

    for (unsigned int i = 0; i < height; ++i)
    {
        putPixel(x, y + i, color);
        putPixel(x + width - 1, y + i, color);
    }
}

void Renderer::fillRect(unsigned int x, unsigned int y, unsigned int width, unsigned int height, Colors color)
{
    for (unsigned int i = 0; i < height; ++i) for (unsigned int j = 0; j < width; ++j) putPixel(x + j, y + i, color);
}

void Renderer::drawLine(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, Colors color)
{
    int dx = static_cast<int>(x1 - x0), dy = static_cast<int>(y1 - y0), temp;

    if (dx < 0)
    {
        temp = static_cast<int>(x0);
        x0 = x1;
        x1 = temp;
        y0 = y1;

        dx = -dx;
        dy = -dy;
    }

    int x = static_cast<int>(x0), y = static_cast<int>(y0), p = 2 * dy - dx;

    while (x <= static_cast<int>(x1))
    {
        putPixel(x, y, color);

        if (p >= 0)
        {
            y++;
            p += 2 * dy - 2 * dx;
        } else p += 2 * dy;

        x++;
    }
}

void Renderer::drawCircle(unsigned int x, unsigned int y, unsigned int radius, Colors color)
{
    int x0 = 0, y0 = static_cast<int>(radius), d = 3 - 2 * static_cast<int>(radius);

    while (x0 <= y0)
    {
        putPixel(x + x0, y + y0, color);
        putPixel(x + x0, y - y0, color);
        putPixel(x - x0, y + y0, color);
        putPixel(x - x0, y - y0, color);
        putPixel(x + y0, y + x0, color);
        putPixel(x + y0, y - x0, color);
        putPixel(x - y0, y + x0, color);
        putPixel(x - y0, y - x0, color);

        if (d < 0) d += 4 * x0 + 6;
        else
        {
            d += 4 * (x0 - y0) + 10;
            y0--;
        }

        x0++;
    }
}

void Renderer::fillCircle(unsigned int x, unsigned int y, unsigned int radius, Colors color)
{
    int x0 = 0, y0 = static_cast<int>(radius), d = 3 - 2 * static_cast<int>(radius);

    while (x0 <= y0)
    {
        drawLine(x - x0, y + y0, x + x0, y + y0, color);
        drawLine(x - x0, y - y0, x + x0, y - y0, color);
        drawLine(x - y0, y + x0, x + y0, y + x0, color);
        drawLine(x - y0, y - x0, x + y0, y - x0, color);

        if (d < 0) d += 4 * x0 + 6;
        else
        {
            d += 4 * (x0 - y0) + 10;
            y0--;
        }

        x0++;
    }
}
