#include "include/renderer.h"
#include "include/memory.h"

struct BootInfo
{
    PSF1Font *font;
    Framebuffer *framebuffer;
    MemoryDescriptor *memoryMap;
    size_t memoryMapSize, memoryMapDescriptorSize;
};

unsigned char bloat[20];

extern "C" int main(BootInfo *bootInfo)
{
    Renderer renderer(bootInfo->framebuffer, bootInfo->font);
    renderer.clear(Colors::Black);

    PageFrameAllocator allocator;
    allocator.readMemoryMap(bootInfo->memoryMap, bootInfo->memoryMapSize, bootInfo->memoryMapDescriptorSize);

    renderer.putString("Free memory: ", Colors::White);
    renderer.putString(toString(allocator.getFreeRAM() / 1024), Colors::White);
    renderer.putString(" KB\n", Colors::White);
    renderer.putString("Used memory: ", Colors::White);
    renderer.putString(toString(allocator.getUsedRAM() / 1024), Colors::White);
    renderer.putString(" KB\n", Colors::White);
    renderer.putString("Reserved memory: ", Colors::White);
    renderer.putString(toString(allocator.getReservedRAM() / 1024), Colors::White);
    renderer.putString(" KB\n", Colors::White);

    while (true);
}
