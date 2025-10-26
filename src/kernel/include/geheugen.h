#include "bootloader.h"
#include "debugger.h"

#define MEMORY_TYPE_RESERVED 0
#define MEMORY_TYPE_LOADER_CODE 1
#define MEMORY_TYPE_LOADER_DATA 2
#define MEMORY_TYPE_BOOT_SERVICES_CODE 3
#define MEMORY_TYPE_BOOT_SERVICES_DATA 4
#define MEMORY_TYPE_RUNTIME_SERVICES_CODE 5
#define MEMORY_TYPE_RUNTIME_SERVICES_DATA 6
#define MEMORY_TYPE_CONVENTIONAL_MEMORY 7
#define MEMORY_TYPE_UNUSABLE_MEMORY 8
#define MEMORY_TYPE_ACPI_RECLAIM_MEMORY 9
#define MEMORY_TYPE_ACPI_MEMORY_NVS 10
#define MEMORY_TYPE_MEMORY_MAPPED_IO 11
#define MEMORY_TYPE_MEMORY_MAPPED_IO_PORT_SPACE 12
#define MEMORY_TYPE_PAL_CODE 13
#define MEMORY_TYPE_PERSISTENT_MEMORY 14

#define MEMORY_PAGE_SIZE 4096

#define MEMORY_MIN_SIZE_FOR_PAGING 0x200 // 2MB
#define MEMORY_MIN_SIZE_FOR_ALLOCATION 0x20

extern void _KernelStart();
extern void _KernelEnd();
extern MemoryDescriptor *paging_geheugen_blok;
extern MemoryDescriptor *allocatie_geheugen_blok;

void geheugen_initialiseer(MemoryInfo *memory_info);
char* geheugen_geheugenblok_type_naar_string(uint32_t type);
void geheugen_kaart_debug(MemoryDescriptor *desc);