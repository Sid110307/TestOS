// memory management
#pragma once

#include "defs.h"

#define PAGE_SIZE 4096

struct MemoryDescriptor
{
    unsigned int type;
    void *physicalAddress, *virtualAddress;
    size_t numberOfPages, attributes;
};

static const char *memoryTypes[]{
        "EfiReservedMemoryType",
        "EfiLoaderCode",
        "EfiLoaderData",
        "EfiBootServicesCode",
        "EfiBootServicesData",
        "EfiRuntimeServicesCode",
        "EfiRuntimeServicesData",
        "EfiConventionalMemory",
        "EfiUnusableMemory",
        "EfiACPIReclaimMemory",
        "EfiACPIMemoryNVS",
        "EfiMemoryMappedIO",
        "EfiMemoryMappedIOPortSpace",
        "EfiPalCode",
        "EfiPersistentMemory",
        "EfiMaxMemoryType"
};

class PageFrameAllocator
{
public:
    void readMemoryMap(MemoryDescriptor *descriptor, size_t memoryMapSize, size_t memoryMapDescSize);

    void freePage(void *address);
    void freePages(void *address, size_t pageCount);
    void lockPage(void *address);
    void lockPages(void *address, size_t pageCount);

    void *requestPage();

    size_t getFreeRAM() const;
    size_t getUsedRAM() const;
    size_t getReservedRAM() const;

    static size_t getMemorySize(MemoryDescriptor *memoryMap, size_t memoryMapEntries, size_t memoryMapDescSize);
    Bitmap pageBitmap = {};

private:
    void initBitmap(size_t size, void *bufferAddress);
    void reservePage(void *address);
    void reservePages(void *address, size_t pageCount);
    void unreservePage(void *address);
    void unreservePages(void *address, size_t pageCount);

    size_t freeMemory, usedMemory, reservedMemory, bitmapIndex;
    static bool initialized;
};
