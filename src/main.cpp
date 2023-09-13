#include "include/renderer.h"

struct BootInfo
{
    PSF1Font *font;
    Framebuffer *framebuffer;
};

extern "C" int main(BootInfo *bootInfo)
{
    Renderer renderer(bootInfo->framebuffer, bootInfo->font);

    renderer.clear(Colors::Cyan);
    renderer.putString(0, 0, Colors::White, "Hello, World!");

    renderer.fillCircle(bootInfo->framebuffer->width / 2, bootInfo->framebuffer->height / 2, 100, Colors::Red);
    renderer.drawRect(bootInfo->framebuffer->width / 2 - 50, bootInfo->framebuffer->height / 2 - 50, 100, 100,
                      Colors::Green);

    renderer.putString(0, 16, Colors::White, "Width: ");
    renderer.putNumber(56, 16, Colors::White, bootInfo->framebuffer->width);
    renderer.putString(0, 32, Colors::White, "Height: ");
    renderer.putNumber(64, 32, Colors::White, bootInfo->framebuffer->height);
    renderer.putString(0, 48, Colors::White, "Max text width: ");
    renderer.putNumber(128, 48, Colors::White, bootInfo->framebuffer->width / 8);
    renderer.putString(0, 64, Colors::White, "Max text height: ");
    renderer.putNumber(136, 64, Colors::White, bootInfo->framebuffer->height / 16);

    for (unsigned int i = 0; i < bootInfo->framebuffer->width / 8; ++i)
    {
        renderer.putChar(i * 8, bootInfo->framebuffer->height - 16, Colors::White, '0');
        Renderer::delay(10);
    }

    renderer.clear(Colors::DarkBlue);
    while (true);
}
