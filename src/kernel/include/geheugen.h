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

#define IDT_OFFSET 0x20

#define GDT_KERNEL_CODE 0x08

#define PIC1		0x20		/* IO base address for master PIC */
#define PIC2		0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)

#define IA32_APIC_BASE_MSR 0x1B
#define IA32_APIC_BASE_BSP (1 << 8)
#define IA32_APIC_BASE_ENABLE (1 << 11)

#define LAPIC_BASE 0xFEE00000
#define LAPIC_REG(offset) (*(volatile uint32_t*)((uintptr_t)LAPIC_BASE + (offset)))

#define LAPIC_SVR 0xF0  // Spurious Interrupt Vector Register

#define LAPIC_TIMER_VECTOR 0x20

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
    uint64_t rdi, rsi, rbp, rbx, rdx, rcx, rax;
    uint64_t rip, cs, rflags, rsp, ss;
    uint64_t cr3;
} cpu_context_t;

typedef struct {
    cpu_context_t context;
    uint8_t state; // 0 = inactive, 1 = active
    uint64_t timer;
    char name[32];
} task_t;

extern void _KernelStart();
extern void _KernelEnd();
extern MemoryDescriptor *paging_geheugen_blok;
extern MemoryDescriptor *allocatie_geheugen_blok;
extern MemoryDescriptor *kernel_geheugen_blok;
extern MemoryDescriptor *video_geheugen_blok;
extern PageTable *master_page_table;
extern MemoryInfo *memory_info_pointer;
extern void initialise_gdt();
extern IDTR idtr;
extern __attribute__ ((aligned(0x10))) IDTDescEntry idt[256];
extern __attribute__((interrupt)) void default_interrupt_handler(interrupt_frame* frame);
extern __attribute__((interrupt)) void error_interrupt_handler_0x00_(interrupt_frame* frame);
extern __attribute__((interrupt)) void error_interrupt_handler_0x01_(interrupt_frame* frame);
extern __attribute__((interrupt)) void error_interrupt_handler_0x02_(interrupt_frame* frame);
extern __attribute__((interrupt)) void error_interrupt_handler_0x03_(interrupt_frame* frame);
extern __attribute__((interrupt)) void error_interrupt_handler_0x04_(interrupt_frame* frame);
extern __attribute__((interrupt)) void error_interrupt_handler_0x05_(interrupt_frame* frame);
extern __attribute__((interrupt)) void error_interrupt_handler_0x06_(interrupt_frame* frame);
extern __attribute__((interrupt)) void error_interrupt_handler_0x07_(interrupt_frame* frame);
extern __attribute__((interrupt)) void error_interrupt_handler_0x08_(interrupt_frame* frame);
extern __attribute__((interrupt)) void error_interrupt_handler_0x09_(interrupt_frame* frame);
extern __attribute__((interrupt)) void error_interrupt_handler_0x0A_(interrupt_frame* frame);
extern __attribute__((interrupt)) void error_interrupt_handler_0x0B_(interrupt_frame* frame);
extern __attribute__((interrupt)) void error_interrupt_handler_0x0C_(interrupt_frame* frame);
extern __attribute__((interrupt)) void error_interrupt_handler_0x0D_(interrupt_frame* frame);
extern __attribute__((interrupt)) void error_interrupt_handler_0x0E_(interrupt_frame* frame);
extern __attribute__((interrupt)) void error_interrupt_handler_0x0F_(interrupt_frame* frame);
extern __attribute__((interrupt)) void error_interrupt_handler_0x10_(interrupt_frame* frame);
extern __attribute__((interrupt)) void error_interrupt_handler_0x11_(interrupt_frame* frame);
extern __attribute__((interrupt)) void error_interrupt_handler_0x12_(interrupt_frame* frame);
extern __attribute__((interrupt)) void error_interrupt_handler_0x13_(interrupt_frame* frame);
extern __attribute__((interrupt)) void error_interrupt_handler_0x14_(interrupt_frame* frame);
extern __attribute__((interrupt)) void error_interrupt_handler_0x15_(interrupt_frame* frame);
extern __attribute__((interrupt)) void error_interrupt_handler_0x16_(interrupt_frame* frame);
extern __attribute__((interrupt)) void error_interrupt_handler_0x17_(interrupt_frame* frame);
extern __attribute__((interrupt)) void error_interrupt_handler_0x18_(interrupt_frame* frame);
extern __attribute__((interrupt)) void error_interrupt_handler_0x19_(interrupt_frame* frame);
extern __attribute__((interrupt)) void error_interrupt_handler_0x10_(interrupt_frame* frame);
extern __attribute__((interrupt)) void error_interrupt_handler_0x1A_(interrupt_frame* frame);
extern __attribute__((interrupt)) void error_interrupt_handler_0x1B_(interrupt_frame* frame);
extern __attribute__((interrupt)) void error_interrupt_handler_0x1C_(interrupt_frame* frame);
extern __attribute__((interrupt)) void error_interrupt_handler_0x1D_(interrupt_frame* frame);
extern __attribute__((interrupt)) void error_interrupt_handler_0x1E_(interrupt_frame* frame);
extern __attribute__((interrupt)) void error_interrupt_handler_0x1F_(interrupt_frame* frame);
extern task_t ct[10];
extern int current_task;
extern int max_task;

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
void outportl(uint16_t port, uint32_t value);
uint32_t inportl(uint16_t port);
void acknowledge_interrupt();
void sleep(uint32_t timer);
void cpuid(uint32_t eax, uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d);
uint64_t read_msr(uint32_t msr);
uint8_t check_apic();
void enable_lapic();
void write_msr(uint32_t msr, uint64_t value);
uint8_t get_active_int();
void taskswitchstub();
cpu_context_t* scheduler_tick(cpu_context_t* current);
void memcpy(void* dest, const void* src, uint64_t n);
void task_create(char* name, void (*func)());
void timersleep(uint64_t secs);
void hlt();