#pragma once

#include "defs.h"

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

    void *requestPage(bool clear = false);

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

enum PageTableFlag
{
    Present = 0,
    Writable = 1,
    UserAccessible = 2,
    WriteThrough = 3,
    CacheDisabled = 4,
    Accessed = 5,
    PageSize = 7,
    Custom0 = 9,
    Custom1 = 10,
    Custom2 = 11,
    NoExecute = 63
};

struct PageDirectoryEntry
{
    size_t value;
    void setFlag(PageTableFlag flag, bool enabled);
    bool getFlag(PageTableFlag flag) const;
    void setAddress(size_t address);
    size_t getAddress() const;
};

struct PageTable
{
    PageDirectoryEntry entries[PAGE_TABLE_SIZE];
} __attribute__((aligned(PAGE_SIZE)));

class PageMapIndexer
{
public:
    explicit PageMapIndexer(size_t virtualAddress);
    size_t pageDirPtrTableIndex, pageDirTableIndex, pageTableIndex, pageIndex;
};

class PageTableManager
{
public:
    explicit PageTableManager(PageTable *address);
    void mapMemory(void *virtualMemory, void *physicalMemory);
private:
    PageTable *address;
};

extern PageFrameAllocator globalAllocator;
