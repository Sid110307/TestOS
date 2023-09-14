#include "include/renderer.h"
#include "include/memory.h"

struct BootInfo
{
    PSF1Font *font;
    Framebuffer *framebuffer;
    MemoryDescriptor *memoryMap;
    size_t memoryMapSize, memoryMapDescriptorSize;
};

extern "C" int main(BootInfo *bootInfo)
{
    Renderer renderer(bootInfo->framebuffer, bootInfo->font);
    size_t memoryMapEntries = bootInfo->memoryMapSize / bootInfo->memoryMapDescriptorSize;

    renderer.clear(Colors::Black);
    renderer.putString("Hello, World!\n\n", Colors::White);
//    renderer.putString("Memory Map:\n", Colors::White);

//    for (size_t i = 0; i < memoryMapEntries; ++i)
//    {
//        auto *desc = reinterpret_cast<MemoryDescriptor *>(reinterpret_cast<size_t>(bootInfo->memoryMap) +
//                                                          i * bootInfo->memoryMapDescriptorSize);
//
//        renderer.putString(memoryTypes[desc->type], Colors::White);
//        renderer.putString(": ", Colors::White);
//        renderer.putNumber(desc->numberOfPages * 4096 / 1024, Colors::White);
//        renderer.putString(" KB\n", Colors::White);
//    }

    renderer.putString("\nMemory Size: ", Colors::White);
    renderer.putNumber(
            MemoryManager::getMemorySize(bootInfo->memoryMap, memoryMapEntries, bootInfo->memoryMapDescriptorSize) /
            1024 / 1024, Colors::White);
    renderer.putString(" MB\n", Colors::White);

    Renderer::delay(5000);
    renderer.clear(Colors::DarkGray);
    renderer.cursorPosition = {0, 0};

    for (unsigned int i = 0; i < bootInfo->framebuffer->width / 8 * bootInfo->framebuffer->height / 16; ++i)
        renderer.putChar('0', i);

    while (true);
}
