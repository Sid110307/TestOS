// memory management
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

class MemoryManager
{
public:
    static size_t getMemorySize(MemoryDescriptor *memoryMap, size_t memoryMapEntries, size_t memoryMapDescriptorSize);
};
