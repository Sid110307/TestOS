#include "include/renderer.h"
#include "include/memory.h"

struct BootInfo
{
    PSF1Font *font;
    Framebuffer *framebuffer;
    MemoryDescriptor *memoryMap;
    size_t memoryMapSize, memoryMapDescriptorSize;
};

extern size_t _kernelStart, _kernelEnd;

extern "C" int main(BootInfo *bootInfo)
{
    Renderer renderer(bootInfo->framebuffer, bootInfo->font);
    renderer.clear(Colors::Black);

    globalAllocator = PageFrameAllocator();
    globalAllocator.readMemoryMap(bootInfo->memoryMap, bootInfo->memoryMapSize, bootInfo->memoryMapDescriptorSize);

    size_t kernelSize = reinterpret_cast<size_t>(&_kernelEnd) - reinterpret_cast<size_t>(&_kernelStart),
            kernelPages = kernelSize / PAGE_SIZE + 1;
    globalAllocator.lockPages(&_kernelStart, kernelPages);

    auto *pageTable = reinterpret_cast<PageTable *>(globalAllocator.requestPage());
    Memory::memset(pageTable, 0, PAGE_SIZE);

    PageTableManager pageTableManager(pageTable);
    for (size_t i = 0; i < PageFrameAllocator::getMemorySize(bootInfo->memoryMap, bootInfo->memoryMapSize,
                                                             bootInfo->memoryMapDescriptorSize); i += PAGE_SIZE)
        pageTableManager.mapMemory(reinterpret_cast<void *>(i), reinterpret_cast<void *>(i));

    auto framebufferBase = reinterpret_cast<size_t>(bootInfo->framebuffer->baseAddress);
    auto framebufferSize = bootInfo->framebuffer->bufferSize + PAGE_SIZE;

    for (size_t i = framebufferBase; i < framebufferBase + framebufferSize; i += PAGE_SIZE)
        pageTableManager.mapMemory(reinterpret_cast<void *>(i), reinterpret_cast<void *>(i));

    asm volatile("mov %0, %%cr3" : : "r"(pageTable));
    pageTableManager.mapMemory(reinterpret_cast<void *>(0x600000000), reinterpret_cast<void *>(0x80000));

    auto *test = reinterpret_cast<size_t *>(0x600000000);
    *test = 0xDEADBEEF;
    renderer.putString(toString(*test, 16), Colors::White);

    while (true);
}
