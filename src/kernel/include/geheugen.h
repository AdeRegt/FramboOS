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
#define PAGE_GAP_SIZE 0x200000 // 2MB

#define MEMORY_MIN_SIZE_FOR_PAGING 0xA00 // 2MB
#define MEMORY_MIN_SIZE_FOR_ALLOCATION 0xA00

#define IDT_MAX_DESCRIPTORS 256

#define IDT_TYPE_INTERRUPT_GATE 0x8E
#define IDT_TYPE_TRAP_GATE      0x8F

typedef struct{
    uint8_t present: 1;
    uint8_t readwrite: 1;
    uint8_t usersuper: 1;
    uint8_t writetrough: 1;
    uint8_t cachedisabled: 1;
    uint8_t accessed: 1;
    uint8_t ignore1: 1;
    uint8_t largepages: 1;
    uint8_t ignore2: 1;
    uint8_t available: 3;
    uint64_t address: 52;
}Page;

typedef struct{
    Page pages[512];
}PageTable;

typedef struct{
    uint64_t page_map_level_4_table_index;
    uint64_t page_directory_pointer_table_index;
    uint64_t page_directory_table_index;
    uint64_t page_table_index;
}PageLookupResult;

typedef struct __attribute__((packed)) {
    uint16_t offset0;
    uint16_t selector;
    uint8_t ist;
    uint8_t type_attr;
    uint16_t offset1;
    uint32_t offset2;
    uint32_t ignore;
}IDTDescEntry;
_Static_assert(sizeof(IDTDescEntry) == 16, "IDTDescEntry must be 16 bytes");

typedef struct __attribute__((packed)) {
    uint16_t Limit;
    uint64_t Offset;
} IDTR;

typedef struct{
    uint64_t ip;
    uint64_t cs;
    uint64_t flags;
    uint64_t sp;
    uint64_t ss;
}interrupt_frame;

typedef struct cpu_context {
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
    uint64_t rsi, rdi, rbp, rdx, rcx, rbx, rax;
    uint64_t rip, rsp, rflags;
} cpu_context_t;

extern void _KernelStart();
extern void _KernelEnd();
extern MemoryDescriptor *paging_geheugen_blok;
extern MemoryDescriptor *allocatie_geheugen_blok;
extern MemoryDescriptor *kernel_geheugen_blok;
extern MemoryDescriptor *video_geheugen_blok;
extern PageTable *master_page_table;
extern MemoryInfo *memory_info_pointer;
extern cpu_context_t current_task;
extern void initialise_gdt();
extern IDTR idtr;
extern __attribute__ ((aligned(0x10))) IDTDescEntry idt[256];
extern __attribute__((interrupt)) void default_interrupt_handler(interrupt_frame* frame);
extern __attribute__((interrupt)) void error_interrupt_handler(interrupt_frame* frame);
extern void context_switch(cpu_context_t *old, cpu_context_t *new);
extern void timer_interrupt_stub();

void laad_geheugen(BootInfo *memory_info);
char* geheugen_geheugenblok_type_naar_string(uint32_t type);
void geheugen_kaart_debug(MemoryDescriptor *desc);
PageLookupResult page_map_indexer(uint64_t virtual_address);
void map_memory(void* pml4mem, void *virtualmemory,void* physicalmemory);
void define_linear_memory_block(void *address);
void define_page_memory_range_from_memory_descriptor(MemoryDescriptor *desc);
void* alloc_page();
void *memset(void *s, char val, uint64_t count);
MemoryDescriptor* geheugenblok_van_address(uint64_t address);
void outportb(uint16_t port, uint8_t value);
uint8_t inportb(uint16_t port);
void reset_pic();
void sti();
void cli();
void idt_set_entry(IDTDescEntry *entry, void (*handler)(), uint16_t selector, uint8_t ist, uint8_t type_attr);
void timer_interrupt_handler();
void outportl(uint16_t port, uint32_t value);
uint32_t inportl(uint16_t port);