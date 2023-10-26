#pragma once

#include "renderer.h"
#include "memory.h"
#include "interrupts.h"
#include "interruptHandlers.h"
#include "gdt.h"

struct BootInfo
{
    PSF1Font *font;
    Framebuffer *framebuffer;
    MemoryDescriptor *memoryMap;
    size_t memoryMapSize, memoryMapDescriptorSize;
};

struct Kernel
{
    Renderer *renderer;
    PageTableManager pageTableManager;
    PageFrameAllocator pageFrameAllocator;
    GDTDescriptor gdtDescriptor;
};

extern size_t _kernelStart, _kernelEnd;
Renderer renderer(nullptr, nullptr);
PageTableManager pageTableManager(nullptr);
IDTDescriptor descriptor;

void registerHandler(unsigned char interrupt, void *handler)
{
    auto *entry = reinterpret_cast<IDTEntry *>(descriptor.offset + interrupt * sizeof(IDTEntry));

    entry->setOffset(reinterpret_cast<size_t>(handler));
    entry->typeAttr = IDT_INTERRUPT_GATE;
    entry->selector = KERNEL_CS;
}

//void remapPIC()
//{
//    auto masterMask = PortIO::inPort(PIC1_DATA), slaveMask = PortIO::inPort(PIC2_DATA);
//
//    PortIO::outPort(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
//    PortIO::outPort(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
//
//    PortIO::outPort(PIC1_DATA, 0x20);
//    PortIO::outPort(PIC2_DATA, 0x28);
//
//    PortIO::outPort(PIC1_DATA, 0x04);
//    PortIO::outPort(PIC2_DATA, 0x02);
//
//    PortIO::outPort(PIC1_DATA, ICW4_8086);
//    PortIO::outPort(PIC2_DATA, ICW4_8086);
//
//    PortIO::outPort(PIC1_DATA, masterMask);
//    PortIO::outPort(PIC2_DATA, slaveMask);
//}

Kernel initKernel(BootInfo *bootInfo)
{
    renderer = Renderer(bootInfo->framebuffer, bootInfo->font);
    globalRenderer = &renderer;

    GDTDescriptor gdtDescriptor = {sizeof(GDT) - 1, reinterpret_cast<size_t>(&globalGDT)};
    loadGDT(&gdtDescriptor);

    globalAllocator = PageFrameAllocator();
    globalAllocator.readMemoryMap(bootInfo->memoryMap, bootInfo->memoryMapSize, bootInfo->memoryMapDescriptorSize);

    size_t kernelSize = reinterpret_cast<size_t>(&_kernelEnd) - reinterpret_cast<size_t>(&_kernelStart),
            kernelPages = kernelSize / PAGE_SIZE + 1;
    globalAllocator.lockPages(&_kernelStart, kernelPages);

    auto *pageTable = reinterpret_cast<PageTable *>(globalAllocator.requestPage());
    Memory::memset(pageTable, 0, PAGE_SIZE);

    pageTableManager = PageTableManager(pageTable);
    for (size_t i = 0; i < PageFrameAllocator::getMemorySize(
            bootInfo->memoryMap, bootInfo->memoryMapSize / bootInfo->memoryMapDescriptorSize,
            bootInfo->memoryMapDescriptorSize); i += PAGE_SIZE)
        pageTableManager.mapMemory(reinterpret_cast<void *>(i), reinterpret_cast<void *>(i));

    auto framebufferBase = reinterpret_cast<size_t>(bootInfo->framebuffer->baseAddress);
    auto framebufferSize = bootInfo->framebuffer->bufferSize + PAGE_SIZE;

    globalAllocator.lockPages(reinterpret_cast<void *>(framebufferBase), framebufferSize / PAGE_SIZE + 1);
    for (size_t i = framebufferBase; i < framebufferBase + framebufferSize; i += PAGE_SIZE)
        pageTableManager.mapMemory(reinterpret_cast<void *>(i), reinterpret_cast<void *>(i));

    asm ("mov %0, %%cr3" : : "r"(pageTable));
    Memory::memset(bootInfo->framebuffer->baseAddress, 0, bootInfo->framebuffer->bufferSize);

    descriptor.limit = IDT_SIZE * sizeof(IDTEntry) - 1;
    descriptor.offset = reinterpret_cast<size_t>(globalAllocator.requestPage());

//    registerHandler(0x0, reinterpret_cast<void *>(Interrupts::divideByZeroHandler));
//    registerHandler(0x1, reinterpret_cast<void *>(Interrupts::debugHandler));
//    registerHandler(0x2, reinterpret_cast<void *>(Interrupts::nonMaskableInterruptHandler));
//    registerHandler(0x3, reinterpret_cast<void *>(Interrupts::breakpointHandler));
//    registerHandler(0x4, reinterpret_cast<void *>(Interrupts::overflowHandler));
//    registerHandler(0x5, reinterpret_cast<void *>(Interrupts::boundRangeExceededHandler));
//    registerHandler(0x6, reinterpret_cast<void *>(Interrupts::invalidOpcodeHandler));
//    registerHandler(0x7, reinterpret_cast<void *>(Interrupts::deviceNotAvailableHandler));
//    registerHandler(0x8, reinterpret_cast<void *>(Interrupts::doubleFaultHandler));
//    registerHandler(0xA, reinterpret_cast<void *>(Interrupts::invalidTSSHandler));
//    registerHandler(0xB, reinterpret_cast<void *>(Interrupts::segmentNotPresentHandler));
//    registerHandler(0xC, reinterpret_cast<void *>(Interrupts::stackSegmentFaultHandler));
//    registerHandler(0xD, reinterpret_cast<void *>(Interrupts::generalProtectionFaultHandler));
    registerHandler(0xE, reinterpret_cast<void *>(Interrupts::pageFaultHandler));
//    registerHandler(0x10, reinterpret_cast<void *>(Interrupts::x87FloatingPointExceptionHandler));
//    registerHandler(0x11, reinterpret_cast<void *>(Interrupts::alignmentCheckHandler));
//    registerHandler(0x12, reinterpret_cast<void *>(Interrupts::machineCheckHandler));
//    registerHandler(0x13, reinterpret_cast<void *>(Interrupts::simdFloatingPointExceptionHandler));
//    registerHandler(0x14, reinterpret_cast<void *>(Interrupts::virtualizationExceptionHandler));
//    registerHandler(0x15, reinterpret_cast<void *>(Interrupts::controlProtectionExceptionHandler));
//    registerHandler(0x1C, reinterpret_cast<void *>(Interrupts::hypervisorInjectionExceptionHandler));
//    registerHandler(0x1D, reinterpret_cast<void *>(Interrupts::vmmCommunicationExceptionHandler));
//    registerHandler(0x1E, reinterpret_cast<void *>(Interrupts::securityExceptionHandler));
//    registerHandler(0x80, reinterpret_cast<void *>(Interrupts::syscallHandler));

//    registerHandler(0x20, timerHandler);
//    registerHandler(0x21, keyboardHandler);
//    registerHandler(0x2C, mouseHandler);

    asm ("lidt %0" : : "m"(descriptor));
//    remapPIC();

    return {globalRenderer, pageTableManager, globalAllocator, gdtDescriptor};
}
