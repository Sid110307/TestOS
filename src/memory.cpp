#include "include/memory.h"

bool PageFrameAllocator::initialized = false;

void PageFrameAllocator::readMemoryMap(MemoryDescriptor *descriptor, size_t memoryMapSize, size_t memoryMapDescSize)
{
    if (initialized) return;
    initialized = true;

    void *largestFreeMemorySegment = nullptr;
    size_t memoryMapEntries = memoryMapSize / memoryMapDescSize, largestFreeMemorySegmentSize = 0;

    for (size_t i = 0; i < memoryMapEntries; ++i)
    {
        auto *desc = reinterpret_cast<MemoryDescriptor *>(reinterpret_cast<size_t>(descriptor) + i * memoryMapDescSize);
        if (desc->type == 7 && desc->numberOfPages * PAGE_SIZE > largestFreeMemorySegmentSize)
        {
            largestFreeMemorySegment = desc->physicalAddress;
            largestFreeMemorySegmentSize = desc->numberOfPages * PAGE_SIZE;
        }
    }

    size_t totalMemory = getMemorySize(descriptor, memoryMapEntries, memoryMapDescSize);
    freeMemory = totalMemory;
    size_t bitmapSize = totalMemory / PAGE_SIZE / 8 + 1;

    initBitmap(bitmapSize, largestFreeMemorySegment);
    lockPages(&pageBitmap, pageBitmap.size / PAGE_SIZE + 1);

    for (size_t i = 0; i < memoryMapEntries; ++i)
    {
        auto *desc = reinterpret_cast<MemoryDescriptor *>(reinterpret_cast<size_t>(descriptor) + i * memoryMapDescSize);
        if (desc->type != 7) reservePages(desc->physicalAddress, desc->numberOfPages);
    }
}

void PageFrameAllocator::freePage(void *address)
{
    size_t index = reinterpret_cast<size_t>(address) / PAGE_SIZE;

    if (!pageBitmap[index]) return;
    pageBitmap.set(index, false);

    freeMemory += PAGE_SIZE;
    usedMemory -= PAGE_SIZE;

    if (index < bitmapIndex) bitmapIndex = index;
}

void PageFrameAllocator::freePages(void *address, size_t pageCount)
{
    for (size_t i = 0; i < pageCount; ++i)
        freePage(reinterpret_cast<void *>(reinterpret_cast<size_t>(address) + i * PAGE_SIZE));
}

void PageFrameAllocator::lockPage(void *address)
{
    size_t index = reinterpret_cast<size_t>(address) / PAGE_SIZE;

    if (pageBitmap[index]) return;
    pageBitmap.set(index, true);

    freeMemory -= PAGE_SIZE;
    usedMemory += PAGE_SIZE;
}

void PageFrameAllocator::lockPages(void *address, size_t pageCount)
{
    for (size_t i = 0; i < pageCount; ++i)
        lockPage(reinterpret_cast<void *>(reinterpret_cast<size_t>(address) + i * PAGE_SIZE));
}

void *PageFrameAllocator::requestPage()
{
    for (; bitmapIndex < pageBitmap.size * 8; ++bitmapIndex)
    {
        if (pageBitmap[bitmapIndex]) continue;
        lockPage(reinterpret_cast<void *>(bitmapIndex * PAGE_SIZE));

        return reinterpret_cast<void *>(bitmapIndex * PAGE_SIZE);
    }

    return nullptr;
}

size_t PageFrameAllocator::getFreeRAM() const { return freeMemory; }
size_t PageFrameAllocator::getUsedRAM() const { return usedMemory; }
size_t PageFrameAllocator::getReservedRAM() const { return reservedMemory; }

size_t PageFrameAllocator::getMemorySize(MemoryDescriptor *memoryMap, size_t memoryMapEntries, size_t memoryMapDescSize)
{
    static size_t memorySizeBytes = 0;
    if (memorySizeBytes > 0) return memorySizeBytes;

    for (size_t i = 0; i < memoryMapEntries; ++i)
    {
        auto *desc = reinterpret_cast<MemoryDescriptor *>(reinterpret_cast<size_t>(memoryMap) + i * memoryMapDescSize);
        memorySizeBytes += desc->numberOfPages * PAGE_SIZE;
    }

    return memorySizeBytes;
}

void PageFrameAllocator::initBitmap(size_t size, void *bufferAddress)
{
    pageBitmap.size = size;
    pageBitmap.buffer = reinterpret_cast<unsigned char *>(bufferAddress);

    for (size_t i = 0; i < pageBitmap.size; ++i) *reinterpret_cast<unsigned char *>(pageBitmap.buffer + i) = 0;
}

void PageFrameAllocator::reservePage(void *address)
{
    size_t index = reinterpret_cast<size_t>(address) / PAGE_SIZE;

    if (pageBitmap[index]) return;
    pageBitmap.set(index, true);

    freeMemory -= PAGE_SIZE;
    reservedMemory += PAGE_SIZE;
}

void PageFrameAllocator::reservePages(void *address, size_t pageCount)
{
    for (size_t i = 0; i < pageCount; ++i)
        reservePage(reinterpret_cast<void *>(reinterpret_cast<size_t>(address) + i * PAGE_SIZE));
}

void PageFrameAllocator::unreservePage(void *address)
{
    size_t index = reinterpret_cast<size_t>(address) / PAGE_SIZE;

    if (!pageBitmap[index]) return;
    pageBitmap.set(index, false);

    freeMemory += PAGE_SIZE;
    reservedMemory -= PAGE_SIZE;

    if (index < bitmapIndex) bitmapIndex = index;
}

void PageFrameAllocator::unreservePages(void *address, size_t pageCount)
{
    for (size_t i = 0; i < pageCount; ++i)
        unreservePage(reinterpret_cast<void *>(reinterpret_cast<size_t>(address) + i * PAGE_SIZE));
}
