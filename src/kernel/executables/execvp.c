#include "elfh.h"
#include "bestandensysteem.h"

static inline Elf64_Phdr *elf_pheader(Elf64_Ehdr *hdr) {
	return (Elf64_Phdr *)((uint64_t)hdr + hdr->e_phoff);
}

static inline Elf64_Phdr *elf_section(Elf64_Ehdr *hdr, int idx) {
	return &elf_pheader(hdr)[idx];
}


int execvp (const char *file, char *const argv[]){
    void* g = bestand((char*)file);

    Elf64_Ehdr *elfheader = (Elf64_Ehdr*) g;
    if(elfheader->e_type!=2){
        printk("ELF: e_type!=0x2 \n");
        return 1;
    }
    if(elfheader->e_machine!=0x3E){
        printk("ELF: e_machine!=0x3E \n");
        return 1;
    }
    /**
     * WARNING: we need to do a paging check here
    **/
    for(Elf64_Half sec = 0 ; sec < elfheader->e_phnum ; sec++){
        Elf64_Phdr* phdrs = (Elf64_Phdr*) elf_section(elfheader,sec);
        if(phdrs->p_type==PT_LOAD){
            memcpy((void*)phdrs->p_vaddr,(void*)((uint64_t)g + phdrs->p_offset),phdrs->p_filesz);
        }
    }
    uint64_t addr = elfheader->e_entry;
    register int64_t rax __asm__ ("rax") = 60;
    register int rdi __asm__ ("rdi") = 1;
    __asm__ __volatile__ (
        "syscall"
        : "+r" (rax)
        : "r" (rdi)
        : "rcx", "r11", "memory"
    );
    printk("klaar\nentrypoint is op %x !\n",addr);
    ((void (*)())addr)();
    return 0;
}