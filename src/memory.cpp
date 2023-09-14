#include "include/memory.h"

size_t MemoryManager::getMemorySize(MemoryDescriptor *memoryMap, size_t memoryMapEntries,
                                    size_t memoryMapDescriptorSize)
{
    static size_t memorySizeBytes = 0;
    if (memorySizeBytes > 0) return memorySizeBytes;

    for (size_t i = 0; i < memoryMapEntries; ++i)
    {
        auto *desc = reinterpret_cast<MemoryDescriptor *>(reinterpret_cast<size_t>(memoryMap) +
                                                          i * memoryMapDescriptorSize);
        memorySizeBytes += desc->numberOfPages * 4096;
    }

    return memorySizeBytes;
}
