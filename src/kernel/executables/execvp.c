#include "elfh.h"
#include "bestandensysteem.h"

static inline Elf64_Phdr *elf_pheader(Elf64_Ehdr *hdr) {
	return (Elf64_Phdr *)((uint64_t)hdr + hdr->e_phoff);
}

static inline Elf64_Phdr *elf_section(Elf64_Ehdr *hdr, int idx) {
	return &elf_pheader(hdr)[idx];
}


int execvp (const char *file, char *const argv[]){
    printk("=== execvp START: file=%s ===\n", file);
    
    void* g = bestand((char*)file);
    if(!g) {
        printk("ERROR: bestand() returned NULL\n");
        return 1;
    }
    printk("File loaded at: %x\n", g);

    Elf64_Ehdr *elfheader = (Elf64_Ehdr*) g;
    
    printk("ELF e_type: %d (expect 2)\n", elfheader->e_type);
    if(elfheader->e_type!=2){
        printk("ELF: e_type!=0x2 \n");
        return 1;
    }
    
    printk("ELF e_machine: %d (expect 62)\n", elfheader->e_machine);
    if(elfheader->e_machine!=0x3E){
        printk("ELF: e_machine!=0x3E \n");
        return 1;
    }
    
    printk("ELF e_phnum (segments): %d\n", elfheader->e_phnum);
    
    /**
     * WARNING: we need to do a paging check here
    **/
    for(Elf64_Half sec = 0 ; sec < elfheader->e_phnum ; sec++){
        Elf64_Phdr* phdrs = (Elf64_Phdr*) elf_section(elfheader,sec);
        printk("  Segment %d: type=%d, vaddr=%x, filesz=%d\n", 
               sec, phdrs->p_type, phdrs->p_vaddr, phdrs->p_filesz);
        
        if(phdrs->p_type==PT_LOAD){
            printk("    -> Copying to %x, size %d bytes\n", 
                   phdrs->p_vaddr, phdrs->p_filesz);
            memcpy((void*)phdrs->p_vaddr,(void*)((uint64_t)g + phdrs->p_offset),phdrs->p_filesz);
            printk("    -> Copy done\n");
        }
    }
    
    uint64_t addr = elfheader->e_entry;
    printk("klaar\nentrypoint is op %x !\n",addr);
    printk("About to call entrypoint...\n");
    
    // Voer de ELF entrypoint uit
    // Dit moet in kernelspace gebeuren, niet met syscall
    ((void (*)())addr)();
    
    // Zou hier nooit moeten bereiken, maar voor veiligheid:
    printk("RETURNED FROM ENTRYPOINT (unexpected!)\n");
    return 0;
}