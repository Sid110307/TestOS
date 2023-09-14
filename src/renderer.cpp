#include "include/renderer.h"

Renderer::Renderer(Framebuffer *framebuffer, PSF1Font *font)
        : cursorPosition({0, 0}), framebuffer(framebuffer), font(font) {}

void Renderer::putPixel(unsigned int x, unsigned int y, unsigned int color)
{
    if (x >= framebuffer->width || y >= framebuffer->height) return;

    unsigned int *pixel = framebuffer->baseAddress + y * framebuffer->pixelsPerScanLine + x;
    *pixel = color;
}

void Renderer::putChar(char c, unsigned int color)
{
    switch (c)
    {
        case '\n':
            cursorPosition.x = 0;
            cursorPosition.y += font->fontHeader->charSize;
            break;
        case '\r':
            cursorPosition.x = 0;
            break;
        case '\t':
            cursorPosition.x += INDENT * 8;
            break;
        case '\b':
            cursorPosition.x -= 8;
            break;
        default:
        {
            unsigned char *glyph = static_cast<unsigned char *>(font->glyphBuffer) + (c * font->fontHeader->charSize);

            for (unsigned int i = 0; i < font->fontHeader->charSize; ++i)
            {
                for (unsigned int j = 0; j < 8; ++j)
                    if ((*glyph & 0B10000000 >> j) > 0) putPixel(cursorPosition.x + j, cursorPosition.y + i, color);
                glyph++;
            }

            cursorPosition.x += 8;
            break;
        }
    }

    if (cursorPosition.x >= framebuffer->width)
    {
        cursorPosition.x = 0;
        cursorPosition.y += font->fontHeader->charSize;
    }
}

void Renderer::putString(const char *str, unsigned int color)
{
    while (*str != '\0')
    {
        putChar(*str, color);
        str++;
    }
}

void Renderer::putNumber(unsigned long long n, unsigned int color)
{
    if (n == 0)
    {
        putChar('0', color);
        return;
    }

    while (n > 0)
    {
        putChar(static_cast<char>(n % 10 + '0'), color);
        n /= 10;
    }
}

void Renderer::clear(unsigned int color)
{
    for (unsigned int y = 0; y < framebuffer->height; ++y)
        for (unsigned int x = 0; x < framebuffer->width; ++x) putPixel(x, y, color);
}

// TODO: Only delay the renderer, not the whole system
void Renderer::delay(unsigned int ms)
{
    for (unsigned int i = 0; i < ms; ++i) for (unsigned int j = 0; j < 500000; ++j) asm volatile("nop");
}

void Renderer::drawRect(unsigned int width, unsigned int height, unsigned int color)
{
    for (unsigned int i = 0; i < width; ++i)
    {
        putPixel(cursorPosition.x + i, cursorPosition.y, color);
        putPixel(cursorPosition.x + i, cursorPosition.y + height - 1, color);
    }

    for (unsigned int i = 0; i < height; ++i)
    {
        putPixel(cursorPosition.x, cursorPosition.y + i, color);
        putPixel(cursorPosition.x + width - 1, cursorPosition.y + i, color);
    }
}

void Renderer::fillRect(unsigned int width, unsigned int height, unsigned int color)
{
    for (unsigned int i = 0; i < height; ++i)
        for (unsigned int j = 0; j < width; ++j) putPixel(cursorPosition.x + j, cursorPosition.y + i, color);
}

void Renderer::drawLine(unsigned int destX, unsigned int destY, unsigned int color)
{
    int dx = static_cast<int>(destX - cursorPosition.x), dy = static_cast<int>(destY - cursorPosition.y), temp;

    if (dx < 0)
    {
        temp = static_cast<int>(cursorPosition.x);
        cursorPosition.x = destX;
        destX = temp;
        cursorPosition.y = destY;

        dx = -dx;
        dy = -dy;
    }

    int x = static_cast<int>(cursorPosition.x), y = static_cast<int>(cursorPosition.y), p = 2 * dy - dx;

    while (x <= static_cast<int>(destX))
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

void Renderer::drawCircle(unsigned int radius, unsigned int color)
{
    int x0 = 0, y0 = static_cast<int>(radius), d = 3 - 2 * static_cast<int>(radius);

    while (x0 <= y0)
    {
        putPixel(cursorPosition.x + x0, cursorPosition.y + y0, color);
        putPixel(cursorPosition.x + x0, cursorPosition.y - y0, color);
        putPixel(cursorPosition.x - x0, cursorPosition.y + y0, color);
        putPixel(cursorPosition.x - x0, cursorPosition.y - y0, color);
        putPixel(cursorPosition.x + y0, cursorPosition.y + x0, color);
        putPixel(cursorPosition.x + y0, cursorPosition.y - x0, color);
        putPixel(cursorPosition.x - y0, cursorPosition.y + x0, color);
        putPixel(cursorPosition.x - y0, cursorPosition.y - x0, color);

        if (d < 0) d += 4 * x0 + 6;
        else
        {
            d += 4 * (x0 - y0) + 10;
            y0--;
        }

        x0++;
    }
}

void Renderer::fillCircle(unsigned int radius, unsigned int color)
{
    int x0 = 0, y0 = static_cast<int>(radius), d = 3 - 2 * static_cast<int>(radius);

    while (x0 <= y0)
    {
        drawLine(cursorPosition.x - x0, cursorPosition.y + y0, color);
        drawLine(cursorPosition.x - x0, cursorPosition.y - y0, color);
        drawLine(cursorPosition.x + x0, cursorPosition.y + y0, color);
        drawLine(cursorPosition.x + x0, cursorPosition.y - y0, color);
        drawLine(cursorPosition.x - y0, cursorPosition.y + x0, color);
        drawLine(cursorPosition.x - y0, cursorPosition.y - x0, color);
        drawLine(cursorPosition.x + y0, cursorPosition.y + x0, color);
        drawLine(cursorPosition.x + y0, cursorPosition.y - x0, color);

        if (d < 0) d += 4 * x0 + 6;
        else
        {
            d += 4 * (x0 - y0) + 10;
            y0--;
        }

        x0++;
    }
}
