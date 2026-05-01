#include "bootloader.h"
#include "debugger.h"

#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>

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
#define GDT_KERNEL_DATA 0x10

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


#define SYSCALL_REG_STAR 0xC0000081
#define SYSCALL_REG_LSTAR 0xC0000082
#define SYSCALL_REG_CSTAR 0xC0000083
#define SYSCALL_REG_MASK 0xC0000084
#define IA32_EFER 0xC0000080

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
    uint64_t gs, fs, es, ds, cpl;
} cpu_context_t;

typedef struct {
    cpu_context_t context;
    uint8_t state; // 0 = inactive, 1 = active
    uint64_t timer;
    uint8_t timerswatch;
    char name[32];
} task_t;

// GDT Entry structuur (8 bytes)
typedef struct {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_middle;
    uint8_t  access;
    uint8_t  granularity;
    uint8_t  base_high;
} __attribute__((packed)) GDTEntry;

// TSS Descriptor voor 64-bit (16 bytes)
typedef struct {
    GDTEntry common;
    uint32_t base_upper32;
    uint32_t reserved;
} __attribute__((packed)) TSSEntry;

// De TSS Structuur zelf (104 bytes)
typedef struct {
    uint32_t reserved0;
    uint64_t rsp0; // Stack pointer voor Ring 0
    uint64_t rsp1;
    uint64_t rsp2;
    uint64_t reserved1;
    uint64_t ist1; // Interrupt Stack Table
    uint64_t ist2;
    uint64_t ist3;
    uint64_t ist4;
    uint64_t ist5;
    uint64_t ist6;
    uint64_t ist7;
    uint64_t reserved2;
    uint16_t reserved3;
    uint16_t iopb_offset;
} __attribute__((packed)) TSS;

// De volledige GDT tabel
typedef struct {
    GDTEntry null;          // 0x00
    GDTEntry kernel_code;   // 0x08
    GDTEntry kernel_data;   // 0x10
    GDTEntry user_data;     // 0x18
    GDTEntry user_code;     // 0x20
    TSSEntry tss;           // 0x28 (neemt 16 bytes in beslag!)
} __attribute__((packed)) __attribute__((aligned(0x1000))) GDT;

// Het pointer-object voor het LGDT instructie
typedef struct {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) GDTPointer;

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
extern void syscallentrypoint();
extern GDT gdt;
extern TSS tss;

void laad_geheugen(BootInfo *memory_info);
char* geheugen_geheugenblok_type_naar_string(uint32_t type);
void geheugen_kaart_debug(MemoryDescriptor *desc);
PageLookupResult page_map_indexer(uint64_t virtual_address);
void map_memory(void* pml4mem, void *virtualmemory,void* physicalmemory);
void define_linear_memory_block(void *address);
void define_page_memory_range_from_memory_descriptor(MemoryDescriptor *desc);
void* alloc_page();
void* memset(void* s, int c, uint64_t n);
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
void scheduler_tick();
void* memcpy(void* restrict dest, const void* restrict src, size_t n);
void task_create(char* name, void (*func)());
void timersleep(uint64_t secs);
void hlt();
void enable_syscall();
void cpu_get_specific_registers(uint32_t msr, uint32_t *lo, uint32_t *hi);
void cpu_set_specific_registers(uint32_t msr, uint32_t lo, uint32_t hi);
void yield();
uint32_t get_lapic_id();
void write_tss(TSSEntry* entry, uint64_t tss_addr);
size_t strlen(const char* s);
char* strstr(const char* h, const char* n);
int memcmp(const void* s1, const void* s2, size_t n);
void free(void* ptr);
void abort(void);
long time(long* t);
int strcmp(const char* s1, const char* s2);
int strncmp(const char* s1, const char* s2, size_t n);
char* strchr(const char* s, int c);
long long kernel_atoll(const char* s);
long long pow(long long b, long long e);
long long fmod(long long x, long long y);
int abs(int j);
size_t strspn(const char* s, const char* a);