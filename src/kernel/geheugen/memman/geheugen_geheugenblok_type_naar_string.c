#include "geheugen.h"

char* geheugen_geheugenblok_type_naar_string(uint32_t type)
{
    switch(type)
    {
        case MEMORY_TYPE_RESERVED:
            return "EfiReservedMemoryType";
        case MEMORY_TYPE_LOADER_CODE:
            return "EfiLoaderCode";
        case MEMORY_TYPE_LOADER_DATA:
            return "EfiLoaderData";
        case MEMORY_TYPE_BOOT_SERVICES_CODE:
            return "EfiBootServicesCode";
        case MEMORY_TYPE_BOOT_SERVICES_DATA:
            return "EfiBootServicesData";
        case MEMORY_TYPE_RUNTIME_SERVICES_CODE:
            return "EfiRuntimeServiceCode";
        case MEMORY_TYPE_RUNTIME_SERVICES_DATA:
            return "EfiRuntimeServicesData";
        case MEMORY_TYPE_CONVENTIONAL_MEMORY:
            return "EfiConventionalMemory";
        case MEMORY_TYPE_UNUSABLE_MEMORY:
            return "EfiUnusableMemory";
        case MEMORY_TYPE_ACPI_RECLAIM_MEMORY:
            return "EfiACPIReclaimMemory";
        case MEMORY_TYPE_ACPI_MEMORY_NVS:
            return "EfiACPIMemoryNVS";
        case MEMORY_TYPE_MEMORY_MAPPED_IO:
            return "EfiMemoryMappedIO";
        case MEMORY_TYPE_MEMORY_MAPPED_IO_PORT_SPACE:
            return "EfiMemoryMappedIOPortSpace";
        case MEMORY_TYPE_PAL_CODE:
            return "EfiPalCode";
        case MEMORY_TYPE_PERSISTENT_MEMORY:
            return "EfiPersistentMemory";
        default:
            return "Onbekend";
    }
}