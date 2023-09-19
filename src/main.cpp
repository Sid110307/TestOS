#include "include/renderer.h"
#include "include/memory.h"
#include "include/gdt.h"

struct BootInfo
{
    PSF1Font *font;
    Framebuffer *framebuffer;
    MemoryDescriptor *memoryMap;
    size_t memoryMapSize, memoryMapDescriptorSize;
};

struct Kernel
{
    Renderer renderer;
    PageTableManager pageTableManager;
    PageFrameAllocator pageFrameAllocator;
    GDTDescriptor gdtDescriptor;
};

extern size_t _kernelStart, _kernelEnd;

Kernel initKernel(BootInfo *bootInfo)
{
    GDTDescriptor gdtDescriptor = {sizeof(GDT) - 1, reinterpret_cast<size_t>(&globalGDT)};
    loadGDT(&gdtDescriptor);

    globalRenderer = Renderer(bootInfo->framebuffer, bootInfo->font);

    globalAllocator = PageFrameAllocator();
    globalAllocator.readMemoryMap(bootInfo->memoryMap, bootInfo->memoryMapSize, bootInfo->memoryMapDescriptorSize);

    size_t kernelSize = reinterpret_cast<size_t>(&_kernelEnd) - reinterpret_cast<size_t>(&_kernelStart),
            kernelPages = kernelSize / PAGE_SIZE + 1;
    globalAllocator.lockPages(&_kernelStart, kernelPages);

    auto *pageTable = reinterpret_cast<PageTable *>(globalAllocator.requestPage());
    Memory::memset(pageTable, 0, PAGE_SIZE);

    PageTableManager pageTableManager(pageTable);
    for (size_t i = 0; i < PageFrameAllocator::getMemorySize(bootInfo->memoryMap, bootInfo->memoryMapSize,
                                                             bootInfo->memoryMapDescriptorSize) / PAGE_SIZE; ++i)
        pageTableManager.mapMemory(reinterpret_cast<void *>(i * PAGE_SIZE), reinterpret_cast<void *>(i * PAGE_SIZE));

    auto framebufferBase = reinterpret_cast<size_t>(bootInfo->framebuffer->baseAddress);
    auto framebufferSize = bootInfo->framebuffer->bufferSize + PAGE_SIZE;

    globalAllocator.lockPages(reinterpret_cast<void *>(framebufferBase), framebufferSize / PAGE_SIZE + 1);
    for (size_t i = 0; i < framebufferSize; i += PAGE_SIZE)
        pageTableManager.mapMemory(reinterpret_cast<void *>(framebufferBase + i),
                                   reinterpret_cast<void *>(framebufferBase + i));

    asm volatile("mov %0, %%cr3" : : "r"(pageTable));
    Memory::memset(bootInfo->framebuffer->baseAddress, 0, bootInfo->framebuffer->bufferSize);

    return {globalRenderer, pageTableManager, globalAllocator, gdtDescriptor};
}

extern "C" void main(BootInfo *bootInfo)
{
    auto kernel = initKernel(bootInfo);

    kernel.renderer.clear(Colors::Black);
    kernel.renderer.putString("Hello, World!", Colors::White);

    while (true);
}
